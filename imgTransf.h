/*
 * imgTransf.h
 *
 *  Created on: Nov 15, 2010
 *      Author: paco
 *
 *      Changelog: changed functions process input and inputFrimImage which no longer reuire resizing
 *
 *      Methods for Image capture from camera and transformation to
 *      detect ellipse and crop image for recognition stage.
 *
 */
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
#include "globalvals.h"


#ifndef IMGTRANSF_H_
#define IMGTRANSF_H_

/*
 * void processInputFromImage
 *#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
 * Inputs: picture filename, image to store frames, Image to store, output to process, gray Image for tresholding
 * Image to copy crop result.WIlll make the Image transforms, ellipse identification and further Histogram Image and
 * Neural net training and validation parts.
 *
  */
CvMat* processInputFromImage(char* filename);

/*
 * void processInputFromCamera
 * Inputs: camera capture, image to store frames, Image to store, output to process, gray Image for tresholding
 * Image to copy crop result.
 *
 * Will make the Image transforms, ellipse identification and further Histogram Image and
 * Neural net Input from camera frame sequences, will be implemented when training of neural net is complete.
 * */

void processInputFromCamera(CvCapture* capture,IplImage* frame,IplImage* output,IplImage* grayImage,IplImage* crop,IplImage* resized);
/*
 * void processInput
 * Inputs:  image to store frames, Image to store, output to process, gray Image for tresholding
 * Image to copy crop result.
 *
 *Takes inputs from either image frames or Image files.
 * Will make the Image transforms, ellipse identification and further Histogram Image and
 * Neural net Input from camera frame sequences, will be implemented when training of neural net is complete.
 * */

CvMat* processInput(IplImage* frame,IplImage* output,IplImage* grayImage,IplImage* crop);




/*
	 * void Preprocessing
	 *
	 * Inputs:
	 * Source Colour Image
	 * Destination GrayScale Image
	 * Treshold parameters for canny edge: tresh1 and tresh2
	 * Number of Iterations for Dilate Operation
	 *
	 *
	 *
	 * Perform pre-processing stage of Image:
	 * 	Reduce size
	 * 	Convert to grayscale
	 * 	Perform canny edge detection
	 * 	Dilate Image
	 *
	 *
	 *
	 * */




void preProcessing(IplImage* src,IplImage* grayImg,int cannyTresh1,int cannyTresh2,int dilateIter);


/*
 *Ipl Image  findCropImage
 *	Finds Image contours of binary Image, then uses FitEllipse to get set the ROI
 *	of the source Image and crop it. The fit ellipse function will onbtain the best fitting contour
 *	using the treeNode Iterator. And will draw a rectangle on the source Image where the ellipse was
 *	found.
 *
 *
 *	Inputs:
 *	- Original source Image
 *	- Pre-processed Image
 *	- Image to store cropped contour containing ellipse that was found
 *	-
 *	Returns : - cropped Image or NULL if no ellipse contours were found
 *
 * */



IplImage* findCropImage(IplImage* src,IplImage* grayImg);

/*
 * histogram Function.
 *
 * Future Function for further image processing.
 *
 * Inputs : cropped Image, histogram.
 * Outputs: calculation of histogram inputs for ANN_MLP.
 *
 *
 * */

/*
	 * void getDFTArray
	 * Inputs: Image , CvMat to store array of DFT
	 *
	 * Output: computes the DFT of image and magnitude,
	 *
	 * Notes: Obtained from OpenCV Samples to test efficay as input for ANN.
	 *
	 *
	 *
	 *
	 * */
	CvMat* getFFTArray(IplImage* grayImage);
/*
 *
 * cvMat* get histogram Array
 * Creates a 128 bin histogram and sends values to array, display hist for debugging
 *	fase
 * */
	CvMat* getHistogramArray (IplImage* grayImage);


/*draw hist to probe functionality*/
	IplImage* DrawHistogram(CvHistogram *hist);

/*shift image to center to test picture of DFT*/
	void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr );



#endif /* IMGTRANSF_H_ */
