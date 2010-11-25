/*
 * globalvals.h
 *
 *  Created on: Nov 18, 2010
 *      Author: paco
 */

#ifndef INVECIL
#define INVECIL
/*
 * globals.h
 *
 *  Created on: Nov 15, 2010
 *      Author: paco
 *      Global variable definitions for ATS program.
 */
#include<stdio.h>
#include<stdlib.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"



#define CVX_DONT_CARE -1

#define CVX_CAMERA	0
#define CVX_AVI		1

#define CVX_RED		CV_RGB(0xff,0x00,0x00)
#define CVX_GREEN	CV_RGB(0x00,0xff,0x00)
#define CVX_BLUE	CV_RGB(0x00,0x00,0xff)

#define CVX_CYAN	CV_RGB(0x00,0xff,0xff)
#define CVX_MAGENTA	CV_RGB(0xff,0x00,0xff)
#define CVX_YELLOW	CV_RGB(0xff,0xff,0x00)

#define CVX_WHITE	CV_RGB(0xff,0xff,0xff)
#define CVX_BLACK	CV_RGB(0x00,0x00,0x00)
#define CVX_GRAY50	CV_RGB(0x88,0x88,0x88)
#define ESC 27
#define CANNY_TRES1  20  // first cannyedge treshold
#define CANNY_TRES2  60 //  second cannt edge treshold
#define DILATE_ITR  4  // Dilate  Itaeration Number
/*capture and frame for camera capture
 * */
#define INPUT_SIZE  256
#define CVX_RESIZE cvSize(INPUT_SIZE,INPUT_SIZE)

/*Definition of the */
#define IS_GLASS 0
#define IS_PLAST 1
#define TRAIN_INPUTS 50 // Number of rows for training Matrix, chang to 1000
#define VERIFYINPUTS 20
#define VALIDATION_INPUTS 20

/*
 * void ReleaseGlobalVals
 *
 * Release Space allocated in all global variables previously defined.
 *
 * */
#endif /* GLOBALS_H_ */
