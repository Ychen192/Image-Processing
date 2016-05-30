#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//ImageProcessing Class
class ImageProcessing {
	public:
	int** imgAry;
	int** mirrorFramedImgAry;
	int*  histogram; 
	int   numRows, numCols;
	int   minVal, maxVal;
	
	ImageProcessing(int numRows, int numCols, int minVal, int maxVal);
	void loadImage( ifstream& inFile ); 	// load imgAry from input file
	int** zeroFramed(); // In your project 0
	void mirrorFramed(); // In your project 0
	void computeHistogram(); //tally up the histogram into an array
	void printHistogram();//print histogram in a graph view
	void computeThreshold( int thresholdNum );
	void prettyPrint( string filename );
	void avg3x3();
};
//ImageProcessing Constructor
ImageProcessing::ImageProcessing(int numRows, int numCols, int minVal, int maxVal) 
{
	this->numRows  = numRows;
	this->numCols  = numCols;
	this->minVal   = minVal;
	this->maxVal   = maxVal;
	this->imgAry   = new int*[numRows];
		
	for (int i = 0; i<numRows; i++){
		this->imgAry[i] = new int[numCols];
	}
}

//Load Array using file data
void ImageProcessing::loadImage( ifstream& inFile )
{
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			inFile>>imgAry[i][j];
		}
	}		
};

//compute histogram for GreyScale ImageProcessing
void ImageProcessing::computeHistogram()
{
	histogram = new int[maxVal+1]  {};
		
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			histogram[ imgAry[i][j] ]++;
		}
	}	
};

//print the histogram
void ImageProcessing::printHistogram()
{
	for(int i = 0; i < maxVal+1; i++){
		cout<<i<<"\t:";		
		if(histogram[i]>80)
			for(int j = 0; j<80; j++){ cout<<"+"; }	
		else
			for(int j = 0; j<histogram[i]; j++){ cout<<"+"; }	
		cout<<endl;
	}
};

//the grey scale image array is converted into binary using threshold number
void ImageProcessing::computeThreshold( int thresholdNum)
{
	string file = "Threshold_";
	file.append( to_string(thresholdNum) );
	file.append( ".txt" );
	ofstream outFile(file);
	outFile<<numRows<<" "<<numCols<<" "<< "0" <<" "<<  "1"  <<endl;
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			if(imgAry[i][j]<thresholdNum)
				outFile<<"0"<<" ";
			else
				outFile<<"1"<<" ";
		}
		outFile<<"\n\r";
	}	
};

//print the the binary where the zero is replaced by space
void ImageProcessing::prettyPrint( string filename )
{
	int input;
	ifstream inFile(filename);
	//ofstream outFile("prettyPrint" + filename );
	//outFile<<numRows<<" "<<numCols<<" "<< "0" <<" "<<  "1"  <<endl;
	cout<<filename<<endl;
	cout<<numRows<<" "<<numCols<<" "<< "0" <<" "<<  "1"  <<endl;
	
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			inFile>>input;
			if( input <= 0 )
				cout<<' ';
			else
				cout<<'1';
		}
		//outFile<<"\n\r";
		cout<<endl;
	}	
};

//frame the image array with zeros on all four side
int** ImageProcessing::zeroFramed()
{
	int** newBinaryImageArray = new int*[numRows+2];
	for(int i =0; i<numRows+2; i++ ){
		newBinaryImageArray[i] = new int[numCols+1];
	}	
	//zeroing the array
	for(int i =0; i<(numRows+2); i++ ){
		for(int j =0; j<(numCols+2); j++){
			newBinaryImageArray[i][j]=0;
		}
	}
   //filling the array	 	
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			newBinaryImageArray[i][j] = imgAry[i-1][j-1];
		}
	}
	
  return newBinaryImageArray;
	
}; 

