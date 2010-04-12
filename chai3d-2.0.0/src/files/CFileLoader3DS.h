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
    \author    Lev Povalahev
    \author    Dan Morris
    \version   2.0.0 $Rev: 251 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CFileLoader3DSH
#define CFileLoader3DSH
//---------------------------------------------------------------------------
#include "math/CMatrix3d.h"
#include "math/CVector3d.h"
#include "graphics/CVertex.h"
#include "graphics/CTriangle.h"
#include "scenegraph/CMesh.h"
#include "graphics/CMaterial.h"
#include "graphics/CTexture2D.h"
#include "scenegraph/CWorld.h"
#include "scenegraph/CLight.h"
#include <string>
#include <vector>
#include <stdio.h>
//---------------------------------------------------------------------------
using std::string;
using std::vector;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       CFileLoader3DS.h

    \brief    
    <b> Files </b> \n 
    3DS Format 3D Model Loader.
*/
//===========================================================================

//---------------------------------------------------------------------------
// GLOBAL UTILITY FUNCTIONS:
//--------------------------------------------------------------------------- 

/*!
    \ingroup    files
    \brief
    Loads a 3DS image file by providing a filename and mesh in which object is loaded.
*/
bool cLoadFile3DS(cMesh* iMesh, const string& iFileName);

/*!
    Clients can use this to tell the 3ds loader how to behave in terms
    of vertex merging. \n

    If \b true (default), loaded 3ds files will have three _distinct_ vertices
    per triangle, with no vertex re-use.
*/
extern bool g_3dsLoaderShouldGenerateExtraVertices;


//---------------------------------------------------------------------------
#ifndef DOXYGEN_SHOULD_SKIP_THIS
//---------------------------------------------------------------------------

//===========================================================================
/*
  The remainder of this file comes from Lev Povalahev's l3ds.h
  copyright (c) 2001-2002 Lev Povalahev
*/
//===========================================================================

#if defined(_MSVC)
// Tell VC++ not to pad any structures
#pragma pack(push)
#pragma pack(1)
#endif

//===========================================================================
// copyright (c) 2001-2002 Lev Povalahev
// this is a 3ds importer version 2
//
// Modified by Dan Morris, 2004
//===========================================================================

typedef unsigned int uint;
typedef unsigned char byte;

enum LShading {sWireframe, sFlat, sGouraud, sPhong, sMetal};

enum LOptimizationLevel {oNone, oSimple, oFull};

// for internal use
struct LChunk;
struct LTri;

//---------------------------------------------------------------------------

struct LVector4
{
    float x;
    float y;
    float z;
    float w;
};

//---------------------------------------------------------------------------

struct LVector3
{
    float x;
    float y;
    float z;
    LVector3()
    {
        x = y = z = 0;
    }
    LVector3(float x, float y, float z)
    {
        this->x = x; this->y = y; this->z = z;
    }

};

//---------------------------------------------------------------------------

struct LVector2
{
    float x;
    float y;
};

//---------------------------------------------------------------------------

struct LColor3
{
    float r;
    float g;
    float b;
};

//---------------------------------------------------------------------------

struct LChunk
{
    unsigned short id;
    uint start;
    uint end;
};

//---------------------------------------------------------------------------

struct LTri
{
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned long smoothingGroups;
    LVector3 normal;
    LVector3 tangent;
    LVector3 binormal;
    uint materialId;
    LTri()
    {
      a = b = c = 0;
      smoothingGroups = 0;
      materialId = 0;
    }
};

//---------------------------------------------------------------------------

struct LTriangle
{
    unsigned short a;
    unsigned short b;
    unsigned short c;
};

//---------------------------------------------------------------------------

struct LMatrix4
{
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

//---------------------------------------------------------------------------

struct LTriangle2
{
    LVector4 vertices[3];
    LVector3 vertexNormals[3];
    LVector2 textureCoords[3];
    LVector3 faceNormal;
    LColor3 color[3];
    uint materialId;
};

//---------------------------------------------------------------------------

// a structure for a texture map
struct LMap
{
    // the strength of the texture map
    float strength;
    // the file name of the map. only 8.3 format in 3ds files :(
    char mapName[255];
    float uScale;
    float vScale;
    float uOffset;
    float vOffset;
    float angle;
};

//---------------------------------------------------------------------------

class LObject
{
  public:
    // the default constructor, initializes the m_name here
    LObject();

    // the destructor frees memory (m_name)
    virtual ~LObject();

