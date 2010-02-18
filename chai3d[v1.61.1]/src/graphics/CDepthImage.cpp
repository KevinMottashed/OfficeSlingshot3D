  //===========================================================================
/*
    This file is part of the CHAI 3D visualization and haptics libraries.
    Copyright (C) 2003-2004 by CHAI 3D. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License("GPL") version 2
    as published by the Free Software Foundation.

    For using the CHAI 3D libraries with software that can not be combined
    with the GNU GPL, and for taking advantage of the additional benefits
    of our support services, please contact CHAI 3D about acquiring a
    Professional Edition License.

    \author:    <http://www.chai3d.org>
    \author:    Francois Conti
    \author:    Dan Morris
    \author:    Chris Sewell
    \version    1.1
    \date       01/2004
*/
//===========================================================================

//===========================================================================
#include "CDepthImage.h"
#include "CVertex.h"
#include "CTriangle.h"
#include "CCollisionBrute.h"
#include <algorithm>
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of cMesh

    \fn       cMesh::cMesh(cWorld* a_parent)
    \param    a_parent  Pointer to parent world.
*/
//===========================================================================
cDepthImage::cDepthImage(cWorld *a_parent)
{
    // initialize parent object of mesh. Not yet a child on an other object.
    m_parent = NULL;

    // set parent world of mesh
    m_parentWorld = a_parent;

    // should we use the material property?
    m_useImageMaterial = true;

	// not updated yet
	m_bFirstUpdate = false;

	// proxy graph does not finish its loop
	m_bUpdate = false;

	// Now proxy graph algorithm is not working
	m_bProxyGraph = false;

	// Initialized the Elapsed graphic & haptic time
	m_lElapsedGraphicTime = 31000;
	m_lElapsedHapticTime = 0;
	m_timeRatio = 0.0;

	// initialize previous surface depth
	m_previousSurfacePoint.set(0.0, 0.0, 255.0);

	// smoothing
	m_useSmoothing = true;
	m_smoothingArea = 1;
	
	// rgb image and depth image initialization
	m_colorData = NULL;
	m_nextColorData = NULL;
	m_tempColorData = NULL;
	m_colorTotalChannel = 0;
	m_colorChannelNum = 0;
	m_colorImageSizeX = 0;
	m_colorImageSizeY = 0;
	
	m_depthData = NULL;
	m_nextDepthData = NULL;
	m_tempDepthData = NULL;
	m_depthTotalChannel = 0;
	m_depthChannelNum = 0;
	m_depthImageSizeX = 0;
	m_depthImageSizeY = 0;

	m_tag = DEPTH_IMAGE_TAG;
	m_dynamicMesh = new cMesh(a_parent);
	m_dynamicMesh->m_tag = DEPTH_IMAGE_TAG;
	// Dynamic mesh represents one grid of voxel composed on four verties.
	// This mesh is updated with each grid of candidate voxel for collision detection.
	// In order to save information of collided triangle, additional triangle is set.
	cVector3d point(CHAI_LARGE, CHAI_LARGE, CHAI_LARGE);
	for(int i=0; i<7; i++)
		m_dynamicMesh->newVertex(point);
	// one voxel --> two triangle
	m_dynamicMesh->newTriangle(0, 1, 2);
	m_dynamicMesh->newTriangle(0, 2, 3);
	// additional triangle
	m_dynamicMesh->newTriangle(4, 5, 6);

	this->addChild(m_dynamicMesh);

	m_collisionCandidateVoxelNum = 0;
	m_collisionVoxelNum = 0;

	m_useImageMaterial = true;
	m_useImageBump = true;

	m_bColorData = false;
	m_bDepthData = false;

	updateBoundaryBox();

	//////////////////////////////////////////////////
	// for first test
	m_bFirstUpdateForProxyGraph = false;

	// test
	m_scaleFactor = 2.0;


	// test
	m_focalLength = 0.47;
	m_pixelWidth = 0.00112;
	m_primaryDistance = 50.0;
	m_primaryWidth = 250.0;

	m_renderSample = 4;
}


cDepthImage::~cDepthImage()
{
	// not yet : clear the memory
	if(m_colorData)
		delete m_colorData;
	if(m_nextColorData)
		delete m_nextColorData;
	if(m_tempColorData)
		delete m_tempColorData;
	if(m_depthData)
		delete m_depthData;
	if(m_nextDepthData)
		delete m_nextDepthData;
	if(m_tempDepthData)
		delete m_tempDepthData;
}

void cDepthImage::clear()
{
	// not yet

}

void cDepthImage::scaleObject(double a_scaleFactor)
{
	// not yet
}


//===========================================================================
/*!
     Compute the axis-aligned boundary box that encloses all triangles in this mesh

     \fn       void cMesh::updateBoundaryBox()
*/
//===========================================================================
void cDepthImage::updateBoundaryBox()
{
	// not yet : scaling

	// local boundary box include the depth image
	cVector3d localBoundaryBoxMin;
	cVector3d localBoundaryBoxMax;
	
	localBoundaryBoxMin.set(1.0, 1.0, 0.0);
	localBoundaryBoxMax.set((double)m_depthImageSizeX, (double)m_depthImageSizeY-1.0, (double)(300));

	// local to global boundary box
	m_boundaryBoxMin = localBoundaryBoxMin;
	m_boundaryBoxMax = localBoundaryBoxMax;
}


//===========================================================================
/*!
     Resize the current depth image by scaling all my vertex positions

     \fn        void setScale(double a_scaleFactor)
     \param     a_scaleFactor   Scale factor.
*/
//===========================================================================
void cDepthImage::setScale(const double a_scaleFactor)
{
	m_scaleFactor = a_scaleFactor;
}


//===========================================================================
/*!
     Resize the current depth image by scaling all my vertex positions

     \fn        void getScale(double a_scaleFactor)
     \param     a_scaleFactor   Scale factor.
*/
//===========================================================================
double cDepthImage::getScale()
{
	return m_scaleFactor;
}


