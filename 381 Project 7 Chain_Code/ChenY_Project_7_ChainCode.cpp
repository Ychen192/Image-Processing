#include<iostream>
#include<string>
#include<fstream>
#include <cmath>
using namespace std;

class ImageProcessing {	
	public:
	int** imgAry;
	int*  histogram;
	int   numRows, numCols;
	int   minVal, maxVal;
	
	ImageProcessing( ifstream& inFile );
	void loadImage ( ifstream& inFile ); 
	void mirroredFrame();
	void prettyPrintAry ( int** image, ofstream& outFile );
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

//load the image from textfile into 2D array imgAry** with single layer zero frame
void ImageProcessing::loadImage( ifstream& inFile )
{
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			inFile>>imgAry[i][j];
		}
	}		
};

//pretty print 2DArray image to file
void ImageProcessing::prettyPrintAry ( int** image, ofstream& outFile )
{
	for(int i =0; i<numRows+2; i++ ){
	 for(int j =0; j<numCols+2; j++){
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


class ChainCode 
{	

	struct pixel{
		int rowPosition;
		int colPosition;	
	};
	
	public:
	
	int lastZeroTable [8] = {6, 6, 0, 0, 2, 2, 4, 4};
	int neighborTable [8][2] = { {0,1},  {-1,1}, {-1,0}, {-1,-1}, 
								 {0,-1}, {1,-1}, {1,0},  {1,1} 	}; // look up table for p(i,j)'s 8 neighbor's row and col positions
	
	pixel firstNoneZero;
	int label;

	int lastZeroDir;		  // from P
	int nextDir; 			  // from Q

	pixel currPixel;  // P
	pixel nextPixel; // Q
	int chainDir; // the direction from P to Q
	
	// scan the image to find the next connected component in the image, 
	// returns the first none zero pixel 
	void findNextComponent ( ImageProcessing* image ) 
	{
		for( int i = 0; i < image->numRows+2; i++){
			for( int j = 0; j < image->numCols+2; j++ ){
				if( image->imgAry[i][j] == label ) {
					firstNoneZero.rowPosition = i;
					firstNoneZero.colPosition = j;
					return;
				}	
			}
		}
	};
	
	// with respect to the current pixel, P(i,j), from the
	// the last zero neight, find the first 
	// none zero of P's 8 neighbors, call it Q, then
	// returns the direction from  P to Q.
	int findNextNoneZeroNeighbor( int Dir, int** image, ofstream& outFile ) 
	{
		int neighborDir = Dir;
		int r;
		int c;
		for( int k = 0; k< 8; k++ ){
			r = currPixel.rowPosition + neighborTable[neighborDir][0];
			c =	currPixel.colPosition + neighborTable[neighborDir][1];
			outFile<<" "<<neighborDir;
			if( image[r][c] == label  ){
				outFile<<endl;
				return neighborDir;
			}
			neighborDir = (++Dir) % 8;
		}
		outFile<<endl;
		return 8;
	};
	
	bool pixelIsSame( pixel a, pixel b){
		if(a.rowPosition == b.rowPosition && a.colPosition == b.colPosition)
			return true;
		else 
			return false;	
	}
	
	ChainCode()
	{
		lastZeroDir = 4;
		label = 1;
	}

  };

int main(int argc, char* argv[])
{
	if(argc< 3){
		cout<<"Check parameters! "<<endl;
		return 0;
	}
	
	/*
	step 0: - read the image header
	- dynamically allocate the imageAry	*/
	ifstream inFile ( argv[1] );
	ImageProcessing* image1 = new ImageProcessing( inFile );
	
	/*
	step 1: 1.1 zeroFrame imageAry
	1.2 read from the input onto imageAry, begin at imageAry[1][1]
	1.3 pretty print the imageAry to argv[3]	*/
	image1->loadImage(inFile);
	
	ofstream outFile2 ( argv[3] );
	image1->prettyPrintAry(image1->imgAry, outFile2 );
	
	ChainCode* nextCC = new ChainCode();
	
	
	ofstream outFile1 ( argv[2] );
	
	int maxLabel = image1->maxVal;
	for(nextCC->label  = 1; nextCC->label <= maxLabel; nextCC->label++){	
	/*
 	step 2: nextCC <-- findNextComponent // nextCC is a chainCode object
	output nextCC's rowPostion, colPosition, label to argv[2] 
	lastZeroDir <-- 4
	currPixel <-- firstNoneZero of nextCC	*/
		nextCC->findNextComponent( image1 );
		nextCC->currPixel = nextCC->firstNoneZero;
		outFile1<<nextCC->currPixel.rowPosition<<" "<<nextCC->currPixel.colPosition<<" "<<nextCC->label<<"  ";
	
	/*	
	step 3: nextDir <-- lastZeroDir + 1
		chainDir <-- findNextNoneZeroNeighbor (nextDir)
		output chainDir to argv[2]	
		lastZeroDir <-- lookup lastZeroTable[chainDir]
		currPixel <-- lookup neighborTable[chainDir]	
	step 4: repeat step 3 until currPixel is matched with firstNoneZero of nextCC	*/

		do {
			nextCC->nextDir = nextCC->lastZeroDir+1;
			nextCC->chainDir = nextCC->findNextNoneZeroNeighbor ( nextCC->nextDir, image1->imgAry, outFile1 );
			if(nextCC->chainDir > 7) {	//when there is no neighbors
				outFile1<<nextCC->currPixel.rowPosition<<" "<<nextCC->currPixel.colPosition<<" "<<nextCC->label<<"  ";
				goto caseOnePixelComponent;
			}
			nextCC->lastZeroDir = nextCC->lastZeroTable[nextCC->chainDir];			
			nextCC->currPixel = { nextCC->currPixel.rowPosition + nextCC->neighborTable[nextCC->chainDir][0],
								  nextCC->currPixel.colPosition + nextCC->neighborTable[nextCC->chainDir][1] };
			outFile1<<nextCC->currPixel.rowPosition<<" "<<nextCC->currPixel.colPosition<<" "<<nextCC->label<<"  ";
		} while( !nextCC->pixelIsSame(nextCC->currPixel, nextCC->firstNoneZero) );
		// end of one chain code of a connected component
		caseOnePixelComponent:;
		outFile1<<endl<<endl;
	}
	/*
	step 5: repeat step 2 to step 4 until no more connected componenets in the input image.	*/
	
	
	/*
	step 6: close all files	*/
	inFile.close();
	outFile1.close();
	outFile2.close();
	
	cout<<"Success"<<endl;
}


