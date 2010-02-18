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
    \author:    Chris Sewell
    \author:    Dan Morris
    \version    1.2
    \date       03/2005
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "CProxyGraphPointForceAlgo.h"
//---------------------------------------------------------------------------
#include "CMesh.h"
#include "CWorld.h"
#include <float.h>

// by gaecha
#include "cDepthImage.h"
//

// performance test
/*
#define NUM_SAV 25000
#define START_TIME 5000
FILE *g_file;
long g_elapsedTime[NUM_SAV];
int g_count = 0;
long g_begin = 0, g_end = 0;

FILE *g_file2;
long g_numLoop[2][NUM_SAV];
long g_outerLoop, g_innerLoop;
cVector3d g_localProxy, g_localGoal, g_localForce;
cVector3d g_globalProxy, g_globalGoal, g_globalForce;
cVector3d g_savLocalProxy[NUM_SAV], g_savLocalGoal[NUM_SAV], g_savLocalForce[NUM_SAV];
cVector3d g_savGlobalProxy[NUM_SAV], g_savGlobalGoal[NUM_SAV], g_savGlobalForce[NUM_SAV];
*/
// performance test
cVector3d g_localProxy, g_localGoal, g_localForce;
cVector3d g_globalProxy, g_globalGoal, g_globalForce;


//---------------------------------------------------------------------------

// Controls whether the proxy searches invisible objects
//
// If this constant is zero, the proxy will use visible _and_
// invisible objects.
#define CHAI_PROXY_ONLY_USES_VISIBLE_OBJECTS 0

// Using the Melder et al friction model, it becomes useful to decouple the
// _angle_ used for dynamic friction from the actual friction coefficient.
//
// In CHAI's proxy, the angle computed from the coefficient is multiplied
// by this constant to avoid rapidly oscillating between slipping and sticking
// without having to turn the dynamic friction level way down.
#define DYNAMIC_FRICTION_HYSTERESIS_MULTIPLIER 0.6

bool cCollideLine(cVector3d begin0, cVector3d end0, cVector3d begin1, cVector3d end1, double& mul)
{
	double denom = ((end1.y - begin1.y)*(end0.x- begin0.x)) -
				  ((end1.x - begin1.x)*(end0.y - begin0.y));

	double nume_a = ((end1.x - begin1.x)*(begin0.y - begin1.y)) -
				   ((end1.y - begin1.y)*(begin0.x - begin1.x));

	double nume_b = ((end0.x- begin0.x)*(begin0.y - begin1.y)) -
				   ((end0.y - begin0.y)*(begin0.x - begin1.x));

	if(fabs(denom) <= CHAI_SMALL)
	{
		if(nume_a == 0.0 && nume_b == 0.0)
		{
			return false;//COINCIDENT
		}
		return false;//PARALLEL
	}

	double ua = nume_a / denom;
	double ub = nume_b / denom;

	//if(ua >= -CHAI_SMALL && (ua <= 1.0  + CHAI_SMALL)&& ub >= -CHAI_SMALL && (ub <= 1.0 + CHAI_SMALL))
	//if(ua >= -1.0E-2 && ua <= 1.0 && ub >= -1.0E-2 && ub <= 1.0 + (1.0E-2))
	if(ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0)
	{
		mul = ub;
		return true;//INTERESECTING
	}

	return false;//NOT_INTERESECTING
}


// by gaecha
// primitives implementation
// primitiveTriangle
Primitive::Primitive(cVector3d a_goal, cVector3d a_proxy, cDepthImage* a_depthImage)
{
	m_depthImage = a_depthImage;
	m_goal = a_goal;
	m_proxy = a_proxy;

	m_selectedIndex = -1;
}

cVector3d Primitive::getProxy()
{
	cVector3d result = m_proxy;

	//result.sub(m_depthImage->m_imageSizeX/2.0, m_depthImage->m_imageSizeY/2.0, 0.0);
	result = m_depthImage->trans2cartesian(result);

	m_depthImage->getGlobalRot().mul(result);
	result.add(m_depthImage->getGlobalPos());

	return result;
}


// !!!!!!!
// Think more. This looks wrong
cVector3d Primitive::getNormal()
{
	cVector3d result = m_normal;

	// the normal of point is 0.0, 0.0, 0.0
	if(m_normal.length() == 0.0)
		return result;

	cVector3d zero;
	zero.zero();

	result = m_depthImage->trans2cartesian(result);
	zero = m_depthImage->trans2cartesian(zero);
	result.sub(zero);

	result.normalize();

	m_depthImage->getGlobalRot().mul(result);

	return result;
}

cVector3d Primitive::computeShadedNorm(cVector3d a_proxy)
{
	cVector3d result = m_shadedNorm;

	cVector3d zero;
	zero.zero();

	result = m_depthImage->trans2cartesian(result);
	zero = m_depthImage->trans2cartesian(zero);
	result.sub(zero);

	result.normalize();

	m_depthImage->getGlobalRot().mul(result);

	return result;
}

PrimitiveTriangle::PrimitiveTriangle(cVector3d a_goal, cVector3d a_proxy, cDepthImage* a_depthImage, cVector3d a_vertex0, cVector3d a_vertex1, cVector3d a_vertex2)
 : Primitive(a_goal, a_proxy, a_depthImage)
{
	m_num_neighbor = 3;

	m_vertices[0] = a_vertex0;
	m_vertices[1] = a_vertex1;
	m_vertices[2] = a_vertex2;
	m_vertices[3] = m_vertices[0];

	cVector3d e1 = m_vertices[1] - m_vertices[0];
	cVector3d e2 = m_vertices[2] - m_vertices[0];
	m_normal = cCross(e1, e2);
	m_normal.normalize();

	m_type = PRIMITIVE_TRIANGLE;

	for(int i=0; i<m_num_neighbor; i++)
		m_neighborPrimitives[i] = NULL;
}

PrimitiveTriangle::~PrimitiveTriangle()
{
	for(int i=0; i<m_num_neighbor; i++)
		if(i != m_selectedIndex)
			delete m_neighborPrimitives[i];
}

void PrimitiveTriangle::refreshDepth()
{
	m_vertices[0].z = m_depthImage->getDepth( int(m_vertices[0].x), int(m_vertices[0].y) );
	m_vertices[1].z = m_depthImage->getDepth( int(m_vertices[1].x), int(m_vertices[1].y) );
	m_vertices[2].z = m_depthImage->getDepth( int(m_vertices[2].x), int(m_vertices[2].y) );
	m_vertices[3].z = m_vertices[0].z;
		
	cVector3d e1 = m_vertices[1] - m_vertices[0];
	cVector3d e2 = m_vertices[2] - m_vertices[0];
	m_normal = cCross(e1, e2);
	m_normal.normalize();
}

void PrimitiveTriangle::setNeighbors()
{
	m_neighborPrimitives[0] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1]);
	m_neighborPrimitives[1] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[1], m_vertices[2]);
	m_neighborPrimitives[2] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[2], m_vertices[0]);
}

void PrimitiveTriangle::releaseNeighbors()
{
	for(int i=0; i<m_num_neighbor; i++)
		delete m_neighborPrimitives[i];
}

bool PrimitiveTriangle::computeNextPrimitive(Primitive*& a_nextPrimitive)
{
	bool bNext = false;
	PrimitiveLine* neighborLines[3];
	cVector3d goal2proxy = m_proxy-m_goal;

	cVector3d candidateProxy = m_goal + m_normal*(m_normal.dot(goal2proxy));

	double mul = 0.0;
	for(int i=0; i<m_num_neighbor; i++) {
		
		neighborLines[i] = (PrimitiveLine*)m_neighborPrimitives[i];
		neighborLines[i]->setNeighbors();
		cVector3d E1 = neighborLines[i]->m_vertices[0];
		cVector3d E2 = neighborLines[i]->m_vertices[1];
		cVector3d e1 = neighborLines[i]->m_endPlaneNorm[0];
		cVector3d e2 = neighborLines[i]->m_endPlaneNorm[1];
		cVector3d m = neighborLines[i]->m_voronoiFaceNorm[0];
		// this is very critical point.
		// Because of numerical error, sometimes, cCollideLine doesn't detect collision.
		if( (m.dot(candidateProxy-E1) < 0.0) && cCollideLine(m_proxy, candidateProxy, E1, E2, mul)) {
		//if(cCollideLine(m_proxy, candidateProxy, E1, E2, mul)) {
			neighborLines[i]->m_proxy = E1+mul*(E2-E1);
			bNext = true;
			m_selectedIndex = i;
			break;
		}
		cVector3d koko = m_proxy - E1;
		double kaka = m.dot(koko);
		if(kaka < -CHAI_SMALL)
			int puhaha = 0;
	}
	
	if(bNext)
		a_nextPrimitive = m_neighborPrimitives[m_selectedIndex];
	else {
		m_proxy = candidateProxy;
	}

	return bNext;
}

bool PrimitiveTriangle::neighborCollisionDetection()
{
	double minX, maxX, minY, maxY;
	minX = min(m_vertices[0].x, m_vertices[1].x);
	minX = min(minX, m_vertices[2].x);
	minY = min(m_vertices[0].y, m_vertices[1].y);
	minY = min(minY, m_vertices[2].y);
	maxX = max(m_vertices[0].x, m_vertices[1].x);
	maxX = max(maxX, m_vertices[2].x);
	maxY = max(m_vertices[0].y, m_vertices[1].y);
	maxY = max(maxY, m_vertices[2].y);

	if(minX < 2.0 || maxX > (m_depthImage->m_depthImageSizeX-2.0) || 
		minY < 2.0 || maxY > (m_depthImage->m_depthImageSizeY-2.0))
		return false;

	cVector3d edge1, edge2, normal, proxy2goal;

	proxy2goal = m_goal - m_proxy;
	proxy2goal.normalize();

	if(cDot(proxy2goal, m_normal) <= 0.0)
		return true;
	else
		return false;
}