//===========================================================================
/*!
    Test whether a ray intersects this object. The test segment is described
    by a start point /e a_segmentPointA and end point /e a_segmentPointB.

    If a collision occurs, the squared distance between the segment start point
    and the collision point in measured and compared to any previous collision
    information stored in parameters \e a_colObject, \e a_colTriangle,
    \e a_colPoint, and \e a_colSquareDistance.
    
    If the new collision is located nearer to the ray origin than the previous
    collision point, it is stored in the corresponding parameters \e a_colObject,
    \e a_colTriangle, \e a_colPoint, and \e a_colSquareDistance.

    \param  a_segmentPointA      Start point of segment.
    \param  a_segmentPointB      End point of segment.
    \param  a_colObject          Pointer to nearest collided object.
    \param  a_colTriangle        Pointer to nearest colided triangle.
    \param  a_colPoint           Position to the nearest collision
    \param  a_colSquareDistance  Squared distance between ray origin and nearest
                                 collision point

    \param  a_proxyCall  If this is > 0, this is a call from a proxy, and the value
                         of a_proxyCall specifies which call this is.  -1 is passed
                         for non-proxy calls.
*/
//===========================================================================
bool cDepthImage::computeCollisionDetection(cVector3d& a_segmentPointA, const cVector3d& a_segmentPointB,
         cGenericObject*& a_colObject, cTriangle*& a_colTriangle, cVector3d& a_colPoint,
         double& a_colSquareDistance, const bool a_visibleObjectsOnly, const int a_proxyCall)
{
	if( (m_depthData == NULL) || (m_nextDepthData == NULL))
		return false;
	// not yet : bounding box

	// no collision found yet
    bool hit = false;
    
    // convert collision segment into local coordinate system.
    cMatrix3d transLocalRot;
    m_localRot.transr(transLocalRot);

    cVector3d localSegmentPointA = a_segmentPointA;
    localSegmentPointA.sub(m_localPos);
    transLocalRot.mul(localSegmentPointA);

    cVector3d localSegmentPointB = a_segmentPointB;
    localSegmentPointB.sub(m_localPos);
    transLocalRot.mul(localSegmentPointB);

	// cartesian to image coordinate
	//localSegmentPointA.add(m_depthImageSizeX/2.0, m_depthImageSizeY/2.0, 0.0);
	//localSegmentPointB.add(m_depthImageSizeX/2.0, m_depthImageSizeY/2.0, 0.0);
	localSegmentPointA = trans2image(localSegmentPointA);
	localSegmentPointB = trans2image(localSegmentPointB);

    // initialize objects for calls
    cGenericObject* t_colObject;
    cTriangle *t_colTriangle;
    cVector3d t_colPoint;
	double t_colSquareDistance = a_colSquareDistance;
    
	
	// conversion of the colPoint and colSquareDistance in this local coordinate
	cVector3d t_localColPoint = a_colPoint;
	t_localColPoint.sub(m_localPos);
    transLocalRot.mul(t_localColPoint);
	double a_localColSquareDistance = localSegmentPointA.distance(t_localColPoint);
	double t_localColSquareDistance = a_localColSquareDistance;


	// proxy position correction
	cVector3d correctionProxy;

	// for proxy collision detection
	if( cBoxContains(localSegmentPointA, m_boundaryBoxMin, m_boundaryBoxMax) ) {
		// When the goal is outside the depth image,
		// no collision occurs.
		if(localSegmentPointB.x < 1.0 || localSegmentPointB.y < 1.0 ||
			localSegmentPointB.x > m_depthImageSizeX-1.0 || localSegmentPointB.y > m_depthImageSizeY-1.0)
			return false;
		//

		// proxy position correction for deforming surface
		// (dynamic moving object that is deforming)
		correctionProxy = correctProxy(localSegmentPointA);
		
		lineDDA(correctionProxy.x, correctionProxy.y, correctionProxy.z, localSegmentPointB.x, localSegmentPointB.y, localSegmentPointB.z);

		if(m_collisionVoxelNum != 0) {		
			int x, y;

			int notContainedNum = 0;
			int index = 0;
			bool hit1 = false, hit2 = false;
			// convert two point segment into a segment described by a point and
			// a directional vector
			cVector3d dir;
			localSegmentPointB.subr(correctionProxy, dir);

			vector<cVertex>* dynamic_vertex = (vector<cVertex>*)m_dynamicMesh->pVertices();
			// for test
			cVertex v1 = dynamic_vertex->at(0);
			cVertex v2 = dynamic_vertex->at(1);
			cVertex v3 = dynamic_vertex->at(2);
			cVertex v4 = dynamic_vertex->at(3);

			for(int i=0; i<m_collisionVoxelNum; i++) {
				x = m_collisionVoxel[i][0];
				y = m_collisionVoxel[i][1];

				if(x < (m_depthImageSizeX-1) && x >=1 && y < (m_depthImageSizeY-2) && y >= 1) {
					index = i-notContainedNum;
					dynamic_vertex->at(0).setPos((double)x, (double)y,			getDepth(x, y));
					dynamic_vertex->at(1).setPos((double)x+1.0, (double)y,		getDepth(x+1, y));
					dynamic_vertex->at(2).setPos((double)x+1.0, (double)y+1.0,	getDepth(x+1, y+1));
					dynamic_vertex->at(3).setPos((double)x, (double)y+1.0,		getDepth(x, y+1));

					// As you can see just above, the vertex values are updated in each voxel.
					// That changes the triangle position.
					// Therefore, the collided triangle position can be updated with the vertex change.
					// To prevent this, when collision occurs, we save the collided triangle information into
					// additional triangle.
					if( hit1 = m_dynamicMesh->getTriangle(0)->computeCollision(correctionProxy,
						dir, t_colObject, t_colTriangle, t_localColPoint, t_localColSquareDistance) ) {
						dynamic_vertex->at(4).setPos(dynamic_vertex->at(0).getPos());
						dynamic_vertex->at(5).setPos(dynamic_vertex->at(1).getPos());
						dynamic_vertex->at(6).setPos(dynamic_vertex->at(2).getPos());
						hit = true;
					}
					if( hit2 = m_dynamicMesh->getTriangle(1)->computeCollision(correctionProxy,
						dir, t_colObject, t_colTriangle, t_localColPoint, t_localColSquareDistance) ) {
						dynamic_vertex->at(4).setPos(dynamic_vertex->at(0).getPos());
						dynamic_vertex->at(5).setPos(dynamic_vertex->at(2).getPos());
						dynamic_vertex->at(6).setPos(dynamic_vertex->at(3).getPos());
						hit = true;
					}
				}
				else notContainedNum++;
			}
			if(hit) {
				if (t_localColSquareDistance < a_localColSquareDistance)
				{
					a_colObject = t_colObject;

					// The collided triangle information need to be reported to caller.
					vector<cTriangle>* colTriangle = (vector<cTriangle>*)m_dynamicMesh->pTriangles();
					a_colTriangle = &colTriangle->at(2);

					// covert based on parent coordinate
					
					cVector3d globalColPoint = t_localColPoint;
					globalColPoint = trans2cartesian(globalColPoint);
					m_localRot.mul(globalColPoint);
					globalColPoint.add(m_localPos);
					
					t_colPoint = globalColPoint;
					a_colPoint = t_colPoint;
					/*
					t_colPoint = t_localColPoint;
					a_colPoint = t_colPoint;
					*/
					t_colSquareDistance = a_segmentPointA.distance(globalColPoint);
					a_colSquareDistance = t_colSquareDistance;
				}
 			}
		}
	}

	// search for collision with all child objects
	// Since the first child, dymamicMesh, is used for collision detection of this depth image,
	// it is skipped for children collision detection
    for (unsigned int i=2; i<m_children.size(); i++)
    {
        if( m_children[i]->computeCollisionDetection(localSegmentPointA, localSegmentPointB,
            t_colObject, t_colTriangle, t_colPoint, t_colSquareDistance, a_visibleObjectsOnly, a_proxyCall))
        {
            // object was hit
            hit = true;

            if (t_colSquareDistance < a_colSquareDistance)
            {
              a_colObject = t_colObject;
              a_colTriangle = t_colTriangle;
              a_colPoint = t_colPoint;
              a_colSquareDistance = t_colSquareDistance;

              // convert collision point into parent coordinate
              m_localRot.mul(a_colPoint);
              a_colPoint.add(m_localPos);
            }
        }
    }

    // return result
    return (hit);

}

