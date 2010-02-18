
#ifndef _ZCAM_HEADER1_H_
#define _ZCAM_HEADER1_H_

#define X_DIMENSION_MAX				(512)
#define Y_DIMENSION_MAX				(512)


bool initialize_DMachine_Engine();

void process_zcam_data(short handTrackingMode = 0, short pixel_zoom_on = 0); // 0 = single hand tracking mode
																			 // 1 = Both hands tracking mode
void update_screen_multiplication_factors(int new_x_dimension, int new_y_dimension); // when the window is resized..

// Query functions for zcam
void get_zcam_params(int &left_hand_flag, int &right_hand_flag, float &x_SHM, float &y_SHM, float &z_SHM, int &fingerUp_SHM, float &fingerDir_SHM); // single hand mode
void get_zcam_params_both_hands(int &left_hand_flag, int &right_hand_flag, float &x_RIGHT, float &y_RIGHT, float &z_RIGHT, int &fingerUp_RIGHT, float &x_LEFT, float &y_LEFT, float &z_LEFT, int &fingerUp_LEFT); // both hands mode

void close_DMachine_Engine();


// filter control variables..
void reset_filter_SHM(void);

// Access to the most recent depth frame
void get_recent_IR_frame(unsigned char* recent_IR_frame);


#endif // for #define ZCAM_HEADER1_H_