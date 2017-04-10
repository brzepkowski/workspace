import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.ListIterator;
import java.util.Random;
import java.util.Scanner;

import javax.swing.text.html.HTMLDocument.Iterator;

import org.jgrapht.alg.ConnectivityInspector;
import org.jgrapht.alg.DijkstraShortestPath;
import org.jgrapht.graph.*;


public final class Exercise_2 {
    
	public static ConnectivityInspector<Integer, DefaultWeightedEdge> connectivityInspector;
	public static SimpleWeightedGraph<Integer, DefaultWeightedEdge> G;
	public static DefaultWeightedEdge[][] E; //Edges will be stored at places 1 lower than their number
							//for example: e(1,2) is on place [0][1]
	public static int[] V;
	public static double[][] H;
	public static int numberOfVertices = 10;
	public static int numberOfEdges = 14;
	
	//Additional structures needed for exercise 2
	public static int[][] N;	//Intensity matrix (Macierz natężeń)
	public static int[][] C;	//Capacity matrix (Macierz przepustowości)
	public static int sizeOfSinglePackage = 1500;
	
    private Exercise_2() {} // ensure non-instantiability.

    
    public static void main(String [] args)
    {        
    	//E = new DefaultWeightedEdge[numberOfVertices][numberOfVertices];
    	V = new int[numberOfVertices];
        G = createWeightedGraph();
        
        //Exercise A
        C = createCapacityMatrix();
        N = createIntensityMatrix();
        
        
        //System.out.println(G.toString());
        //downloadGraph(args[0]);
        
        //Exercise B
        traverseGraph(G);
        
        double T = calculateDelayOfPackage(G);
        System.out.println("T = " + T);
        
        //Exercise C
        double modified_Reliability = estimateModifiedReliablility(10000.0, 0.8 ,2.0940545283104925E-5);
        System.out.println("modified_Reliability = " + modified_Reliability);
    }
    
    private static void downloadGraph(String source) {
    	G = new SimpleWeightedGraph<Integer, DefaultWeightedEdge>(DefaultWeightedEdge.class);
    	 FileReader fr = null;
    	   String linia = "";

    	   // OTWIERANIE PLIKU:
    	   try {
    	     fr = new FileReader(source);
    	   } catch (FileNotFoundException e) {
    	       System.out.println("BŁĄD PRZY OTWIERANIU PLIKU!");
    	       System.exit(1);
    	   }  	
    	   
    	   BufferedReader bfr = new BufferedReader(fr);
    	   int numberOfVertices = 0;
    	   // ODCZYT KOLEJNYCH LINII Z PLIKU:
    	   try {
    		   numberOfVertices = Integer.parseInt(bfr.readLine());
    		   System.out.println("Number of vertices = " + numberOfVertices);
    		   C = new int[numberOfVertices][numberOfVertices];
    		   N = new int[numberOfVertices][numberOfVertices];
    		   String[] vertices = new String[numberOfVertices];
    		   vertices = bfr.readLine().split(" ");
    		   for (int i = 0; i < numberOfVertices; i++) {
    			   G.addVertex(Integer.parseInt(vertices[i]));
    		   }
    		   System.out.println(G.toString());
    		   linia = bfr.readLine();
    	     while(linia != null && !linia.equals("C")){ 
    	    	 String[] edge = new String[2];
    	    	 edge = linia.split("-");
    	    	 G.addEdge(Integer.parseInt(edge[0]), Integer.parseInt(edge[1]));
    	    	 System.out.println("v1 = " + edge[0] + " v2 = " + edge[1]);
    	    	 linia = bfr.readLine();
    	     }
    	     linia = bfr.readLine();
    	     while(linia != null && !linia.equals("N")){ 
    	    	 String[] capacity = new String[3];
    	    	 capacity = linia.split("-");
    	    	 C[Integer.parseInt(capacity[0])-1][Integer.parseInt(capacity[1])-1] = Integer.parseInt(capacity[2]);
    	    	 linia = bfr.readLine();
    	     }
    	     linia = bfr.readLine();
    	     while(linia != null){ 
    	    	 String[] intensity = new String[3];
    	    	 intensity = linia.split("-");
    	    	 N[Integer.parseInt(intensity[0])-1][Integer.parseInt(intensity[1])-1] = Integer.parseInt(intensity[2]);
    	    	 System.out.println(Integer.parseInt(intensity[0]) + " " + Integer.parseInt(intensity[1]) + " = " + Integer.parseInt(intensity[2]));
    	    	 linia = bfr.readLine();
    	     }
    	    } catch (IOException e) {
    	        System.out.println("BŁĄD ODCZYTU Z PLIKU!");
    	        System.exit(2);
    	   }

    	   // ZAMYKANIE PLIKU
    	   try {
    	     fr.close();
    	    } catch (IOException e) {
    	         System.out.println("BŁĄD PRZY ZAMYKANIU PLIKU!");
    	         System.exit(3);
    	        }
    }
    

