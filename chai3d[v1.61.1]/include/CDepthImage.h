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

//---------------------------------------------------------------------------
//#ifdef _MSVC
//#pragma warning (disable : 4786)
//#endif
//---------------------------------------------------------------------------
#ifndef CDepthImageH
#define CDepthImageH
//---------------------------------------------------------------------------
#include "CVector3d.h"
#include "CGenericObject.h"
#include "CCollisionBrute.h"
#include "CFileLoaderBMP.h"
#include "cTriangle.h"
#include "CFileLoaderBMP.h"
#include "CProxyGraphPointForceAlgo.h"
#include <float.h>
//---------------------------------------------------------------------------
#include "gl/glu.h"
#include <vector>
#include <list>
#include <conio.h>
// by gaecha
#include "CImageMaterial.h"
//
//---------------------------------------------------------------------------
using std::list;
using std::vector;
//---------------------------------------------------------------------------
class cWorld;
class cTriangle;
class cVertex;
//---------------------------------------------------------------------------
#define SGN(x) x==0.0?0:(int)(x/fabs(x))
#define FLOOR(x) (int)(x)
#define CEIL(x) x-(double)((int)x)==0.0?(int)x:(int)x+1
#define ROUND(x) (x<0?CEIL((x)-0.5):FLOOR((x)+0.5))

#ifndef MIN
#define MIN(X, Y) ((X)<(Y)?(X):(Y))
#endif
#ifndef MAX
#define MAX(X, Y) ((X)>(Y)?(X):(Y))
#endif

//===========================================================================
/*!
      \class      cDepthImage
      \brief      cDepthImage represents a collection of vertices, triangles, materials,
                  and texture properties that can be rendered graphically and
                  haptically.
*/
//===========================================================================
class cDepthImage : public cGenericObject
{
public:
	// for first test
	bool m_bFirstUpdateForProxyGraph;

	// camera parameters
	double m_focalLength;
	double m_pixelWidth;
	double m_primaryDistance;
	double m_primaryWidth;

	// smoothing
	int m_smoothingArea;
	bool m_useSmoothing;

	// by gaecha
	//! previous surface depth for proxy position correction
	cVector3d m_previousSurfacePoint;
	//! Is proxy graph algorithm working:
	bool m_bProxyGraph;
	//! Elapsed haptic time
	long m_lElapsedGraphicTime;
	//! Elapsed haptic time
	long m_lElapsedHapticTime;
	//! Time ratio
	double m_timeRatio;

	int m_collisionVoxelNum;
	int m_collisionCandidateVoxelNum;

	cMesh* m_dynamicMesh;

	void useSmoothing(const bool a_useSmoothing);
	
	bool m_useImageMaterial;
	bool m_useImageBump;
	cImageMaterial m_imageMaterial;
	unsigned int m_depthImageSizeX;
	unsigned int m_depthImageSizeY;
	unsigned int m_halfDepthImageSizeX;
	unsigned int m_halfDepthImageSizeY;
	unsigned int m_colorImageSizeX;
	unsigned int m_colorImageSizeY;
	unsigned int m_halfColorImageSizeX;
	unsigned int m_halfColorImageSizeY;

	cVector3d correctProxy(const cVector3d a_proxy);
	bool correctProxyOnImage(cVector3d& a_proxy);

	void useImageBump(const bool a_useImageBump, const bool a_affectChildren = false);
	void setImageData(cImageLoader *a_colorImage, cImageLoader *a_depthImage);
	void setImageData(unsigned char * a_colorData, unsigned char * a_depthData);
	void setImageData(cImageLoader *a_image);
	void setImageData(unsigned char * a_imageData);
	void setColorData(unsigned char * a_colorData);
	void setDepthData(unsigned char * a_depthData);
	
	void setElapsedGraphicTime(float a_frameRate);
	void setColorImageProperties(unsigned int a_sizeX, unsigned int a_sizeY, unsigned char a_totalChannel, unsigned char a_channelNum);
	void setDepthImageProperties(unsigned int a_sizeX, unsigned int a_sizeY, unsigned char a_totalChannel, unsigned char a_channelNum);
	double getDepth(int a_x, int a_y);

	double getStiffness(cVector3d a_proxy);
	double getStaticFriction(cVector3d a_proxy);
	double getDynamicFriction(cVector3d a_proxy);
	double getBump(int a_x, int a_y);
	double getStiffnessLocal(cVector3d a_proxy);
	double getStaticFrictionLocal(cVector3d a_proxy);
	double getDynamicFrictionLocal(cVector3d a_proxy);
	double getBumpLocal(int a_x, int a_y);

	cVector3d projectOnDepthImage(cVector3d a_point);
	cVector3d projectOnDepthImageGlobal(cVector3d a_point);

