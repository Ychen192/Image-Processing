#include<iostream>
#include<string>
#include<fstream>
#include <cmath>
using namespace std;

class ImageProcessing {	
	friend class SobelEdge;
	
	public:
	int** imgAry;
	int*  histogram;
	int   numRows, numCols;
	int   minVal, maxVal;
	
	ImageProcessing( ifstream& inFile );
	void loadImage ( ifstream& inFile ); 
	
	void mirroredFrame();
	void computeHistogram(int** image, int histSize);
	void printHistogram(int histSize);
	void computeThreshold(int** image, int thresholdNum);
	void prettyPrintAry ( int** image, ofstream& outFile, int Rows, int Cols );
	void regularPrintAry( int** image, ofstream& outFile );
	void testPrintAry( int** image );	
};

//ImageProcessing Constructor
ImageProcessing::ImageProcessing( ifstream& inFile ) 
{
	inFile>>this->numRows  ;
	inFile>>this->numCols  ;
	inFile>>this->minVal   ;
	inFile>>this->maxVal   ;
	
	this->imgAry   = new int*[numRows+2];	
	for (int i = 0; i<numRows+2; i++){
		this->imgAry[i] = new int[numCols+2] {};
	}	
}

//load the image from textfile into variable imgAry**
void ImageProcessing::loadImage( ifstream& inFile )
{
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			inFile>>imgAry[i][j];
		}
	}		
};

//mirrorFrame imgAry which already have zero borders
void ImageProcessing::mirroredFrame()
{	
	//filling the original array inside the frame	
	for(int i =0; i<numRows+2; i++ ){
		imgAry[i][0] 		 = imgAry[i][1];
		imgAry[i][numCols+1] = imgAry[i][numCols];
	}	
	for(int j =0; j<numCols+2; j++){
		imgAry[0][j] 		 = imgAry[1][j];
		imgAry[numRows+1][j] = imgAry[numRows][j];
	}
}

//compute histogram for GreyScale Image
void ImageProcessing::computeHistogram(int** image, int histSize)
{
	histogram = new int[histSize+1] {};
		
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			histogram[ image[i][j] ]++;
		}
	}	
};

//print the current histogram
void ImageProcessing::printHistogram(int histSize)
{
	for(int i = 0; i < histSize+1; i++){
		cout<<i<<" "<<histogram[i]<<"\t:";		
		if(histogram[i]>80)
			for(int j = 0; j<80; j++){ cout<<"+"; }	
		else
			for(int j = 0; j<histogram[i]; j++){ cout<<"+"; }	
		cout<<endl;
	}
};

//the grey scale image array is converted into binary using threshold number
void ImageProcessing::computeThreshold(int** image, int thresholdNum)
{
	for(int i =0; i<numRows+2; i++ ){
		for(int j =0; j<numCols+2; j++){
			if(image[i][j]<thresholdNum)
				cout<<" ";
			else
				cout<<"1";
		}
		cout<<endl;
	}	
};

//pretty print 2DArray image to file
void ImageProcessing::prettyPrintAry ( int** image, ofstream& outFile, int Rows, int Cols )
{
	for(int i =0; i<Rows; i++ ){
	 for(int j =0; j<Cols; j++){
			if(image[i][j] == 0 )
				outFile<<" ";
			else
				outFile<<image[i][j];
	 }
		outFile<<endl;
	}	
	outFile<<endl;
};

//regular print 2DArray image to file
void ImageProcessing::testPrintAry( int** image )
{
    	for(int i =0; i<numRows+2; i++ ){
		 for(int j =0; j<numCols+2; j++ ){
			if(image[i][j] == 0 )
					cout<<"0 ";
			else
				cout<<image[i][j]<<" ";
		 }
		cout<<endl;
	}	
	cout<<endl;
};

//test print 2DArray image to console
void ImageProcessing::regularPrintAry( int** image, ofstream& outFile )
{
    	for(int i =0; i<numRows+2; i++ ){
		 for(int j =0; j<numCols+2; j++ ){
			if(image[i][j] == 0 )
					outFile<<"0 ";
			else
				outFile<<image[i][j]<<" ";
		 }
		outFile<<endl;
	}	
	outFile<<endl;
};

class SobelEdge
{
	public:
	int** SobelVertical;
	int** SobelHorizontal;
	int** SobelRightDiag;
	int** SobelLeftDiag;
	int** SobelVHSum;
	int** SobelDiagSum;
	
	int VHSumMax   = 0;
	int DiagSumMax = 0;
	
	int maskVertical[3][3]   = { {1,2,1},   {0,0,0},   {-1,-2,-1} };	
	int	maskHorizontal[3][3] = { {1,0,-1},  {2,0,-2},  {1,0,-1 }  };	
	int	maskRightDiag[3][3]  = { {0,1,2},   {-1,0,1},  {0,-1,-2 } };
	int	maskLeftDiag[3][3]   = { {2,1,0},   {-1,0,-1}, {0,-1,-2 } };
	
	SobelEdge( int numRows, int numCols );
	int convolute(int i, int j, int mask[3][3], ImageProcessing* imgP );
};

