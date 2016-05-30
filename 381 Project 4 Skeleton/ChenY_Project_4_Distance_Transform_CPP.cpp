#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//ImageProcessing Class
class ImageProcessing {
	public:
	int** imgAry;
	int** zeroFramedAry;
	int** tempAry;
	int   maxValueOf8DTpassII;
	int   numRows, numCols;
	int   minVal, maxVal;
	
	ImageProcessing( ifstream& inFile );
	void loadImage ( ifstream& inFile ); 
	void zeroFramed(); 

	void eight_Distance_Transform( string filename);
	void computeSkeleton();
	
	void prettyPrintAry ( int** image, ofstream& outFile, int Rows, int Cols );
	void regularPrintAry( int** image, ofstream& outFile );
	
	int findMinOfNeighborsABCD(int x, int y);
	int findMinOfNeighborsEFGH(int x, int y);
	int findMaxOf8Neighbors   (int x, int y);

};

//ImageProcessing Constructor
ImageProcessing::ImageProcessing( ifstream& inFile ) 
{
	inFile>>this->numRows  ;
	inFile>>this->numCols  ;
	inFile>>this->minVal   ;
	inFile>>this->maxVal   ;
	this->imgAry   = new int*[numRows];
	this->maxValueOf8DTpassII = 0;
		
	for (int i = 0; i<numRows; i++){
		this->imgAry[i] = new int[numCols] {};
	}
}
//load the image from textfile into variable imgAry**
void ImageProcessing::loadImage( ifstream& inFile )
{
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			inFile>>imgAry[i][j];
		}
	}		
};

//frame the imgAry with 0's on all edges and store it to zeroFramedAry**
void ImageProcessing::zeroFramed()
{
	zeroFramedAry = new int*[numRows+2];
	for(int i =0; i<numRows+2; i++ ){
		zeroFramedAry[i] = new int[numCols+2] {};
	}	
   //filling the array	 	
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			zeroFramedAry[i][j] = imgAry[i-1][j-1];
		}
	}		
};//zeroFramed()

int ImageProcessing::findMinOfNeighborsABCD(int x, int y) 
{
	int min = zeroFramedAry[x-1][y-1];
	
	if( zeroFramedAry[x-1][y]   < min ) 	min = zeroFramedAry[x-1][y];	
	if( zeroFramedAry[x-1][y+1] < min )		min = zeroFramedAry[x-1][y+1];
	if( zeroFramedAry[x][y-1]   < min ) 	min = zeroFramedAry[x][y-1];
	
	return min;
	
};//findMinOfNeighborsABCD

int ImageProcessing::findMinOfNeighborsEFGH(int x, int y) 
{
	int min = zeroFramedAry[x][y+1];
	
	if( zeroFramedAry[x+1][y-1] < min ) 	min = zeroFramedAry[x+1][y-1];	
	if( zeroFramedAry[x+1][y]   < min ) 	min = zeroFramedAry[x+1][y];
	if( zeroFramedAry[x+1][y+1] < min ) 	min = zeroFramedAry[x+1][y+1];
	
	return min;
	
};//findMinOfNeighborsEFGH

void ImageProcessing::eight_Distance_Transform( string filename )
{
   //PASS I
	for( int i = 1; i< numRows+1; i++){
		for( int j = 1; j< numCols+1; j++){
			
			if(zeroFramedAry[i][j] > 0){
				zeroFramedAry[i][j] = findMinOfNeighborsABCD(i, j) + 1;			
			}
		}
	}
	ofstream outFile(filename);
	outFile<<"Pass I	"<<endl;
	prettyPrintAry(zeroFramedAry, outFile, numRows+2, numCols+2 );

   //PASS II
   	for( int i = numRows+1; i> 1; i--){
		for( int j = numCols+1; j>1; j--){
			
			if(zeroFramedAry[i][j] > 0){
				int minPlusOne = findMinOfNeighborsEFGH(i, j) + 1;
				
				if(zeroFramedAry[i][j] < minPlusOne)
					zeroFramedAry[i][j] = zeroFramedAry[i][j];
				else
					zeroFramedAry[i][j] = minPlusOne;
					
				if(zeroFramedAry[i][j] > maxValueOf8DTpassII)	maxValueOf8DTpassII = zeroFramedAry[i][j]; 	//header purpose
					
			}	
		}
	}
	outFile<<"Pass II	"<<endl;
	prettyPrintAry(zeroFramedAry, outFile, numRows+2, numCols+2 );
	outFile.close();
	
};//eight_Distance_Transform

