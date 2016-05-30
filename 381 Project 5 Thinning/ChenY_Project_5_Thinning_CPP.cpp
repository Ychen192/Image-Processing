#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//ImageProcessing Class
class ImageProcessing {
	public:
	int** firstAry;
	int** secondAry;
	bool  changeflag;
	int   numRows, numCols;
	int   minVal, maxVal;
	int   count;
	
	ImageProcessing( ifstream& inFile );
	void loadImage ( ifstream& inFile ); 
	void zeroFramed(); 
	void OneCycleThinning(ofstream& outFile);
	int  findNumOfZerosOfNeighbors(int x, int y);
	void copyAry (int** copyFrom, int** copyTo);
	
	void prettyPrintAry ( int** image, ofstream& outFile, int Rows, int Cols );
	void regularPrintAry( int** image, ofstream& outFile );
};

//ImageProcessing Constructor
ImageProcessing::ImageProcessing( ifstream& inFile ) 
{
	inFile>>this->numRows  ;
	inFile>>this->numCols  ;
	inFile>>this->minVal   ;
	inFile>>this->maxVal   ;
	count = 0;
	
	this->firstAry   = new int*[numRows+2];	
	for (int i = 0; i<numRows+2; i++){
		this->firstAry[i] = new int[numCols+2] {};
	}
	
	this->secondAry  = new int*[numRows+2];	
	for (int i = 0; i<numRows+2; i++){
		this->secondAry[i] = new int[numCols+2] {};
	}	
}

//load the image from textfile into variable imgAry**
void ImageProcessing::loadImage( ifstream& inFile )
{
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			inFile>>firstAry[i][j];
		}
	}		
};

//frame the imgAry with 0's on all edges and store it to firstAry**
void ImageProcessing::zeroFramed()
{
	firstAry = new int*[numRows+2];
	for(int i =0; i<numRows+2; i++ ){
		firstAry[i] = new int[numCols+2] {};
	}	
   //filling the array	 	
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			firstAry[i][j] = firstAry[i-1][j-1];
		}
	}		
};//zeroFramed()

int ImageProcessing::findNumOfZerosOfNeighbors(int x, int y) 
{
	int numOfZeros = 0;

	if( firstAry[x-1][y-1] == 0 )  numOfZeros++;
	if( firstAry[x-1][y]   == 0 )  numOfZeros++;
	if( firstAry[x-1][y+1] == 0 )  numOfZeros++;
	
	if( firstAry[x][y-1]   == 0 )  numOfZeros++;
	if( firstAry[x][y+1]   == 0 )  numOfZeros++;
	
	if( firstAry[x+1][y-1] == 0 )  numOfZeros++;	
	if( firstAry[x+1][y]   == 0 )  numOfZeros++;
	if( firstAry[x+1][y+1] == 0 )  numOfZeros++; 
	
	return numOfZeros;
	
};//findNumOfZerosOfNeighbors

void ImageProcessing::OneCycleThinning( ofstream& outFile )
{	
	
	for(int i = 1; i<numRows+1; i++ ){
	for(int j = 1; j<numCols+1; j++ ){	
		if( firstAry[i][j]>0 ){
			//North 
			if( firstAry[i-1][j]==0 && findNumOfZerosOfNeighbors(i, j) <= 3){
				secondAry[i][j] = 0;
				changeflag = true;	
			}
			//South
			else if( firstAry[i+1][j]==0 && findNumOfZerosOfNeighbors(i, j) <= 3){
				secondAry[i][j] = 0;
				changeflag = true;
			}
			//West
			else if( firstAry[i][j-1]==0 && findNumOfZerosOfNeighbors(i, j) <= 3){
				secondAry[i][j] = 0;
				changeflag = true;
			}
			//East
			else if( firstAry[i][j+1]==0 && findNumOfZerosOfNeighbors(i, j) <= 3){
				secondAry[i][j] = 0;
				changeflag = true;
			}
			else{
				secondAry[i][j] = firstAry[i][j];
			}
		}
	}
	}
	if(count%3 == 0){
		outFile<<"Image after cycle: "<<count<<endl;
		prettyPrintAry ( secondAry, outFile, numRows+2, numCols+2 );
	}
};