cVector3d PrimitiveTriangle::computeShadedNorm(cVector3d a_proxy)
{
	int i;
	cVector3d proxy;
	PrimitivePoint* primitivePoints[3];
	cVector3d triNorm[3];
	for(i=0; i<3; i++) {
		primitivePoints[i] = new PrimitivePoint(m_goal, m_proxy, m_depthImage, m_vertices[i]);
		triNorm[i] = primitivePoints[i]->computeShadedNorm(a_proxy);
	}
	double area[3];
	
	for(i=0; i<3; i++)
		area[i] = fabs((m_vertices[i].x*a_proxy.y-a_proxy.x*m_vertices[i].y)+(m_vertices[i+1].x*m_vertices[i].y-m_vertices[i].x*m_vertices[i+1].y)+(a_proxy.x*m_vertices[i+1].y-m_vertices[i+1].x*a_proxy.y))/2;
	m_shadedNorm = (area[1]*triNorm[0] + area[2]*triNorm[1] + area[0]*triNorm[2])/(area[0]+area[1]+area[2]);

	cVector3d result = m_shadedNorm;
	m_depthImage->getGlobalRot().mul(result);

	for(i=0; i<3; i++)
		delete primitivePoints[i];

	return result;
}

// primitiveLine
PrimitiveLine::PrimitiveLine(cVector3d a_goal, cVector3d a_proxy, cDepthImage* a_depthImage, cVector3d a_vertex0, cVector3d a_vertex1)
: Primitive(a_goal, a_proxy, a_depthImage)
{
	m_num_neighbor = 4;

	m_vertices[0] = a_vertex0;
	m_vertices[1] = a_vertex1;

	double x, y, z;
	if(m_vertices[0].y == m_vertices[1].y) {
		x = min( m_vertices[0].x, m_vertices[1].x );
		y = m_vertices[0].y - 1.0;
		z = m_depthImage->getDepth( (int)x, int(y) );
		m_vertices[2].set(x, y, z);
		x = x + 1.0;
		y = m_vertices[0].y + 1.0;
		z = m_depthImage->getDepth( (int)x, int(y) );
		m_vertices[3].set(x, y, z);
	}
	else if(m_vertices[0].x == m_vertices[1].x) {
		x = m_vertices[0].x + 1.0;
		y = max( m_vertices[0].y, m_vertices[1].y );
		z = m_depthImage->getDepth( (int)x, int(y) );
		m_vertices[2].set(x, y, z);
		x = m_vertices[0].x - 1.0;
		y = y - 1.0;
		z = m_depthImage->getDepth( (int)x, int(y) );
		m_vertices[3].set(x, y, z);
	}
	else {
		x = max( m_vertices[0].x, m_vertices[1].x );
		y = min( m_vertices[0].y, m_vertices[1].y );
		z = m_depthImage->getDepth( (int)x, int(y) );
		m_vertices[2].set(x, y, z);
		x = x - 1.0;
		y = y + 1.0;
		z = m_depthImage->getDepth( (int)x, int(y) );
		m_vertices[3].set(x, y, z);
	}

	m_endPlaneNorm[0] = m_vertices[1] - m_vertices[0];
	m_endPlaneNorm[0].normalize();
	m_endPlaneNorm[1] = (-1)*m_endPlaneNorm[0];
	cVector3d onLine;
	onLine = cProjectPointOnLine(m_goal, m_vertices[0], m_endPlaneNorm[0]);
	m_normal = onLine-m_goal;
	m_normal.normalize();

	m_shadedNorm = m_normal;
	
	m_type = PRIMITIVE_LINE;

	for(int i=0; i<m_num_neighbor; i++)
		m_neighborPrimitives[i] = NULL;
}

PrimitiveLine::~PrimitiveLine()
{
	for(int i=0; i<m_num_neighbor; i++)
		if(i != m_selectedIndex)
			delete m_neighborPrimitives[i];
}

void PrimitiveLine::refreshDepth()
{
	m_vertices[0].z = m_depthImage->getDepth( int(m_vertices[0].x), int(m_vertices[0].y) );
	m_vertices[1].z = m_depthImage->getDepth( int(m_vertices[1].x), int(m_vertices[1].y) );
	m_vertices[2].z = m_depthImage->getDepth( int(m_vertices[2].x), int(m_vertices[2].y) );
	m_vertices[3].z = m_depthImage->getDepth( int(m_vertices[3].x), int(m_vertices[3].y) );

	m_endPlaneNorm[0] = m_vertices[1] - m_vertices[0];
	m_endPlaneNorm[0].normalize();
	m_endPlaneNorm[1] = (-1)*m_endPlaneNorm[0];
	cVector3d onLine;
	onLine = cProjectPointOnLine(m_goal, m_vertices[0], m_endPlaneNorm[0]);
	m_normal = onLine-m_goal;
	m_normal.normalize();

	m_shadedNorm = m_normal;
}

bool PrimitiveLine::computeNextPrimitive(Primitive*& a_nextPrimitive)
{
	bool bNext = false;
	PrimitiveTriangle* neighborTriangles[2];
	PrimitivePoint* neighborPoints;
	
	cVector3d proxy2goal = m_goal - m_proxy;
	cVector3d norm_proxy2goal = proxy2goal;
	norm_proxy2goal.normalize();
	cVector3d candidateProxy;
	double grad, minGrad = CHAI_LARGE;

	cVector3d m, t, E;
	for(int i=0; i<m_num_neighbor-2; i++) {
		neighborTriangles[i] = (PrimitiveTriangle*) m_neighborPrimitives[i];
		neighborTriangles[i]->setNeighbors();
		m = m_voronoiFaceNorm[i];
		grad = -proxy2goal.dot(m);
		if(grad < minGrad) {
			minGrad = grad;
			m_selectedIndex = i;
		}
	}
	if(minGrad < 0.0) {
		bNext = true;
		neighborTriangles[m_selectedIndex]->m_proxy = m_proxy;
		a_nextPrimitive = m_neighborPrimitives[m_selectedIndex];
	}
	else {
		m_selectedIndex = -1;

		if(m_endPlaneNorm[0].dot(proxy2goal) != 0) {
			if(m_endPlaneNorm[0].dot(proxy2goal) > 0) {
				t = m_endPlaneNorm[0];
				E = m_vertices[1];
				m_selectedIndex = 3;
			}
			else {
				t = m_endPlaneNorm[1];
				E = m_vertices[0];
				m_selectedIndex = 2;
			}
			candidateProxy = m_proxy + t*(t.dot(proxy2goal));
			if( m_proxy.distance(candidateProxy) > m_proxy.distance(E) ) {
				neighborPoints = (PrimitivePoint*) m_neighborPrimitives[m_selectedIndex];
				neighborPoints->setNeighbors();
				neighborPoints->m_proxy = E;
				bNext = true;
				a_nextPrimitive = m_neighborPrimitives[m_selectedIndex];
			}
			else {
				bNext = false;
				m_selectedIndex = -1;
				m_proxy = candidateProxy;
			}
		}
	}
	return bNext;
}

void PrimitiveLine::setNeighbors()
{
	if(m_vertices[0].y == m_vertices[1].y) {
		if(m_vertices[0].x < m_vertices[1].x) {
			m_neighborPrimitives[0] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1], m_vertices[3]);
			m_neighborPrimitives[1] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[2], m_vertices[1]);
		}
		else {
			m_neighborPrimitives[0] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1], m_vertices[2]);
			m_neighborPrimitives[1] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[3], m_vertices[1]);
		}
	}
	else if(m_vertices[0].x == m_vertices[1].x) {
		if(m_vertices[0].y < m_vertices[1].y) {
			m_neighborPrimitives[0] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1], m_vertices[3]);
			m_neighborPrimitives[1] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[2], m_vertices[1]);
		}
		else {
			m_neighborPrimitives[0] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1], m_vertices[2]);
			m_neighborPrimitives[1] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[3], m_vertices[1]);
		}
	}
	else {
		if(m_vertices[0].x < m_vertices[1].x) {
			m_neighborPrimitives[0] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1], m_vertices[3]);
			m_neighborPrimitives[1] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[2], m_vertices[1]);
		}
		else {
			m_neighborPrimitives[0] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1], m_vertices[2]);
			m_neighborPrimitives[1] = new PrimitiveTriangle(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[3], m_vertices[1]);
		}
	}

	PrimitiveTriangle* tri0 = (PrimitiveTriangle*)m_neighborPrimitives[0];
	PrimitiveTriangle* tri1 = (PrimitiveTriangle*)m_neighborPrimitives[1];
	m_voronoiFaceNorm[0] = cCross(tri0->m_normal, m_endPlaneNorm[0]);
	m_voronoiFaceNorm[1] = cCross(tri1->m_normal, m_endPlaneNorm[1]);

	m_neighborPrimitives[2] = new PrimitivePoint(m_goal, m_proxy, m_depthImage, m_vertices[0]);
	m_neighborPrimitives[3] = new PrimitivePoint(m_goal, m_proxy, m_depthImage, m_vertices[1]);
}

void PrimitiveLine::releaseNeighbors()
{
	for(int i=0; i<m_num_neighbor; i++)
		delete m_neighborPrimitives[i];
}

