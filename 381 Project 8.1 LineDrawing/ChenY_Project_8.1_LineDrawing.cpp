#include<iostream>
#include<string>
#include<fstream>
#include <cmath>
using namespace std;
const double pi = 3.14159265358;

class ImageProcessing {	
	public:
	int   numRows = 128;
	int   numCols = 128;
	int   minVal  = 0; 
	int   maxVal  = 1;
	int** imgAry;
	
	int rowTranslate;
	int colTranslate;
	
	ImageProcessing();
	void prettyPrintAry ( int** image, ofstream& outFile );
	void regularPrintAry( int** image, ofstream& outFile );
	void testPrintAry( int** image );	
};

//ImageProcessing Constructor
ImageProcessing::ImageProcessing() 
{	
	this->imgAry   = new int*[numRows];	
	for (int i = 0; i<numRows; i++){
		this->imgAry[i] = new int[numCols] {};
	}	
}

//pretty print 2DArray image to file
void ImageProcessing::prettyPrintAry ( int** image, ofstream& outFile )
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

//regular print 2DArray image to file
void ImageProcessing::testPrintAry( int** image )
{
    	for(int i =0; i<numRows; i++ ){
		 for(int j =0; j<numCols; j++ ){
			if(image[i][j] == 0 )
					cout<<"  ";
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

class LineDrawing
{	
	public:
	double angleInDegree;
	double angleInRadian;
	
	LineDrawing( int angleInDeg );
	void drawLine( ImageProcessing* image);
	void translateRow();
	void translateCol();

};

LineDrawing::LineDrawing( int angleInDeg )
{
	angleInDegree = angleInDeg;
	angleInRadian = angleInDeg * pi / 180;
	
}


void LineDrawing::drawLine( ImageProcessing* image )
{
	float x = image->numRows/2;
	float y = image->numCols/2;
	
	float xx = image->numRows/2;
	float yy = image->numCols/2;
		
	while( true ){
		
		//Due to difference in arrays labels and graph labels, (x,y) -> (-y,x)
		//which is same as rotate ccw 90 degree
		x += cos(angleInRadian);
		y -= sin(angleInRadian);
		
		xx -= cos(angleInRadian);
		yy += sin(angleInRadian);
		
		if( x>127 || x<0 || y>127 || y<0 || xx>127 || xx<0 || yy>127 || yy<0 )
		{
			goto outsideOfWhile;
		}
		
		int x1 = y;
		int y1 = x;
		
		int x2 = yy;
		int y2 = xx;
		
		image->imgAry[x1][y1] = 1;
		image->imgAry[x2][y2] = 1;
		
	} 
	outsideOfWhile:;
}

void LineDrawing::translateRow()
{	
}
void LineDrawing::translateCol()
{	
}
	
int main(int argc, char* argv[])
{
	if(argc< 2){
		cout<<"Check parameters! "<<endl;
		return 0;
	}
	
	//step 0: dynamically allocate imgAry
	ImageProcessing* image = new ImageProcessing();
	
	//step 1: angleInDegree <-- ask the user for it
	//	   angleInRadians <-- angleInDegree * pi /180
	int angleInDeg;
	cout<<"Enter desired angle in degree. "<<endl;
	cin>>angleInDeg;
	angleInDeg %= 180;
	LineDrawing* drawLine = new LineDrawing( angleInDeg );
	
	cout<<"Angle in Degree: "<<drawLine->angleInDegree<<endl;
	cout<<"Angle in Radian: "<<drawLine->angleInRadian<<endl;
	
	//step 2: compute points on the line (in user's given angle)
	//	   passing thru the origin, at the center of imgArry.
	drawLine->drawLine( image );
	
	//step 3: output imgArry to argv[1] // don't forget the header!!!
	ofstream outFile1( argv[1] );
	outFile1<<image->numRows<<" "<<image->numCols<<" 0 1"<<endl;
	image->regularPrintAry( image->imgAry, outFile1 );
	
	//step 4: pretty imgArray to argv[2] 
	ofstream outFile2( argv[2] );
	outFile2<<"Pretty Print of line drawn"<<endl;
	image->prettyPrintAry( image->imgAry, outFile2 );
	
	image->testPrintAry(image->imgAry);
	
	cout<<"Success"<<endl;
}