    // call this to get the name of the object
    virtual const string& GetName();

    // this methods should not be used by the "user", they're used internally to fill the class
    // with valid data when reading from file. If you're about to add an importer for another format you'LL
    // have to use these methods
    // call this to set the name of the object
    virtual void SetName(const string& value);

    // returns true if the object's name is the name passed as parameter
    bool IsObject(const string &name);

  protected:
    // the name of the object
    string m_name;
};

//---------------------------------------------------------------------------

class LMaterial : public LObject
{
  public:
    // the default constructor, does the initialization
    LMaterial();

    // the destructor
    virtual ~LMaterial();

    // returns the material ID
    uint GetID();

    // returns the pointer to the texture map 1
    LMap& GetTextureMap1();

    // returns the pointer to the texture map 2
    LMap& GetTextureMap2();

    // returns the pointer to the opacity map
    LMap& GetOpacityMap();

    // returns the pointer to the specular (gloss) map
    LMap& GetSpecularMap();

    // returns the pointer to the bump map
    LMap& GetBumpMap();

    // returns the pointer to the reflection map
    LMap& GetReflectionMap();

    // returns the ambient color of the material
    LColor3 GetAmbientColor();

    // returns the diffuse color of the material
    LColor3 GetDiffuseColor();

    // returns the specular color of the material
    LColor3 GetSpecularColor();

    // returns the shininess of the material, ranging from 0(matte) to 1(shiny)
    float GetShininess();

    // returns the transparency of the material, ranging from 1(fully transparent) to 0(opaque)
    float GetTransparency();

    // returns the type of shading, see LShading type
    LShading GetShadingType();

    // this methods should not be used by the "user", they're used internally to fill the class
    // with valid data when reading from file. If you're about to add an importer for another format you'LL
    // have to use these methods
    // sets the material ID to "value"
    void SetID(uint value);

    // call this to set the ambient color of the material
    void SetAmbientColor(const LColor3 &color);

    // sets the diffuse color of the material
    void SetDiffuseColor(const LColor3 &color);

    // sets the specular color of the material
    void SetSpecularColor(const LColor3 &color);

    // sets the shininess of the material
    void SetShininess(float value);

    // sets the transparency of the material
    void SetTransparency(float value);

    // sets the shading type
    void SetShadingType(LShading shading);

  protected:
    // the unique material ID
    int m_id;

    // the first texture map
    LMap m_texMap1;

    // the second texture map
    LMap m_texMap2;

    // the opacity map
    LMap m_opacMap;

    // the reflection map
    LMap m_reflMap;

    // the bump map
    LMap m_bumpMap;

    // specular map
    LMap m_specMap;

    // material ambient color
    LColor3 m_ambient;

    // material diffuse color
    LColor3 m_diffuse;

    // material specular color
    LColor3 m_specular;

    // shininess
    float m_shininess;

    // transparency
    float m_transparency;

    // the shading type for the material
    LShading m_shading;
};

//---------------------------------------------------------------------------

class LMesh : public LObject
{
  public:
    // the default constructor
    LMesh();

    // the destructor
    virtual ~LMesh();

    // clears the mesh, deleting all data
    void Clear();

    // returns the number of vertices in the mesh
    uint GetVertexCount();

    // sets the the size of the vertex array - for internal use
    void SetVertexArraySize(uint value);

    // returns the number of triangles in the mesh
    uint GetTriangleCount();

    // sets the size of the triangle array - for internal use
    void SetTriangleArraySize(uint value);

    // returns given vertex
    const LVector4& GetVertex(uint index);

    // returns the given normal
    const LVector3& GetNormal(uint index);

    // returns the given texture coordinates vector
    const LVector2& GetUV(uint index);

    // return the color given the index
    LColor3& GetColor(uint index);

    // returns the pointer to the array of tangents
    const LVector3& GetTangent(uint index);

    // returns the pointer to the array of binormals
    const LVector3& GetBinormal(uint index);

    // sets the vertex at a given index to "vec" - for internal use
    void SetVertex(const LVector4 &vec, uint index);

    // sets the normal at a given index to "vec" - for internal use
    void SetNormal(const LVector3 &vec, uint index);

    // sets the texture coordinates vector at a given index to "vec" - for internal use
    void SetUV(const LVector2 &vec, uint index);

    // sets the color
    void SetColor(const LColor3 &vec, uint index);

    // sets the tangent at a given index to "vec" - for internal use
    void SetTangent(const LVector3 &vec, uint index);

