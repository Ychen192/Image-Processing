
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws IOException {

		/*step 0: - open input files and output files
			  (numRows, numCols, minVal, maxVal, label, numPts ) <- read from argv[1]
			  - dynamically allocate boundPtAry with size of numPts
			  - Kchord <-- get from argv[2]
			  - dynamically allocate chordAry with size of Kchord // initiallied to 0.0
			  - index  <-- 0	*/	
		Scanner    inStream1  = new Scanner( new FileReader( args[0] ) );
		Scanner    inStream2  = new Scanner( new FileReader( args[1] ) );
		BufferedWriter outStream1 = new BufferedWriter(new FileWriter( args[2] ) );
		BufferedWriter outStream2 = new BufferedWriter(new FileWriter( args[3] ) );
		BufferedWriter outStream3 = new BufferedWriter(new FileWriter( args[4] ) );
		
		Image 			 image1 = new Image( inStream1 );
		ArcChordDistance acDist = new ArcChordDistance( inStream1, inStream2 );
		 
		/* step 1-3: (x, y) <-- read from input until file empty */
		acDist.loadData( inStream1 );
		
		/* step 4:  intiallize P1, P2 	*/
		acDist.P1 = acDist.Kchord;
		acDist.P2 = 0;
		
		
		while( acDist.P1 != (acDist.Kchord / 2) ){
			/* step 5: index <-- 0
				   currPt <-- P2 + 1	*/
			int index = 0;
			int currPt = acDist.P1+1;
			
			/* step 6: dist <-- computeDistance (P1, P2, currPt )
			        store dist to chordAry[index]
				    index ++
				    currPt ++	*/
			while(index< acDist.Kchord){
				double dist = acDist.computeDistance( acDist.P1, acDist.P2, currPt);
				acDist.chordAry[index] = dist;
				index++;
				currPt++;
			}/* step 7: repeat step 6 while index < Kchord	*/
			
			/* step 8: print chordAry to debugging file (argv[5])	*/
			for( int i=0; i<acDist.Kchord; i++){
				outStream3.write(acDist.chordAry[i] + " ");
			}
			outStream3.newLine();
			outStream3.flush();
			
			
			/* step 9: maxIndex <-- findMaxDist // find the max in chordAry and return that index
			        whichIndex <-- P1 + maxIndex  	   
				    increment bountdPtAry[whichIndex]'s maxCount by one
			        update bountdPtAry[whichIndex]'s maxDist if necessary	*/
			int maxIndex = acDist.findMaxDist();
		
			int whichIndex = (acDist.P1 + maxIndex)%acDist.numPts;
			acDist.boundAryPt[whichIndex].maxCount++;
			acDist.boundAryPt[whichIndex].maxDistance = acDist.chordAry[maxIndex];
			
			/* step 10: print bountPtAry from P1 to P2 	*/
		/*	for(int i = acDist.P2; i< acDist.P1; i++){
				outStream3.write( acDist.boundAryPt[i].x + " " +acDist.boundAryPt[i].y + "\t" );
			}
			outStream3.newLine();
			outStream3.flush();
		*/
			
			/* step 11: Increment P1, and P2, then mod numPts	*/
			acDist.P1 = (acDist.P1+1) % acDist.numPts;
			acDist.P2 = (acDist.P1+1) % acDist.numPts;
		}
		/* step 12: repeat step 5 to step 11 until P1 == (Kchord / 2)	*/
		outStream3.newLine();

		
		/* step 13: print the info (x, y, maxCount, maxDist) of the entire boundaryAry to argv[5] // five pts per textline	*/
		for(int i =0; i< acDist.numPts; i++ ){	
			outStream3.write(acDist.boundAryPt[i].x + " " );
			outStream3.write(acDist.boundAryPt[i].y + " " );
			outStream3.write(acDist.boundAryPt[i].maxCount + " " );
			outStream3.write(acDist.boundAryPt[i].maxDistance + " " );
			outStream3.write("\r\n");
		}
		
		/* step 14: compute LocalMaxima for all point in boundPtAry[index], index from 0 to numPts-1	*/
		acDist.computeLocalMaxima();
		
		/* step 15: compute isCorner for all point in boundPtAry[index], index from 0 to numPts-1	*/
		acDist.isCorner();
		
		/* step 16: output the info (x, y, corner) of the entire boundaryAry to argv[3]	*/
		outStream1.write(image1.numRows + " " + image1.numCols + " " );
		outStream1.write(image1.minVal + " " + image1.maxVal + " \r\n");
		outStream1.write(acDist.label + " \r\n");
		outStream1.write(acDist.numPts + " \r\n");
		for(int i = 0; i<acDist.numPts; i++){
			outStream1.write(acDist.boundAryPt[i].x + "  ");
			outStream1.write(acDist.boundAryPt[i].y + "  ");
			outStream1.write(acDist.boundAryPt[i].corner + " \r\n");
		}
		
		
		/* step 17: Img <-- create an image of size numRows by numCols	
		 * step 18: for each boundaryAry[i], put it corner value at Img(x, y)	*/
		for(int i = 0; i<acDist.numPts; i++){
			if(acDist.boundAryPt[i].corner > 1 )
				image1.imgAry[ acDist.boundAryPt[i].x ][ acDist.boundAryPt[i].y ] = 9;
			else
				image1.imgAry[ acDist.boundAryPt[i].x ][ acDist.boundAryPt[i].y ] = 1;
		}
		
		/* step 19: prettyPrint Img to argv[4]		    */
		image1.prettyPrintAry( image1.imgAry, outStream2);

		inStream1.close();
		inStream2.close();
		outStream1.close();
		outStream2.close();
		outStream3.close();
		
		System.out.println("Success! ");
	}

}
