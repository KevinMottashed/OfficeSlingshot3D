// HumanModel.cpp: implementation of the CHumanModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "HumanModel.h"

// changed
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// by gaecha
// chai3d를 쓸라고
#include "CWorld.h"
#include "CGenericObject.h"
#include "CFalcon3dofPointer.h"
#include "CMeta3dofPointer.h"
#include "CPrecisionTimer.h"
#include "CMaterial.h"
#include "CProxyGraphPointForceAlgo.h"
#include "CTriangle.h"
#include "CViewport.h"
#include "CDepthImage.h"
#include "CLight.h"
///////////////////////////////////////////////////////

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
void getModelCoord(cVector3d newObjectPos, cVector3d newObjectPos1, cVector3d newObjectPos2, cMatrix3d newObjectRot,cMatrix3d  newObjectRot1, cMatrix3d newObjectRot2);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHumanModel::CHumanModel()
{
	int i;
	m_zTranslateErr = 45.0;

	// Setting geometrical parameters of the human model in cm
	m_partPara[CHEST][0] = 35;
	m_partPara[CHEST][1] = 20;
	m_partPara[CHEST][2] = 12;

	m_partPara[RIGHT_UPPER_ARM][0] = 12;
	m_partPara[RIGHT_UPPER_ARM][1] = 25;
	m_partPara[RIGHT_UPPER_ARM][2] = 12;

	m_partPara[RIGHT_LOWER_ARM][0] = 12;
	m_partPara[RIGHT_LOWER_ARM][1] = 25;
	m_partPara[RIGHT_LOWER_ARM][2] = 12;


	for(i=0; i<TOTAL_NUM_PART; i++) {
		m_partPose.m_pos[i].set(0.0, 0.0, 0.0);
		m_partPose.m_rot[i].set(1.0,0,0,
						0,1.0,0,
						0,0,1.0);
	}

	// Initialize ARToolKit
	strcpy(m_pattName[0], "Data/patt.hiro");
	strcpy(m_pattName[1], "Data/patt.sample1");
	strcpy(m_pattName[2], "Data/patt.sample2");

	for(i=0; i<TOTAL_NUM_PART; i++) {
		m_ar.RegisterMarker(m_pattName[i]);
	}

	for(i=0; i<TOTAL_NUM_PART; i++) {
		m_partPose.m_pos[i].set(CHAI_LARGE, CHAI_LARGE, CHAI_LARGE);
		m_partPose.m_rot[i].identity();
	}
}

CHumanModel::~CHumanModel()
{

}

void CHumanModel::SetPartPose(int a_partNum, cVector3d a_pos, cMatrix3d a_rot)
{
	m_partPose.m_pos[a_partNum] = a_pos; 
	m_partPose.m_rot[a_partNum] = a_rot;

	m_pPart[a_partNum]->setPos(m_partPose.m_pos[a_partNum]);
	m_pPart[a_partNum]->setRot(m_partPose.m_rot[a_partNum]);
}

void CHumanModel::SetPartPose(HumanPartPose a_partPose)
{
	m_partPose = a_partPose;
	
	for(int i=0; i<TOTAL_NUM_PART; i++) {
		m_pPart[i]->setPos(m_partPose.m_pos[i]);
		m_pPart[i]->setRot(m_partPose.m_rot[i]);
	}
}