// primitivePoint
PrimitivePoint::PrimitivePoint(cVector3d a_goal, cVector3d a_proxy, cDepthImage* a_depthImage, cVector3d a_vertex)
: Primitive(a_goal, a_proxy, a_depthImage)
{
	m_num_neighbor = 6;

	m_vertices[0] = a_vertex;
	m_vertices[1].set(m_vertices[0].x-1.0, m_vertices[0].y-1.0, 
		m_depthImage->getDepth((int)m_vertices[0].x-1, (int)m_vertices[0].y-1));
	m_vertices[2].set(m_vertices[0].x, m_vertices[0].y-1.0, 
		m_depthImage->getDepth((int)m_vertices[0].x, (int)m_vertices[0].y-1));
	m_vertices[3].set(m_vertices[0].x+1.0, m_vertices[0].y, 
		m_depthImage->getDepth((int)m_vertices[0].x+1, (int)m_vertices[0].y));
	m_vertices[4].set(m_vertices[0].x+1.0, m_vertices[0].y+1.0, 
		m_depthImage->getDepth((int)m_vertices[0].x+1, (int)m_vertices[0].y+1));
	m_vertices[5].set(m_vertices[0].x, m_vertices[0].y+1.0, 
		m_depthImage->getDepth((int)m_vertices[0].x, (int)m_vertices[0].y+1));
	m_vertices[6].set(m_vertices[0].x-1.0, m_vertices[0].y, 
		m_depthImage->getDepth((int)m_vertices[0].x-1, (int)m_vertices[0].y));
	m_vertices[7] = m_vertices[1];

	m_normal.zero();

	m_type = PRIMITIVE_POINT;

	for(int i=0; i<m_num_neighbor; i++)
		m_neighborPrimitives[i] = NULL;
}

void PrimitivePoint::refreshDepth()
{
	m_vertices[0].z = m_depthImage->getDepth( int(m_vertices[0].x), int(m_vertices[0].y) );
	m_vertices[1].z = m_depthImage->getDepth( int(m_vertices[1].x), int(m_vertices[1].y) );
	m_vertices[2].z = m_depthImage->getDepth( int(m_vertices[2].x), int(m_vertices[2].y) );
	m_vertices[3].z = m_depthImage->getDepth( int(m_vertices[3].x), int(m_vertices[3].y) );
	m_vertices[4].z = m_depthImage->getDepth( int(m_vertices[4].x), int(m_vertices[4].y) );
	m_vertices[5].z = m_depthImage->getDepth( int(m_vertices[5].x), int(m_vertices[5].y) );
	m_vertices[6].z = m_depthImage->getDepth( int(m_vertices[6].x), int(m_vertices[6].y) );
	m_vertices[7].z = m_vertices[1].z;
}

PrimitivePoint::~PrimitivePoint()
{
	for(int i=0; i<m_num_neighbor; i++)
		if(i != m_selectedIndex)
			delete m_neighborPrimitives[i];
}

void PrimitivePoint::setNeighbors()
{
	m_neighborPrimitives[0] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[1]);
	m_neighborPrimitives[1] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[2]);
	m_neighborPrimitives[2] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[3]);
	m_neighborPrimitives[3] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[4]);
	m_neighborPrimitives[4] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[5]);
	m_neighborPrimitives[5] = new PrimitiveLine(m_goal, m_proxy, m_depthImage, m_vertices[0], m_vertices[6]);
}

void PrimitivePoint::releaseNeighbors()
{
	for(int i=0; i<m_num_neighbor; i++)
		delete m_neighborPrimitives[i];
}

bool PrimitivePoint::computeNextPrimitive(Primitive*& a_nextPrimitive)
{
	bool bNext = false;
	double grad, minGrad = CHAI_LARGE;
	cVector3d outVector;
	PrimitiveLine* primitiveLine;

	cVector3d goal2proxy = m_proxy - m_goal;
	cVector3d norm_goal2proxy = goal2proxy;
	norm_goal2proxy.normalize();

	for(int i=0; i<m_num_neighbor; i++) {
		primitiveLine = (PrimitiveLine*) m_neighborPrimitives[i];
		primitiveLine->setNeighbors();
		outVector = primitiveLine->m_endPlaneNorm[0];
		outVector.normalize();
		grad = outVector.dot(norm_goal2proxy);
		if(grad < minGrad) {
			minGrad = grad;
			m_selectedIndex = i;
		}
	}
	if(minGrad < 0.0) {
		m_neighborPrimitives[m_selectedIndex]->m_proxy = m_vertices[0];
		bNext = true;
		a_nextPrimitive = m_neighborPrimitives[m_selectedIndex];
	}
	else {
		bNext = false;
		m_proxy = m_vertices[0];
		m_selectedIndex = -1;
	}
	return bNext;
}

cVector3d PrimitivePoint::computeShadedNorm(cVector3d a_proxy)
{
	cVector3d E1, E2, tempNorm;
	m_shadedNorm.zero();
	for(int i=0; i<6; i++) {
		E1 = m_vertices[i+1] - m_vertices[0];
		E2 = m_vertices[i+2] - m_vertices[0];
		tempNorm = cCross(E1, E2);
		tempNorm.normalize();
		m_shadedNorm += tempNorm;
	}
	m_shadedNorm.normalize();

	cVector3d result = m_shadedNorm;
	m_depthImage->getGlobalRot().mul(result);

	return result;
}

//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of cProxyGraphPointForceAlgo.

    \fn       cProxyGraphPointForceAlgo::cProxyGraphPointForceAlgo()
*/
//===========================================================================
cProxyGraphPointForceAlgo::cProxyGraphPointForceAlgo()
{
    // initialize world pointer
    m_world = NULL;

    // no contacts yet between proxy and environment
    m_numContacts = 0;

    // initialize device and proxy positions
    m_deviceGlobalPos.zero();
    m_proxyGlobalPos.zero();
    m_lastGlobalForce.zero();

    // set default colors
    m_colorProxy.set(1.0f, 0.7f, 0.0, 1.0f);
    m_colorLine.set(0.5f, 0.5f, 0.5f);

    // this will generally be over-written by the calling pointer
    m_radius = 0.1f;

    // by default, we do not use dynamic proxy (which handles moving objects)
    m_dynamicProxy = false;

    // initialize dynamic proxy members
    m_lastObjectGlobalPos.set(0,0,0);
    m_lastObjectGlobalRot.identity();
    m_touchingObject = 0;
    m_numContacts = 0;
    m_movingObject = 0;

    m_slipping = true;
    m_useFriction = true;
    m_useZillesFriction = false;
    m_useMelderFriction = true;


	// by gaecha
	// first time to execute?
	m_bFirst = false;
	m_oldGoal.zero();

	m_numDepthImage = 0;

	//
	m_clock = new cPrecisionClock();

	m_bDepthImageFirstUpdate = false;

	m_shadedProxy.zero();

	m_useForceShading = true;

	m_bLastCollision = false;
	m_lastPrimitive = NULL;

	// performance test
	/*
	g_file = fopen("elapsedTime.txt", "w");
	g_file2 = fopen("loop.txt", "w");
	*/
	// performance test
}

cProxyGraphPointForceAlgo::~cProxyGraphPointForceAlgo() {
	// performance test
	/*
	for(int i=0; i<NUM_SAV; i++)
		fprintf(g_file, "%d\n", g_elapsedTime[i]);
	fclose(g_file);
	for(i=0; i<NUM_SAV; i++) {
		fprintf(g_file2, "%d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", g_numLoop[0][i], g_numLoop[1][i],
		g_savLocalProxy[i].x, g_savLocalProxy[i].y,g_savLocalProxy[i].z,
		g_savLocalGoal[i].x, g_savLocalGoal[i].y, g_savLocalGoal[i].z,
		g_savLocalForce[i].x, g_savLocalForce[i].y, g_savLocalForce[i].z,
		g_savGlobalProxy[i].x, g_savGlobalProxy[i].y,g_savGlobalProxy[i].z,
		g_savGlobalGoal[i].x, g_savGlobalGoal[i].y,g_savGlobalGoal[i].z,
		g_savGlobalForce[i].x, g_savGlobalForce[i].y,g_savGlobalForce[i].z);
	}
	fclose(g_file2);
	*/
	// performance test
}

//===========================================================================
/*!
    Initialize the algorithm, including setting the pointer to the world
    in which the algorithm is to operate, and setting the initial position
    of the device.

    \fn       void cProxyGraphPointForceAlgo::initialize(cWorld* a_world,
              const cVector3d& a_initialPos)
    \param    a_world  Pointer to world in which force algorithm is operating.
    \param    a_initialPos  Initial position of the device.
*/
//===========================================================================
void cProxyGraphPointForceAlgo::initialize(cWorld* a_world, const cVector3d& a_initialPos)
{
    // no contacts yet between proxy and environment
    m_numContacts = 0;

    m_slipping = 1;

    // initialize device and proxy positions
    m_deviceGlobalPos = a_initialPos;
    m_proxyGlobalPos = a_initialPos;
    m_lastGlobalForce.zero();

    // set pointer to world in which force algorithm operates
    m_world = a_world;


	// by gaecha
	m_previousProxyGlobalPos = a_initialPos;
	// depth image가 여러장일경우를 처리해 보라구 했는데 아직 안함.
    for (unsigned int i=0; i<m_world->getNumChildren(); i++) {
		cGenericObject * child = m_world->getChild(i);
		if(child->m_tag == DEPTH_IMAGE_TAG){
			m_depthImage = (cDepthImage*)child;
			m_depthImage->m_bProxyGraph = true;
			m_numDepthImage++;
		}
    }

	// initialize clock
	m_clock->initialize();
	m_startTime = 0;
	m_clock->start();
}