//frame the image array with mirror edges
void ImageProcessing::mirrorFramed()
{
	mirrorFramedImgAry = new int*[numRows+2];
	for(int i =0; i<numRows+2; i++ ){
		mirrorFramedImgAry[i] = new int[numCols+2] {};
	}
	
	//filling the original array inside the frame	
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			mirrorFramedImgAry[i][j] = imgAry[i-1][j-1];
		}
	}
	
	//filling the edges	
	for(int i =0; i<numRows+2; i++ ){
		for(int j =0; j<numCols+2; j++){
			if		(i == 0 && j == 0)      { mirrorFramedImgAry[i][j] = mirrorFramedImgAry[i+1][j+1]; }   //top left
			else if (i == 0 && j==numCols+1){ mirrorFramedImgAry[i][j] = mirrorFramedImgAry[i+1][j-1]; }   //top right corner
			else if	(i == 0)         {	mirrorFramedImgAry[i][j] = mirrorFramedImgAry[i+1][j]  ; }   //top edge
			else if (i == numRows+1) {	mirrorFramedImgAry[i][j] = mirrorFramedImgAry[i-1][j]  ; }	//bottom edge
			else if (j == 0)    	 {	mirrorFramedImgAry[i][j] = mirrorFramedImgAry[i]  [j+1]; }	//left edge
			else if (j == numCols+1) {	mirrorFramedImgAry[i][j] = mirrorFramedImgAry[i]  [j-1]; }	//right edge
		}
	}
}; 

void ImageProcessing::avg3x3()
{
	int** tempAry = new int*[numRows];
	for(int i = 0; i<numCols; i++){
		tempAry[i] = new int[numCols] {};
	}
	
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			tempAry[i][j] = ( mirrorFramedImgAry[i]  [j] + mirrorFramedImgAry[i]  [j+1] + mirrorFramedImgAry[i] [j+2]  + 
							  mirrorFramedImgAry[i+1][j] + mirrorFramedImgAry[i+1][j+1] + mirrorFramedImgAry[i+1][j+2] +
							  mirrorFramedImgAry[i+2][j] + mirrorFramedImgAry[i+2][j+1] + mirrorFramedImgAry[i+2][j+2] )/9;
		}
	}
	
	int newMax = 0;
	
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			if(newMax <= tempAry[i][j]){
				newMax = tempAry[i][j];
			}
		}

	}	
	
	ofstream outFile("AVG3X3Out.txt");
	outFile<<numRows<<" "<<numCols<<" "<< "0" <<" "<<  newMax <<endl;
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			outFile<<tempAry[i][j]<<" ";
		}
		outFile<<"\n\r";
	}	
};

int main(int argc, char* argv[]) 
{
	//string inFileName = "2D_binaryDiamond.txt";
	//string inFileName2 = "2D_greyScaleDiamond.txt";
	
	ifstream inFile(argv[1]);
	
	int rowSize, colSize, min, max;
	
	inFile>>rowSize;
	inFile>>colSize;
	inFile>>min;
	inFile>>max; 
		
   //Create ImageProcessing Object
   ImageProcessing* img1 = new ImageProcessing(rowSize, colSize, min, max);
   
   //loading the Object array with file data
   img1->loadImage(inFile);
   inFile.close(); 
   
   //apply mirror frame to imgAry and store in mirrorFramedImgAry in ImageProcessing Class
   img1->mirrorFramed();   
 	
 	//apply 3x3 averaging to mirroredFramedImgAry
 	img1->avg3x3();
 	
 	ifstream inFile1("AVG3X3Out.txt");
 	inFile1>>rowSize;
	inFile1>>colSize;
	inFile1>>min;
	inFile1>>max; 
	
	//create new object
 	ImageProcessing* avgImg = new ImageProcessing(rowSize, colSize, min, max);
 	
 	//load the median3x3 applied imgAry from file into Class
 	avgImg->loadImage(inFile1);
 	
 	//compute and print Histogram of averaged image
 	avgImg->computeHistogram();
 	avgImg->printHistogram();
 	
 	int thresholdNum = stoi(argv[2]);
 	avgImg->computeThreshold(thresholdNum);
 	
 	//apply the threshold number from 2nd parameter 
	string thresholdFilename = "Threshold_";
	thresholdFilename.append( to_string(thresholdNum) );
	thresholdFilename.append( ".txt" );
 	
	//print the filtered image on console with no 0s
 	avgImg->prettyPrint(thresholdFilename);


}//main