    private static DefaultWeightedEdge e(int i, int j) {
    	DefaultWeightedEdge e = G.getEdge(i,j);
    	return e;
    }
    
    private static int v(int i) {
    	int v = V[i-1];
    	return v;
    }
    
    private static void h(DefaultWeightedEdge e, double weight) {
    	G.setEdgeWeight(e, weight);
    }
    
    //a(edge) gets value of given edge from matrix N
    private static int a(DefaultWeightedEdge e) {
    	int v1 = G.getEdgeSource(e);
    	int v2 = G.getEdgeTarget(e);
    	//System.out.println("V1 = " + v1 + " V2 = " + v2);
    	int a = N[v1 - 1][v2 - 1];
    	return a;
    }
    
    //a(v1, v2) gets value from given vertices
    private static double a(int v1, int v2) {
    	//System.out.println("V1 = " + v1 + " V2 = " + v2);
    	double a = N[v1 - 1][v2 - 1];
    	return a;
    }
    
    private static double c(DefaultWeightedEdge e) {
    	int v1 = G.getEdgeSource(e);
    	int v2 = G.getEdgeTarget(e);
    	double c = C[v1 - 1][v2 - 1];
    	return c;
    }
    
 
    private static double estimateModifiedReliablility(double numberOfSimulations, double p, double T_max) {
    	double failedSimulations = 0;
    	//DefaultWeightedEdge edge;
    	Random random = new Random();
    	float x; //It will be random float for every edge, to check, if we will be removing it
    	//boolean isConnected = true; //Set to true, if graph HAS NO CUT POINT 
    	int v1, v2; //source and target vertices of edge
    	
    	for (int i = 0; i < numberOfSimulations; i++) {
    		SimpleWeightedGraph<Integer, DefaultWeightedEdge> G_prim = new SimpleWeightedGraph<Integer, DefaultWeightedEdge>(DefaultWeightedEdge.class);
        	G_prim = (SimpleWeightedGraph<Integer, DefaultWeightedEdge>) G.clone();
        	Object[] edgesArray = G_prim.edgeSet().toArray();
        	
        	
        	for (int j = 0; j < edgesArray.length; j++) {
        		//edge = randomEdge();
    			//DefaultWeightedEdge edge = iterator.next();
    			DefaultWeightedEdge edge = (DefaultWeightedEdge) edgesArray[j];
        		x = random.nextFloat();
        		v1 = G_prim.getEdgeSource(edge);
				v2 = G_prim.getEdgeTarget(edge);
    			if (x > p) {
    				G_prim.removeEdge(edge);
    			}
    		}
        	connectivityInspector = new ConnectivityInspector<Integer, DefaultWeightedEdge>(G_prim);
			boolean isConnected = connectivityInspector.isGraphConnected();
			if (isConnected == true) {
				boolean edgeNotRemoved = traverseGraph(G_prim);
				if (edgeNotRemoved == true) {					
						double T = calculateDelayOfPackage(G_prim);
						//System.out.println("T = " + T);
						//System.out.println("New T = " + T);
						if (T == 0) {
							System.out.println("Czas opóźnienia wynosi T = 0!");
							return 0;
						}
						else if (T > T_max) {
							failedSimulations++;
						}
				}
				else {
					System.out.println("Przepływ jest większy od przepustowości - PORAŻKA");
					return 0;
				}
			}
			else {
				failedSimulations++;
			}
    	}
    	double proportion = (numberOfSimulations - failedSimulations) / numberOfSimulations;
    	System.out.println("Successful simulations = " + (numberOfSimulations - failedSimulations) + " | All Simulations = " + numberOfSimulations + " Proportion = " + proportion);
    	return proportion;
    	
    }

    private static boolean traverseGraph(SimpleWeightedGraph G_prim) {
    	for (int i = 0; i < numberOfVertices; i++) {
    		//System.out.println("i Increased = " + i);
    		for (int j = 0; j < numberOfVertices; j++) {
    			//System.out.println("j increased = " + j);
    			if (N[i][j] != 0) {
    				DijkstraShortestPath<Integer, DefaultWeightedEdge> dijkstraPath = new DijkstraShortestPath<Integer, DefaultWeightedEdge>(G, i+1, j+1);
    				for(java.util.Iterator<DefaultWeightedEdge> iterator = dijkstraPath.getPathEdgeList().iterator(); iterator.hasNext(); ) {
    					DefaultWeightedEdge edge = iterator.next();
    					double weight = G_prim.getEdgeWeight(edge);
        				G_prim.setEdgeWeight(edge, weight + N[i][j]);
        				//If flow is higher than capacity we have to remove edge
        				int v1 = (int) G_prim.getEdgeSource(edge);
        				int v2 = (int) G_prim.getEdgeTarget(edge);
        				if (G_prim.getEdgeWeight(edge) > C[v1-1][v2-1]) {
        					//G_prim.removeEdge(edge);
        					System.out.println("Przepływ wiekszy od przepustowości - PORAŻKA");
        					return false;
        				}
        				//System.out.println("Weight of " + edge.toString() + " = " + G.getEdgeWeight(edge));
    				}
    			}
    			else {
    				//System.out.println("----------------------------------");
    			}
    		}
    	}
    	return true;
    }