ContactInfo CHumanModel::proxymityCheck(cVector3d proxyPos)
{
	m_contactInfo.isContacted = false;

	double minDistance = 0.1E24;
	double distance;
	cVector3d localPos;

	//check for chest
	cMatrix3d transLocalRot;
    m_partPose.m_rot[CHEST].transr(transLocalRot);
	
	localPos = proxyPos;
	localPos.sub(m_partPose.m_pos[CHEST]);
	transLocalRot.mul(localPos);

	if( localPos.x <= m_partPara[CHEST][0]/2.0 &&
		localPos.x >= -m_partPara[CHEST][0]/2.0 &&
		localPos.y <= m_partPara[CHEST][1]/2.0 &&
		localPos.y >= -m_partPara[CHEST][1]/2.0) {
		distance = fabs(localPos.z - m_partPara[CHEST][2]/2.0);
		if(distance < minDistance && distance < 6.0) {
			minDistance = distance;
			m_contactInfo.isContacted = true;

			m_contactInfo.contactPosT.x = (localPos.x + m_partPara[CHEST][0]/2.0)/m_partPara[CHEST][0];
			m_contactInfo.contactPosT.y = (localPos.y + m_partPara[CHEST][1]/2.0)/m_partPara[CHEST][1];
	
			m_contactInfo.contactPosH.x = m_contactInfo.contactPosT.x*m_partPara[CHEST][0] - m_partPara[CHEST][0]/2.0;
			m_contactInfo.contactPosH.y = m_contactInfo.contactPosT.y*m_partPara[CHEST][1] - m_partPara[CHEST][1]/2.0;
			m_contactInfo.contactPosH.z = m_partPara[CHEST][2]/2.0;
			m_partPose.m_rot[CHEST].mul(m_contactInfo.contactPosH);
			m_contactInfo.contactPosH.add(m_partPose.m_pos[CHEST]);
			m_contactInfo.tagConPart = CHEST;
		}
	}



	//check for upper arm
    m_partPose.m_rot[RIGHT_UPPER_ARM].transr(transLocalRot);
	
	localPos = proxyPos;
	localPos.sub(m_partPose.m_pos[RIGHT_UPPER_ARM]);
	transLocalRot.mul(localPos);
	if( localPos.y <= m_partPara[RIGHT_UPPER_ARM][1]/2.0 &&
		localPos.y >= -m_partPara[RIGHT_UPPER_ARM][1]/2.0 ) {
		distance = sqrt(localPos.x*localPos.x + localPos.z*localPos.z);
		if(distance < minDistance && distance < 20.0) {
			double theta = atan2(localPos.x,localPos.z);

			//if(theta < 0.0)
			//	theta += 2.0*PI;
			
			if( theta > -PI/2.0 && theta < PI/2.0 ) {
				cMatrix3d temp;
				minDistance = distance;
				m_contactInfo.isContacted = true;			

				//m_contactInfo.contactPosT.x = theta/(2.0*PI);
				m_contactInfo.contactPosT.x = theta/PI + 0.5;
				m_contactInfo.contactPosT.y = (localPos.y + m_partPara[RIGHT_UPPER_ARM][1]/2.0)/m_partPara[RIGHT_UPPER_ARM][1];
			
				m_contactInfo.contactPosH.set(0.0, m_contactInfo.contactPosT.y*m_partPara[RIGHT_UPPER_ARM][1]-m_partPara[RIGHT_UPPER_ARM][1]/2.0, m_partPara[RIGHT_UPPER_ARM][2]/2.0);
				temp.set(cVector3d(0.0, 1.0, 0.0), theta);
				temp.mul(m_contactInfo.contactPosH);
				m_partPose.m_rot[RIGHT_UPPER_ARM].mul(m_contactInfo.contactPosH);
				m_contactInfo.contactPosH.add(m_partPose.m_pos[RIGHT_UPPER_ARM]);
				m_contactInfo.tagConPart = RIGHT_UPPER_ARM;
			}
		}
	}

	//check for lower arm
    m_partPose.m_rot[RIGHT_LOWER_ARM].transr(transLocalRot);
	
	localPos = proxyPos;
	localPos.sub(m_partPose.m_pos[RIGHT_LOWER_ARM]);
	transLocalRot.mul(localPos);
	if( localPos.y <= m_partPara[RIGHT_LOWER_ARM][1]/2.0 &&
		localPos.y >= -m_partPara[RIGHT_LOWER_ARM][1]/2.0 ) {
		distance = sqrt(localPos.x*localPos.x + localPos.z*localPos.z);
		if(distance < minDistance && distance < 20.0 ) {
			cMatrix3d temp;
			minDistance = distance;
			m_contactInfo.isContacted = true;
			double theta = atan2(localPos.x,localPos.z);

			if(theta < 0.0)
				theta += 2.0*PI;
			m_contactInfo.contactPosT.x = theta/(2.0*PI);
			m_contactInfo.contactPosT.y = (localPos.y + m_partPara[RIGHT_LOWER_ARM][1]/2.0)/m_partPara[RIGHT_LOWER_ARM][1];
		
			m_contactInfo.contactPosH.set(0.0, m_contactInfo.contactPosT.y*m_partPara[RIGHT_LOWER_ARM][1]-m_partPara[RIGHT_LOWER_ARM][1]/2.0, m_partPara[RIGHT_LOWER_ARM][2]/2.0);
			temp.set(cVector3d(0.0, 1.0, 0.0), theta);
			temp.mul(m_contactInfo.contactPosH);
			m_partPose.m_rot[RIGHT_LOWER_ARM].mul(m_contactInfo.contactPosH);
			m_contactInfo.contactPosH.add(m_partPose.m_pos[RIGHT_LOWER_ARM]);
			m_contactInfo.tagConPart = RIGHT_LOWER_ARM;
		}
	}


	return m_contactInfo;
}

