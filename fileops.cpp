/*
 * fileops.cpp
 *
 *  Created on: Nov 20, 2010
 *      Author: paco
 *
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
#include "fileops.h"
#include"globalvals.h"


/*
 *groups the strings for the adress of the four possible folders where the training and
 *validation images are located.
 * */
char *imgDir[4]= {
TGLASS,TPLAST,VGLASS,VPLAST
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
int  getNumberOfFiles(char* dir)
{
	int fileCount;
	DIR *dp;
	struct dirent *ep;

	dp = opendir (dir);
	if (dp != NULL)
	{
		for(fileCount = 0;ep = readdir (dp);fileCount++)
		{
	           // printf("%s\n",ep->d_name);
		}
		(void) closedir (dp);
		return fileCount;
	}
	else
		perror ("Couldn't open the directory");
		return -1;
}

/*
 * inputs : set, number indicating the type of set *
 * initialices the type of set in the set structure.
 *
 *
 * */

void typeOfSet(set* src,int type){
	src->type = type;
}

/*
 * void typeOfMaterial
 *inputs : set , indication of the type of material for the validation set, set to glass
 *when material is of training type and will be set randomly between glass and plastic.
 *
 *Sets the type of Material and number of metarial elements present
 *for the validation and test sets.
 *If the type is a training set, the material will be set randomly.And if the material is
 *a validation will be set to either plastic or glass. Depending on the previous set value.
 *a
 *
 * */
void typeOfMaterial(set* src, material mat){
	int material = 0;
	if (src->type == training){
		// srand ( time(NULL) ); only use when testing program
		 material = rand()%2;
		// printf("%d",material);
		 mat = (material ==0) ? glass :plastic;
		// printf("%d",material);// test for right param
		 src->material = mat; //  set type of material and folder to look for.
		 src->dir = imgDir[mat];
	}
	if (src->type == validation){
		src->material = mat;
		src->dir = imgDir[mat+2];// sets dir to validation index of material
	}
	src->NumOfElements = getNumberOfFiles(src->dir); //  sets number of images in folder

}

/*
 * void getRandomSample
 *
 * input set
 * sets the filename for a sample image directory, generated randomly
 *
 *
 * */
void setRandomSample(set* src){

	int fileCount;
	DIR *dp;
	struct dirent *ep;
	char fileDir[strlen(src->dir)+1];
	dp = opendir(src->dir);
	strcpy(fileDir,src->dir);
	while(true){
		ep = NULL;
		int randpos = (rand()%src->NumOfElements);
		int index = 0;			ep = readdir(dp) ;

		rewinddir(dp); // set dp to the start of the stream
		while( randpos > index++ ){ // go to the index of file specified.
			ep = readdir(dp) ;
		}
		if ((strcmp(ep->d_name,"..")&&(strcmp(ep->d_name,"."))) ) // test for valid names if not try another ramdom name
			break;
	}
	src->sampleName = fileDir; // adds path to the sample image
	strcat(src->sampleName,ep->d_name);  // adds the name of sample Image
	closedir(dp);
}