//===========================================================================
/*!
    This method computes the force to add to the device due to any collisions
    with meshes by calling computeNextBestProxyPosition() to update the
    proxy location and then computeForce() to calculate a force vector based
    on the proxy location.

    \fn       cVector3d cProxyGraphPointForceAlgo::computeForces(
              const cVector3d& a_nextDevicePos)
    \param    a_nextDevicePos  Current position of the device.
    \return   Return the force to add to the device due to any collisions
              with meshes.
*/
//===========================================================================
cVector3d cProxyGraphPointForceAlgo::computeForces(const cVector3d& a_nextDevicePos)
{
    // check if world has been defined; if so, compute forces
    if (m_world != NULL)
    {
      // update position of device
      m_deviceGlobalPos =  a_nextDevicePos;

      // if dynamic proxy is enabled, account for object motion
      if (m_dynamicProxy) correctProxyForObjectMotion();

	  // performance test
	  //g_begin = m_clock->getCurrentTime();
	  // performance test

	  // by gaecha
	  // The proxy graph algorithm loop is finished.
	  // update the depth image
  	  if(m_numDepthImage != 0) {
		  m_depthImage->m_lElapsedHapticTime = m_clock->getCurrentTime() - m_startTime;
		  m_depthImage->m_timeRatio = (double)m_depthImage->m_lElapsedHapticTime/(double)m_depthImage->m_lElapsedGraphicTime;
		  if( m_depthImage->m_timeRatio >= 1.0)
			  m_depthImage->m_timeRatio=1.0;
		  if(m_depthImage->updateImage()) {
			  m_startTime = m_clock->getCurrentTime();
			  m_bDepthImageFirstUpdate = true;
		  }
	  }

      // compute next best position of proxy
      computeNextBestProxyPosition(m_deviceGlobalPos);

      // update proxy to next best position
      m_proxyGlobalPos = m_nextBestProxyGlobalPos;


	  // by gaecha
	  // sudden force increase problem
	  //if(m_proxyGlobalPos.distance(m_previousProxyGlobalPos) > 30.0)
		//  m_proxyGlobalPos = m_deviceGlobalPos;

	  // by gaecha
	  // update the previous surface depth
	  if(m_numDepthImage > 0)
		  if(m_depthImage)
			  m_depthImage->updatePrevisouSurfaceDepthGlobal( m_proxyGlobalPos );


      // compute force vector applied to device
      computeForce();

	  // performance test
	  /*
	  g_end = m_clock->getCurrentTime();
	  if(g_count >= START_TIME && g_count < (START_TIME + NUM_SAV) ) {
		g_elapsedTime[g_count-START_TIME] = g_end-g_begin;
		g_numLoop[0][g_count-START_TIME] = g_outerLoop;
		g_numLoop[1][g_count-START_TIME] = g_innerLoop;
		g_savLocalProxy[g_count-START_TIME] = g_localProxy;
		g_savLocalGoal[g_count-START_TIME] = g_localGoal;
		g_savLocalForce[g_count-START_TIME] = g_localForce;
		g_savGlobalProxy[g_count-START_TIME] = g_globalProxy;
		g_savGlobalGoal[g_count-START_TIME] = g_globalGoal;
		g_savGlobalForce[g_count-START_TIME] = g_globalForce;
	  }
	  g_count++;
	  */
	  // performance test

      // Update "last-state" dynamic contact information
      //updateDynamicContactState();


	  //((cMesh*)m_touchingObject)->computeGlobalCurrentObjectOnly(true);
      if(m_touchingObject) {
		  if(m_touchingObject->m_tag != DEPTH_IMAGE_TAG)
			  updateDynamicContactState();
	  }

	  //////////////////////////////////////////////////////////////////////////

      
      // return result
      return (m_lastGlobalForce);
    }

    // if no world has been defined in which algorithm operates, there is no force
    else
    {
        return (cVector3d(0.0, 0.0, 0.0));
    }
}