cVector3d cDepthImage::updatePrevisouSurfaceDepthGlobal(cVector3d& a_proxy)
{
    cMatrix3d transLocalRot;
    m_globalRot.transr(transLocalRot);

    cVector3d localProxy = a_proxy;
    localProxy.sub(m_globalPos);
    transLocalRot.mul(localProxy);

	localProxy = trans2image(localProxy);

	if( !cBoxContains(localProxy, m_boundaryBoxMin, m_boundaryBoxMax) ) {
		cVector3d zero;
		zero.zero();
		return zero;
	}

	// to get the previous surface point projected from the previous proxy
	m_previousSurfacePoint = projectOnDepthImage(localProxy);

	return m_previousSurfacePoint;
}

cVector3d cDepthImage::updatePrevisouSurfaceDepthLocal(cVector3d& a_proxy)
{
	if( !cBoxContains(a_proxy, m_boundaryBoxMin, m_boundaryBoxMax) ) {
		cVector3d zero;
		zero.zero();
		return zero;
	}

	// to get the previous surface point projected from the previous proxy
	m_previousSurfacePoint = projectOnDepthImage(a_proxy);

	return m_previousSurfacePoint;
}

void cDepthImage::getSize(int &sizeX, int &sizeY) const
{
	sizeX = m_depthImageSizeX;
	sizeY = m_depthImageSizeY;
}

void cDepthImage::lineDDA(double x1, double y1, double z1, double x2, double y2, double z2)
{
	m_collisionCandidateVoxelNum = 0;

	int i;
	double dx = x2 - x1;
	double dy = y2 - y1;
	int sgnX = SGN(dx);
	int sgnY = SGN(dy);
	if(dx == 0.0 && dy == 0.0) {
		m_collisionCandidateVoxelNum = 1;
		m_collisionCandidateVoxel[0][0] = FLOOR(x1);
		m_collisionCandidateVoxel[0][1] = FLOOR(y1);
	}
	else if(dx == 0.0) {
		addCandidateVoxel(FLOOR(x1), FLOOR(y1), FLOOR(y2), sgnY, m_collisionCandidateVoxelNum);
	}
	else {
		double m = dy/dx;

		int stepX = SGN(dx);
		double stepY = m*(double)stepX;


		if(CEIL(x2) - FLOOR(x1) == 1) {
			addCandidateVoxel(FLOOR(x1), FLOOR(y1), FLOOR(y2), sgnY, m_collisionCandidateVoxelNum);
		}
		else {
			double y;
			int startX, endX;

			if(x2 - x1 >= 0.0) {
				y = y1 + m*((double)(CEIL(x1)) - x1);	// (CEIL(x1)) 꼭 괄호 안에 넣기
				startX = CEIL(x1);
				endX = FLOOR(x2);
			}
			else {
				y = y1 + m*((double)(FLOOR(x1)) - x1);	// (FLOOR(x1)) 꼭 괄호 안에 넣기
				startX = FLOOR(x1)-1;
				endX = CEIL(x2)-1;
			}
			addCandidateVoxel(FLOOR(x1), FLOOR(y1), FLOOR(y), sgnY, m_collisionCandidateVoxelNum);
			for(i = startX; i != endX; i+=stepX) {
				addCandidateVoxel(i, FLOOR(y), FLOOR(y+stepY), sgnY, m_collisionCandidateVoxelNum);
				y+=stepY;
			}
			addCandidateVoxel(i, FLOOR(y), FLOOR(y2), sgnY, m_collisionCandidateVoxelNum);
		}
	}

	double gridVertexDepth[4], maxDepth = 0.0, minDepth = 255.0;
	double delta, zEnter, zLeave;
	double offsetX = 0.0, offsetY = 0.0;

	if(sgnX < 0)
		offsetX = 1.0;
	if(sgnY < 0)
		offsetY = 1.0;
	m_collisionVoxelNum = 0;
	if(m_collisionCandidateVoxelNum == 1){
			int x, y;
			x = m_collisionCandidateVoxel[0][0];
			y = m_collisionCandidateVoxel[0][1];
			gridVertexDepth[0] = getDepth(x, y);
			gridVertexDepth[1] = getDepth(x+1, y);
			gridVertexDepth[2] = getDepth(x+1, y+1);
			gridVertexDepth[3] = getDepth(x, y+1);
			maxDepth = MAX(maxDepth, gridVertexDepth[0]);
			maxDepth = MAX(maxDepth, gridVertexDepth[1]);
			maxDepth = MAX(maxDepth, gridVertexDepth[2]);
			maxDepth = MAX(maxDepth, gridVertexDepth[3]);
			minDepth = MIN(minDepth, gridVertexDepth[0]);
			minDepth = MIN(minDepth, gridVertexDepth[1]);
			minDepth = MIN(minDepth, gridVertexDepth[2]);
			minDepth = MIN(minDepth, gridVertexDepth[3]);

			if(z1 >= minDepth && z2 <= maxDepth) {
				m_collisionVoxel[0][0] = m_collisionCandidateVoxel[0][0];
				m_collisionVoxel[0][1] = m_collisionCandidateVoxel[0][1];
				m_collisionVoxelNum = 1;
			}
	}
	else {
		for(int j = 0; j < m_collisionCandidateVoxelNum; j++) {
			int x, y;
			maxDepth = 0.0, minDepth = 255.0;
			x = m_collisionCandidateVoxel[j][0];
			y = m_collisionCandidateVoxel[j][1];
			gridVertexDepth[0] = getDepth(x, y);
			gridVertexDepth[1] = getDepth(x+1, y);
			gridVertexDepth[2] = getDepth(x+1, y+1);
			gridVertexDepth[3] = getDepth(x, y+1);
			maxDepth = MAX(maxDepth, gridVertexDepth[0]);
			maxDepth = MAX(maxDepth, gridVertexDepth[1]);
			maxDepth = MAX(maxDepth, gridVertexDepth[2]);
			maxDepth = MAX(maxDepth, gridVertexDepth[3]);
			minDepth = MIN(minDepth, gridVertexDepth[0]);
			minDepth = MIN(minDepth, gridVertexDepth[1]);
			minDepth = MIN(minDepth, gridVertexDepth[2]);
			minDepth = MIN(minDepth, gridVertexDepth[3]);

			if(j == 0) {
				zEnter = z1;
				if(m_bConnectivity[1]) {
					delta = fabs((double)m_collisionCandidateVoxel[1][0]+offsetX-x1);
					zLeave = z1 + delta*(z2-z1)/fabs(x2-x1);
				}
				else {
					delta = fabs((double)m_collisionCandidateVoxel[1][1]+offsetY-y1);
					zLeave = z1 + delta*(z2-z1)/fabs(y2-y1);
				}
				if(m_collisionCandidateVoxelNum == 1)
					break;
			}
			else if(j == (m_collisionCandidateVoxelNum-1)) {
				zEnter = zLeave;
				zLeave = z2;
			}
			else {
				zEnter = zLeave;
				if(m_bConnectivity[j+1]) {
					delta = fabs((double)m_collisionCandidateVoxel[j+1][0]+offsetX-x1);
					zLeave = z1 + delta*(z2-z1)/fabs(x2-x1);
				}
				else {
					delta = fabs((double)m_collisionCandidateVoxel[j+1][1]+offsetY-y1);
					zLeave = z1 + delta*(z2-z1)/fabs(y2-y1);
				}
			}
			// because of the numerical error
			// The small value difference is allowed
			if((zEnter+0.0000001) >= minDepth && (zLeave-0.0000001) <= maxDepth) {
			//if(zEnter >= minDepth && zLeave <= maxDepth) {
				m_collisionVoxel[m_collisionVoxelNum][0] = m_collisionCandidateVoxel[j][0];
				m_collisionVoxel[m_collisionVoxelNum][1] = m_collisionCandidateVoxel[j][1];
				m_collisionVoxelNum++;
			}
		}
	}

}