void ImageProcessing::copyAry (int** copyFrom, int** copyTo)
{
	for(int i = 0; i<numRows+2; i++ ){
	for(int j = 0; j<numCols+2; j++ ){
		copyTo[i][j] = copyFrom[i][j];
	}
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
void ImageProcessing::regularPrintAry( int** image, ofstream& outFile )
{
    	for(int i =0; i<numRows+2; i++ ){
		for(int j =0; j<numCols+2; j++){
			if(image[i][j] == 0 )
					outFile<<"0 ";
			else
				outFile<<image[i][j]<<" ";
		}
		outFile<<endl;
	}	
	outFile<<endl;
};


int main(int argc, char* argv[]) 
{
	if(argc < 4){
		cout<<"Error: Check Parameters!";
		return 0;
	}
	
	ifstream inFile    (argv[1]);

	/*
	step 0: read the image header & dynamically allocate firstAry and secondary	*/   
    ImageProcessing* img1 = new ImageProcessing( inFile );
   
   /*
   step 1: 1.1 zeroFrame the two arrays.
           1.2 read from the input file onto firstAry	*/  
    img1->loadImage(inFile);
    inFile.close();
 
   /*
   step 2: OneCycleThinning // first cycle, keep track of changeflag
        - NorthThinning // look at firstAry and write the result to secondAry
		- prettyPrint // print secondAry  with caption, in argv[3]
		- SouthThinning // look at firstAry and write the result to secondAry
		- prettyPrint // print secondAry  with caption, in argv[3]
		- WestThinning // look at firstAry and write the result to secondAry
		- prettyPrint // print secondAry  with caption, in argv[3]
		- EastThinning // look at firstAry and write the result to secondAry
		- prettyPrint // print secondAry with caption, in argv[3]	*/  
	ofstream outFile(argv[3]);
	img1->OneCycleThinning( outFile );
	
	if(img1->changeflag)
   		outFile<<"After cycle: "<<img1->count<<", changeflag = true"<<endl;
   	else
   		outFile<<"After cycle: "<<img1->count<<", changeflag = false"<<endl;
   	
   	img1->count++;

   /*	
   step 3: if (changeFlag is true)
        - set changeFlag to false
        - copy secondAry to firstAry
        - OneCycleThinning // N, S, W, E, keep track of changeflag
        - prettyPrint // print secondAry with caption, on argv[3],
         for every third (3rd) cycle.	
   step 4: repeat step 3 while changeFlag == true	*/  
   
   while( img1->changeflag == true){
   		img1->changeflag = false;
   		img1->copyAry (img1->secondAry, img1->firstAry);
   		img1->OneCycleThinning( outFile );
   		
		if(img1->changeflag)
   			outFile<<"After cycle: "<<img1->count<<", changeflag = true"<<endl;
   		else
   			outFile<<"After cycle: "<<img1->count<<", changeflag = false"<<endl;
   		img1->count++;
   }
   
   
   /*
   step 5: create a image file from secondAry in argv[2]
        prettyPring // print the final secondAry to argv[3]	*/  
   	ofstream outFile1(argv[2]);
   	outFile1<<img1->numRows+2<<" "<<img1->numCols+2<<" 0 "<<"1 "<<endl;
    img1->regularPrintAry ( img1->firstAry, outFile1 );
    
    outFile<<endl<<"Final secondAry Pretty Print	"<<endl;
    img1->prettyPrintAry ( img1->secondAry, outFile, img1->numRows+2, img1->numCols+2 );
    
    outFile.close();
    outFile1.close();
	cout<<"Program successful!"<<endl;

}//main