//===========================================================================
/*!
    Given the new position of the device and considering the current
    position of the proxy, this function attempts to move the proxy towards
    the device position (the goal).  If its path is blocked by an obstacle
    (e.g., a triangle in a mesh), the proxy is moved to this intersection
    point and a new goal is calculated as the closest point to the original
    goal in the half-plane above the intersection triangle.
    The process is repeated if necessary, bringing the proxy to its
    final location.

    \fn   void cProxyGraphPointForceAlgo::computeNextBestProxyPosition(cVector3d a_goal)
	\param  a_goal  The goal towards which to move the proxy, subject to constraints
*/
//===========================================================================
void cProxyGraphPointForceAlgo::computeNextBestProxyPosition(cVector3d a_goal)
{
    // local variable declarations

    // Each of these variables is re-used in each of three collision passes...

    // Did we hit a triangle this pass?
    bool hit = false;

    //! Initialize normal and tangential forces.
    m_normalForce.set(0,0,0);
    m_tangentialForce.set(0,0,0);

    // What object, triangle, and point did we collide with?
    cGenericObject* colObject = 0;
    cTriangle* colTriangle = 0;
    cVector3d colPoint;
    
    // This is the parent object of the triangle we hit, which should really
    // be the same as colObject...
    cGenericObject* parent = 0;

    // How far away was the collision from the current proxy position?
    double colDistance=DBL_MAX;

    // The three vertices of the triangle we most recently collided with...
    cVector3d vertex0, vertex1, vertex2;

    // The current positions of the proxy and the goal
    cVector3d proxy, goal;

    // temporary variable to used to describe a segment path from the proxy to its
    // next goal position
    cVector3d segmentPointB;

    // A vector from the proxy to the goal
    cVector3d vProxyGoal;

    // A vector from the most recent collision point to the proxy
    cVector3d vColProxy;

    // The offset we'll apply to each goal position to "push it away" from
    // the collision plane, to account for the proxy's radius.
    cVector3d goalOffset;

    // The goal we try to move the proxy to _after_ finding a constraint plane
    // (i.e. after the proxy->goal segment is found to hit an object)
    cVector3d colGoal;

    // The force exerted on the proxy by the user
    cVector3d force;

    // The surface normal of each of the three triangles we come in contact with.
    // These variables are not re-used across collisions.
    cVector3d normal0, normal1, normal2;

    // Used for maintaining a minimum distance between the proxy and all
    // constraint planes...
    double cosAngle, distanceTriangleProxy;

    // A vector from the device to the proxy; used to orient surface normals
    cVector3d vDeviceProxy;

    // Read the current position of proxy and device; the device position
    // is considered the initial goal position for the proxy.
    proxy = m_proxyGlobalPos;
    goal = a_goal;

    // Store the previous number of contacts for debugging...
    int previousNumberOfContacts = m_numContacts;

    // No contacts with triangles have occurred yet.
    m_numContacts = 0;

    // If the distance between the proxy and the goal position (device) is
    // very small then we can be considered done.
	
    if (goalAchieved(proxy,goal))
    {
        m_nextBestProxyGlobalPos = proxy;
        m_numContacts = 0;
        m_touchingObject = 0;
		// by gaecha
		m_shadedProxy = proxy;
		//
        return;
    }
	

    // Test whether the path from the proxy to the goal is obstructed.
    // For this we create a segment that goes from the proxy position to
    // the goal position plus a little extra to take into account the
    // physical radius of the proxy.
    segmentPointB = goal;
    offsetGoalPosition(segmentPointB,proxy);
    
	// !!!!!!!
	// by gaecha
	// I changes the cTriangle implementation in the computeCollision method
	// When the line segment from the proxy to the goal collide with the triangle
	// with its direction same as the triangle normal,
	// there will be no collision.
	// This make the proxy go through the mesh when going from inside to outside.

	// performance test
	//g_outerLoop = 0, g_innerLoop = 0;
	// performance test

	bool hit2 = false;

	if(m_bLastCollision == true) {
		cDepthImage* depthImage = m_lastPrimitive->m_depthImage;

		cMatrix3d transLocalRot;
		depthImage->getGlobalRot().transr(transLocalRot);

		cVector3d localGoal = goal;
		localGoal.sub(depthImage->getGlobalPos());
		transLocalRot.mul(localGoal);
		localGoal = depthImage->trans2image(localGoal);

		m_lastPrimitive->m_goal = localGoal;
		
		m_lastPrimitive->refreshDepth();
		m_lastPrimitive->setNeighbors();
		// when the depth image is updated and the depth value under the previous proxy is decreased,
		// the collision detection should be recomputed.
		// Therefore, when it is decreased, last information is deleted and last collision is changed to false.
		if( depthImage->correctProxyOnImage(m_lastPrimitive->m_proxy) ) {
			if(hit2 = computeNextBestProxyPositionForDepthImage(m_lastPrimitive)) {
				return;
			}
			proxy = m_nextBestProxyGlobalPos;
			colPoint = proxy;
			colDistance = DBL_MAX;

			// When the primitive moves through triangle-line-triangle,
			// sometimes the proxy is not changed but it doesn't collide with the neighbor primitives.
			// In this situation, the global collision is detected but local collision is not detected.
			// Then the process cannot escape from this loop.
			// To resolve this problem, when the proxy is not changed, I finish this loop although collision is occured.
			// When this triangle doesn't collide with the line segment,
			// move the proxy towards the goal by small amount
			// in order to prevent previous triangle from colliding.
			proxy += CHAI_SMALL*(goal - proxy);
		}
		else {
			m_lastPrimitive->releaseNeighbors();
			m_bLastCollision = false;
		}
	}

	int count = 0;
    while (hit = m_world->computeCollisionDetection( proxy, segmentPointB, colObject,
		colTriangle, colPoint, colDistance, CHAI_PROXY_ONLY_USES_VISIBLE_OBJECTS, 1) && count < 1000) {
		count++;
		if(count > 998)
			int puhaha = 0;
		// by gaecha
		// If the collided object is depth image. start the algorithm
		// CHAI3D doesn't use cGenericObject.m_tag.
		// However, in order to diffenciate cDepthImage from cMesh,
		// cGenericObject.m_tag is set to 100 in cDepthImage's constructor.

		// performance test
		//g_outerLoop++;
		// performance test

		if (colObject->m_tag == DEPTH_IMAGE_TAG)
		{
			
			cDepthImage* depthImage = (cDepthImage*)(colObject->getParent());

			// with the collided triangle, first primitive is set
			cVector3d vertex0 = colTriangle->getVertex0()->getPos();
			cVector3d vertex1 = colTriangle->getVertex1()->getPos();
			cVector3d vertex2 = colTriangle->getVertex2()->getPos();

			// In the primitive, all calculation is performed in the depth image's local coordinate.
			// Therefore, the global proxy and goal is transformed in to the local coordinate.
			// Then, the first triangular primitive is set
			
			cMatrix3d transLocalRot, transLocalRotLocal;
			depthImage->getGlobalRot().transr(transLocalRot);
			depthImage->getRot().transr(transLocalRotLocal);

			cVector3d localGoal = goal;
			localGoal.sub(depthImage->getGlobalPos());
			transLocalRot.mul(localGoal);
			localGoal = depthImage->trans2image(localGoal);

			cVector3d localProxy = colPoint;
			localProxy.sub(depthImage->getGlobalPos());
			transLocalRot.mul(localProxy);
			localProxy = depthImage->trans2image(localProxy);

			Primitive* nextPrimitive = new PrimitiveTriangle(localGoal, localProxy, depthImage, vertex0, vertex1, vertex2);
			
			// Instantiate the neighbor primitives
			nextPrimitive->setNeighbors();
			
			if(hit2 = computeNextBestProxyPositionForDepthImage(nextPrimitive)) {
				return;
			}
			else {
				proxy = m_nextBestProxyGlobalPos;
				// to prevent collision detection from making mistake
				colDistance = DBL_MAX;
				// When this triangle doesn't collide with the line segment,
				// move the proxy towards the goal by small amount
				// in order to prevent previous triangle from colliding.
				proxy += CHAI_SMALL*(goal - proxy);
			}
		}
		else {
			//-----------------------------------------------------------------------
			// FIRST COLLISION:
			//-----------------------------------------------------------------------

			// Since a collision has occurred with a triangle, we store a pointer to
			// the intersected triangle and increment the number of contacts.
			m_triangle0 = colTriangle;
			m_numContacts = 1;

			// A first collision has occurred; we first compute the global positions
			// of each vertex of the triangle we hit...
			parent = colTriangle->getParent();

			cMatrix3d rot = parent->getGlobalRot();
			cVector3d pos = parent->getGlobalPos();

			rot.mulr(colTriangle->getVertex0()->getPos(), vertex0);
			vertex0.add(pos);
			rot.mulr(colTriangle->getVertex1()->getPos(), vertex1);
			vertex1.add(pos);
			rot.mulr(colTriangle->getVertex2()->getPos(), vertex2);
			vertex2.add(pos);

			// Compute the triangle surface normal in world coordinates
			normal0 = cComputeSurfaceNormal(vertex0, vertex1, vertex2);

			// Align the surface normal to point away from the device
			vDeviceProxy = cSub(proxy,a_goal);
			vDeviceProxy.normalize();
			if (cDot(normal0,vDeviceProxy) < 0) normal0.negate();

			// Compute a vector from the proxy to the goal (device)
			goal.subr(proxy, vProxyGoal);

			// We want the center of the proxy to move as far toward the triangle as it can,
			// but we want it to stop when the _sphere_ representing the proxy hits the
			// triangle.  We want to compute how far the proxy center will have to
			// be pushed _away_ from the collision point - along the vector from the proxy
			// to the goal - to keep a distance m_radius between the proxy center and the
			// triangle.
			//
			// So we compute the cosine of the angle between the normal and proxy-goal vector...
			vProxyGoal.normalize();
			cosAngle = vProxyGoal.dot(normal0);

			// Now we compute how far away from the collision point - _backwards_
			// along vProxyGoal - we have to put the proxy to keep it from penetrating
			// the triangle.
			//
			// If only ASCII art were a little more expressive...
			distanceTriangleProxy = m_radius / cAbs(cosAngle);

			// We compute the projection of the vector between the proxy and the collision
			// point onto the normal of the triangle.  This is the direction in which
			// we'll move the _goal_ to "push it away" from the triangle (to account for
			// the radius of the proxy).

			// A vector from the collision point to the proxy...
			proxy.subr(colPoint, vColProxy);

			// Move the proxy to the collision point, minus the distance along the
			// movement vector that we computed above.
			//
			// Note that we're adjusting the 'proxy' variable, which is just a local
			// copy of the proxy position.  We still might decide not to move the
			// 'real' proxy due to friction.
			vColProxy.normalize();
			vColProxy.mul(distanceTriangleProxy);
			colPoint.addr(vColProxy, proxy);

			// If the distance between the proxy and the goal position (device) is
			// very small then we can be considered done.
			if (goalAchieved(proxy,goal))
			{
				m_nextBestProxyGlobalPos = proxy;
				m_touchingObject = parent;
				return;
			}

			// The proxy is now constrained on a plane; we now calculate the nearest
			// point to the original goal (device position) on this plane; this point
			// is computed by projecting the ideal goal onto the plane defined by the
			// intersected triangle
			goal = cProjectPointOnPlane(a_goal, vertex0, vertex1, vertex2 );

			// Since the proxy has a radius, the new goal position is offset to be
			// on the same side of the plane as the proxy, so the proxy will not
			// penetrate the triangle.
			goalOffset = normal0;
			goalOffset.mul(m_radius);
			goal.add(goalOffset);

			// Before moving the proxy to this new goal position, we need to check
			// if a second triangle could stop the proxy from reaching its new goal.
			//
			// We compute a new collision segment going from the proxy towards
			// the goal position...
			goal.subr(proxy, colGoal);

			// If the distance between the proxy and the new goal position is
			// smaller than CHAI_SMALL, we consider the proxy to be at the same position
			// as the goal, and we're done.
			if (colGoal.length() < CHAI_SMALL)
			{
				m_nextBestProxyGlobalPos = proxy;
				m_touchingObject = parent;
				return;
			}


			if (m_useZillesFriction)
			{
				// Calculate static friction, and see if we should move the _real_ proxy
				// position...

				// Calculate the force exerted on the proxy by the user as the stiffness
				// coefficient of the intersected triangle's mesh times the vector between the
				// proxy and the device...
				force = cMul(colTriangle->getParent()->m_material.getStiffness(),
						cSub(proxy, a_goal));

				// Calculate the normal component of that force...
				m_normalForce = cProject(force, normal0);

				// Calculate the tangential component of that force...
				m_tangentialForce = cSub(force, m_normalForce);

				// If the magnitude of the tangential force is less than the
				// static friction coefficient times the magnitude of the normal force,
				// the user's position is in the friction cone of the surface, and the
				// proxy should not be moved.
				if (m_tangentialForce.length() <
					colTriangle->getParent()->m_material.getStaticFriction()*m_normalForce.length())
				{
					m_nextBestProxyGlobalPos = m_proxyGlobalPos;
					m_touchingObject = parent;
					return;
				}
			}

			// If the distance between the proxy and the goal position (device) is
			// very small then we can be considered done.
			if (goalAchieved(proxy,goal))
			{
				m_nextBestProxyGlobalPos = proxy;
				m_numContacts = 0;
				m_touchingObject = 0;
				return;
			}

			// Test whether the path along the virtual line between the updated proxy
			// and its new goal is obstructed
			segmentPointB = goal;
			offsetGoalPosition(segmentPointB,proxy);

			hit = m_world->computeCollisionDetection(proxy, segmentPointB, colObject,
					colTriangle, colPoint, colDistance, CHAI_PROXY_ONLY_USES_VISIBLE_OBJECTS, 2);

			// If no collision occurs, we move the proxy to its goal, unless
			// friction prevents us from doing so.

			if (hit == false)
			{
				testFrictionAndMoveProxy(goal, proxy, normal0, parent);
				return;
			}

			//-----------------------------------------------------------------------
			// SECOND COLLISION:
			//-----------------------------------------------------------------------

			// Since a collision has occurred with a triangle, we store a pointer to
			// the intersected triangle and increment the number of contacts.
			m_triangle1 = colTriangle;
			m_numContacts = 2;

			// A second collision has occurred; we first compute the global positions
			// of each vertex of the triangle from the mesh to which the triangle
			// belongs (its parent)
			parent = colTriangle->getParent();

			rot = parent->getGlobalRot();
			pos = parent->getGlobalPos();

			rot.mulr( colTriangle->getVertex0()->getPos(), vertex0);
			vertex0.add(pos);
			rot.mulr( colTriangle->getVertex1()->getPos(), vertex1);
			vertex1.add(pos);
			rot.mulr(colTriangle->getVertex2()->getPos(), vertex2);
			vertex2.add(pos);

			// Next we compute the triangle surface normal in world coordinates...
			normal1 = cComputeSurfaceNormal(vertex0, vertex1, vertex2);

			// Align the surface normal to point away from the device
			vDeviceProxy = cSub(proxy,a_goal);
			vDeviceProxy.normalize();
			if (cDot(normal1,vDeviceProxy) < 0) normal1.negate();

			// Compute a vector from the proxy to the goal...
			goal.subr(proxy, vProxyGoal);

			// Compute the distance we'll have to push the proxy "backwards" along
			// the proxy-goal vector to account for the proxy's radius.
			//
			// See above for a more detailed explanation of what's happening here.
			vProxyGoal.normalize();
			cosAngle = vProxyGoal.dot(normal1);
			distanceTriangleProxy = m_radius / cAbs(cosAngle);

			// Move the proxy to its new position, preventing it from quite reaching
			// the goal to account for the proxy radius.
			vProxyGoal.mul(distanceTriangleProxy);
			colPoint.subr(vProxyGoal, proxy);

			// If the distance between the proxy and the new goal position is
			// smaller than CHAI_SMALL, we consider the proxy to be at the same position
			// as the goal, and we're done.
			if (goalAchieved(proxy,goal))
			{
				m_nextBestProxyGlobalPos = proxy;
				m_touchingObject = parent;
				return;
			}

			// The proxy is now constrained by two triangles and can only move along
			// a virtual line; we now calculate the nearest point to the original
			// goal (device position) along this line by projecting the ideal
			// goal onto the line.
			//
			// The line is expressed by the cross product of both surface normals,
			// which have both been oriented to point away from the device
			cVector3d line;
			normal0.crossr(normal1, line);

			// check result.
			if (line.equals(cVector3d(0,0,0)))
			{
				m_nextBestProxyGlobalPos = proxy;
				m_touchingObject = parent;
				return;
			}

			// Compute the projection of the device position (goal) onto the line; this
			// gives us the new goal position.
			goal = cProjectPointOnLine(a_goal, proxy, line);

			// Before moving the proxy to this new goal position, we need to check
			// if a third triangle could stop the proxy from reaching its new goal.
			goal.subr(proxy, colGoal);

			// If the distance between the proxy and the new goal position is
			// smaller than CHAI_SMALL, we consider the proxy to be at the same position
			// as the goal, and we're done.
			if (colGoal.length() < CHAI_SMALL)
			{
				m_nextBestProxyGlobalPos = proxy;
				m_touchingObject = parent;
				return;
			}

			if (m_useZillesFriction)
			{
				// Calculate static friction, and see if we should move the _real_ proxy
				// position...

				// Calculate the force exerted on the proxy by the user as the stiffness
				// coefficient of the intersected triangle's mesh times the vector between the
				// proxy and the device.
				force = cMul(colTriangle->getParent()->m_material.getStiffness(),
						cSub(proxy, a_goal));

				// Calculate the normal component of this force
				m_normalForce = cProject(force, normal1);

				// Calculate the tangential component of the force
				m_tangentialForce = cSub(force, m_normalForce);

				// If the magnitude of the tangential force is less than the
				// static friction coefficient times the magnitude of the normal force,
				// the user's position is in the friction cone of the surface, and the
				// proxy should not be moved.
				if (m_tangentialForce.length() <
					colTriangle->getParent()->m_material.getStaticFriction()*m_normalForce.length())
				{
					m_nextBestProxyGlobalPos = m_proxyGlobalPos;
					m_touchingObject = parent;
					return;
				}
			}

			// If the distance between the proxy and the goal position (device) is
			// very small then we can be considered done.
			if (goalAchieved(proxy,goal))
			{
				m_nextBestProxyGlobalPos = proxy;
				m_numContacts = 0;
				m_touchingObject = 0;
				return;
			}

			// Test whether the path along the virtual line between the updated proxy
			// and its new goal is obstructed
			segmentPointB = goal;
			offsetGoalPosition(segmentPointB,proxy);

			hit = m_world->computeCollisionDetection(proxy, segmentPointB, colObject,
					colTriangle, colPoint, colDistance, CHAI_PROXY_ONLY_USES_VISIBLE_OBJECTS, 3);

			// If no collision occurs, we move the proxy to its goal, unless
			// friction prevents us from doing so

			if (hit == false)
			{
				testFrictionAndMoveProxy(goal, proxy, cMul(0.5,cAdd(normal0,normal1)), parent);
				return;
			}

			//-----------------------------------------------------------------------
			// THIRD COLLISION:
			//-----------------------------------------------------------------------

			// The proxy is now constrained to a single point just above the third triangle
			m_triangle2 = colTriangle;
			m_numContacts = 3;

			// A third collision has occurred; we first compute the global positions
			// of each vertex of the triangle...
			parent = colTriangle->getParent();

			rot = parent->getGlobalRot();
			pos = parent->getGlobalPos();

			rot.mulr(colTriangle->getVertex0()->getPos(), vertex0);
			vertex0.add(pos);
			rot.mulr(colTriangle->getVertex1()->getPos(), vertex1);
			vertex1.add(pos);
			rot.mulr(colTriangle->getVertex2()->getPos(), vertex2);
			vertex2.add(pos);

			// Next we compute the triangle surface normal in world coordinates...
			normal2 = cComputeSurfaceNormal(vertex0, vertex1, vertex2);

			// Compute a vector from the proxy to the goal point...
			goal.subr(proxy, vProxyGoal);

			// Compute the distance we'll have to push the proxy "backwards" along
			// the proxy-goal vector to account for the proxy's radius.
			//
			// See above for a more detailed explanation of what's happening here.
			vProxyGoal.normalize();
			cosAngle = vProxyGoal.dot(normal2);
			distanceTriangleProxy = m_radius / cAbs(cosAngle);

			// Note that we don't really have to do another "goal offset" here, because
			// we're moving the proxy along a line, and we will make sure not to move
			// the proxy too close to the colliding triangle...

			// Move the proxy to the collision point, minus the distance along the
			// movement vector that we computed above.
			//
			// Note that we're adjusting the 'proxy' variable, which is just a local
			// copy of the proxy position.  We still might decide not to move the
			// 'real' proxy due to friction.
			vProxyGoal.normalize();
			vProxyGoal.mul(distanceTriangleProxy);
			colPoint.subr(vProxyGoal, proxy);

			// TODO: There actually should be a third friction test to see if we
			// can make it to our new goal position, but this is generally such a
			// small movement in one iteration that it's irrelevant...
			m_nextBestProxyGlobalPos = proxy;
			m_touchingObject = parent;
			return;
		}
	}
	// If no collision occurs, then we move the proxy to its goal, and we're done
	m_nextBestProxyGlobalPos = goal;
	m_numContacts = 0;
	m_touchingObject = 0;
	m_slipping = 0;
	m_shadedProxy = m_nextBestProxyGlobalPos;

	return;
}


