#include<iostream>
#include<string>
#include<fstream>
#include <cmath>
using namespace std;
const double pi = 3.14159265358;

class ImageProcessing {	
	public:
	int   numRows;
	int   numCols;
	int   minVal; 
	int   maxVal;
	int** imgAry;
	
	ImageProcessing();
	void loadImage ( ifstream& inFile );
	void prettyPrintAry ( int** image, ofstream& outFile );
	void regularPrintAry( int** image, ofstream& outFile );
	void testPrintAry( int** image );	
};

//ImageProcessing Constructor
ImageProcessing::ImageProcessing() 
{	
}

//load the image from textfile into 2D array imgAry** with single layer zero frame
void ImageProcessing::loadImage( ifstream& inFile )
{
	inFile>>numRows;
	inFile>>numCols;
	inFile>>minVal; 
	inFile>>maxVal;
	
	this->imgAry   = new int*[numRows];	
	for (int i = 0; i<numRows; i++){
		this->imgAry[i] = new int[numCols] {};
	}	
	
	for(int i = 0; i<numRows; i++ ){
		for(int j = 0; j<numCols; j++){
			inFile>>imgAry[i][j];
		}
	}		
	
};

class HoughTransform
{	
	public:
	class xyCoord{
		public:
		int x;
		int y;
		
		xyCoord() {};
		void setCoord( int x, int y) {	
			this->x = x; 
			this->y = y;
		};
	};
	
	xyCoord* point;
	int 	angleInDegree;
	double  angleInRadians;
	int numRows;
	int numCols;
	int min, max;
	
	int** HoughAry;
	
	HoughTransform( int rows, int cols );
	int computeDistance();
	void determineHeader();
	void testPrintAry( int** image );
	void prettyPrintAry ( int** image, ofstream& outFile );
	void regularPrintAry( int** image, ofstream& outFile );
};


HoughTransform::HoughTransform( int rows, int cols )
{	
	numRows = 180;
	numCols = sqrt( rows*rows + cols*cols );
	
	this->HoughAry   = new int*[numRows];	
	for (int i = 0; i<numRows; i++){
		this->HoughAry[i] = new int[numCols] {};
	}
	point = new xyCoord();
}


int HoughTransform::computeDistance()
{
	double distance; 
	double angleT;
	
	angleT   = angleInRadians - atan2( point->y, point->x ) - pi/2;
	distance = sqrt( pow(point->y,2) + pow(point->x,2)) * cos(angleT);
	
	return abs( distance + 0.5);
}

void HoughTransform::determineHeader()
{
	min = 0;
	max = 0;
	for(int i = 0; i< numRows; i++){
		for(int j = 0; j< numCols; j++){
			if( HoughAry[i][j] > max ){
				max = HoughAry[i][j];
			}
		}
	}
	
};

//regular print 2DArray image to file
void HoughTransform::testPrintAry( int** image )
{
    	for(int i =0; i<numRows; i++ ){
		 for(int j =0; j<numCols; j++ ){
			if(image[i][j] == 0 )
					cout<<" ";
			else
				cout<<image[i][j]<<" ";
		 }
		cout<<endl;
	}	
	cout<<endl;
};

//test print 2DArray image to console
void HoughTransform::regularPrintAry( int** image, ofstream& outFile )
{
	outFile<<numRows<<" "<<numCols<<" "<<min<<" "<<max<<endl;
    	for(int i =0; i<numRows; i++ ){
		 for(int j =0; j<numCols; j++ ){
			if(image[i][j] == 0 )
					outFile<<"0 ";
			else
				outFile<<image[i][j]<<" ";
		 }
		outFile<<endl;
	}	
	outFile<<endl;
};

//pretty print 2DArray image to file
void HoughTransform::prettyPrintAry ( int** image, ofstream& outFile )
{
	for(int i =0; i<numRows; i++ ){
	 for(int j =0; j<numCols; j++){
			if(image[i][j] == 0 )
				outFile<<" ";
			else
				outFile<<image[i][j];
	 }
		outFile<<endl;
	}	
	outFile<<endl;
};
	
int main(int argc, char* argv[])
{
	if(argc< 3){
		cout<<"Check parameters! "<<endl;
		return 0;
	}
	
	ifstream inFile( argv[1] );
	ImageProcessing* image = new ImageProcessing();
	image->loadImage( inFile );
	
	/*
	step 0: - open input file and output files
	   - dynamically allocate Hough2DAry and initiallize to zero	*/
	   
	HoughTransform* hough = new HoughTransform( image->numRows, image->numCols );
 
	/*  
	step 1:  scan imgAry left to right and top to bottom 
	         if pixel (i,j) > 0
		       point <-- (i, j)		*/
		
	for(int i = 0; i< image->numRows; i++){
		for(int j = 0; j< image->numCols; j++){
			if( image->imgAry[i][j] > 0 ){
				hough->point->setCoord( i, j );	
				
			/*  
			step 2:  angleInDegree  <-- 0
	    			 angleInRadians <-- angleInDegree / 180.00 * pi 	*/
			hough->angleInDegree = 0;
			hough->angleInRadians = hough->angleInDegree / 180.00 * pi;
	        
				while( hough->angleInDegree < 180 ){
					/*	 
					step 3: dist <-- computeDistance (point, angleInRadians) // use the distance formula given in class 
	       			HoughAry[angleInDegree][dist]++
		   			angleInDegree ++	*/
					int dist = hough->computeDistance();
					hough->HoughAry[ hough->angleInDegree ][ dist ]++;
					
					hough->angleInDegree++;
					hough->angleInRadians = hough->angleInDegree / 180.00 * pi;
				}
				/*
				step 4: repeat step 3 until angleInDegree > 179		*/
			}
		}
	}
	/*
	step 5: repeat step 1 to step 4 while input file is not empty	*/
	
	/*
	step 6: determineHeader (HoughAry) 
		   write the header to argv[2]
		   write HoughAry to argv[2]	*/
	hough->determineHeader();
	ofstream outFile1 ( argv[2] );
	hough->regularPrintAry( hough->HoughAry, outFile1 );
	
	/*
	step 7: prettyPrint HoughAry to argv[3]	*/
	ofstream outFile2 ( argv[3] );
	hough->prettyPrintAry( hough->HoughAry, outFile2 );
	hough->testPrintAry( hough->HoughAry );
	
	cout<<"Success"<<endl;
}