void cDepthImage::addCandidateVoxel(int x, int y1, int y2, int sgnY, int &num)
{
	int i;
	m_bConnectivity[num] = true;
	if(sgnY == 0) {
		m_collisionCandidateVoxel[num][0] = x;
		m_collisionCandidateVoxel[num][1] = y1;
		num++;
	}
	else {
		for(i=y1;i!=y2+sgnY;i+=sgnY) {
			m_collisionCandidateVoxel[num][0] = x;
			m_collisionCandidateVoxel[num][1] = i;
			num++;
			m_bConnectivity[num] = false;
		}
	}

}

//===========================================================================
/*!
     Set the current material for this mesh, possibly recursively affecting children

     \fn        void cMesh::setMaterial(cMaterial& a_mat,
                const bool a_affectChildren)                
     \param     a_mat The material to apply to this object
     \param     a_affectChildren  If \b true, then children are also modified.

     Note that this does not affect whether material rendering is enabled;
     it sets the maetrial that will be rendered _if_ material rendering is
     enabled.  Call useMaterial to enable / disable material rendering.
*/
//===========================================================================
void cDepthImage::setImageMaterial(cImageMaterial& a_mat, const bool a_affectChildren)
{
  cMaterial mat;
  m_imageMaterial = a_mat;
  mat.setStiffness(a_mat.getStiffness());
  mat.setStaticFriction(a_mat.getStaticFriction());
  mat.setDynamicFriction(a_mat.getDynamicFriction());

  // to embedded neighborMesh and dynamicMesh
  m_dynamicMesh->setMaterial(mat, a_affectChildren);

  // propagate changes to children
  if (a_affectChildren)
  {
      for (unsigned int i=3; i<m_children.size(); i++)
      {
          cGenericObject *nextObject = m_children[i];

          cDepthImage *nextDepthImage = dynamic_cast<cDepthImage*>(nextObject);
          if (nextDepthImage)
          {
              nextDepthImage->setImageMaterial(a_mat, a_affectChildren);
          }
      }
  }
}


//===========================================================================
/*!
     Enable or disable the use of material properties.

     \fn        void cMesh::useMaterial(const bool a_useMaterial,
                const bool a_affectChildren)
     \param     a_useMaterial If \b true, then material properties are used for rendering.
     \param     a_affectChildren  If \b true, then children are also modified.
*/
//===========================================================================
void cDepthImage::useImageMaterial(const bool a_useImageMaterial, const bool a_affectChildren)
{
    // update changes to object
    m_useImageMaterial = a_useImageMaterial;

	// update changes to embedded meshes
	m_dynamicMesh->useMaterial(a_useImageMaterial, a_affectChildren);

    // propagate changes to my children
    if (a_affectChildren)
    {
        for (unsigned int i=3; i<m_children.size(); i++)
        {
            cGenericObject *nextObject = m_children[i];

            cDepthImage *nextDepthImage = dynamic_cast<cDepthImage*>(nextObject);
            if (nextDepthImage)
            {                
                nextDepthImage->useImageMaterial(a_useImageMaterial, a_affectChildren);
            }
        }
    }
}