//===========================================================================
/*!
    Test whether the proxy has reached the goal point, allowing for subclass-
    specific approximations.

    \fn   virtual bool cProxyGraphPointForceAlgo::goalAchieved(const cVector3d& a_proxy, const cVector3d& a_goal) const;
    \param    a_goal        The location to which we'd like to move the proxy
    \param    a_proxy       The current position of the proxy
    \return   true is the proxy has effectively reached the goal
*/
//===========================================================================
bool cProxyGraphPointForceAlgo::goalAchieved(const cVector3d& a_proxy, const cVector3d& a_goal) const
{
    if (m_dynamicProxy) return false;
    return (a_proxy.distance(a_goal) < CHAI_SMALL);
}


//===========================================================================
/*!
    Offset the current goal position to account for the volume/shape of the proxy.
   
    \fn   virtual void cProxyGraphPointForceAlgo::offsetGoalPosition(cVector3d& a_goal, const cVector3d& a_proxy) const;
    \param    a_goal        The location to which we'd like to move the proxy, offset upon return
    \param    a_proxy       The current position of the proxy
*/
//===========================================================================
void cProxyGraphPointForceAlgo::offsetGoalPosition(cVector3d& a_goal, const cVector3d& a_proxy) const
{
    if (m_dynamicProxy) return;
    a_goal = cAdd(a_goal, cMul(m_radius, cNormalize( cSub(a_goal, a_proxy))));
}



