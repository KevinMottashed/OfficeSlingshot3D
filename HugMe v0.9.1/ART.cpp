#include "ART.h"

ART::ART() : m_vconf("Data//WDM_camera_flipV.xml"),
			 m_cparam_name("Data//camera_para.dat"),
			 m_num_patt(0), m_thresh(100), m_pImage(NULL)
{
	// set the initial camera parameters //
	if( arParamLoad(m_cparam_name, 1, &m_wparam) < 0 ) {
		printf("Camera parameter load error !!\n");
		exit(0);
	}
}

ART::~ART()
{
}

void ART::SetImageSize(int a_imageSizeX, int a_imageSizeY)
{	
	m_imageSizeX = a_imageSizeX;
	m_imageSizeY = a_imageSizeY;

	arParamChangeSize( &m_wparam, m_imageSizeX, m_imageSizeY, &m_cparam );
    arInitCparam( &m_cparam );

	m_pImage = new unsigned char[4*m_imageSizeX*m_imageSizeY*sizeof(unsigned char)];
}

void ART::RegisterMarker(char * a_patt_name, double a_patt_width)
{
	int a_patt_id;
	if( (a_patt_id=arLoadPatt(a_patt_name)) < 0 ) {
		printf("pattern load error !!\n");
		exit(0);
	}
	
	MarkerInfo marker_info;
	marker_info.m_patt_name = new char[strlen(a_patt_name)];
	strcpy(marker_info.m_patt_name, a_patt_name);
	marker_info.m_patt_id = a_patt_id;
	marker_info.m_patt_width = a_patt_width;
	marker_info.m_patt_center[0] = 0.0;
	marker_info.m_patt_center[1] = 0.0;

	m_marker_info.push_back(marker_info);
	
	m_num_patt++;
}

void ART::SetImage(unsigned char *a_pImage)
{
	memcpy(m_pImage, a_pImage, 4*m_imageSizeX*m_imageSizeY*sizeof(unsigned char));

	if( m_count == 0 ) arUtilTimerReset();
		m_count++;
}

void ART::DetectMarker(void)
{
	int i, j;

	if(!m_pImage)
		return;

	ARMarkerInfo    *marker_info;
	int             detected_num_marker;
	
	// detect the markers in the video frame //
	if( arDetectMarker(m_pImage, m_thresh, &marker_info, &detected_num_marker) < 0 ) {
		//cleanup();
		exit(0);
	}

	for(i=0; i < m_num_patt; i++)
		m_marker_info[i].m_is_detected = false;

	// check for object visibility //
	if(detected_num_marker == 0)
		return;

	for(i=0; i < m_num_patt; i++) {
		int k = -1;
		for( j = 0; j < detected_num_marker; j++ ) {
			/*
			//if( m_patt_id[i] == marker_info[j].id ) {
			if( patt_id == marker_info[j].id ) {
				if( k == -1 ) k = j;
				else if( marker_info[k].cf < marker_info[j].cf ) k = j;
			}*/
			if( m_marker_info[i].m_patt_id == marker_info[j].id ) {
				if( k == -1 ) k = j;
				else if( marker_info[k].cf < marker_info[j].cf ) k = j;
			}
		}
		// get the transformation between the marker and the real camera //
		if(k!= -1) {
			//arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);
			m_marker_info[i].m_is_detected = true;
			arGetTransMat(&marker_info[k], m_marker_info[i].m_patt_center, m_marker_info[i].m_patt_width, m_marker_info[i].m_patt_trans);
			argConvGlpara(m_marker_info[i].m_patt_trans, m_marker_info[i].m_gl_para);
			gl2chai(m_marker_info[i].m_gl_para, m_marker_info[i].m_chai_pos, m_marker_info[i].m_chai_rot);			
		}
	}
}


// from transform matrix in OpenGL workspace to in hl 
// matrix1 : source in opengl
// matrix2 : destination in hl
void ART::gl2chai(double * transform, cVector3d & pos, cMatrix3d & rot)
{
	pos.set(-transform[12],-transform[13],-transform[14]);
	rot.set(transform[0], -transform[4], -transform[8],
			transform[1], -transform[5], -transform[9],
			transform[2], -transform[6], -transform[10]);
}

// from transform matrix in hl workspace to in OpenGL
// matrix1 : source in hl
// matrix2 : destination in opengl
void ART::chai2gl(cVector3d & pos, cMatrix3d & rot, double * transform)
{
	transform[0] = rot.m[0][0];
	transform[1] = -rot.m[1][0];
	transform[2] = -rot.m[2][0];
	transform[3] = 0.0;
	transform[4] = rot.m[0][1];
	transform[5] = -rot.m[1][1];
	transform[6] = -rot.m[2][1];
	transform[7] = 0.0;
	transform[8] = rot.m[0][2];
	transform[9] = -rot.m[1][2];
	transform[10] = -rot.m[2][2];
	transform[11] = 0.0;
	transform[12] = pos[0];
	transform[13] = -pos[1];
	transform[14] = -pos[2];
	transform[15] = 1.0;
}