import java.util.Scanner;

public class ArcChordDistance {
	
	public int Kchord;
	public int numPts;
	public BoundaryPt[] boundAryPt;
	public double[] chordAry;
	public int P1;
	public int P2;
	public int label;
	
	
	public ArcChordDistance( Scanner inStream1, Scanner inStream2 ){
		label  = inStream1.nextInt();
		numPts = inStream1.nextInt();
		Kchord = inStream2.nextInt(); 
		
		boundAryPt = new BoundaryPt[ numPts ];
		chordAry   = new double[ Kchord ];
		
		for( int i =0; i<numPts; i++){
			boundAryPt[i] = new BoundaryPt();
		}
	}
	
	//store data to boundAryPt 
	public void loadData( Scanner inStream1 ){
		for( int i =0; i<numPts; i++){
			
			boundAryPt[i].x = inStream1.nextInt();
			boundAryPt[i].y = inStream1.nextInt();
		}
	}
	
	// computes and returns the shortest distance from P to the line formed by P1 and P2
	public double computeDistance (int P1, int P2, int Pt){
		Pt = Pt%numPts;
		
		int A = boundAryPt[P1].y - boundAryPt[P2].y;
		int B = boundAryPt[P2].y - boundAryPt[P1].y; 
		double C = (boundAryPt[P1].x*boundAryPt[P2].y) - (boundAryPt[P2].x*boundAryPt[P1].y ) ;
		
		int x = boundAryPt[Pt].x;
		int y = boundAryPt[Pt].y;
		
		double d = Math.abs( A*x + B*y + C )/(Math.sqrt( A*A + B*B )+0.000001);
		
		return d;
	}
	// find the maximum distance in chordAry, and returns the index of the maximum
	public int findMaxDist(){
		double max = 0 ;
		int index = 0;
		
		for( int i =0; i<Kchord; i++){
			if( chordAry[i] > max ){
				max = chordAry[i];
				index = i;
			}
		}
		
		return index;
	}
	// P(i) is a local maxima iff the maxCount of p(i) is >= all the maxCount of 
	// its linear neighbors: p(i-2), p(i-1), p(i+1), p(i+2)
	public void computeLocalMaxima(){
		for( int i = 2; i< numPts+2; i++ ){
			int index = i % numPts;
			
			if( boundAryPt[index].maxCount > boundAryPt[ (i-2)%numPts ].maxCount &&
			    boundAryPt[index].maxCount > boundAryPt[ (i-2)%numPts ].maxCount &&
				boundAryPt[index].maxCount > boundAryPt[ (i+1)%numPts ].maxCount &&
				boundAryPt[index].maxCount > boundAryPt[ (i+2)%numPts ].maxCount ){
				boundAryPt[index].localMax = 1;
			}
			else{
				boundAryPt[index].localMax = 0;
			}
		}
	}
	
	// p(i) is a corner (returns 9) if p(i) is a local maxima AND for all p(i-2), p(i-1), p(i+1), p(i+2)
	// only p(i-1) or p(i+1) can be a local maxima, otherwise, p(i) is not a corner (return 1).
	public void isCorner(){
		for( int i = 2; i< numPts+2; i++ ){
			int index = i % numPts;
			
			if( boundAryPt[index].localMax > 0 &&
				((boundAryPt[ (i-1)%numPts ].maxCount > boundAryPt[(i-2)%numPts].maxCount && 
				boundAryPt[(i-1)%numPts].maxCount > boundAryPt[(i+2)%numPts].maxCount) ||
				(boundAryPt[(i+1)%numPts].maxCount > boundAryPt[(i-2)%numPts].maxCount &&
				boundAryPt[(i+1)%numPts].maxCount > boundAryPt[(i+2)%numPts].maxCount )) )
				{		
				boundAryPt[index].corner = 9;
			}
			else{
				boundAryPt[index].corner = 1;
			}
		}
	}
}
