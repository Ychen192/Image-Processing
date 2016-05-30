/*
Yan Chen
CSC 381 Spring 2016: Image Processing
Assignment 0: Binary Image and Greyscale Image Framing
*/

#include<iostream>
#include<string>
#include<fstream>
using namespace std;

int main(int argc, char* argv[]) 
{
	//string inFileName = "2D_binaryDiamond.txt";
	//string inFileName2 = "2D_greyScaleDiamond.txt";
	
	if(argv[1] == NULL){
		cout<<"Invalid File!";
		return 0;
	}
	
	ifstream inFile(argv[1]);
	
	int RowSize, ColSize, min, max;
	
	inFile>>RowSize;
	inFile>>ColSize;
	inFile>>min;
	inFile>>max;

	int** newBinaryImageArray = new int*[RowSize+2];
	for(int i =0; i<RowSize+2; i++ ){
		newBinaryImageArray[i] = new int[ColSize+1];
	}
	
//when the image is an binary image
if(max == 1){	
   //zeroing the array
	for(int i =0; i<(RowSize+2); i++ ){
		for(int j =0; j<(ColSize+2); j++){
			newBinaryImageArray[i][j]=0;
		}
	}
   //filling the array	 	
	for(int i =1; i<RowSize+1; i++ ){
		for(int j =1; j<ColSize+1; j++){
			inFile>>newBinaryImageArray[i][j];
		}
	}
   //printing out the new copy array with frame
	for(int i =0; i<(RowSize+2); i++ ){
		for(int j =0; j<(ColSize+2); j++){
			cout<<newBinaryImageArray[i][j]<<" ";
		}
		cout<<"\n";
	}
}//if

//when it's not binary image
else{
	//filling the original array inside the frame	
	for(int i =1; i<RowSize+1; i++ ){
		for(int j =1; j<ColSize+1; j++){
			inFile>>newBinaryImageArray[i][j];
		}
	}
	
//filling the edges of the frame
	//top edge
	for(int i = 0; i<ColSize+2; i++){
		newBinaryImageArray[0][i] = newBinaryImageArray[1][i];
	}
	//left edge
	for(int i = 0; i<RowSize+2; i++){
		newBinaryImageArray[i][0] = newBinaryImageArray[i][1];
	}
	//bottom edge
	for(int i = 0; i<ColSize+2; i++){
		newBinaryImageArray[RowSize+1][i] = newBinaryImageArray[RowSize][i];
	}
	//right edge
	for(int i = 0; i<RowSize+2; i++){
		newBinaryImageArray[i][ColSize+1] = newBinaryImageArray[i][ColSize];
	}
	
	//top left corner
	 newBinaryImageArray[0][0] = newBinaryImageArray[1][1];
	//top right corner
	 newBinaryImageArray[0][ColSize+1] = newBinaryImageArray[1][ColSize];
	//bottom left corner 
	 newBinaryImageArray[RowSize+1][0] = newBinaryImageArray[RowSize][1];
	//bottom right corner
	 newBinaryImageArray[RowSize+1][ColSize+1] =newBinaryImageArray[RowSize][ColSize];
	 
	//printing out the new copy array with frame
	for(int i =0; i<(RowSize+2); i++ ){
		for(int j =0; j<(ColSize+2); j++){
			cout<<newBinaryImageArray[i][j]<<" ";
		}
		cout<<"\n";
	}	
}//else
}//main