//===========================================================================
/*!
     Set the static and dynamic friction for this mesh, possibly recursively affecting children

     \fn        void cMesh::setFriction(double a_staticFriction, double a_dynamicFriction, const bool a_affectChildren=0)
     \param     a_staticFriction The static friction to apply to this object
     \param     a_dynamicFriction The dynamic friction to apply to this object
     \param     a_affectChildren  If \b true, then children are also modified.
     
*/
//===========================================================================
void cDepthImage::setFriction(double a_staticFriction, double a_dynamicFriction, const bool a_affectChildren)
{

  m_imageMaterial.setStaticFriction(a_staticFriction);
  m_imageMaterial.setDynamicFriction(a_dynamicFriction);

  // to embedded meshes
  m_dynamicMesh->setFriction(a_staticFriction, a_dynamicFriction, a_affectChildren);

  // propagate changes to children
  if (a_affectChildren)
  {
      for (unsigned int i=3; i<m_children.size(); i++)
      {
          cGenericObject *nextObject = m_children[i];

          cDepthImage *nextDepthImage = dynamic_cast<cDepthImage*>(nextObject);
          if (nextDepthImage)
          {
              nextDepthImage->setFriction(a_staticFriction,a_dynamicFriction,a_affectChildren);
          }
      }
  }
}


//===========================================================================
/*!
     Set the haptic stiffness for this mesh, possibly recursively affecting children

     \fn        void cMesh::setStiffness(double a_stiffness, const bool a_affectChildren=0);
     \param     a_stiffness  The stiffness to apply to this object
     \param     a_affectChildren  If \b true, then children are also modified.
     
*/
//===========================================================================
void cDepthImage::setStiffness(double a_stiffness, const bool a_affectChildren)
{

  m_imageMaterial.setStiffness(a_stiffness);

  // to embedded meshes
  m_dynamicMesh->setStiffness(a_stiffness, a_affectChildren);

  // propagate changes to children
  if (a_affectChildren)
  {
      for (unsigned int i=0; i<m_children.size(); i++)
      {
          cGenericObject *nextObject = m_children[i];

          // 2007.1.16
		  // by gaecha
		  // For crash from dynamic_cast
		  //cMesh *nextMesh = dynamic_cast<cMesh*>(nextObject);
		  cDepthImage *nextDepthImage = (cDepthImage*)(nextObject);
          if (nextDepthImage)
          {
              nextDepthImage->setStiffness(a_stiffness, a_affectChildren);
          }
      }
  }
}

cVector3d cDepthImage::projectOnDepthImage(cVector3d a_point)
{
	int i, j;
	cVector3d vertex[4];
	double X, Y, Z[4];
	i = (int)a_point.x;
	j = (int)a_point.y;
	cVector3d result;
	
	vertex[0].set((double)i, (double)j,			getDepth(i, j));
	vertex[1].set((double)i+1.0, (double)j,		getDepth(i+1, j));
	vertex[2].set((double)i+1.0, (double)j+1.0, getDepth(i+1, j+1));
	vertex[3].set((double)i, (double)j+1.0,		getDepth(i, j+1));

	X = a_point.x - (double)i;
	Y = a_point.y - (double)j;

	Z[0] = vertex[0].z;
	Z[1] = vertex[1].z - Z[0];
	Z[2] = vertex[2].z - Z[0];
	Z[3] = vertex[3].z - Z[0];

	if(Y>=X)
		result.set(a_point.x, a_point.y, Z[0] + X*Z[2] + (Y-X)*Z[3]);
	else
		result.set(a_point.x, a_point.y, Z[0] + (X-Y)*Z[1] + Y*Z[2]);

	// !!!!!!!!!!!!!!!!!!!!!
	// After computing the proper proxy, in the next haptic loop,
	// the collision detection can be failed because of the numerical error
	// In CHAI3D, they elevate the proxy by radius of the proxy in the direction of normal
	// of contacted mesh.
	// But, in that case, the static friction cannot be accomplised.
	// Anyway, here, the proxy is moved very small amount for escaping the problem from numerical error.	
	result.z += 0.1E-5;
	//
	
	return result;
}

cVector3d cDepthImage::projectOnDepthImageGlobal(cVector3d a_point)
{
    cMatrix3d transLocalRot;
    m_globalRot.transr(transLocalRot);

    cVector3d localPoint = a_point;
    localPoint.sub(m_globalPos);
    transLocalRot.mul(localPoint);

	localPoint = trans2image(localPoint);

	int i, j;
	cVector3d vertex[4];
	double X, Y, Z[4];
	i = (int)localPoint.x;
	j = (int)localPoint.y;
	cVector3d result;
	
	vertex[0].set((double)i, (double)j,			getDepth(i, j));
	vertex[1].set((double)i+1.0, (double)j,		getDepth(i+1, j));
	vertex[2].set((double)i+1.0, (double)j+1.0, getDepth(i+1, j+1));
	vertex[3].set((double)i, (double)j+1.0,		getDepth(i, j+1));

	X = localPoint.x - (double)i;
	Y = localPoint.y - (double)j;

	Z[0] = vertex[0].z;
	Z[1] = vertex[1].z - Z[0];
	Z[2] = vertex[2].z - Z[0];
	Z[3] = vertex[3].z - Z[0];

	if(Y>=X)
		result.set(localPoint.x, localPoint.y, Z[0] + X*Z[2] + (Y-X)*Z[3]);
	else
		result.set(localPoint.x, localPoint.y, Z[0] + (X-Y)*Z[1] + Y*Z[2]);
	
	// !!!!!!!!!!!!!!!!!!!!!
	// After computing the proper proxy, in the next haptic loop,
	// the collision detection can be failed because of the numerical error
	// In CHAI3D, they elevate the proxy by radius of the proxy in the direction of normal
	// of contacted mesh.
	// But, in that case, the static friction cannot be accomplised.
	// Anyway, here, the proxy is moved very small amount for escaping the problem from numerical error.	
	result.z += 0.1E-7;
	//
	
	result = trans2cartesian(result);

    cVector3d globalPoint = result;
    m_globalRot.mul(globalPoint);
    globalPoint.add(m_globalPos);

	return globalPoint;
}

void cDepthImage::setColorImageProperties(unsigned int a_sizeX, unsigned int a_sizeY,
										  unsigned char a_totalChannel, unsigned char a_channelNum)
{
	if( (m_colorImageSizeX != a_sizeX) || (m_colorImageSizeY != a_sizeY)) {
		m_colorTotalChannel = a_totalChannel;
		m_colorChannelNum = a_channelNum;
		m_colorImageSizeX = a_sizeX;
		m_colorImageSizeY = a_sizeY;
		m_halfColorImageSizeX = m_depthImageSizeX/2;
		m_halfColorImageSizeY = m_depthImageSizeY/2;

		if(m_colorData)
			free(m_colorData);
		if(m_nextColorData)
			free(m_nextColorData);
		if(m_tempColorData)
			free(m_tempColorData);
		m_colorData = new unsigned char[m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char)];
		memset(m_colorData, 0, m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char));
		m_nextColorData = new unsigned char[m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char)];
		memset(m_nextColorData, 0, m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char));
		m_tempColorData = new unsigned char[m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char)];
		memset(m_tempColorData, 0, m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char));

		m_bColorData = true;
	}
}

