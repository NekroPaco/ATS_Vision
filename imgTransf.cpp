/*
 * imgTransf.cpp
 *
 *
 * Methods for image extraction from files or camera frames and
 * image processing and histogram computation
 *
 * changelog: eliminated Resizing of Image
 *
 *  Created on: Nov 15, 2010
 *      Author: paco
 */
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
#include"imgTransf.h"
#include "globalvals.h"



/*
 *
 * void GetInputFromImage
 * Inputs: Filename string
 * opens Image and converts it in IplImage data type.
 * */





		CvMat* processInputFromImage(char* filename){
 			IplImage* frame = NULL;
			IplImage* output = NULL;
			IplImage* grayImage = NULL;
			IplImage* crop = NULL;
			CvMat*input;
			//printf("\n %s\n",filename);
			//printf("Input from Image ");
			frame = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR );
			input = processInput(frame,output,grayImage,crop);
			//cvShowImage("Frame",grayImage);
		//	while(char c = cvWaitKey(33)!= ESC );
			cvReleaseImage(&crop);
			cvReleaseImage(&output);
			cvReleaseImage(&grayImage);
			cvReleaseImage(&frame);
			cvDestroyWindow("Image Input");
			cvDestroyAllWindows();
			return input;
		}

		/*
		 * void processInputFromCamera
		 * Inputs: camera capture, image to store frames, Image to store, output to process, gray Image for tresholding
		 * Image to copy crop result.
		 *
		 * Will make the Image transforms, ellipse identification and further Histogram Image and
		 * Neural net Input from camera frame sequences, will be implemented when training of neural net is complete.
		 * */


		void processInputFromCamera(CvCapture* capture,IplImage* frame,IplImage* output,IplImage* grayImage,IplImage* crop){
			/* Declare windows that will show Images for canny edge transformed Images and */

			printf("input from camera \n");
			capture = cvCreateCameraCapture(0);
			while(true){
				frame = cvQueryFrame( capture );
				output = cvCreateImage(cvSize( frame->width/2, frame->height/2 ),IPL_DEPTH_8U,frame->nChannels);
				cvPyrDown( frame, output);
				grayImage = cvCreateImage(cvSize( output->width, output->height ),IPL_DEPTH_8U,1);
				preProcessing(output,grayImage,CANNY_TRES1,CANNY_TRES2,DILATE_ITR);
				;
				cvShowImage("Tresholded", grayImage);

			                if ((crop = findCropImage(output,grayImage)) != NULL){

			                	cvReleaseImage(&grayImage);
			           			grayImage  =  cvCreateImage(cvSize( output->width, output->height ),IPL_DEPTH_8U,1);
			                	cvSmooth(grayImage, grayImage, CV_GAUSSIAN, 3, 3 );
			                	cvShowImage("cropped", crop);
			                	cvShowImage("Frame",output);
			                	cvShowImage("cropped", crop);
			                	cvShowImage("Input",grayImage);
			                	cvReleaseImage(&crop);
			                }
			     //   if(char c = cvWaitKey(23)== ESC)
		     		break;
		        cvReleaseImage (&grayImage);
		        cvReleaseImage(&output);
			}

		}



		/*
		 * CvMat* processInput
		 * Inputs:  image to store frames, Image to store, output to process, gray Image for tresholding
		 * Image to copy crop result.
		 *`Returns : CvMAt with histogram array or NULL if no ellipse was found, meaning that an image cannot be obtained
		 *Takes inputs from either image frames or Image files.
		 * Will make the Image transforms, ellipse identification and further Histogram Image and
		 * Neural net Input from camera frame sequences, will be implemented when training of neural net is complete.
		 * */
	CvMat* processInput(IplImage* frame,IplImage* output,IplImage* grayImage,IplImage* crop){
		CvMat* trainMat;
		cvNamedWindow("Tresholded",CV_WINDOW_AUTOSIZE);
		//cvNamedWindow("Frame",CV_WINDOW_AUTOSIZE);
		//cvNamedWindow("cropped",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Input ",CV_WINDOW_AUTOSIZE);
		output = cvCreateImage(cvSize( frame->width, frame->height ),IPL_DEPTH_8U,frame->nChannels);
		cvCopy( frame, output);
		grayImage = cvCreateImage(cvSize( output->width, output->height ),IPL_DEPTH_8U,1);
		preProcessing(output,grayImage,70,150,3);

		cvShowImage("Tresholded", grayImage);

		if ((crop = findCropImage(output,grayImage)) != NULL){
		 // crop Image found resize it and transform to grayscale
			cvReleaseImage (&grayImage);
			grayImage  =  cvCreateImage(cvSize(crop->width,crop->height),IPL_DEPTH_8U,1);
			cvCvtColor(crop,grayImage,CV_BGR2GRAY);
			// cvSmooth(grayImage, grayImage, CV_GAUSSIAN, 3, 3 );
			//printf("get hist Array");
			//printf("%d %d",grayImage->width,grayImage->height);
			//if(!(trainMat  = getFFTArray(grayImage))) //  try DFT sample, need to debug.
			trainMat = getHistogramArray(grayImage);
			//cvShowImage("Frame",output);
			//cvShowImage("cropped", crop);
			cvShowImage("Input",grayImage);
			return trainMat;
		}
		else{
			printf("No input Crop Image Found \n");
			return NULL;
		}

	}



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
	void preProcessing(IplImage* src,IplImage* grayImg,int cannyTresh1,int cannyTresh2,int dilateIter)
	{
		        cvCvtColor( src, grayImg, CV_BGR2GRAY );
		        cvCanny( grayImg, grayImg, cannyTresh1, cannyTresh2, 3 );
		        cvDilate(grayImg,grayImg,NULL,dilateIter);
	}

	/*
	 *IplImage findCropImage
	 *	Finds Image contours of binary Image, then uses FitEllipse to get set the ROI
	 *	of the source Image and crop it. The fit ellipse function will onbtain the best fitting contour
	 *	using the treeNode Iterator. And will draw a rectangle on the source Image where the ellipse was
	 *	found.
	 *
	 *	Inputs:
	 *	- Original source Image
	 *	- Pre-processed Image
	 *	- Image to store cropped contour containing ellipse that was found
	 *	Returns : - cropped Image or NULL if no ellipse contours were found
	 *
	 *
	 * */

	IplImage* findCropImage(IplImage* src,IplImage* grayImg){
		IplImage* cropped = NULL;
		CvMemStorage *storage = cvCreateMemStorage (0);
		CvBox2D ellipse;
		CvSeq *contours = 0;
		CvTreeNodeIterator it;
		CvPoint2D32f pt[4];
		int nContours = cvFindContours (grayImg, storage, &contours, sizeof (CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint (0, 0));
		        printf( "Total Contours Detected: %d\n", nContours );
		        if(nContours > 0 ){
		        	cvInitTreeNodeIterator (&it, contours, 2) ;
		        	int n = 0 ;
		        	while ((contours = (CvSeq *) cvNextTreeNode (&it)) != NULL && n==0 ) {
		        		//printf("Contour #%d\n", n );
		        		//printf("%d elements:\n", contours->total );
		        		if (contours->total > 250) {
		        			printf("try to find ellipse");
		        			ellipse = cvFitEllipse2 (contours);
		        			ellipse.angle = 0 ; //  sets ange to best fitting angle for recttangle position
		        			printf("w:%f,h:%f\n",ellipse.size.width,ellipse.size.height);
		        			if (!(ellipse.size.width > 2*ellipse.size.height)/*|| !(ellipse.size.height > 2*ellipse.size.width)*/ ){

		        				 // prints size of ellipse , size relation could be used as input to ANN
		        				cvBoxPoints (ellipse, pt);
		        				CvPoint p1  = cvPointFrom32f (pt[0]);
		        				CvPoint p2  = cvPointFrom32f (pt[2]);
		        				cvSetImageROI(src,cvRect(p1.x ,p1.y ,(p2.x - p1.x),(p2.y - p1.y)));
		        				cropped = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
		        				cvCopy(src, cropped, NULL);
		        				cvResetImageROI(src);
		        				cvDrawContours (src, contours, CV_RGB (255, 0, 0), CV_RGB (255, 0, 0), 0, 1, CV_AA, cvPoint (0, 0));
		        				             	cvEllipseBox (src, ellipse,CV_RGB (0, 255, 0), 2);
		        				             	cvLine (src, cvPointFrom32f (pt[0]), cvPointFrom32f (pt[1]), CVX_YELLOW);
		        				             	cvLine (src, cvPointFrom32f (pt[1]), cvPointFrom32f (pt[2]), CVX_YELLOW);
		        				             	cvLine (src, cvPointFrom32f (pt[2]), cvPointFrom32f (pt[3]), CVX_YELLOW);
		        				             	cvLine (src, cvPointFrom32f (pt[3]), cvPointFrom32f (pt[0]), CVX_YELLOW);
		             	n++;

		             }
		             }
		             }
		           }
		        cvReleaseMemStorage(&storage);
		        return cropped;

	}

	/* Creates a 128 bin histogram and sends values to array, display hist for debugging
	 * fase
	 * */

	CvMat* getHistogramArray (IplImage* grayImage){
		CvMat* histArr = cvCreateMat(1,256,CV_32FC1); //  create 1 channel matrix of 128 elemens
		int numBins = 256;
		float range[] = {0, 255};
		float *ranges[] = { range };
		CvHistogram *hist = cvCreateHist(1, &numBins, CV_HIST_ARRAY, ranges, 1);
		cvClearHist(hist);
		cvCalcHist(&grayImage, hist, 0, NULL);
		float* ptr = (histArr->data.fl); // pointer to Matrix array , initialized at star
		for(int i=0;i<255;i++){
			float value =  cvQueryHistValue_1D(hist,i);
		//	printf(" before %f \n",(float)cvGetReal1D(histArr,i) );
			*(ptr) = value;
			//printf("after %f  %f \n",(float)cvGetReal1D(histArr,i),value );
			ptr++;
		}
		//IplImage* imgHist = DrawHistogram(hist);
		//cvNamedWindow("hist",1);
		//cvShowImage("hist",imgHist);
		//cvReleaseImage(&imgHist);
		cvReleaseHist(&hist); // get rid of histogram
		return histArr; //  average value  of each histogram.

	}

	/*
	 *
	 *	cvMat get FFT Array,
	 *	Inputs:  IplImage* grayImage
	 *	cvMat calculates FFT of Image and computes the magnitude, then its maximun, minimun,
	 *	mean and standard deviation values are calculated.S
	 *	Code Taken from openCV samples.
	 *	To do:optimize code.
	 *
	 *
	 *
	 *
	 *
	 * */

	CvMat*  getFFTArray(IplImage* grayImage){
			if (grayImage->width < 128 || grayImage->height < 128)
					return 0;
			CvMat* FFTArr = cvCreateMat(1,4,CV_32FC1);
		    IplImage * realInput;
		    IplImage * imaginaryInput;
		    IplImage * complexInput;
		    int dft_M, dft_N;
		    CvMat* dft_A, tmp;
		    IplImage * image_Re;
		    IplImage * image_Im;

		    double m, M;


		    realInput = cvCreateImage( cvGetSize(grayImage), IPL_DEPTH_64F, 1);
		    imaginaryInput = cvCreateImage( cvGetSize(grayImage), IPL_DEPTH_64F, 1);
		    complexInput = cvCreateImage( cvGetSize(grayImage), IPL_DEPTH_64F, 2);

		    cvScale(grayImage, realInput, 1.0, 0.0);
		    cvZero(imaginaryInput);
		    cvMerge(realInput, imaginaryInput, NULL, NULL, complexInput);

		    dft_M = cvGetOptimalDFTSize( grayImage->height - 1 );
		    dft_N = cvGetOptimalDFTSize( grayImage->width - 1 );

		    dft_A = cvCreateMat( dft_M, dft_N, CV_64FC2 );
		    image_Re = cvCreateImage( cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);
		    image_Im = cvCreateImage( cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);

		    // copy A to dft_A and pad dft_A with zeros
		    cvGetSubRect( dft_A, &tmp, cvRect(0,0, grayImage->width, grayImage->height));
		    cvCopy( complexInput, &tmp, NULL );
		    if( dft_A->cols > grayImage->width )
		    {
		        cvGetSubRect( dft_A, &tmp, cvRect(grayImage->width,0, dft_A->cols - grayImage->width, grayImage->height));
		        cvZero( &tmp );
		    }

		    // no need to pad bottom part of dft_A with zeros because of
		    // use nonzero_rows parameter in cvDFT() call below

		    cvDFT( dft_A, dft_A, CV_DXT_FORWARD, complexInput->height );

		   // cvNamedWindow("win", 0);
		    cvNamedWindow("magnitude", 0);
		   // cvShowImage("win", grayImage);

		    // Split Fourier in real and imaginary parts
		    cvSplit( dft_A, image_Re, image_Im, 0, 0 );

		    // Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
		    cvPow( image_Re, image_Re, 2.0);
		    cvPow( image_Im, image_Im, 2.0);
		    cvAdd( image_Re, image_Im, image_Re, NULL);
		    cvPow( image_Re, image_Re, 0.5 );

		    // Compute log(1 + Mag)
		    cvAddS( image_Re, cvScalarAll(1.0), image_Re, NULL ); // 1 + Mag
		    cvLog( image_Re, image_Re ); // log(1 + Mag)


		    // Rearrange the quadrants of Fourier image so that the origin is at
		    // the image center
		    cvShiftDFT( image_Re, image_Re );

		    cvMinMaxLoc(image_Re, &m, &M, NULL, NULL, NULL);

            /***************************************************************
             **********  Set FFT Arr with input values for training.********
             ***************************************************************
             * */
            cvSetReal1D(FFTArr,0,m); //  sets min value of FFT
            cvSetReal1D(FFTArr,1,M); // Sets max value of FFT
            CvScalar mean,sd;
            cvAvgSdv(image_Re,&mean,&sd,NULL);
            cvSetReal1D(FFTArr,2,mean.val[0]);
            cvSetReal1D(FFTArr,3,sd.val[0]);
           int i= 0 ;
         //  printf("\n min val: %f \n",cvGetReal1D(FFTArr,0));
           //printf("max val: %f \n",cvGetReal1D(FFTArr,1));
           //printf("mean val: %f \n",cvGetReal1D(FFTArr,2));
           //printf("standar dev val: %f \n",cvGetReal1D(FFTArr,3));

            /*****************************
             *
             *****************************
             */
		    cvScale(image_Re, image_Re, 1.0/(M-m), 1.0*(-m)/(M-m));
		    cvShowImage("magnitude", image_Re);
	            cvReleaseImage(&realInput);
	            cvReleaseImage(&imaginaryInput);
	            cvReleaseImage(&complexInput);
	            cvReleaseImage(&image_Re);
	            cvReleaseImage(&image_Im);
	            cvReleaseMat(&dft_A);

		return FFTArr;

	}

	/*
	 *CvMat edgeArray
	 *Input: IplImage grayImage
	 *REturns: array containing 4 values of edge
	 *
	 *
	 *
	 * */



	/*
	 * IplImage * Drawhist
	 * return picture to display Histogram , to test hist functionality.
	 *
	 *
	 *
	 * */
	IplImage* DrawHistogram(CvHistogram *hist){
		 float histMax = 0;
			 IplImage* histImg = cvCreateImage(cvSize(256*4,64*4),8,1);
			 cvZero(histImg);
		    cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0);
		    for(int i=0;i<255;i++)
		      {
		          float histValue = cvQueryHistValue_1D(hist, i);
		          float nextValue = cvQueryHistValue_1D(hist, i+1);

		          CvPoint pt1 = cvPoint(i*4, 64*4);
		          CvPoint pt2 = cvPoint(i*4+4, 64*4);
		          CvPoint pt3 = cvPoint(i*4+4, (64-nextValue*64/histMax));
		          CvPoint pt4 = cvPoint(i*4, (64-histValue*64/histMax)*4);

		          int numPts = 5;
		          CvPoint pts[] = {pt1, pt2, pt3, pt4, pt1};
		          cvFillConvexPoly(histImg, pts, numPts, cvScalar(255));
		      }
		    return histImg;
	}