//===========================================================================
/*!
  Attempt to move the proxy, subject to friction constraints.  This is called
  from computeNextBestProxyPosition when the proxy is ready to move along a
  known surface.

  \fn   void cProxyGraphPointForceAlgo::testFrictionAndMoveProxy(const cVector3d& goal, const cVector3d& proxy,
             cVector3d normal, cGenericObject* parent)
  \param    goal        The location to which we'd like to move the proxy
  \param    proxy       The current position of the proxy
  \param    normal      The surface normal at the obstructing surface
  \param    parent      The surface along which we're moving  
*/
//===========================================================================
void cProxyGraphPointForceAlgo::testFrictionAndMoveProxy(const cVector3d& goal, const cVector3d& proxy,
  cVector3d normal, cGenericObject* parent)
{
    if (m_useFriction == false || m_useMelderFriction == false)
    {
        m_nextBestProxyGlobalPos = goal;
        m_touchingObject = parent;
        return;
    }

    // Compute penetration depth; how far is the device "behind" the
    // plane of the obstructing surface
    cVector3d projectedGoal = cProjectPointOnPlane(m_deviceGlobalPos,proxy,normal);
    double penetrationDepth = cSub(m_deviceGlobalPos,projectedGoal).length();

    // Find the appropriate friction coefficient

    // Our dynamic and static coefficients...
    cMesh* parent_mesh = dynamic_cast<cMesh*>(parent);

    // Right now we can only work with cMesh's
    if (parent_mesh == 0)
    {
        m_nextBestProxyGlobalPos = goal;
        m_touchingObject = parent;
        return;
    }

    double mud = parent_mesh->m_material.getDynamicFriction();
    double mus = parent_mesh->m_material.getStaticFriction();

    // No friction; don't try to compute friction cones
    if (mud == 0 && mus == 0)
    {
        m_nextBestProxyGlobalPos = goal;
        m_touchingObject = parent;
        return;
    }

    // The corresponding friction cone radii
    double atmd = atan(mud);
    double atms = atan(mus);

    // Compute a vector from the device to the proxy, for computing
    // the angle of the friction cone
    cVector3d vDeviceProxy = cSub(proxy,m_deviceGlobalPos);
    vDeviceProxy.normalize();

    // Now compute the angle of the friction cone...
    double theta = acos(vDeviceProxy.dot(normal));

    // Manage the "slip-friction" state machine

    // If the dynamic friction radius is for some reason larger than the
    // static friction radius, always slip
    if (mud > mus)
    {
        m_slipping = 1;
    }

    // If we're slipping...
    else if (m_slipping)
    {
        if (theta < atmd * DYNAMIC_FRICTION_HYSTERESIS_MULTIPLIER) m_slipping = false;
        else m_slipping = true;
    }

    // If we're not slipping...
    else
    {
      if (theta > atms) m_slipping = true;
      else m_slipping = false;
    }

    // The friction coefficient we're going to use...
    double mu;
    if (m_slipping) mu = mud;
    else mu = mus;

    // Calculate the friction radius as the absolute value of the penetration
    // depth times the coefficient of friction
    double frictionRadius = fabs(penetrationDepth * mu);

    // Calculate the distance between the proxy position and the current
    // goal position.
    double r = proxy.distance(goal);

    // If this distance is smaller than CHAI_SMALL, we consider the proxy
    // to be at the same position as the goal, and we're done...
    if (r < CHAI_SMALL)
    {
        m_nextBestProxyGlobalPos = proxy;
    }

    // If the proxy is outside the friction cone, update its position to
    // be on the perimeter of the friction cone...
    else if (r > frictionRadius)
    {
        m_nextBestProxyGlobalPos = cAdd(goal,cMul(frictionRadius/r,cSub(proxy, goal)));
    }

    // A hack to prevent the proxy from getting stuck in corners...
    else if (m_numContacts >= 2)
    {
        m_nextBestProxyGlobalPos = proxy;
    }

    // Otherwise, if the proxy is inside the friction cone, the proxy
    // should not be moved (set next best position to current position)
    else
    {
        m_nextBestProxyGlobalPos = proxy;
    }

    // We're done; record the fact that we're still touching an object...
    m_touchingObject = parent;
    return;
}

//===========================================================================
/*!
    This method uses the information computed earlier in
    computeNextProxyPosition() to determine the force to apply to the device.
    The function computes a force proportional to the distance between the
    positions of the proxy and the device and scaled by the average
    stiffness of each contact triangle.

    \fn       void cProxyGraphPointForceAlgo::computeForce()
*/
//===========================================================================
void cProxyGraphPointForceAlgo::computeForce()
{
    // A local stiffness is computed by computing the average stiffness
    // for each triangle we're in contact with...
    double stiffness;

    // if there are no contacts between proxy and environment, no force is applied
    if (m_numContacts == 0)
    {
        m_lastGlobalForce.zero();
        return;
    }

    // if there is one contact point, the stiffness is equal to the stiffness
    // of the one intersected triangle's mesh

    else if (m_numContacts == 1)
    {
		// by gaecha
		// when the depth image is contacted, the stiffness is read other ways.
		if(m_touchingObject->m_tag == DEPTH_IMAGE_TAG) {
			cDepthImage* depth_image = (cDepthImage*) m_touchingObject;
			stiffness = depth_image->getStiffness(m_proxyGlobalPos);
		}
		else stiffness = ( m_triangle0->getParent()->m_material.getStiffness() );
		//stiffness = ( m_triangle0->getParent()->m_material.getStiffness() );
    }

    // if there are two contact points, the stiffness is the average of the
    // stiffnesses of the two intersected triangles' meshes
    else if (m_numContacts == 2)
    {
        stiffness = ( m_triangle0->getParent()->m_material.getStiffness() +
                      m_triangle1->getParent()->m_material.getStiffness() ) / 2.0;
    }

    // if there are three contact points, the stiffness is the average of the
    // stiffnesses of the three intersected triangles' meshes
    else if (m_numContacts == 3)
    {
        stiffness = ( m_triangle0->getParent()->m_material.getStiffness() +
                      m_triangle1->getParent()->m_material.getStiffness() +
                      m_triangle2->getParent()->m_material.getStiffness() ) / 3.0;
    }
		
    // compute the force by modeling a spring between the proxy and the device
	/*
    cVector3d force;
    m_proxyGlobalPos.subr(m_deviceGlobalPos, force);
    force.mul(stiffness);
    m_lastGlobalForce = force;
	*/
	// by gaecha
	// for force shading
	if(m_useForceShading && m_touchingObject->m_tag == DEPTH_IMAGE_TAG) {
		cVector3d force;
		m_shadedProxy.subr(m_deviceGlobalPos, force);
		force.mul(stiffness);
		m_lastGlobalForce = force;
	}
	else {
		cVector3d force;
		m_proxyGlobalPos.subr(m_deviceGlobalPos, force);
		force.mul(stiffness);
		m_lastGlobalForce = force;
	}
	// performance test
	cVector3d force;
	g_localProxy.subr(g_localGoal, force);
	force.mul(stiffness);
	g_localForce = force;
	g_globalForce = m_lastGlobalForce;
	// performance test
}


//===========================================================================
/*!
    Return the number of current contacts, and the associated triangles in the
    output parameters.

    \fn       unsigned int cProxyGraphPointForceAlgo::getContacts(cTriangle*& a_t0,
              cTriangle*& a_t1, cTriangle*& a_t2);
    \param    a_t0  Returns pointer to first contact triangle.
    \param    a_t1  Returns pointer to second contact triangle.
    \param    a_t2  Returns pointer to third contact triangle.
    \return   Return the number of current contacts.
*/
//===========================================================================
unsigned int cProxyGraphPointForceAlgo::getContacts(cTriangle*& a_t0, cTriangle*& a_t1,
                                               cTriangle*& a_t2)
{
    // set the triangle pointers to the contact triangles
    a_t0 = (m_numContacts>0)?m_triangle0:0;
    a_t1 = (m_numContacts>1)?m_triangle1:0;
    a_t2 = (m_numContacts>2)?m_triangle2:0;

    // return the number of triangle intersections
    return m_numContacts;
}


//===========================================================================
/*!
    This method sets the dynamic proxy state to reflect new contact
    information.

    \fn       void cProxyGraphPointForceAlgo::updateDynamicContactState();
*/
//===========================================================================
void cProxyGraphPointForceAlgo::updateDynamicContactState()
{
    // Update "last-state" dynamic contact information
    if (m_dynamicProxy && m_movingObject)
    {
        cGenericObject* savedTouchingObject = m_touchingObject;
        m_touchingObject = m_movingObject;      
        if (m_touchingObject != 0) 
        {
            m_lastObjectGlobalPos = m_touchingObject->getGlobalPos();
            m_lastObjectGlobalRot = m_touchingObject->getGlobalRot();
        }
        m_touchingObject = savedTouchingObject;
        return;
    }

    // if the proxy is not currently in contact with any object, no update
    // is needed
    if (m_touchingObject == 0) return;

    // update the position and rotation of the object currently being touched
    m_lastObjectGlobalPos = m_touchingObject->getGlobalPos();
    m_lastObjectGlobalRot = m_touchingObject->getGlobalRot();
}


//===========================================================================
/*!
    This method lets the proxy move along with the object it's touching, if
    the object has moved since the previous proxy iteration.

    \fn       void cProxyGraphPointForceAlgo::correctProxyForObjectMotion();
*/
//===========================================================================
void cProxyGraphPointForceAlgo::correctProxyForObjectMotion()
{
    if (m_dynamicProxy && m_movingObject) m_touchingObject = m_movingObject;

    // if the proxy is not currently in contact with any object, no update
    // is needed
    if (m_touchingObject == NULL) return;

	// by gaecha
    if( m_touchingObject->m_tag == DEPTH_IMAGE_TAG ) return;

    // start with the non-dynamic proxy position
    cVector3d newGlobalProxy = m_proxyGlobalPos;

    // get the position and rotation of the object contacted in the last iteration
    cVector3d lastGlobalPos;
    getContactObjectLastGlobalPos(lastGlobalPos);
    cMatrix3d lastGlobalRot;
    getContactObjectLastGlobalRot(lastGlobalRot);

    // combine the rotation and translation into one matrix
    cMatrix3d lastGlobalRotT;
    lastGlobalRot.transr(lastGlobalRotT);

    // convert the last segment point A (proxy position) into the
    // _current_ reference frame of the object
    newGlobalProxy.sub(lastGlobalPos);

    // apply rotation as necessary
    lastGlobalRotT.mul(newGlobalProxy);
    m_touchingObject->getGlobalRot().mul(newGlobalProxy);

    // convert to global coordinates
    newGlobalProxy.add(m_touchingObject->getGlobalPos());

    // this ends up being the proxy in global coordinates
    m_proxyGlobalPos = newGlobalProxy;
}

