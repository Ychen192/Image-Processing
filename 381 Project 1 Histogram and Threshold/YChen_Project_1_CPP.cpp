#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//Image Class
class Image {
	public:
	int** imgAry;
	int*  histogram; 
	int   numRows, numCols;
	int   minVal, maxVal;
	
	Image(int numRows, int numCols, int minVal, int maxVal);
	void loadImage( ifstream& inFile ); 	// load imgAry from input file
	int** zeroFramed(); // In your project 0
	int** mirrorFramed(); // In your project 0
	void computeHistogram(); //tally up the histogram into an array
	void printHistogram();//print histogram in a graph view
	void computeThreshold( int thresholdNum );
	void prettyPrint( string filename );
};
//Image Constructor
Image::Image(int numRows, int numCols, int minVal, int maxVal) 
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
void Image::loadImage( ifstream& inFile )
{
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			inFile>>imgAry[i][j];
		}
	}		
};

//compute histogram for GreyScale Image
void Image::computeHistogram()
{
	histogram = new int[maxVal+1];
	for(int i = 0; i<maxVal+1; i++)
		histogram[i] = 0;
		
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			histogram[ imgAry[i][j] ]++;
		}
	}	
};

//print the histogram
void Image::printHistogram()
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
void Image::computeThreshold( int thresholdNum)
{
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
void Image::prettyPrint( string filename )
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
int** Image::zeroFramed()
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
int** Image::mirrorFramed()
{
	int** newBinaryImageArray = new int*[numRows+2];
	for(int i =0; i<numRows+2; i++ ){
		newBinaryImageArray[i] = new int[numCols+1];
	}
	
	//filling the original array inside the frame	
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			newBinaryImageArray[i][j] = imgAry[i-1][j-1];
		}
	}
	
//filling the edges of the frame
	//top edge
	for(int i = 0; i<numCols+2; i++){
		newBinaryImageArray[0][i] = newBinaryImageArray[1][i];
	}
	//left edge
	for(int i = 0; i<numRows+2; i++){
		newBinaryImageArray[i][0] = newBinaryImageArray[i][1];
	}
	//bottom edge
	for(int i = 0; i<numCols+2; i++){
		newBinaryImageArray[numRows+1][i] = newBinaryImageArray[numRows][i];
	}
	//right edge
	for(int i = 0; i<numRows+2; i++){
		newBinaryImageArray[i][numCols+1] = newBinaryImageArray[i][numCols];
	}
	
	//top left corner
	 newBinaryImageArray[0][0] = newBinaryImageArray[1][1];
	//top right corner
	 newBinaryImageArray[0][numCols+1] = newBinaryImageArray[1][numCols];
	//bottom left corner 
	 newBinaryImageArray[numRows+1][0] = newBinaryImageArray[numRows][1];
	//bottom right corner
	 newBinaryImageArray[numRows+1][numCols+1] =newBinaryImageArray[numRows][numCols];
	
	return newBinaryImageArray;
	
}; 

int main(int argc, char* argv[]) 
{
	//string inFileName = "2D_binaryDiamond.txt";
	//string inFileName2 = "2D_greyScaleDiamond.txt";
	
	if(argv[2] != NULL){
		cout<<"Only need one argument!!!!"<<endl;
		cout<<"Please enter the input filename as argument!!"<<endl;
		return 0;
	}
	
	ifstream inFile(argv[1]);
	
	int rowSize, colSize, min, max;
	
	inFile>>rowSize;
	inFile>>colSize;
	inFile>>min;
	inFile>>max; 
		
   //Create Image Object
   Image* img1 = new Image(rowSize, colSize, min, max);
   
   //loading the Object array with file data
   img1->loadImage(inFile);
   inFile.close();   
   //compute Histogram with the objects image array
	img1->computeHistogram();
    img1->printHistogram();
   
   //given the threshold number create a binary file and save to text file
    img1->computeThreshold(15);
    img1->computeThreshold(30);
    img1->computeThreshold(45);
    img1->computeThreshold(47);
    
   //print the new computed binary file to onsole from given textFiles
    img1->prettyPrint("Threshold15.txt");
    img1->prettyPrint("Threshold30.txt");
    img1->prettyPrint("Threshold45.txt");
    img1->prettyPrint("Threshold47.txt");

}//main
