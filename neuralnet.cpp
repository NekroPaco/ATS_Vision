/*
 * neuralnet.cpp
 *
 * File operations for Saving Information from Image Data, Loading that Information.
 * And training MLP backprop Algorithm and validate it.
 *
 *
 *  Created on: Nov 19, 2010
 *      Author: Paco Rodriguez
 */

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
#include"neuralnet.h"

#include "globalvals.h"
#include "imgTransf.h"

/*

 *
 * Input Array. OutOut Array.
 *
 * Will Enter the parameters for a new neural network.Perform the training and validation as
 * well.
 * Sets each of the rows for training samples, for input and output.
 *
 * Nedd to optimize function usin gpointer arithmetic
 *
 * */

	void setInputRow(CvMat* twoDim,CvMat* oneDim,int row){
		float* ptrTwo = (float*)(twoDim->data.fl + row * (twoDim->step)/sizeof(float)); // sets pointer to specific Row and index 0
		 const float* ptrOne = oneDim->data.fl; //  sets one dimention pointer to index 0 to read from.
		// printf("%f"void saveInputTraining(),ptrOne);
		 for( int col=0; col<twoDim->cols; col++ ) {
			//printf("%f :     ",*ptrOne);
			*(ptrTwo++) = *(ptrOne++);
		//	cvSetReal2D(twoDim,row,col,cvGetReal1D(oneDim,col));
		//	printf("%f \n",cvGetReal2D(twoDim,row,col));
			//printf("%f \n",*(ptrTwo-1));
		}
	}


/*
 *
 *void SetOutputRow
 *	inputs :  int material, CvMat2d output matrix.
 *	Performs assignment of output values depending on the specified material.
 *
 *
 * */
void setOutputRow(CvMat* twoDim,int row,int material){
	float* ptr = (float*)(twoDim->data.fl + row * (twoDim->step)/sizeof(float));
	if(material == 0) {// materia is plastic
		//	cvSetReal2D(twoDim,row,0,1);
		//cvSetReal2D(twoDim,row,1,0);
	*ptr = 1;
	*(++ptr)=0;
	}
	else if (material == 1){ // material is glass
		*ptr = 0;
		*(++ptr)=1;

	}
	else{
		printf("Invalid Parameters");

	}
	printf("     %d",material);

}

/*void saveInputTraining()
 *
 *inputs:  Cvmat trainInputs CvMat trainOutputs CvMat validation inputs CvMat Validation Outputs
 *
 *saves files with training data . Input file
 *
 *
*/
void saveInputAndOutput(CvMat* input,CvMat* output,char* filenameInput,char*filenameOutput ){
	cvSave( filenameInput, input);
	cvSave(filenameOutput, output);
}

/*void loadInputAndOutput()
 *
 *inputs:  Cvmat input CvMat trainOutputs CvMat output CvMat Validation Outputs
 *
 *saves files with training data . Input file
 *
 *
*/