    // sets the binormal at a given index to "vec" - for internal use
    void SetBinormal(const LVector3 &vec, uint index);

    // returns the triangle with a given index
    const LTriangle& GetTriangle(uint index);

    // returns the triangle with a given index, see LTriangle2 structure description
    LTriangle2 GetTriangle2(uint index);

    // returns the mesh matrix, should be identity matrix after loading
    LMatrix4 GetMatrix();

    // sets the mesh matrix to a given matrix - for internal use
    void SetMatrix(LMatrix4 m);

    // optimizises the mesh using a given optimization level
    void Optimize(LOptimizationLevel value);

    // sets an internal triangle structure with index "index" - for internal use only
    void SetTri(const LTri &tri, uint index);

    // returns the pointer to the internal triangle structure - for internal use only
    LTri& GetTri(uint index);

    // returns the material id with a given index for the mesh
    uint GetMaterial(uint index);

    // adds a material to the mesh and returns its index - for internal use
    uint AddMaterial(uint id);

    // returns the number of materials used in the mesh
    uint GetMaterialCount();

    // the vertices, normals, etc.
    vector<LVector4> m_vertices;
    vector<LVector3> m_normals;
    vector<LVector3> m_binormals;
    vector<LVector3> m_tangents;
    vector<LVector2> m_uv;
    vector<LColor3> m_colors;

    // triangles
    vector<LTriangle> m_triangles;

    //used internally
    vector<LTri> m_tris;

    // the transformation matrix.
    LMatrix4 m_matrix;

    // the material ID array
    vector<uint> m_materials;

    // calculates the normals, either using the smoothing groups information or not
    void CalcNormals(bool useSmoothingGroups);

    // calculates the texture(tangent) space for each vertex
    void CalcTextureSpace();

    // transforms the vertices by the mesh matrix
    void TransformVertices();
};

//---------------------------------------------------------------------------

class LCamera : public LObject
{
  public:
    // the default constructor
    LCamera();

    // the destructor
    virtual ~LCamera();

    // clears the data the class holds
    void Clear();

    // sets the position of the camera - for internal use
    void SetPosition(LVector3 vec);

    // returns the position of the camera
    LVector3 GetPosition();

    // sets the target of the camera - internal use
    void SetTarget(LVector3 target);

    // returns the target of the camera
    LVector3 GetTarget();

    // sets the fov - internal use
    void SetFOV(float value);

    // returns the fov
    float GetFOV();

    // sets the bank - internal use
    void SetBank(float value);

    // returns the bank
    float GetBank();

    // sets the near plane - internal use
    void SetNearplane(float value);

    // returns the near plane distance
    float GetNearplane();

    // sets the far plane - internal use
    void SetFarplane(float value);

    // returns the far plane distance
    float GetFarplane();

  protected:
    LVector3 m_pos;
    LVector3 m_target;
	float m_bank;
    float m_fov;
	float m_near;
	float m_far;
};

//---------------------------------------------------------------------------

class LLight : public LObject
{
  public:
    // the default constructor
    LLight();

    // the destructor
    virtual ~LLight();

    // clears the data the class holds
    void Clear();

    // sets the position of the light source - for internal use
    void SetPosition(LVector3 vec);

    // returns the position of the light source
    LVector3 GetPosition();

    // sets the color of the light - for internal use
    void SetColor(LColor3 color);

    // returns the color of the light
    LColor3 GetColor();

    // sets whether the light is a spotlight or not - internal use
    void SetSpotlight(bool value);

    // returns true if the light is a spotlight
    bool GetSpotlight();

    // sets the target of the light - internal use
    void SetTarget(LVector3 target);

    // returns the target of the spotlight
    LVector3 GetTarget();

    // sets the hotspot - internal use
    void SetHotspot(float value);

    // returns the hotspot
    float GetHotspot();

    // sets falloff - internal use
    void SetFalloff(float value);

    // returns falloff
    float GetFalloff();

    // sets attenuationstart - internal use
    void SetAttenuationstart(float value);

    // returns attenuationstart
    float GetAttenuationstart();

    // sets attenuationend - internal use
    void SetAttenuationend(float value);

    // returns attenuationend
    float GetAttenuationend();

  protected:
    LVector3 m_pos;
    LColor3 m_color;
    bool m_spotlight;
    LVector3 m_target;
    float m_hotspot;
    float m_falloff;
	float m_attenuationstart;
	float m_attenuationend;
};

//---------------------------------------------------------------------------

class LImporter
{
  public:
    // the default constructor
    LImporter();

