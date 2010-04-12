//===========================================================================
/*
    This file is part of the GEL dynamics engine.
    Copyright (C) 2003-2009 by Francois Conti, Stanford University.
    All rights reserved.

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   2.0.0 $Rev: 251 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CGELMassParticleH
#define CGELMassParticleH
//---------------------------------------------------------------------------
#include "chai3d.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       CGELMassParticle.h

    \brief 
    <b> GEL Module </b> \n 
    Simple Mass Particle.
*/
//===========================================================================

//===========================================================================
/*!
    \class      cGELMassParticle
    \ingroup    GEL

    \brief      
    cGELMassParticle defines a simple mass particle point.
*/
//===========================================================================
class cGELMassParticle
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of cGELMassParticle.
    cGELMassParticle();

    //! Destructor of cGELMassParticle.
    virtual ~cGELMassParticle();


	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    //! Set the mass of the particle.
    void setMass(double a_mass);

    //! Add force to mass particle.
    inline void addForce(cVector3d &a_force)
    {
        m_force.add(a_force);
    }

    //! Set an external force to mass particle.
    inline void setExternalForce(cVector3d &a_force)
    {
        m_externalForce = a_force;
    }

    //! Compute next position.
    inline void computeNextPose(double a_timeInterval)
    {
        if (!m_fixed)
        {
            // Euler double integration for position
            cVector3d damping;
            m_vel.mulr(-m_kDampingPos * m_mass, damping);
            m_force.add(damping);
            m_acc = cDiv(m_mass, cAdd(m_force, m_externalForce));
            m_vel = cAdd(m_vel, cMul(a_timeInterval, m_acc));
            m_nextPos = cAdd(m_pos, cMul(a_timeInterval, m_vel));
        }
        else
        {
            m_nextPos = m_pos;
        }
    }

    //! Update pose with new computed values.
    inline void applyNextPose()
    {
        m_pos = m_nextPos;
    }

    //! Clear forces.
    inline void clearForces()
    {
        if (m_useGravity)
        {
            m_gravity.mulr(m_mass, m_force);
        }
        else
        {
            m_force.zero();
        }
    }

    //! Clear external force.
    inline void clearExternalForces()
    {
        m_externalForce.zero();
    }

    //! Render node in OpenGL.
    inline void render()
    {
        // draw point
        m_color.render();
        glBegin(GL_POINTS);
          glVertex3dv( (const double *)&m_pos);
        glEnd();

        // render external forces
        glColor4fv( (const float *)&m_color);
        cVector3d v = cAdd(m_pos, cMul(scale_force_vector_display, m_externalForce));
        glBegin(GL_LINES);
          glVertex3dv( (const double *)&m_pos);
          glVertex3dv( (const double *)&v);
        glEnd();
    }


	//-----------------------------------------------------------------------
    // MEMBERS - PHYSICAL PROPERTIES:
    //-----------------------------------------------------------------------

    //! Mass property.
    double m_mass;

    //! Current force being applied.
    cVector3d m_force;

    //! Instant acceleration.
    cVector3d m_acc;

    //! Instant velocity.
    cVector3d m_vel;

    //! Linear damping.
    double m_kDampingPos;

    //! If \b true, then mass is fixed in space and cannot move.
    bool m_fixed;

    //! If \b true, then gravity field is enabled.
    bool m_useGravity;

    //! Gravity field.
    cVector3d m_gravity;


	//-----------------------------------------------------------------------
    // MEMBERS - GRAPHICAL PROPERTIES:
    //-----------------------------------------------------------------------

    //! Color used to display nodes.
    cColorf m_color;


  public:

	//-----------------------------------------------------------------------
    // MEMBERS - POSITION:
    //-----------------------------------------------------------------------

    //! Position.
    cVector3d m_pos;

    //! Next position computed.
    cVector3d m_nextPos;


  private:
 	
    //-----------------------------------------------------------------------
    // MEMBERS - EXTERNAL FORCES:
    //-----------------------------------------------------------------------
      
    //! External force.
    cVector3d m_externalForce;


  public:

	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    //! Default property - linear damping.
    static double default_kDampingPos;

    //! Default property - mass.
    static double default_mass;

    //! Default property - color.
    static cColorf default_color;

    //! Default property - use of gravity field.
    static bool default_useGravity;

    //! Default property - gravity field.
    static cVector3d default_gravity;

    //! Default property - force display status.
    static bool show_forces;

    //! Default property - force vector display magnitude.
    static bool scale_force_vector_display;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------