void cDepthImage::setDepthImageProperties(unsigned int a_sizeX, unsigned int a_sizeY,
										  unsigned char a_totalChannel, unsigned char a_channelNum)
{
	if( (m_depthImageSizeX != a_sizeX) || (m_depthImageSizeY != a_sizeY) ||
		(m_depthTotalChannel != a_totalChannel) ) {
		m_depthTotalChannel = a_totalChannel;
		m_depthChannelNum = a_channelNum;
		m_depthImageSizeX = a_sizeX;
		m_depthImageSizeY = a_sizeY;
		m_halfDepthImageSizeX = m_depthImageSizeX/2;
		m_halfDepthImageSizeY = m_depthImageSizeY/2;

		if(m_depthData)
			free(m_depthData);
		if(m_nextDepthData)
			free(m_nextDepthData);
		if(m_tempDepthData)
			free(m_tempDepthData);
		m_depthData = new unsigned char[m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char)];
		memset(m_depthData, m_boundaryBoxMax.z, m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char));
		m_nextDepthData = new unsigned char[m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char)];
		memset(m_nextDepthData, m_boundaryBoxMax.z, m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char));
		m_tempDepthData = new unsigned char[m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char)];
		memset(m_tempDepthData, m_boundaryBoxMax.z, m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char));

		m_bDepthData = true;

		updateBoundaryBox();
	}
}

void cDepthImage::setElapsedGraphicTime(float a_time)
{
	m_lElapsedGraphicTime = (long)a_time;
}

void cDepthImage::setImageData(cImageLoader *a_colorImage, cImageLoader *a_depthImage)
{
	unsigned char *colorData, *depthData;

	if(!a_depthImage)
		return;
	else
		depthData = (unsigned char *)a_depthImage->getData();

	if(!a_colorImage)
		colorData = NULL;
	else
		colorData = (unsigned char *)a_colorImage->getData();
	
	setImageData(colorData, depthData);
	return;
}

void cDepthImage::setImageData(cImageLoader *a_image)
{
	unsigned char *imageData;

	if(!a_image)
		return;
	else
		imageData = (unsigned char *)a_image->getData();

	setImageData(imageData);

	return;
}

void cDepthImage::setImageData(unsigned char * a_imageData)
{
	// update the temporal image
	if(a_imageData) {
		setColorData(a_imageData);
		setDepthData(a_imageData);
	}
}

void cDepthImage::setImageData(unsigned char * a_colorData, unsigned char * a_depthData)
{
	// update the temporal image
	if(a_colorData)
		setColorData(a_colorData);
	if(a_depthData)
		setDepthData(a_depthData);
}

void cDepthImage::setColorData(unsigned char * a_colorData)
{
	// update the temporal image
	if(a_colorData)
		memcpy(m_tempColorData, a_colorData, m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(char));
}

void cDepthImage::setDepthData(unsigned char * a_depthData)
{
	if(a_depthData == NULL) {
		m_bFirstUpdate = false;
		return;
	}

	// update the temporal image
	memcpy(m_tempDepthData, a_depthData, m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(char));

	m_bUpdate = true;
	
	if(!m_bProxyGraph) {
		updateImage();
	}
}

bool cDepthImage::updateImage()
{
	// by gaecha
	// if the proxy graph algorithm finish its loop
	// and the image is updated temporarily,
	// update the images.
	if(m_bUpdate) {
		if(m_useImageMaterial && (m_imageMaterial.m_bIsHapticData || m_imageMaterial.m_bIsBumpData) )
			m_imageMaterial.updateImage();

		if(m_bColorData) {
			memcpy(m_colorData, m_nextColorData, m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(unsigned char));
			memcpy(m_nextColorData, m_tempColorData, m_colorTotalChannel*m_colorImageSizeX*m_colorImageSizeY*sizeof(unsigned char));
		}
		if(m_bDepthData) {
			memcpy(m_depthData, m_nextDepthData, m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(unsigned char));
			memcpy(m_nextDepthData, m_tempDepthData, m_depthTotalChannel*m_depthImageSizeX*m_depthImageSizeY*sizeof(unsigned char));
		}

		m_bUpdate = false;

		return true;
	}
	else
		return false;
}

