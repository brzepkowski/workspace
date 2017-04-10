import java.util.List;
import java.util.Random;
import java.util.concurrent.CopyOnWriteArrayList;

import org.jgrapht.alg.ConnectivityInspector;
import org.jgrapht.graph.*;


public final class Exercise_1 {
    
	public static ConnectivityInspector<Integer, DefaultWeightedEdge> connectivityInspector;
	public static SimpleWeightedGraph<Integer, DefaultWeightedEdge> G;
	public static DefaultWeightedEdge[][] E; //Edges will be stored at places 1 lower than their number
							//for example: e(1,2) is on place [0][1]
	public static int[] V;
	public static double[][] H;
	public static int numberOfVertices = 20;
	public static int numberOfEdges = 19;
	
    private Exercise_1() {} // ensure non-instantiability.

    
    public static void main(String [] args)
    {        
    	//E = new DefaultWeightedEdge[numberOfVertices][numberOfVertices];
    	V = new int[numberOfVertices];
        G = createWeightedGraph();
        
        //Exercise A
        System.out.println("Exercise A:");
        double proportion = estimateReliablility(10000.0);
        System.out.println("Proportion = " + proportion);
        
        //Exercise B
        System.out.println("Exercise B:");
        numberOfEdges++;
        G.addEdge(v(1), v(20));
        h(e(1,20), 0.95);
        proportion = estimateReliablility(10000.0);
        System.out.println("Proportion = " + proportion);
        
        
        //Exercise C
        System.out.println("Exercise C:");
        numberOfEdges = numberOfEdges + 2;
        G.addEdge(v(1), v(10));
        h(e(1, 10), 0.8);
        G.addEdge(v(5), v(15));
        h(e(5, 15), 0.7);
        proportion = estimateReliablility(10000.0);
        System.out.println("Proportion = " + proportion);
        
        
        //Exercise D
        System.out.println("Exercise D:");
        G.removeEdge(e(1, 20));
        numberOfEdges--;
        G.removeEdge(e(1, 10));
        G.removeEdge(e(5, 15));
        numberOfEdges -= 2;
        
        numberOfEdges += 4;
        Random random = new Random();
        for (int i = 0; i < 4; i++) {
        	int j = random.nextInt(20) + 1;
        	int k = random.nextInt(20) + 1;
        	if (j == k) {
        		i--;	//It will be increased by for, so we will random this edge one more time
        	}
        	else {
        		G.addEdge(v(j), v(k));
        		h(e(j, k), 0.4);
        	}
        }
        proportion = estimateReliablility(10000.0);
        System.out.println("Proportion = " + proportion);
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
   
    /*
    //Complicated, but working random of edge
    private static DefaultWeightedEdge randomEdge() {
    	String setOfEdges = G.edgeSet().toString();
    	setOfEdges = setOfEdges.substring(1, setOfEdges.length() - 1); //We are removing "[" and "]"
    	String[] arr = setOfEdges.split(", ");
    	Random randomSubstringEdge = new Random();
    	String stringEdge = arr[randomSubstringEdge.nextInt(arr.length)];
    	String[] splitedStringEdge = stringEdge.split(" : ");
    	//System.out.println("Edge = " + splitedStringEdge[0] + " | " + splitedStringEdge[1]);
    	String firstVertex = splitedStringEdge[0].substring(1, splitedStringEdge[0].length());
    	String secondVertex = splitedStringEdge[1].substring(0, splitedStringEdge[1].length()-1);
    	//System.out.println("First number: " + firstVertex + "|" + secondVertex);
    	int v1 = Integer.parseInt(firstVertex);
    	int v2 = Integer.parseInt(secondVertex);
    	//System.out.println("Final v1 = " + v1 + ", v2 = " + v2);
    	DefaultWeightedEdge edge = e(v1, v2);
    	return edge;
    }
    */
    
    //It returns the proportion of SUCCESFULL_SIMULATIONS/ALL_SIMULATIONS
    private static double estimateReliablility(double numberOfSimulations) {
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
    			if (x > G_prim.getEdgeWeight(edge)) {
    				v1 = G_prim.getEdgeSource(edge);
    				v2 = G_prim.getEdgeTarget(edge);
    				//System.out.println("Edge " + edge.toString() + " will be removed. Edge's weight = " + G_prim.getEdgeWeight(edge));
    				G_prim.removeEdge(edge);
    				connectivityInspector = new ConnectivityInspector<Integer, DefaultWeightedEdge>(G_prim);
    				boolean isConnected = connectivityInspector.isGraphConnected();
    				if (isConnected == true) {
    					//succesfullSimulations++;
    				}
    				else {
    					failedSimulations++;
    					//System.out.println("REMOVED - NOT CONNECTED");
    					break;
    				}
    				//System.out.println("Edge " + edge.toString() + " will be removed || IsConnected = " + isConnected);
    				//G.addEdge(v1, v2);
    			}
    			else {
    				connectivityInspector = new ConnectivityInspector<Integer, DefaultWeightedEdge>(G_prim);
    				boolean isConnected = connectivityInspector.isGraphConnected();
    				//System.out.println("Edge " + edge.toString() + " will NOT be removed || IsConnected = " + isConnected);
    				if (isConnected == true) {
    					//succesfullSimulations++;
    				}
    				else {
    					failedSimulations++;
    					break;
    				}
    			}
    		}
    	}
    	double proportion = (numberOfSimulations - failedSimulations) / numberOfSimulations;
    	//System.out.println("Successful simulations = " + (numberOfSimulations - failedSimulations) + " | All Simulations = " + numberOfSimulations + " Proportion = " + proportion);
    	return proportion;
    	
    }
    
    
    
    private static SimpleWeightedGraph<Integer, DefaultWeightedEdge> createWeightedGraph() {
    	
    	G = new SimpleWeightedGraph<Integer, DefaultWeightedEdge>(DefaultWeightedEdge.class);

    	//create vertices
    	for (int i = 0; i < 20; i++) {
    		V[i] = i+1;
    		G.addVertex(v(i+1));
    	}
    	
    	//add edges
    	for (int i = 1; i <= numberOfEdges; i++) {
    		DefaultWeightedEdge edge = G.addEdge(v(i), v(i+1));
    		//E[i-1][i] = edge; //we are holding edges at indexes 1 lower than i
    	}
    	
    	//create weights
    	for (int i = 1; i <= numberOfEdges; i++) {
    		h(e(i, i+1), 0.95);
    	}
    	
    	return G;
    }
}