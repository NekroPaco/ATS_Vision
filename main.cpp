/*
 * ATS Vision V0.001
 * Program to traing ANN so it can classify glass and plastic beverage bottles.
 *
 * main.cpp
 *
 *  Created on: Nov 13, 2010
 *      Author: paco
 * changelog: eliminated Resizing of Image
/*
 *
 * */

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
#include "fileops.h"
#include "globalvals.h"
#include "imgTransf.h"
#include "neuralnet.h"trainInputs


int main(int argc, char** argv){
	 CvCapture* capture = NULL ;
	 set* trainingSet = new set;
	 set* validationSet = new set;//  set for training Images
	 CvMat* trainingInput;
	 CvMat* validationInput;
	 CvMat* trainingMatrix =  cvCreateMat(TRAIN_INPUTS,INPUT_SIZE,CV_32FC1); //  matrix that stores ANN inputs
	 CvMat* trainingOutputs = cvCreateMat(TRAIN_INPUTS,2,CV_32FC1); //  matrix that stores ANN outputs. first coumn = plastic, 2nd Column = glass

	 printf ("ATS Vision test \n");
			printf("Will process input from Image or video containing a water or glass container bottom and determine which material is it\n");
			 printf("Choose camera or Image Files\n");
			 if (!strcmp(argv[1],"-Build") ){  //  chose to build new training and validation data
			 		printf("Creating new training and validation  data \n");
			 		printf("call to function ");
			 		typeOfSet(trainingSet,training);
			 		typeOfSet(validationSet,validation);
			 		int j =0;
			 		printf("Creating new training data... \n ");
			 		while(j<TRAIN_INPUTS){
			 			typeOfMaterial(trainingSet,glass);
			 			printf("%s \n",trainingSet->dir);
			 			setRandomSample(trainingSet);
			 			char filename[strlen(trainingSet->sampleName)+1];
			 			strcpy(filename,trainingSet->sampleName);
			 			printf("%s %d \n",filenamtrainingInpute,j);
			 			if (trainingInput = processInputFromImage(filename)){ // sees if an array of extractions values was obtained , if not skip to next image.
							setInputRow(trainingMatrix,trainingInput,j);
							setOutputRow(trainingOutputs,j,trainingSet->material);
							j++;}
			 			cvReleaseMat(&trainingInput);
			 		// get number of elements of training face or validation face
			 		}
			 		printf("Creating new validation data... \n ");
			 		j =0;
			 		while(j<VALIDATION_INPUTS){
			 			typeOfSet(validationSet,validation);
						if(j<VALIDATION_INPUTS/2)
							typeOfMaterial(validationSet,glass);
						else typeOfMaterial(validationSet,plastic);
						printf("%s \n",validationSet->dir);
			 			setRandomSample(validationSet);
			 			char filename[strlen(validationSet->sampleName)+1];
			 			strcpy(filename,trainingSet->sampleName);
			 			printf("%s %d \n",filename,j);
			 			if (trainingInput = processInputFromImage(filename)){ // sees if an array of extractions values was obtained , if not skip to next image.
			 				setInputRow(trainingMatrix,trainingInput,j);
			 				setOutputRow(trainingOutputs,j,trainingSet->material);
			 				j++;}
			 			cvReleaseMat(&validationInput);
			 					 		// get number of elements of training face or validation face
			 					 		}
			 		saveInputAndOutput(trainingMatrix,trainingOutputs,"trainInputs.xml","trainOutputs.xml" );
			 		printf("new data saved succesfully :)\n");
			 		cvReleaseMat(&trainingMatrix); // Release memory
			 		cvReleaseMat(&trainingOutputs);
			 	}
			 else if (!(strcmp(argv[1],"-Load"))){
				 printf("Loading training input data...\n");
				 loadInputAndOutput(trainingMatrix,trainingOutputs,"trainInputs.xml","trainOutputs.xml");
				 printf("Loading training output data...\n");
				 printf("Training and validation data loaded successfully !!!!!!! \n");
			 	 cvReleaseMat(&trainingMatrix); // Release memory
			 	 cvReleaseMat(&trainingOutputs);
			 }
			 		// get Image from filename
			 	else if(!strcmp(argv[1],"camera")) {
			 	//	processInputFromCamera(capture,frame,output,grayImage,crop,resized);
			 		cvReleaseCapture(&capture);
			 	}
					 delete trainingSet;
					 delete validationSet
	return 0;
}