int ImageProcessing::findMaxOf8Neighbors(int x, int y) 
{
	int max = zeroFramedAry[x-1][y-1];
	
	if( zeroFramedAry[x-1][y]   > max ) max = zeroFramedAry[x-1][y];
	if( zeroFramedAry[x-1][y+1] > max ) max = zeroFramedAry[x-1][y+1];	
	
	if( zeroFramedAry[x][y-1]   > max ) max = zeroFramedAry[x][y-1];
	if( zeroFramedAry[x][y+1]   > max ) max = zeroFramedAry[x][y+1];
	
	if( zeroFramedAry[x+1][y-1] > max ) max = zeroFramedAry[x+1][y-1];	
	if( zeroFramedAry[x+1][y]   > max ) max = zeroFramedAry[x+1][y];
	if( zeroFramedAry[x+1][y+1] > max ) max = zeroFramedAry[x+1][y+1];
	
	return max;
	
};//findMaxOf8Neighbors

void ImageProcessing::computeSkeleton()
{
	
	tempAry = new int*[ numRows+2];
	for (int i = 0; i<numRows+2; i++)
		tempAry[i] = new int[numCols+2] {};
		
	for( int i = 1; i< numRows+1; i++){
	for( int j = 1; j< numCols+1; j++){
		if(zeroFramedAry[i][j] > 0){
				
			int max = findMaxOf8Neighbors(i, j);
				
			if( zeroFramedAry[i][j] >= max)
					tempAry[i][j] =  zeroFramedAry[i][j];
			else
					tempAry[i][j] = 0;
		}
	}
	}
	
};//computeSkeleton

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
void ImageProcessing::regularPrintAry( int** image, ofstream& outFile )
{
    	for(int i =0; i<numRows+2; i++ ){
		for(int j =0; j<numCols+2; j++){
			if(image[i][j] == 0 )
					outFile<<"  ";
			else
				outFile<<image[i][j]<<" ";
		}
		outFile<<endl;
	}	
	outFile<<endl;
};


int main(int argc, char* argv[]) 
{
	
	ifstream inFile    (argv[1]);

	/*
	step 0: read the image header & dynamically allocate imgAry	*/   
    ImageProcessing* img1 = new ImageProcessing( inFile );
   
    /*
    step 1: load the input image onto imgAry
    		dynamically allocate zeroFramedAry
			apply zeroframe to imgAry and save to zeroFramedAry	*/
   	img1->loadImage(inFile);
   	img1->zeroFramed();
  	inFile.close();   

	/*
    step 2: 2.1 firstPassDistance // algorithm taught in class

	   2.2 prettyPrintDistace of the result of Pass-1 
		with caption, in argv[4]
	
		step 3: 3.1 secondPassDistance // algorithm taught in class

	   3.2 prettyPrintDistace of the result of Pass-2 
			with caption, in argv[4]						 	*/
    img1->eight_Distance_Transform( argv[4]);
    
    /*
       3.3 Create a distance transform image from the result of Pass-2 
		with image header, in argv[2], for future processing.	*/
    ofstream outFile(argv[2]);
    outFile<<img1->numRows+2<<" "<<img1->numCols+2<<" "<<"0 "<<img1->maxValueOf8DTpassII<<" "<<endl;
    img1->prettyPrintAry(img1->zeroFramedAry, outFile, img1->numRows+2, img1->numCols+2 );
    outFile.close();
	
	
	/*
    step 4: 4.1 computeSkeleton from the result of Pass-2.
		// see method in the above 										 	*/
	img1->computeSkeleton();
	
	
	/*
 	   4.2 prettyPrintDistace of the skeleton 
		with caption, in argv[4]										 	*/	
	outFile.open(argv[4],  ofstream::app );
	outFile<<"prettyPrintDistace of the skeleton: "<<endl;
	img1->prettyPrintAry( img1->tempAry, outFile, img1->numRows+2, img1->numCols+2);
	outFile.close();
		
	/*
	   4.3 Create a skeleton image from the result of computeSkeleton 
		with image header, write to argv[3] for future processing.		 	*/
	outFile.open(argv[3]);
	outFile<<img1->numRows+2<<" "<<img1->numCols+2<<" "<<"0 "<<img1->maxValueOf8DTpassII<<" "<<endl;
	img1->regularPrintAry( img1->tempAry, outFile);
	
    
	cout<<"Program successful!"<<endl;

}//main
