import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Scanner;

public class Image {
	
	public int numRows;
	public int numCols;
	public int minVal; 
	public int maxVal;
	public int[][] imgAry;
		
	public Image( Scanner inStream1 ) 
	{	
		numRows = inStream1.nextInt() ;
		numCols = inStream1.nextInt() ;
		minVal  = inStream1.nextInt() ; 
		maxVal  = inStream1.nextInt() ;
		
		imgAry   = new int[numRows][numCols];
	}
	
	void prettyPrintAry ( int[][] image, BufferedWriter outStream2 ) throws IOException
	{
		for(int i =0; i<numRows; i++ ){
		 for(int j =0; j<numCols; j++){
				if(image[i][j] == 0 )
					outStream2.write("  ");
				else
					outStream2.write( image[i][j] + " ");
		 }
		 outStream2.write("\r\n");
		}	
		outStream2.write("\r\n");
	};
	
	
	

}
