#ifndef ART_H
#define ART_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <vector>
#include "CVector3d.h"
#include "CMatrix3d.h"

using namespace std;



class ART {
public:
	char			*m_vconf;
	int             m_imageSizeX;
	int             m_imageSizeY;
	int             m_thresh;
	int             m_count;

	ARParam			m_wparam;
	char			*m_cparam_name;
	ARParam         m_cparam;
	ARUint8			*m_pImage;
	double			*m_gl_cpara;

	class MarkerInfo {
	public:
		char*	m_patt_name;
		
		int     m_patt_id;
		double  m_patt_width;
		double  m_patt_center[2];
		bool	m_is_detected;
		double  m_patt_trans[3][4];
		double  m_gl_para[16];
		cVector3d m_chai_pos;
		cMatrix3d m_chai_rot;
	};

	int		m_num_patt;
	vector<MarkerInfo> m_marker_info;

	ART();
	virtual ~ART();

	void SetImageSize(int a_imageSizeX, int a_imageSizeY);

	void RegisterMarker(char * a_patt_name, double a_patt_width = 80.0);

	void SetImage(unsigned char *a_pImage);

	void DetectMarker(void);

	void gl2chai(double * transform, cVector3d & pos, cMatrix3d & rot);
	void chai2gl(cVector3d & pos, cMatrix3d & rot, double * transform);
};

#endif ART_H