/*
 *
 * Function to hekp display FFT analisys
 *
 * */


	void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr )
	{
	    CvMat * tmp=0;
	    CvMat q1stub, q2stub;
	    CvMat q3stub, q4stub;
	    CvMat d1stub, d2stub;
	    CvMat d3stub, d4stub;
	    CvMat * q1, * q2, * q3, * q4;
	    CvMat * d1, * d2, * d3, * d4;

	    CvSize size = cvGetSize(src_arr);
	    CvSize dst_size = cvGetSize(dst_arr);
	    int cx, cy;

	    if(dst_size.width != size.width ||
	       dst_size.height != size.height){
	        cvError( CV_StsUnmatchedSizes, "cvShiftDFT", "Source and Destination arrays must have equal sizes", __FILE__, __LINE__ );
	    }

	    if(src_arr==dst_arr){
	        tmp = cvCreateMat(size.height/2, size.width/2, cvGetElemType(src_arr));
	    }

	    cx = size.width/2;
	    cy = size.height/2; // image center

	    q1 = cvGetSubRect( src_arr, &q1stub, cvRect(0,0,cx, cy) );
	    q2 = cvGetSubRect( src_arr, &q2stub, cvRect(cx,0,cx,cy) );
	    q3 = cvGetSubRect( src_arr, &q3stub, cvRect(cx,cy,cx,cy) );
	    q4 = cvGetSubRect( src_arr, &q4stub, cvRect(0,cy,cx,cy) );
	    d1 = cvGetSubRect( src_arr, &d1stub, cvRect(0,0,cx,cy) );
	    d2 = cvGetSubRect( src_arr, &d2stub, cvRect(cx,0,cx,cy) );
	    d3 = cvGetSubRect( src_arr, &d3stub, cvRect(cx,cy,cx,cy) );
	    d4 = cvGetSubRect( src_arr, &d4stub, cvRect(0,cy,cx,cy) );

	    if(src_arr!=dst_arr){
	        if( !CV_ARE_TYPES_EQ( q1, d1 )){
	            cvError( CV_StsUnmatchedFormats, "cvShiftDFT", "Source and Destination arrays must have the same format", __FILE__, __LINE__ );
	        }
	        cvCopy(q3, d1, 0);
	        cvCopy(q4, d2, 0);
	        cvCopy(q1, d3, 0);
	        cvCopy(q2, d4, 0);
	    }
	    else{
	        cvCopy(q3, tmp, 0);
	        cvCopy(q1, q3, 0);
	        cvCopy(tmp, q1, 0);
	        cvCopy(q4, tmp, 0);
	        cvCopy(q2, q4, 0);
	        cvCopy(tmp, q2, 0);
	    }

	}