void CHumanModel::UpdatePose(unsigned char * a_pImage)
{
	m_ar.SetImage(a_pImage);
	m_ar.DetectMarker();

	for(int i=0; i<TOTAL_NUM_PART; i++) {
		if(m_ar.m_marker_info[i].m_is_detected) {
			m_partPose.m_pos[i] = m_ar.m_marker_info[i].m_chai_pos;
			m_partPose.m_rot[i] = m_ar.m_marker_info[i].m_chai_rot;
			m_partPose.m_pos[i].mul(0.1);
		} else {
			m_partPose.m_pos[i].set(CHAI_LARGE, CHAI_LARGE, CHAI_LARGE);
			m_partPose.m_rot[i].identity();
		}
	}

	m_partPose.m_pos[CHEST].z = m_partPose.m_pos[CHEST].z - m_zTranslateErr - (0.5)*m_partPara[CHEST][2];
	m_partPose.m_pos[RIGHT_UPPER_ARM].z = m_partPose.m_pos[RIGHT_UPPER_ARM].z - m_zTranslateErr - (0.5)*m_partPara[RIGHT_UPPER_ARM][2];
	m_partPose.m_pos[RIGHT_LOWER_ARM].z = m_partPose.m_pos[RIGHT_LOWER_ARM].z - m_zTranslateErr - (0.5)*m_partPara[RIGHT_LOWER_ARM][2];
}


void CHumanModel::SetParentWorld(cWorld * a_pParentWorld)
{
	m_pParentWorld = a_pParentWorld;

	m_pPart[0] = new cMesh(m_pParentWorld);
	m_pPart[1] = new cMesh(m_pParentWorld);
	m_pPart[2] = new cMesh(m_pParentWorld);


	m_pParentWorld->addChild(m_pPart[0]);
	m_pParentWorld->addChild(m_pPart[1]);
	m_pParentWorld->addChild(m_pPart[2]);

	m_pPart[0]->loadFromFile("Blender/ChestCube.obj"); // This is the most recent Cube : March 22 @7:13
	m_pPart[1]->loadFromFile("Blender/Armcylinder.obj");
	m_pPart[2]->loadFromFile("Blender/Armcylinder.obj");


	// Move the object over some so the Phantom will not initially be
	// inside the object.
	m_pPart[0]->setHapticEnabled(false, true);
	m_pPart[1]->setHapticEnabled(false, true);
	m_pPart[2]->setHapticEnabled(false, true);
	m_pPart[0]->translate(CHAI_LARGE ,CHAI_LARGE ,CHAI_LARGE);
	m_pPart[1]->translate(CHAI_LARGE ,CHAI_LARGE ,CHAI_LARGE);
	m_pPart[2]->translate(CHAI_LARGE ,CHAI_LARGE ,CHAI_LARGE);

	// Initialize the object's collision-detection state
	m_pPart[0]->computeGlobalPositions();
	m_pPart[1]->computeGlobalPositions();
	m_pPart[2]->computeGlobalPositions();

	m_pPart[0]->computeBoundaryBox(true);
	m_pPart[1]->computeBoundaryBox(true);
	m_pPart[2]->computeBoundaryBox(true);
}

void CHumanModel::ShowAvatar(bool a_show)
{
	if(a_show) {
		m_pPart[0]->setShow(true, true);
		m_pPart[1]->setShow(true, true);
		m_pPart[2]->setShow(true, true);
	} else {
		m_pPart[0]->setShow(false, true);
		m_pPart[1]->setShow(false, true);
		m_pPart[2]->setShow(false, true);
	}
}