//===========================================================================
/*
    This file is part of the CHAI 3D visualization and haptics libraries.
    Copyright (C) 2003-2009 by CHAI 3D. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License("GPL") version 2
    as published by the Free Software Foundation.

    For using the CHAI 3D libraries with software that can not be combined
    with the GNU GPL, and for taking advantage of the additional benefits
    of our support services, please contact CHAI 3D about acquiring a
    Professional Edition License.

    \author    <http://www.chai3d.org>
    \author    Chris Sewell
    \author    Francois Conti
    \version   2.0.0 $Rev: 244 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "collisions/CCollisionSpheresGeometry.h"
//---------------------------------------------------------------------------
// Initialize m_split, axis on which to sort triangle primitives.
int cCollisionSpheresGenericShape::m_split = 0;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of cCollisionSpheresEdge.

    \fn       cCollisionSpheresEdge::initialize(
              cCollisionSpheresPoint *a_a, cCollisionSpheresPoint *a_b)
    \param    a_a  First vertex of the edge.
    \param    a_b  Second vertex of the edge.
*/
//===========================================================================
void cCollisionSpheresEdge::initialize(cCollisionSpheresPoint *a_a, cCollisionSpheresPoint *a_b)
{
    // set the endpoints of the new edge
    m_end[0] = a_a;
    m_end[1] = a_b;

    // insert the edge into the edge maps of both endpoints
    m_end[0]->m_edgeMap.insert(PtEmap::value_type(m_end[1], this));
    m_end[1]->m_edgeMap.insert(PtEmap::value_type(m_end[0], this));

    // calculate the vector between the endpoints
    m_d = cSub((*m_end[1]).m_pos, (*m_end[0]).m_pos);

    // calculate the squared distance of the edge
    m_D = m_d.x*m_d.x + m_d.y*m_d.y + m_d.z*m_d.z;

    // calculate the center of the edge
    double lambda = 0.5;
    m_center.x = (*m_end[0]).m_pos.x + lambda*((*m_end[1]).m_pos.x - (*m_end[0]).m_pos.x);
    m_center.y = (*m_end[0]).m_pos.y + lambda*((*m_end[1]).m_pos.y - (*m_end[0]).m_pos.y);
    m_center.z = (*m_end[0]).m_pos.z + lambda*((*m_end[1]).m_pos.z - (*m_end[0]).m_pos.z);
}


//===========================================================================
/*!
    Constructor of cCollisionSpheresTri, to enclose a single triangle in 
    a sphere.

    \fn       cCollisionSpheresTri::cCollisionSpheresTri(cVector3d a,
                                           cVector3d b,
                                           cVector3d c,
                                           double a_extendedRadius)
    \param    a     First vertex of the triangle.
    \param    b     Second vertex of the triangle.
    \param    c     Third vertex of the triangle.
    \param    a_extendedRadius  Additional radius to add to sphere.
    \return   Return a pointer to new cCollisionSpheresTri instance.
*/
//===========================================================================
cCollisionSpheresTri::cCollisionSpheresTri(cVector3d a,
                                           cVector3d b,
                                           cVector3d c,
                                           double a_extendedRadius)
{
    // Calculate the center of the circumscribing sphere for this triangle:
    // First compute the normal to the plane of this triangle
    cVector3d plane_normal = cCross(a-b, a-c);
    
    // Compute the perpendicular bisector of the edge between points a and b
    cVector3d bisector1_dir = cCross(a-b, plane_normal);
    cVector3d bisector1_pt = (a + b) / 2.0;

    // Compute the perpendicular bisector of the edge between points b and c
    cVector3d bisector2_dir = cCross(b-c, plane_normal);
    cVector3d bisector2_pt = (b + c) / 2.0;
    
    // Find the intersection of the perpendicular bisectors to find the center
    // of the circumscribed sphere, using the formula for 3D line-line
    // intersection given at 
    // http://cglab.snu.ac.kr/research/seminar/data01-1/RealTimeRendering10_1.ppt 
    cVector3d mat[3];
    mat[0] = bisector2_pt - bisector1_pt;
    mat[1] = bisector2_dir;
    mat[2] = cCross(bisector1_dir, bisector2_dir);
    float det = (float)(
                mat[0].x*mat[1].y*mat[2].z + mat[1].x*mat[2].y*mat[0].z +
                mat[2].x*mat[0].y*mat[1].z - mat[0].z*mat[1].y*mat[2].z -
                mat[1].z*mat[2].y*mat[0].x - mat[2].z*mat[0].y*mat[1].x);
    cVector3d cp = cCross(bisector1_dir, bisector2_dir);
    if (cp.lengthsq() > CHAI_SMALL)
    {
      float s = (float)(det / cp.lengthsq());
      m_center = bisector1_pt + s * bisector1_dir;
    }
    else
    {
        m_center = (a + b)/2.0;
    }

    // set the vertices (corners) of the triangle
    m_corner[0].m_pos = a;
    m_corner[1].m_pos = b;
    m_corner[2].m_pos = c;

    // Calculate a radius of the bounding sphere as the largest distance between
    // the sphere center calculated above and any vertex of the triangle
    m_radius = 0;
    unsigned int i, j, k;
    for (i = 0; i < 3; i++)
    {
        double curRadius = m_corner[i].m_pos.distance(m_center);
        if (curRadius > m_radius)
            m_radius = curRadius;
    }

    // See if we could get a smaller bounding sphere by just taking one of the edges
    // of the triangle as a diameter (this may be better for long, skinny triangles)
    for (i=0; i<3; i++)
    {
        for (j=i; j<3; j++)
        {
            // Calculate the center for this edge, and determine necessary sphere radius
            cVector3d candidate_center;
            candidate_center.x = (m_corner[i].m_pos.x + m_corner[j].m_pos.x) / 2.0;
            candidate_center.y = (m_corner[i].m_pos.y + m_corner[j].m_pos.y) / 2.0;
            candidate_center.z = (m_corner[i].m_pos.z + m_corner[j].m_pos.z) / 2.0;
            double candidate_radius = 0.0;
            for (k = 0; k < 3; k++)
            {
                double curRad = m_corner[k].m_pos.distance(candidate_center);
                if (curRad > candidate_radius) candidate_radius = curRad;
            }

            // If this results in a smaller sphere, use it
            if (candidate_radius < m_radius)
            {
                m_radius = candidate_radius;
                m_center = candidate_center;
            }
        }
    }

    // add external radius to sphere
    m_radius = m_radius + a_extendedRadius;
}


