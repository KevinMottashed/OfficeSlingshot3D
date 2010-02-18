// HumanModel.h: interface for the CHumanModel class.
//
//////////////////////////////////////////////////////////////////////

// changed
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// by gaecha
// chai3d를 쓸라고
#include "CVector3d.h"
#include "CMatrix3d.h"
#include "CMesh.h"
#include "CWorld.h"
///////////////////////////////////////////////////////

#include "TactileArray.h"
#include "ART.h"


#if !defined(AFX_HUMANMODEL_H__E7A7CD4C_BA89_4B7C_9286_4BAECD7A7605__INCLUDED_)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PI 3.141592

typedef enum tagHumanPart {
	CHEST = 0,
	RIGHT_UPPER_ARM,
	RIGHT_LOWER_ARM,
	TOTAL_NUM_PART
} HumanPart;

typedef struct tagContactInfo {
	// Is the proxy is collided with the human model?
	bool isContacted; 
	// Tag for contacted part
	HumanPart tagConPart;
	// contact position on human model.
	cVector3d contactPosH;
	// contact position on tactile device. 0.0 - 1.0
	// Imagine that the tactile array is flat.
	cVector3d contactPosT;
} ContactInfo;

typedef struct tagHumanPartPose {
	cVector3d m_pos[TOTAL_NUM_PART];
	cMatrix3d m_rot[TOTAL_NUM_PART];
} HumanPartPose;

class CHumanModel
{
public:
	TactileArray * m_pTactileArray;
	// geometrical parameters for each part of human model
	// 0 : x, 1 : y, 2 : z.
	// For example, in chest 0->width, 1->height, 2-> depth
	double m_partPara[TOTAL_NUM_PART][3];
	
	double m_zTranslateErr;

	ART m_ar;
	
	char m_pattName[TOTAL_NUM_PART][100];
	HumanPartPose m_partPose;

	cMesh * m_pPart[TOTAL_NUM_PART];

	CHumanModel();
	virtual ~CHumanModel();
	void SetPartPose(int a_partNum, cVector3d a_pos, cMatrix3d a_rot);
	void SetPartPose(HumanPartPose a_partPose);
	
	// proximity check
	double distance[TOTAL_NUM_PART];
	ContactInfo m_contactInfo;
	ContactInfo proxymityCheck(cVector3d proxyPos);

	void UpdatePose(unsigned char * a_pImage);

	cWorld * m_pParentWorld;
	void SetParentWorld(cWorld * a_pParentWorld);

	void ShowAvatar(bool a_show);
};

#endif // !defined(AFX_HUMANMODEL_H__E7A7CD4C_BA89_4B7C_9286_4BAECD7A7605__INCLUDED_)