void loadInputAndOutput(CvMat* input,CvMat* output,char* filenameInput,char* filenameOutput ){
	input = (CvMat*)cvLoad(filenameInput );
	output = (CvMat*)cvLoad(filenameOutput);
}

 /* int buildMlpClassifier
 *
 * inputs:  Neural network parameters.*/

	int buildMlpClassifier (CvMat *trainingData,char* fileToSave,char* filenameToLoad){
		 CvMat* data = 0;
		 CvMat trainData;
		 CvMat* responses = 0;
		 CvMat* mlpResponse = 0;
		 CvANN_MLP mlp;
		 int ntrainSamples = 0;

		// mlp.create();
	      //  CvMat* new_responses = cvCreateMat( ntrainSamples, class_count, CV_32F );


	}



	static
	int build_mlp_classifier( char* data_filename,
	   char* filename_to_save, char* filename_to_load )
	{
	    const int class_count = 26;
	    CvMat* data = 0;
	    CvMat train_data;
	    CvMat* responses = 0;
	    CvMat* mlp_response = 0;

	    int ok = read_num_class_data( data_filename, 16, &data, &responses );
	    int nsamples_all = 0, ntrain_samples = 0;
	    int i, j;
	    double train_hr = 0, test_hr = 0;
	    CvANN_MLP mlp;

	    if( !ok )
	    {
	        printf( "Could not read the database %s\n", data_filename );
	        return -1;
	    }

	    printf( "The database %s is loaded.\n", data_filename );
	    nsamples_all = data->rows;
	    ntrain_samples = (int)(nsamples_all*0.8);

	    // Create or load MLP classifier
	    if( filename_to_load )
	    {
	        // load classifier from the specified file
	        mlp.load( filename_to_load );
	        ntrain_samples = 0;
	        if( !mlp.get_layer_count() )
	        {
	            printf( "Could not read the classifier %s\n", filename_to_load );
	            return -1;
	        }
	        printf( "The classifier %s is loaded.\n", data_filename );
	    }
	    else
	    {
	        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	        //
	        // MLP does not support categorical variables by explicitly.
	        // So, instead of the output class label, we will use
	        // a binary vector of <class_count> components for training and,
	        // therefore, MLP will give us a vector of "probabilities" at the
	        // prediction stage
	        //
	        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	        CvMat* new_responses = cvCreateMat( ntrain_samples, class_count, CV_32F );

	        // 1. unroll the responses
	        printf( "Unrolling the responses...\n");
	        for( i = 0; i < ntrain_samples; i++ )
	        {
	            int cls_label = cvRound(responses->data.fl[i]) - 'A';
	            float* bit_vec = (float*)(new_responses->data.ptr + i*new_responses->step);
	            for( j = 0; j < class_count; j++ )
	                bit_vec[j] = 0.f;
	            bit_vec[cls_label] = 1.f;
	        }
	        cvGetRows( data, &train_data, 0, ntrain_samples );

	        // 2. train classifier

	        int layer_sz[] = { data->cols, 100, 100, class_count };
	        CvMat layer_sizes =
	            cvMat( 1, (int)(sizeof(layer_sz)/sizeof(layer_sz[0])), CV_32S, layer_sz );
	        mlp.create( &layer_sizes );
	        printf( "Training the classifier (may take a few minutes)...\n");
	        mlp.train( &train_data, new_responses, 0, 0,
	            CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_ITER,300,0.01),
	//#if 1
	            CvANN_MLP_TrainParams::BACKPROP,0.001));
	//#else
	            //CvANN_MLP_TrainParams::RPROP,0.05));
//	#endif
	        cvReleaseMat( &new_responses );
	        printf("\n");
	    }

	    mlp_response = cvCreateMat( 1, class_count, CV_32F );

	    // compute prediction error on train and test data
	    for( i = 0; i < nsamples_all; i++ )
	    {
	        int best_class;
	        CvMat sample;
	        cvGetRow( data, &sample, i );
	        CvPoint max_loc = {0,0};
	        mlp.predict( &sample, mlp_response );
	        cvMinMaxLoc( mlp_response, 0, 0, 0, &max_loc, 0 );
	        best_class = max_loc.x + 'A';

	        int r = fabs((double)best_class - responses->data.fl[i]) < FLT_EPSILON ? 1 : 0;

	        if( i < ntrain_samples )
	            train_hr += r;
	        else
	            test_hr += r;
	    }

	    test_hr /= (double)(nsamples_all-ntrain_samples);
	    train_hr /= (double)ntrain_samples;
	    printf( "Recognition rate: train = %.1f%%, test = %.1f%%\n",
	            train_hr*100., test_hr*100. );

	    // Save classifier to file if needed
	    if( filename_to_save )
	        mlp.save( filename_to_save );

	    cvReleaseMat( &mlp_response );
	    cvReleaseMat( &data );
	    cvReleaseMat( &responses );

	    return 0;}

	/*
	 * CvMat* GetImageData
	 *
	 * Inputs:	Array of
	 *
	 *
	 * */