//===========================================================================
/*!
    Determine whether there is any intersection between the primitives
    (this triangle and the given line) by calling the collision detection
    method of the cTriangle object associated with this triangle primitive.

    \fn       bool cCollisionSpheresTri::computeCollision(cCollisionSpheresGenericShape *a_other,
                                            cCollisionRecorder& a_recorder,
                                            cCollisionSettings& a_settings)
    \param    a_other  The line primitive to check for intersection.
    \param    a_recorder  Stores all collision events.
    \param    a_settings  Contains collision settings information.
*/
//===========================================================================
bool cCollisionSpheresTri::computeCollision(cCollisionSpheresGenericShape *a_other,
                                            cCollisionRecorder& a_recorder,
                                            cCollisionSettings& a_settings)
{
    // cast the "other" shape to a line primitive; collision detection is
    // currently only set up to handle line segment - triangle intersections
    cCollisionSpheresLine* line = (cCollisionSpheresLine*)a_other;

    // check for a collision between the primitives (one a triangle and the
    // other a line segment, we assume) by calling the collision detection
    // method of the cTriangle object associated with this triangle primitive;
    // it will only return true if the distance between the segment origin and
    // the triangle is less than the current closest intersecting triangle
    // (whose distance squared is kept in a_colSquareDistance)

    // Don't create these as temporary variables in the function call; this 
    // is not technically a C++-allowable thing to do, since they're passed
    // by reference.
    cVector3d segA = line->getSegmentPointA();
    cVector3d segB = line->getSegmentPointB();

    return (m_original->computeCollision(segA, segB,
                                         a_recorder,
                                         a_settings));
}


//===========================================================================
/*!
    Constructor of cCollisionSpheresLine.

    \fn       cCollisionSpheresLine::cCollisionSpheresLine(
              cVector3d& a_segmentPointA, cVector3d& a_segmentPointB)
    \param    a_segmentPointA   First endpoint of the line segment.
    \param    a_segmentPointB   Second endpoint of the line segment.
*/
//===========================================================================
cCollisionSpheresLine::cCollisionSpheresLine(cVector3d& a_segmentPointA,
                                             cVector3d& a_segmentPointB)
{
    // calculate the center of the line segment
    m_center = cMul(0.5, cAdd(a_segmentPointA, a_segmentPointB));

    // calculate the radius of the bounding sphere as the distance from the
    // center of the segment (calculated above) to an endpoint
    m_radius = cDistance(m_center, a_segmentPointA);

    // store segment
    m_segmentPointA = a_segmentPointA;
    m_segmentPointB = a_segmentPointB;
}


//===========================================================================
/*!
    Determine whether there is any intersection between the primitives
    (this line and the given triangle) by calling the collision detection
    method of the triangle primitive.

    \fn       bool cCollisionSpheresLine::computeCollision(cCollisionSpheresGenericShape *a_other,
                                             cCollisionRecorder& a_recorder,
                                             cCollisionSettings& a_settings)
    \param    a_other  The triangle primitive to check for intersection.
    \param    a_recorder  Stores all collision events.
    \param    a_settings  Contains collision settings information.
    \return   Return whether the given triangle intersects this line.
*/
//===========================================================================
bool cCollisionSpheresLine::computeCollision(cCollisionSpheresGenericShape *a_other,
                                             cCollisionRecorder& a_recorder,
                                             cCollisionSettings& a_settings)
{
    // check for a collision between the primitives (one a triangle and the
    // other a line segment, we assume) by calling the collision detection
    // method of the triangle primitive; it will only return true if the
    // distance between the segment origin and the triangle is less than the
    // current closest intersecting triangle (whose distance squared is kept
    // in a_colSquareDistance)
    return a_other->computeCollision(this, a_recorder, a_settings);
}






