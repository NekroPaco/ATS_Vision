/*
 * neuralnet.h
 *	Contains methods for neural net training , validation and Realt-Time detection once it has
 *	been tested.
 *
 *	These functions will process files from training data. Train network and save the weight parameters.
 *
 *
 *
 *  Created on: Nov 19, 2010
 *      Author: paco
 */

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"


#include "globalvals.h"
#include "imgTransf.h"


#ifndef NEURALNET_H_
#define NEURALNET_H_




 /*
 * void SetInputRow
 *
 * Will Enter the parameters for a new neural network.Perform the training and validation as
 * well.
 * Sets each of the rows for training samples, for input and output.
 *
 *
 *
 * */
void setInputRow(CvMat* towDim,CvMat* oneDim,int row);



/*
 *
 *void SetOutputRow
 *	inputs :  int material, CvMat2d output matrix.
 *	Performs assignment of output values depending on the specified material.
 *
 *
 * */
void setOutputRow(CvMat* twoDim,int row,int material);
/*
 * void build_mlp_classifier
 *
 * inputs:  Neural network parameters. File name of
 *
 * Input Array. file to save traing data if training is specifies, filename to
 * load data from if verification is specified.
 *
 * Will Enter the parameters for a new neural network.Perform the training and validation as
 * well.
 * */

/*
*
 *inputs:  Cvmat trainInputs CvMat trainOutputs CvMat validation inputs CvMat Validation Outputs
 *
 *saves files with training data .fie  Input training values  and output training values
 *
 *
 *
 * */

void saveInputAndOutput(CvMat* input,CvMat* output,char* filenameInput,char*filenameOutput );

/*void loadInputTraining()
 *
 *inputs:  Cvmat input CvMat trainOutputs CvMat output CvMat Validation Outputs
 *
 *saves files with training data . Input file
 *
 *
*/
void loadInputAndOutput(CvMat* input,CvMat* output,char* filenameInput,char* filenameOutput );


int buildMlpClassifier (CvMat *trainingData,char* fileToSave,char* filenameToLoad);
#endif /* NEURALNET_H_ */