SobelEdge::SobelEdge ( int numRows, int numCols )
{
	SobelVertical 	= new int*[numRows+2];
	SobelHorizontal = new int*[numRows+2];
	SobelRightDiag 	= new int*[numRows+2];
	SobelLeftDiag	= new int*[numRows+2];
	SobelVHSum		= new int*[numRows+2];
	SobelDiagSum	= new int*[numRows+2];	
	
	for( int i = 0; i<numCols+2; i++ )
	{
		SobelVertical[i]   =new int[numRows+2] {};
		SobelHorizontal[i] =new int[numRows+2] {};
		SobelRightDiag[i]  =new int[numRows+2] {};
		SobelLeftDiag[i]   =new int[numRows+2] {};
		SobelVHSum[i]	   =new int[numRows+2] {};
		SobelDiagSum[i]	   =new int[numRows+2] {};
	}	
};

int SobelEdge::convolute(int i, int j, int mask[3][3], ImageProcessing* imgP )
{
	int sum = 0;
	for(int k = 0; k< 3; k++){
	 for( int m = 0; m< 3; m++ ){
	 	sum += imgP->imgAry[i+(k-1)][j+(m-1)]*mask[k][m] ;
	 } 
	}
	return sum;
};

int main(int argc, char* argv[])
{
	/*
	step 0: open the image and read the image header
	   dynamically allocate imgAry and all the other sobel arrays	*/
	ifstream inFile( argv[1] );
	ImageProcessing* image1 = new ImageProcessing( inFile );

	/*
	step 1: loadImage // load input file to imgAry	*/
	image1->loadImage( inFile );
	
	/*
	step 2: mirrorFramed (imgAry)	*/
	image1->mirroredFrame();
	
	/*
	step 3: process the imgAry, from left to right and top to bottom
	  begin at (1, 1)
	
	SobelVertical(i,j) <-- convolute (i,j, maskVertical)
	SobelHorizontal(i,j) <-- convolute (i,j, maskHorizontal)
	SobelRightDiag(i,j) <-- convolute (i,j, maskRightDiag)
	SobelLeftDiag(i,j) <-- convolute (i,j, maskLeftDiag)
	SobelVHSum(i,j) <-- abs( SobelVertical(i,j)) + abs( SobelHorizontal(i,j))
	SobelDiagSum(i,j) <-- abs( SobelRightDiag(i,j)) + abs( SobelLeftDiag(i,j))	*/
	
	SobelEdge* sb = new SobelEdge(image1->numRows, image1->numCols);

	for(int i = 1; i< image1->numRows+1; i++){
	 for( int j = 1; j< image1->numCols+1; j++ ){
	 	sb->SobelVertical[i][j]   = sb->convolute(i,j, sb->maskVertical, image1 );	
	 	sb->SobelHorizontal[i][j] = sb->convolute(i,j, sb->maskHorizontal, image1 );
	 	sb->SobelRightDiag[i][j]  = sb->convolute(i,j, sb->maskRightDiag, image1 );
	 	sb->SobelLeftDiag[i][j]   = sb->convolute(i,j, sb->maskLeftDiag, image1 );
	 	sb->SobelVHSum[i][j] 	  = abs( sb->SobelVertical[i][j]  ) + abs( sb->SobelHorizontal[i][j] );
	 	sb->SobelDiagSum[i][j] 	  = abs( sb->SobelRightDiag[i][j] ) + abs( sb->SobelLeftDiag[i][j]   );
	 	
	 	//DiagSumMax value finder
	 	if(sb->DiagSumMax < sb->SobelDiagSum[i][j])
	 		sb->DiagSumMax = sb->SobelDiagSum[i][j];
	 	//VHSumMax value finder
	 	if(sb->VHSumMax < sb->SobelVHSum[i][j])
	 		sb->VHSumMax = sb->SobelVHSum[i][j];
	 }
	}
	
	/*
	step 4: 	write SobelVHSum to argv[2] // for further processing 
		write SobelDiagSum to argv[3] // for further processing 		*/
	ofstream output1(argv[2]);
	output1<<image1->numRows+2<<" "<<image1->numCols+2<<" 0 "<<sb->VHSumMax<<" "<<endl;
	image1->regularPrintAry(sb->SobelVHSum, output1);
	
	ofstream output2(argv[3]);
	output2<<image1->numRows+2<<" "<<image1->numCols+2<<" 0 "<<sb->DiagSumMax<<" "<<endl;
	image1->regularPrintAry(sb->SobelDiagSum, output2);	
	
	/*
	step 5: Using your existing programs: to do

 		Compute histogram for SobelVHSum
	     select a threshold value and threshold SobelVHSum
		prettyPring the thresholded SobelVHSum

	     Compute histogram for SobelDiagSum
	     select a threshold value and threshold SobelDiagSum
		prettyPrint the thresholded SobelDiagSum		*/
		
	int thresholdNum;
	char a;	
	//histogram and thresholding for SobelVHSum
	image1->computeHistogram(sb->SobelVHSum, sb->VHSumMax);
	image1->printHistogram(sb->VHSumMax);
	
	cout<<"Enter a threshold number for VHSum! ";
	cin>>thresholdNum;
	image1->computeThreshold(sb->SobelVHSum, thresholdNum);
	cout<<"Type Y to continue! ";
	cin>>a;
	
	//histogram and thresholding for SobelDiagSum
	image1->computeHistogram(sb->SobelDiagSum, sb->DiagSumMax);
	image1->printHistogram(sb->DiagSumMax);
	cout<<"Enter a threshold number for DiagSum! ";
	cin>>thresholdNum;
	image1->computeThreshold(sb->SobelDiagSum, thresholdNum);
	
	cout<<"Success!!!!!!!!!!!!";
}