    private static double calculateDelayOfPackage(SimpleWeightedGraph<Integer, DefaultWeightedEdge> G_prim) {
    	double T = 0, n = 0, SUM_e = 0;
    	//Calculate n (sum of all elements of matrix N)
    	for (int i = 0; i < numberOfVertices; i++) {
    		for (int j = 0; j < numberOfVertices; j++) {
    			n+= N[i][j];
    		}
    	}
    	//System.out.println("n = " + n);
    	
    	//Sum SUM_e
    	for (java.util.Iterator<DefaultWeightedEdge> i = G_prim.edgeSet().iterator(); i.hasNext(); ) {
    		DefaultWeightedEdge edge = i.next();
    		if (((c(edge)/sizeOfSinglePackage) - a(edge)) > 0) {
    			SUM_e += a(edge)/((c(edge)/sizeOfSinglePackage) - a(edge));
    		}
    		else {
    			return 0;
    		}
    		
    	}
    	//System.out.println("SUM_e = " + SUM_e);
    	
    	//Calculate final T
    	T = 1/n * SUM_e;
    	
    	return T;
    }
    
private static SimpleWeightedGraph<Integer, DefaultWeightedEdge> createWeightedGraph() {
    	
    	G = new SimpleWeightedGraph<Integer, DefaultWeightedEdge>(DefaultWeightedEdge.class);

    	//create vertices
    	for (int i = 0; i < 10; i++) {
    		V[i] = i+1;
    		G.addVertex(v(i+1));
    	}
    	
    	//add edges
    	G.addEdge(v(1), v(2));
    	G.addEdge(v(2), v(3));
    	G.addEdge(v(3), v(4));
    	G.addEdge(v(4), v(5));
    	G.addEdge(v(5), v(6));
    	G.addEdge(v(6), v(1));
    	G.addEdge(v(2), v(8));
    	G.addEdge(v(3), v(9));
    	G.addEdge(v(5), v(10));
    	G.addEdge(v(6), v(7));
    	G.addEdge(v(7), v(8));
    	G.addEdge(v(8), v(9));
    	G.addEdge(v(9), v(10));
    	G.addEdge(v(10), v(7));
    	
    	return G;
    }

    private static int[][] createCapacityMatrix() {
    	C = new int[numberOfVertices][numberOfVertices];
    	//Setting all values to 0
    	for (int[] row: C) {
    		Arrays.fill(row, 0);
    	}
    	//Row 1
    	C[0][1] = 1000000000;	//1 Gb/s
    	C[0][5] = 1000000000;
    	//Row 2
    	C[1][0] = 1000000000;
    	C[1][2] = 1000000000;
    	C[1][7] = 10000000;		//10 Mb/s
    	//Row 3
    	C[2][1] = 1000000000;
    	C[2][3] = 1000000000;
    	C[2][8] = 10000000;
    	//Row 4
    	C[3][2] = 1000000000;
    	C[3][4] = 1000000000;
    	//Row 5
    	C[4][3] = 1000000000;
    	C[4][5] = 1000000000;
    	C[4][9] = 10000000;
    	//Row 6
    	C[5][0] = 1000000000;
    	C[5][4] = 1000000000;
    	C[5][6] = 10000000;
    	//Row 7
    	C[6][5] = 10000000;
    	C[6][7] = 10000000;
    	C[6][9] = 10000000;
    	//Row 8
    	C[7][1] = 10000000;
    	C[7][6] = 10000000;
    	C[7][8] = 10000000;
    	//Row 9
    	C[8][2] = 10000000;
    	C[8][7] = 10000000;
    	C[8][9] = 10000000;
    	//Row 10
    	C[9][4] = 10000000;
    	C[9][6] = 10000000;
    	C[9][8] = 10000000;
    	
    	/*
    	System.out.println("Capacity matrix:");
    	for (int i = 0; i < numberOfVertices; i++) {
    		for (int j = 0; j < numberOfVertices; j++) {
    			System.out.print(C[i][j] + " ");
    		}
    		System.out.println();
    	}
    	*/
    	return C;
    }

    private static int[][] createIntensityMatrix() {
    	N = new int[numberOfVertices][numberOfVertices];
    	
    	for (int i = 0; i < numberOfVertices; i++) {
    		for (int j = 0; j < numberOfVertices; j++) {
    			N[i][j] = (j+1)*10;
    		}
    		
    	}

    	//Set "0" on diagonal of matrix
    	for (int i = 0; i < numberOfVertices; i++) {
    		N[i][i] = 0;
    	}
    	
    	/*
    	System.out.println("Intensity matrix: ");
    	for (int i = 0; i < numberOfVertices; i++) {
    		for (int j = 0; j < numberOfVertices; j++) {
    			System.out.print(N[i][j] + " ");
    		}
    		System.out.println();
    	}
    	*/
    	return N;
    }

}