	cVector3d updatePrevisouSurfaceDepthLocal(cVector3d& a_segmentPointA);
	cVector3d updatePrevisouSurfaceDepthGlobal(cVector3d& a_segmentPointA);

	void renderDepthImage(const int a_renderMode);

	cVector3d trans2cartesian(cVector3d a_point);
	cVector3d trans2image(cVector3d a_point);

	void setCameraParameters(double a_focalLength, double a_pixelWidth, 
									  double a_primaryDistance, double a_primaryWidth);

	int m_renderSample;
	void setRenderSample(int a_renderSample) {m_renderSample = a_renderSample;}


    //! If \b true, then normals are displayed.
    bool m_showNormals;
    //! Color used to render lines representing normals
    cColorf m_showNormalsColor;
    //! Length of each normal (for graphic rendering of normals)
    double m_showNormalsLength;

    //===========================================================================
    /*! 
        If true, transparency is enabled... this turns alpha on when the mesh is
        rendered, and - if multipass transparency is enabled in the rendering camera -
        uses the camera's multiple rendering passes to approximate back-to-front
        sorting via culling
    */
    //===========================================================================
    bool m_useTransparency;

    //===========================================================================
    /*! 
        If true, multi-pass transparency is permitted for this mesh... this means
        that if the rendering camera is using multipass transparency, this mesh
        will render back and front faces separately.

        Note that m_useTransparency also has to be 'true' for this variable to
        be meaningful.
    */
    //===========================================================================
    bool m_useMultipassTransparency;

    //===========================================================================
    /*! 
        Should culling be used when rendering triangles?
    
        Note that this option only applies when multipass transparency is 
        disabled or during the non-transparent rendering pass when multipass
        transparency is enabled...
        
        Also note that currently only back-faces are culled during non-transparent
        rendering; you can't cull front-faces.
    */
    //===========================================================================    
    bool m_cullingEnabled;
    
    //! The polygon rendering mode (GL_FILL or GL_LINE)
    int m_triangleMode;
    //! Should per-vertex colors be used?
    bool m_useVertexColors;
    //! Should texture mapping be used?
    bool m_useTextureMapping;
	
    //! Set the material for this mesh, and optionally pass it on to my children
    void setImageMaterial(cImageMaterial& a_mat, const bool a_affectChildren=false);
    //! Enable or disable the use of material properties, optionally propagating the operation to my children
    void useImageMaterial(const bool a_useMaterial, const bool a_affectChildren=true);

	void getSize(int &sizeY, int &sizeZ) const;
	virtual bool computeCollisionDetection(cVector3d& a_segmentPointA, const cVector3d& a_segmentPointB,
         cGenericObject*& a_colObject, cTriangle*& a_colTriangle, cVector3d& a_colPoint,
         double& a_colSquareDistance, const bool a_visibleObjectsOnly=false, const int a_proxyCall=-1);
	void setScale(const double a_scaleFactor);
	double getScale();
	void scaleObject(double a_scaleFactor);
	void clear();
	bool updateImage();
	cDepthImage(cWorld* a_parent);
	virtual ~cDepthImage();

    //! Get parent world
    cWorld* getParentWorld() { return (m_parentWorld); }

    //! Set the haptic stiffness, possibly recursively affecting children
    void setStiffness(double a_stiffness, const bool a_affectChildren=0);
    //! Set the static and dynamic friction for this mesh, possibly recursively affecting children
    void setFriction(double a_staticFriction, double a_dynamicFriction, const bool a_affectChildren=0);

	//! Descend through child objects to compute interaction forces for all cGenericPotentialFields
    virtual cVector3d computeForces(const cVector3d& a_probePosition);

protected:
    //! Render this object in OpenGL
    virtual void render(const int a_renderMode=CHAI_RENDER_MODE_RENDER_ALL);
	cWorld* m_parentWorld;


	// depth image information
	bool m_bDepthData;
	cImageLoader* m_depthImage;
	unsigned char * m_depthData;
	unsigned char * m_nextDepthData;
	unsigned char * m_tempDepthData;
	unsigned int m_depthTotalChannel;
	unsigned int m_depthChannelNum;

	// rgb image information
	bool m_bColorData;
	cImageLoader* m_colorImage;
	unsigned char * m_colorData;
	unsigned char * m_nextColorData;
	unsigned char * m_tempColorData;
	unsigned int m_colorTotalChannel;
	unsigned int m_colorChannelNum;

	double m_scaleFactor;
	bool m_bConnectivity[1000];
	int m_collisionVoxel[1000][2];
	int m_collisionCandidateVoxel[1000][2];
	//! Is the first image data update?
	bool m_bFirstUpdate;
	//! 
	bool m_bUpdate;


	void addCandidateVoxel(int x, int y1, int y2, int sgnY, int & num);
	void lineDDA(double x1, double y1, double z1, double x2, double y2, double z2);
	void updateBoundaryBox();
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