//===========================================================================
/*!
     Render this mesh in OpenGL.  This method actually just prepares some 
     OpenGL state, and uses renderDepthImage to actually do the rendering.

     \fn       void cMesh::render(const int a_renderMode)
     \param    a_renderMode  Rendering mode (see cGenericObject)
*/
//===========================================================================
void cDepthImage::render(const int a_renderMode)
{
    // if transparency is enabled, either via multipass rendering or via
    // standard alpha-blending...
    if (m_useTransparency)
    {

      // if we're using multipass transparency, render only on 
      // the front and back passes
      if (m_useMultipassTransparency)
      {

          // render transparent front triangles
          if (a_renderMode == CHAI_RENDER_MODE_TRANSPARENT_FRONT_ONLY)
          {
              glEnable(GL_CULL_FACE);
              glCullFace(GL_BACK);
              renderDepthImage(a_renderMode);
          }

          // render transparent back triangles
          else if (a_renderMode == CHAI_RENDER_MODE_TRANSPARENT_BACK_ONLY)
          {
              glEnable(GL_CULL_FACE);
              glCullFace(GL_FRONT);
              renderDepthImage(a_renderMode);
          }

          // Multipass is enabled for this object but not for the camera, so do 
          // a simple pass with transparency on...
          else if (a_renderMode == CHAI_RENDER_MODE_RENDER_ALL)
          {
              // Turn culling off for transparent objects...
              glDisable(GL_CULL_FACE);
              renderDepthImage(a_renderMode);
          }
      }

      // multipass transparency is disabled; render only on non-transparent passes
      else
      {
          if ( (a_renderMode == CHAI_RENDER_MODE_NON_TRANSPARENT_ONLY) || (a_renderMode == CHAI_RENDER_MODE_RENDER_ALL) )
          {
              // Turn culling off for transparent objects...
              glDisable(GL_CULL_FACE);
              renderDepthImage(a_renderMode);
          }
      }
    }

    // if transparency is disabled...
    else
    {
        // render only on non-transparent passes
        if ( (a_renderMode == CHAI_RENDER_MODE_NON_TRANSPARENT_ONLY) || (a_renderMode == CHAI_RENDER_MODE_RENDER_ALL) )
        {
          // render a non-transparent mesh
          if (m_cullingEnabled) 
          {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
          }
          
          else
          {
            glDisable(GL_CULL_FACE);
          }

          renderDepthImage(a_renderMode);
        }
    }

    // Restore default OpenGL state
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

cVector3d cDepthImage::trans2cartesian(cVector3d a_point)
{
	double distance;
	
	cVector3d pointOnImage((a_point.x-(double)m_halfDepthImageSizeX)*m_pixelWidth,
		(a_point.y-(double)m_halfDepthImageSizeY)*m_pixelWidth, -m_focalLength);
	cVector3d pointInSpace;

	pointOnImage.normalize();
	distance = m_primaryDistance + m_primaryWidth*( 255.0-a_point.z)/255.0;
	pointInSpace = distance*pointOnImage;
	
	return pointInSpace;
}

cVector3d cDepthImage::trans2image(cVector3d a_point)
{
	cVector3d pointOnImage;
	cVector3d pointInSpace = a_point;

	double length = pointInSpace.length();
	pointInSpace.normalize();

	pointOnImage.z = 255.0/m_primaryWidth*(m_primaryWidth + m_primaryDistance - length);
	pointOnImage.x = pointInSpace.x/pointInSpace.z*-m_focalLength/0.00112;
	pointOnImage.y = pointInSpace.y/pointInSpace.z*-m_focalLength/0.00112;

	pointOnImage.add((double)m_halfDepthImageSizeX, (double)m_halfDepthImageSizeY, 0.0);

	return pointOnImage;
}

void cDepthImage::renderDepthImage(const int a_renderMode)
{
	if(m_depthData == NULL) return;
	// not yet : scaling

	glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT, GL_FILL);

	// Mesh-based
	int D=m_renderSample;
	int baseIndex0, baseIndex1, baseIndex2, baseIndex3;
	float half_depthSizeX = (float)m_depthImageSizeX/2.0;
	float half_depthSizeY = (float)m_depthImageSizeY/2.0;

	cVector3d pointOnImage, pointInSpace;
	for(int x = 0; x < (m_depthImageSizeX-D); x+=D)
		for(int y = 0; y < (m_depthImageSizeY-D); y+=D) {
			baseIndex0 = m_depthImageSizeX*y + x;
			baseIndex1 = baseIndex0+D;
			baseIndex3 = baseIndex0+m_depthImageSizeX*D;
			baseIndex2 = baseIndex3+D;
			
			unsigned char pDepthData0 = m_depthData[m_depthTotalChannel*baseIndex0+m_depthChannelNum];
			unsigned char pDepthData1 = m_depthData[m_depthTotalChannel*baseIndex1+m_depthChannelNum];
			unsigned char pDepthData2 = m_depthData[m_depthTotalChannel*baseIndex2+m_depthChannelNum];
			unsigned char pDepthData3 = m_depthData[m_depthTotalChannel*baseIndex3+m_depthChannelNum];
			
			unsigned char * pColorData0 = &m_colorData[m_colorTotalChannel*baseIndex0+m_colorChannelNum];
			unsigned char * pColorData1 = &m_colorData[m_colorTotalChannel*baseIndex1+m_colorChannelNum];
			unsigned char * pColorData2 = &m_colorData[m_colorTotalChannel*baseIndex2+m_colorChannelNum];
			unsigned char * pColorData3 = &m_colorData[m_colorTotalChannel*baseIndex3+m_colorChannelNum];

			if( (pDepthData0 != 0) && (pDepthData1 != 0) && (pDepthData2 != 0) && (pDepthData3 != 0) ) {
				//!!!!!!!!!!!!!!!!!!!!!!
				// When drawing depth image, the depth value is subtracted by 2.0
				// I don't know why the proxy gets into the image visually not haptically.
				glBegin(GL_TRIANGLE_FAN);
					pointOnImage.set((double)x, (double)y, pDepthData0);
					pointInSpace = trans2cartesian(pointOnImage);
					glColor3ub(pColorData0[2], pColorData0[1], pColorData0[1]);
					glVertex3f( pointInSpace.x, pointInSpace.y, pointInSpace.z);

					pointOnImage.set((double)x+D, (double)y, pDepthData1);
					pointInSpace = trans2cartesian(pointOnImage);
					glColor3ub(pColorData1[2], pColorData1[1], pColorData1[1]);
					glVertex3f( pointInSpace.x, pointInSpace.y, pointInSpace.z);

					pointOnImage.set((double)x+D, (double)y+D, pDepthData2);
					pointInSpace = trans2cartesian(pointOnImage);
					glColor3ub(pColorData2[2], pColorData2[1], pColorData2[1]);
					glVertex3f( pointInSpace.x, pointInSpace.y, pointInSpace.z);

					pointOnImage.set((double)x, (double)y+D, pDepthData3);
					pointInSpace = trans2cartesian(pointOnImage);
					glColor3ub(pColorData3[2], pColorData3[1], pColorData3[1]);
					glVertex3f( pointInSpace.x, pointInSpace.y, pointInSpace.z);
				glEnd();

			}
		}

	///////////////////////////////////////////
}

cVector3d cDepthImage::computeForces(const cVector3d& a_probePosition)
{
    // compute the position of the probe in local coordinates.
    cVector3d probePositionLocal;
    probePositionLocal = cMul(cTrans(m_localRot), cSub(a_probePosition, m_localPos));

    // compute interaction forces with this object
    cVector3d localForce;
    localForce.set(0,0,0);

    // convert the reaction force into my parent coordinates
    cVector3d m_globalForce = cMul(m_localRot, localForce);

    return m_globalForce;
}