bool cProxyGraphPointForceAlgo::computeNextBestProxyPositionForDepthImage(
			Primitive* a_nextPrimitive)
{
	bool bNext = false;
	Primitive* previousPrimitive;
	cVector3d localProxy = a_nextPrimitive->m_proxy;
	cVector3d globalProxy = a_nextPrimitive->getProxy();
	//cVector3d nextGlobalProxy;
	cVector3d nextLocalProxy;

	//test
	cVector3d previousLocalProxy;

	cDepthImage* a_depthImage = a_nextPrimitive->m_depthImage;

	// collision with neighbor primitives?
	bool hit = false;
	// move the proxy?
	// when this value get false, the proxy is in the friction cone
	bool bFrictioned = true;


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// one more thing I should do is
	// correcting the friction test when the user stop the probe keeping collision.
	// It's better use the old goal position.
	// Check collision with neighbor primitives
	int count = 0;
	do {
		
		// performance test
		//g_innerLoop++;
		// performance test
		//if(g_innerLoop > 20)
		//	int puhaha = 0;
		count++;
		if(count > 998)
			int puhaha = 0;

		previousPrimitive = (Primitive*) a_nextPrimitive;
		if(bNext = a_nextPrimitive->computeNextPrimitive(a_nextPrimitive)) {
			//nextGlobalProxy = a_nextPrimitive->getProxy();
			nextLocalProxy = a_nextPrimitive->m_proxy;
			if(m_useMelderFriction) {
				// When the active primitive moves from line to triangle and from point to line,
				// the proxy position is not changed.
				// Therefore, there's no need to do the friction cone test
				if( (previousPrimitive->m_type == PRIMITIVE_TRIANGLE) ||
					(a_nextPrimitive->m_type == PRIMITIVE_POINT) )
				{
					//bFrictioned = testFrictionAndMoveProxyDI(globalProxy, nextGlobalProxy, previousPrimitive, a_depthImage);
					bFrictioned = testFrictionAndMoveProxyDI(localProxy, nextLocalProxy, previousPrimitive, a_depthImage);
				}
				else
				{
					//globalProxy = nextGlobalProxy;
					localProxy = nextLocalProxy;
					bFrictioned = true;
				}
			}
			else {
				bFrictioned = true;
				//globalProxy = nextGlobalProxy;
				localProxy = nextLocalProxy;
			}

			// If the proxy is inside the friction cone, finish updating the proxy
			if(m_useMelderFriction && !bFrictioned) {
				//delete a_nextPrimitive;
				a_nextPrimitive = previousPrimitive;
				hit = true;
				break;
			}
			else {
				hit = a_nextPrimitive->neighborCollisionDetection();
				delete previousPrimitive;
			}
		}
		else {
			// If proxy on current primitive is closest to the goal, finish updating the proxy
			//nextGlobalProxy = a_nextPrimitive->getProxy();
			nextLocalProxy = a_nextPrimitive->m_proxy;
			if(m_useMelderFriction) {
				//bFrictioned = testFrictionAndMoveProxyDI(globalProxy, nextGlobalProxy, previousPrimitive, a_depthImage);
				bFrictioned = testFrictionAndMoveProxyDI(localProxy, nextLocalProxy, previousPrimitive, a_depthImage);
			}
			else
			{
				bFrictioned = true;
				//globalProxy = nextGlobalProxy;
				localProxy = nextLocalProxy;
			}
			hit = a_nextPrimitive->neighborCollisionDetection();
			break;
		}				
	} while(hit && count < 1000);

	if(hit) {
		// numerical error correction
		//globalProxy = a_depthImage->projectOnDepthImageGlobal(globalProxy);
		//localProxy = a_depthImage->projectOnDepthImage(localProxy);
		globalProxy = a_depthImage->trans2cartesian(localProxy);

		a_depthImage->getGlobalRot().mul(globalProxy);
		globalProxy.add(a_depthImage->getGlobalPos());
		//

		m_nextBestProxyGlobalPos = globalProxy;
		m_touchingObject = a_depthImage;
		m_numContacts = 1;

		a_nextPrimitive->releaseNeighbors();

		m_bLastCollision = true;
		m_lastPrimitive = a_nextPrimitive;

		a_nextPrimitive->m_proxy = localProxy;

		a_depthImage->updatePrevisouSurfaceDepthLocal(a_nextPrimitive->m_proxy);

		return true;
	}
	else {
		//
		globalProxy = a_depthImage->trans2cartesian(localProxy);

		a_depthImage->getGlobalRot().mul(globalProxy);
		globalProxy.add(a_depthImage->getGlobalPos());
		//
		m_nextBestProxyGlobalPos = globalProxy;
		m_bLastCollision = false;
		a_nextPrimitive->m_selectedIndex = -1;

		a_depthImage->updatePrevisouSurfaceDepthLocal(a_nextPrimitive->m_proxy);

		delete a_nextPrimitive;

		return false;
	}
}

bool cProxyGraphPointForceAlgo::testFrictionAndMoveProxyDI(cVector3d& proxy, const cVector3d& nextProxy,
  Primitive* a_primitive, cDepthImage* depth_image)
{
    if (m_useFriction == false || m_useMelderFriction == false)
    {
        proxy = nextProxy;
        return true;
    }

    // Compute penetration depth; how far is the device "behind" the
    // plane of the obstructing surface
	//cVector3d proxy2goal = m_deviceGlobalPos - proxy ;
	cVector3d goal = a_primitive->m_goal;
	cVector3d proxy2goal = goal - proxy ;
	//
	cVector3d proxy2nextProxy = nextProxy -proxy;
	// If the proxy is not moved, proxy2nextProxy becomes zero and its normal can be divided with zero.
	// Therefore, if it's not moved, just return true.
	if(proxy2nextProxy.length() < CHAI_SMALL) {
		proxy = nextProxy;
		return true;
	}
	proxy2nextProxy.normalize();
	cVector3d projectedGoal = proxy + cDot(proxy2goal, proxy2nextProxy)* proxy2nextProxy;
	//cVector3d normal = projectedGoal - m_deviceGlobalPos;
	cVector3d normal = projectedGoal - goal;
	normal.normalize();
    //double penetrationDepth = cSub(m_deviceGlobalPos,projectedGoal).length();
	double penetrationDepth = cSub(goal,projectedGoal).length();


    // Right now we can only work with cDepthImage's
    if (depth_image == 0)
    {
        proxy = nextProxy;
        return true;
    }

    // Find the appropriate friction coefficient
    double mud = depth_image->getStaticFrictionLocal(proxy);
    double mus = depth_image->getDynamicFrictionLocal(proxy);


    // No friction; don't try to compute friction cones
    if (mud == 0 && mus == 0)
    {
        proxy = nextProxy;
        return true;
    }

    // The corresponding friction cone radii
    double atmd = atan(mud);
    double atms = atan(mus);

    // Compute a vector from the device to the proxy, for computing
    // the angle of the friction cone
    //cVector3d vDeviceProxy = cSub(proxy,m_deviceGlobalPos);
	cVector3d vDeviceProxy = cSub(proxy,goal);
    vDeviceProxy.normalize();

    // Now compute the angle of the friction cone...
    double theta = acos(vDeviceProxy.dot(normal));

    // Manage the "slip-friction" state machine

    // If the dynamic friction radius is for some reason larger than the
    // static friction radius, always slip
    if (mud > mus)
    {
        m_slipping = 1;
    }

    // If we're slipping...
    else if (m_slipping)
    {
        if (theta < atmd * DYNAMIC_FRICTION_HYSTERESIS_MULTIPLIER)
			m_slipping = false;
        else
			m_slipping = true;
    }

    // If we're not slipping...
    else
    {
      if (theta > atms)
		  m_slipping = true;
      else
		  m_slipping = false;
    }

    // The friction coefficient we're going to use...
    double mu;
    if (m_slipping) mu = mud;
    else mu = mus;

    // Calculate the friction radius as the absolute value of the penetration
    // depth times the coefficient of friction
    double frictionRadius = fabs(penetrationDepth * mu);

    // Calculate the distance between the proxy position and the current
    // goal position.
    double movingDistance = proxy.distance(nextProxy);
	double r1 = proxy.distance(projectedGoal);
	double r2 = nextProxy.distance(projectedGoal);

    // If this distance is smaller than CHAI_SMALL, we consider the proxy
    // to be at the same position as the goal, and we're done...
    if (movingDistance < CHAI_SMALL)
    {
        return false;
    }

	else if (r2 > frictionRadius)
	//if (r2 > frictionRadius)
	{
		proxy = nextProxy;
		return true;
	}

    // If the proxy is outside the friction cone, update its position to
    // be on the perimeter of the friction cone...
    else if (r1 > frictionRadius)
    {
		cVector3d oriProxy = proxy;
		proxy = cAdd(projectedGoal,cMul(frictionRadius/r1,cSub(proxy, projectedGoal)));
		if(oriProxy.distance(nextProxy) < oriProxy.distance(proxy))
			int puhaha = 0;
		if(m_useForceShading) {
			cMatrix3d transLocalRot;
			depth_image->getGlobalRot().transr(transLocalRot);
			cVector3d localTempProxy = proxy;
			localTempProxy.sub(depth_image->getGlobalPos());
			transLocalRot.mul(localTempProxy);
			localTempProxy.add(depth_image->m_depthImageSizeX/2.0, depth_image->m_depthImageSizeY/2.0, 0.0);

			cVector3d shadedNorm = a_primitive->computeShadedNorm(localTempProxy);
			double cos_th = normal.dot(shadedNorm);
			//cVector3d shadedProjectedGoal = m_deviceGlobalPos + (penetrationDepth/cos_th)*shadedNorm;
			cVector3d shadedProjectedGoal = goal + (penetrationDepth/cos_th)*shadedNorm;
			m_shadedProxy = cAdd(shadedProjectedGoal,cMul(frictionRadius/r1,cSub(proxy, projectedGoal)));
		}

		return false;
    }

    // Otherwise, if the proxy is inside the friction cone, the proxy
    // should not be moved (set next best position to current position)
    else
    {
        return false;
    }
}