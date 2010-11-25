/*
 * fileops.h
 *
 *  Created on: Nov 20, 2010
 *      Author: paco
 *
 *      File Related operations and constant values for accesing Images and
 *      Image information. Or writing matrix information.
 */
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <dirent.h>
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include "cxcore.h"
#include"globalvals.h"

/*
 *
 * Constants that define the name of the project directories
 * for training and validation images
 *
 * */

#ifndef FILEOPS_H_
#define FILEOPS_H_
#define IMAGE "images/"
#define TRAINING  IMAGE "training/"
#define TGLASS TRAINING "glass/"
#define TPLAST TRAINING "plast/"
#define VALIDATION IMAGE "validation"
#define VGLASS VALIDATION "glass/"
#define VPLAST VALIDATION "plast/"

#define GLASSIMG "glass.jpg"
#define PLASTICIMG "plastic.jpg"



/*
 *
 * type of element that the structure is going to use in test
 *
 *
 *
 * */
enum material{
	plastic,
	glass
};


enum typeSet{
	training,
	validation
};

/*
 * this structure will save the type of elements that will be looking for
 * as well as the type of set that is being put in the ANN(training,validation)
 * */

struct set{
	int type;
	int material;
	char* dir; // path of files.
	char* sampleName;
	int NumOfElements; // num of elements in the folder obtained with other method
};

/*
 * int  getNumberOfFiles
 *
 *Input:  folder Name string.
 *
 *Returns the total number of files in the folder, so it can be later
 *used to know how many training and validation samples we have.
 *
 * */



int  getNumberOfFiles(char* dir);

/*
 * inputs : set, number indicating the type of set
 *
 * initialices the type of set in the set structure.
 *
 *
 * */
void typeOfSet(set* src,int type);
/*
 *Sets the type of Material and number of metarial elements present
 *for the validation and test sets.
 * */
void typeOfMaterial(set* src, material mat);

/* void setRandomSample
 * sets the filename of the random sample that is going to be used.
 * set must  have been previously set with typeOfSet previously.
 *
 * */
void setRandomSample(set* src);



#endif /* FILEOPS_H_ */