double cDepthImage::getDepth(int a_x, int a_y)
{
	double depth = 0.0;
	if(m_useSmoothing) {
		double num_count = 0.0;
		if(a_x >= (m_smoothingArea - 1) && a_x <= (m_depthImageSizeX-m_smoothingArea) &&
			a_y >= (m_smoothingArea - 1) && a_y <= (m_depthImageSizeY-m_smoothingArea)) { 
			int i, j;
			for(i=0; i<m_smoothingArea; i++)
				for(j=0; j<(i*2+1); j++) {
					num_count+=1.0;
					depth += (1.0-m_timeRatio)*(double)m_depthData[m_depthTotalChannel*(m_depthImageSizeX*(a_y-m_smoothingArea+i+1) + (a_x-i+j)) + m_depthChannelNum] + 
						m_timeRatio*(double)m_nextDepthData[m_depthTotalChannel*(m_depthImageSizeX*(a_y-m_smoothingArea+i+1) + (a_x-i+j)) + m_depthChannelNum];
				}
			for(i=0; i<m_smoothingArea-1; i++)
				for(j=0; j<(i*2+1); j++) {
					num_count+=1.0;
					depth += (1.0-m_timeRatio)*(double)m_depthData[m_depthTotalChannel*(m_depthImageSizeX*(a_y+m_smoothingArea-i-1) + (a_x-i+j)) + m_depthChannelNum] + 
						m_timeRatio*(double)m_nextDepthData[m_depthTotalChannel*(m_depthImageSizeX*(a_y+m_smoothingArea-i-1) + (a_x-i+j)) + m_depthChannelNum];
				}
			depth /= num_count;
		}
		else 
			depth = (1.0-m_timeRatio)*(double)m_depthData[m_depthTotalChannel*(m_depthImageSizeX*a_y + a_x) + m_depthChannelNum] + 
						m_timeRatio*(double)m_nextDepthData[m_depthTotalChannel*(m_depthImageSizeX*a_y + a_x) + m_depthChannelNum];
	}
	else
		depth = (1.0-m_timeRatio)*(double)m_depthData[m_depthTotalChannel*(m_depthImageSizeX*a_y + a_x) + m_depthChannelNum] + 
						m_timeRatio*(double)m_nextDepthData[m_depthTotalChannel*(m_depthImageSizeX*a_y + a_x) + m_depthChannelNum];

	if(m_imageMaterial.m_bIsBumpData && m_useImageBump) {
		depth += m_imageMaterial.getBump((double)a_x/(double)m_depthImageSizeX, (double)a_y/(double)m_depthImageSizeY);
	}

	return depth;
}


double cDepthImage::getStiffness(cVector3d a_proxy)
{
    cMatrix3d transLocalRot;
    m_globalRot.transr(transLocalRot);

    cVector3d localPoint = a_proxy;
    localPoint.sub(m_globalPos);
    transLocalRot.mul(localPoint);

	localPoint = trans2image(localPoint);

	return m_imageMaterial.getStiffness(localPoint.x/(double)m_depthImageSizeX, localPoint.y/(double)m_depthImageSizeY);
}

double cDepthImage::getStaticFriction(cVector3d a_proxy)
{
    cMatrix3d transLocalRot;
    m_globalRot.transr(transLocalRot);

    cVector3d localPoint = a_proxy;
    localPoint.sub(m_globalPos);
    transLocalRot.mul(localPoint);

	localPoint = trans2image(localPoint);

	return m_imageMaterial.getStaticFriction(localPoint.x/(double)m_depthImageSizeX, localPoint.y/(double)m_depthImageSizeY);
}

double cDepthImage::getDynamicFriction(cVector3d a_proxy)
{
    cMatrix3d transLocalRot;
    m_globalRot.transr(transLocalRot);

    cVector3d localPoint = a_proxy;
    localPoint.sub(m_globalPos);
    transLocalRot.mul(localPoint);

	localPoint = trans2image(localPoint);

	return m_imageMaterial.getDynamicFriction(localPoint.x/(double)m_depthImageSizeX, localPoint.y/(double)m_depthImageSizeY);
}

double cDepthImage::getStiffnessLocal(cVector3d a_proxy)
{
	return m_imageMaterial.getStiffness(a_proxy.x/(double)m_depthImageSizeX, a_proxy.y/(double)m_depthImageSizeY);
}

double cDepthImage::getStaticFrictionLocal(cVector3d a_proxy)
{
	return m_imageMaterial.getStaticFriction(a_proxy.x/(double)m_depthImageSizeX, a_proxy.y/(double)m_depthImageSizeY);
}

double cDepthImage::getDynamicFrictionLocal(cVector3d a_proxy)
{
	return m_imageMaterial.getDynamicFriction(a_proxy.x/(double)m_depthImageSizeX, a_proxy.y/(double)m_depthImageSizeY);
}

double cDepthImage::getBump(int a_x, int a_y)
{
	return m_imageMaterial.getDynamicFriction(a_x, a_y);
}

void cDepthImage::useImageBump(const bool a_useImageBump, const bool a_affectChildren)
{
    // update changes to object
    m_useImageBump = a_useImageBump;

    // propagate changes to my children
    if (a_affectChildren)
    {
        for (unsigned int i=3; i<m_children.size(); i++)
        {
            cGenericObject *nextObject = m_children[i];

            cDepthImage *nextDepthImage = dynamic_cast<cDepthImage*>(nextObject);
            if (nextDepthImage)
            {                
                nextDepthImage->useImageMaterial(a_useImageBump, a_affectChildren);
            }
        }
    }
}

void cDepthImage::useSmoothing(const bool a_useSmoothing)
{
	m_useSmoothing = a_useSmoothing;
}

cVector3d cDepthImage::correctProxy(const cVector3d a_proxy)
{
	cVector3d correctionProxy = a_proxy;

	cVector3d currentSurfacePoint = projectOnDepthImage(a_proxy);
	if((currentSurfacePoint.z > m_previousSurfacePoint.z)
		&& (a_proxy.z >= (m_previousSurfacePoint.z))
		&& (a_proxy.z <= currentSurfacePoint.z)
	){
		correctionProxy.z = currentSurfacePoint.z;
	}
	return correctionProxy;
}

bool cDepthImage::correctProxyOnImage(cVector3d& a_proxy)
{
	cVector3d currentSurfacePoint = projectOnDepthImage(a_proxy);
	if(currentSurfacePoint.z >= m_previousSurfacePoint.z || fabs(currentSurfacePoint.z - m_previousSurfacePoint.z) < CHAI_SMALL ) {
		a_proxy.z = currentSurfacePoint.z;
		return true;
	}
	else
		return false;
}

void cDepthImage::setCameraParameters(double a_focalLength, double a_pixelWidth, 
									  double a_primaryDistance, double a_primaryWidth)
{
	m_focalLength = a_focalLength;
	m_pixelWidth = a_pixelWidth;
	m_primaryDistance = a_primaryDistance;
	m_primaryWidth = a_primaryWidth;
}