    // the destructor
    virtual ~LImporter();

    // reads the model from a file, must be overriden by the child classes
    virtual bool LoadFile(const char *filename) = 0;

    // returns the number of meshes in the scene
    uint GetMeshCount();

    // returns the number of lights in the scene
    uint GetLightCount();

    // returns the number of materials in the scene
    uint GetMaterialCount();

	// returns the number of cameras in the scene
	uint GetCameraCount();

    // returns a pointer to a mesh
    LMesh& GetMesh(uint index);

    // returns a pointer to a camera at a given index
    LCamera& GetCamera(uint index);

    // returns a pointer to a light at a given index
    LLight& GetLight(uint index);

    // returns the pointer to the material
    LMaterial& GetMaterial(uint index);

    // returns the pointer to the material with a given name, or NULL if the material was not found
    LMaterial* FindMaterial(const string &name);

    // returns the pointer to the mesh with a given name, or NULL if the mesh with such name
    // is not present in the scene
    LMesh* FindMesh(const string &name);

    // returns the pointer to the camera with a given name, or NULL if not found
    LLight* FindCamera(const string &name);

    // returns the pointer to the light with a given name, or NULL if not found
    LLight* FindLight(const string &name);

    // sets the optimization level to a given value
    void SetOptimizationLevel(LOptimizationLevel value);

    // returns the current optimization level
    LOptimizationLevel GetOptimizationLevel();

 protected:
    // the cameras found in the scene
    vector<LCamera> m_cameras;

    // the lights found in the scene
    vector<LLight> m_lights;

    // triangular meshes
    vector<LMesh> m_meshes;

    // the materials in the scene
    vector<LMaterial> m_materials;

    // level of optimization to perform on the meshes
    LOptimizationLevel m_optLevel;

    // clears all data.
    virtual void Clear();
};

//---------------------------------------------------------------------------

class L3DS : public LImporter
{
  public:
    // the default constructor
    L3DS();

    // constructs the object and loads the file
    L3DS(const char *filename);

    // destructor
    virtual ~L3DS();

    // load 3ds file
    virtual bool LoadFile(const char *filename);

  protected:
    // used internally for reading
    char m_objName[100];

    // true if end of file is reached
    bool m_eof;

    // buffer for loading, used for speedup
    unsigned char *m_buffer;

    // the size of the buffer
    uint m_bufferSize;

    // the current cursor position in the buffer
    uint m_pos;

    // reads a short value from the buffer
    short ReadShort();

    // reads an int value from the buffer
    int ReadInt();

    // reads a char from the buffer
    char ReadChar();

    // reads a float value from the buffer
    float ReadFloat();

    // reads an unsigned byte from the buffer
    byte ReadByte();

    // reads an asciiz string
    int ReadASCIIZ(char *buf, int max_count);

    // seek within the buffer
    void Seek(int offset, int origin);

    // returns the position of the cursor
    uint Pos();

    // read the chunk and return it.
    LChunk ReadChunk();

    // read until given chunk is found
    bool FindChunk(LChunk &target, const LChunk &parent);

    // skip to the end of chunk "chunk"
    void SkipChunk(const LChunk &chunk);

    // goes to the beginning of the data in the given chunk
    void GotoChunk(const LChunk &chunk);

    // the function read the color chunk (any of the color chunks)
    LColor3 ReadColor(const LChunk &chunk);

    // the function that read the percentage chunk and returns a float from 0 to 1
    float ReadPercentage(const LChunk &chunk);

    // this is where 3ds file is being read
    bool Read3DS();

    // read a light chunk
    void ReadLight(const LChunk &parent);

	// read a camera chunk
	void ReadCamera(const LChunk &parent);

    // read a trimesh chunk
    void ReadMesh(const LChunk &parent);

    // reads the face list, face materials, smoothing groups... and fill the information into the mesh
    void ReadFaceList(const LChunk &chunk, LMesh &mesh);

    // reads the material
    void ReadMaterial(const LChunk &parent);

    // reads the map info and fills the given map with this information
    void ReadMap(const LChunk &chunk, LMap& map);

    // reads keyframer data of the OBJECT_NODE_TAG chunk
    void ReadKeyframeData(const LChunk &parent);

    // reads the keyheader structure from the current offset and returns the frame number
    long ReadKeyheader();
};

#if defined(_MSVC)
#pragma pack(pop)
#endif

//---------------------------------------------------------------------------
#endif  // DOXYGEN_SHOULD_SKIP_THIS
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------


