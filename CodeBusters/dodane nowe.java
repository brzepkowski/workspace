import java.util.*;
import java.io.*;
import java.math.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

/**
 * Send your busters out into the fog to trap ghosts and bring them home!
 **/
class Player {

    //public static int[] horizontal = new int[] {1600, 4800, 8000, 11200, 14400}; // Pozioma
    //public static int[] vertical = new int[] {1600, 4500, 7400}; // Pionowa
    
    public static int[] horizontal = new int[] {1600, 4800, 11200, 14400}; // Pozioma
    public static int[] vertical = new int[] {1600, 7400}; // Pionowa
    public static int[][] teamZeroStealingPositions = new int[][] {{0, 13500, 7400}, {0, 14400, 6500}};
    public static int[][] teamOneStealingPositions = new int[][] {{0, 2500, 1600}, {0, 1600, 2500}};
    public static int[][] target;
    public static boolean targetArrayFilled = false;
    public static int[][] stunCounter; /* Array that tells, how many turns a buster 
                                has to wait until he can stun enemy buster again*/
                                
    
    public static int[][] currentCapturedGhosts;
    
    // Method to generate first moves, which will be saved in "target" array
    public static int[] generateMoves(int x, int y) {
        
        Random random = new Random();
        // Top-left square
        if (x <= 8000 && y <= 4500) {
                while (x <= 8000 && y <= 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
        }
        // Top-right square
        else if (x >= 8000 && y <= 4500) {
                while (x >= 8000 && y <= 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
        }
        // Bottom-left square
        else if (x < 8000 && y > 4500) {
                while (x < 8000 && y > 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
        }
        // Bottom-right square
        else if (x > 8000 && y > 4500) {
                while (x > 8000 && y > 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
        }
        
        int[] newMoves = {x, y};
        
        return newMoves;
    }
    
    public static int[] generateMoves(int id, int x, int y) {
        
        Random random = new Random();
        // Top-left square
        if (x <= 8000 && y <= 4500) {
            // Generate new moves only when buster is close to the wall
            if (x <= 1600 || y <= 1600) {
                while (x <= 8000 && y <= 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
                // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                target[i][1] = x;
                target[i][2] = y;
            }
            // x and y stay the same (as in "target" array
            else {
            // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                x = target[i][1];
                y = target[i][2];
            }
        }
        // Top-right square
        else if (x >= 8000 && y <= 4500) {
            // Generate new moves only when buster is close to the wall
            if (x >= 14400 || y <= 1600) {
                while (x >= 8000 && y <= 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
                // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                target[i][1] = x;
                target[i][2] = y;
            }
            // x and y stay the same (as in "target" array
            else {
            // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                x = target[i][1];
                y = target[i][2];
            }
        }
        // Bottom-left square
        else if (x < 8000 && y > 4500) {
            // Generate new moves only when buster is close to the wall
            if (x <= 1600 || y >= 7400) {
                while (x < 8000 && y > 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
                // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                target[i][1] = x;
                target[i][2] = y;
            }
            // x and y stay the same (as in "target" array
            else {
            // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                x = target[i][1];
                y = target[i][2];
            }
        }
        // Bottom-right square
        else if (x > 8000 && y > 4500) {
            // Generate new moves only when buster is close to the wall
            if (x >= 14400 || y >= 7400) {
                while (x > 8000 && y > 4500) {
                    //x = horizontal[random.nextInt(5)];
                    //y = vertical[random.nextInt(3)];
                    x = horizontal[random.nextInt(4)];
                    y = vertical[random.nextInt(2)];
                }
                // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                target[i][1] = x;
                target[i][2] = y;
            }
            // x and y stay the same (as in "target" array
            else {
            // Save new x and y in "target" array
                int i = 0;
                while (target[i][0] != id) i++;
                x = target[i][1];
                y = target[i][2];
            }
        }
        
        
        int[] newMoves = {x, y};
        
        return newMoves;
    }
    
    public static void main(String args[]) {
        
        Scanner in = new Scanner(System.in);
        int bustersPerPlayer = in.nextInt(); // the amount of busters you control
        int ghostCount = in.nextInt(); // the amount of ghosts on the map
        int myTeamId = in.nextInt(); // if this is 0, your base is on the top left of the map, if it is one, on the bottom right
        int myCapturedGhosts = 0;
        
        target = new int[bustersPerPlayer][3];
        int targetPointer = 0; // Pointer in ther array "target"
        stunCounter = new int[bustersPerPlayer][2];
        int stunPointer = 0;
        currentCapturedGhosts = new int[ghostCount][2];
        for (int i = 0; i < ghostCount; i++) currentCapturedGhosts[i][0] = -1;
        int currCaptureCounter = 0;

        // Game loop
        while (true) {

            int numberOfEntities = in.nextInt(); // the number of busters and ghosts visible to you
			List myBusters = new ArrayList<int[]>();
			List enemyBusters = new ArrayList<int[]>();
			List ghosts = new ArrayList<int[]>();
			
			

            for (int i = 0; i < numberOfEntities; i++) {
				int[] entity = new int[6];
                // entityId - buster id or ghost id
                entity[0] = in.nextInt();
                // x and y - position of this buster / ghost
                entity[1] = in.nextInt();
                entity[2] = in.nextInt();
                // entityType - the team id if it is a buster, -1 if it is a ghost.
                entity[3] = in.nextInt();
                // state - For busters: 0=idle, 1=carrying a ghost.
                entity[4] = in.nextInt();
                // value - For busters: Ghost id being carried. For ghosts: number of busters attempting to trap this ghost.
                entity[5] = in.nextInt();


				// Copy data to adequate array
				if (entity[3] == myTeamId) {
					myBusters.add(entity);
					if (!targetArrayFilled) {
					    target[targetPointer][0] = entity[0];
					    int[] firstMoves = generateMoves(entity[1], entity[2]);
					    target[targetPointer][1] = firstMoves[0];
					    target[targetPointer][2] = firstMoves[1];
					    targetPointer++;
					    
					    // At the same time we can fill the stunCounter array with ids
					    stunCounter[stunPointer][0] = entity[0];
					    stunCounter[stunPointer][1] = 0;
					    stunPointer++;
					    
					}
				}
				else if (entity[3] != myTeamId && entity[3] != -1) {
					enemyBusters.add(entity);
				}
				else {
					ghosts.add(entity);
					
					    int x = 0;
					    while (entity[0] != currentCapturedGhosts[x][0] && currentCapturedGhosts[x][0] != -1) x++;
					    currentCapturedGhosts[x][0] = entity[0];
					
				}
            }
            
            // At this point target array has to be filled with ids
            targetArrayFilled = true;
            
            // Lower value in every cell of stunCounter if it is bigger than 0
            for (int i = 0; i < bustersPerPlayer; i++) {
              if (stunCounter[i][1] > 0) stunCounter[i][1] = stunCounter[i][1] - 1;  
            }

			// Write an action using System.out.println()
            // To debug: System.err.println("Debug messages...");
            for (int i = 0; i < bustersPerPlayer; i++) {

			    // No ghosts or other busters visible and buster not carrying any ghost
				if (numberOfEntities == bustersPerPlayer && ((int[]) myBusters.get(i))[4] == 0) {
				    
					int[] buster = (int[]) myBusters.get(i);
				    int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);
				    
                    System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
			    }
				else {
				    // If buster is in not carrying a ghost and isn't STUNed
					if (((int[]) myBusters.get(i))[4] != 1 && ((int[]) myBusters.get(i))[4] != 2) {
						if (!enemyBusters.isEmpty()) { 
						    int[] buster = (int[]) myBusters.get(i);
						    
						    // Check the range betwen buster and enemyBuster
						    int smallestDistance = 18000;
						    int target = 0;
						    int targetCarryingGhost = -1;
						    int distanceCarryingGhost = 18000;
						    for (int j = 0; j < enemyBusters.size(); j++) {
						        int[] enemyBuster = (int[]) enemyBusters.get(j);
								int distance = (int) Math.sqrt(Math.pow(buster[1] - enemyBuster[1],2) + Math.pow(buster[2] - enemyBuster[2],2));
						    
						        if (enemyBuster[4] == 1 && distance < distanceCarryingGhost) {
						            targetCarryingGhost = j;
						            distanceCarryingGhost = distance;
						        }
						        if (distance < smallestDistance) {
						            smallestDistance = distance;
						            target = j;
						        }
						    }
						    
						    // If enemy buster is in the range stun him
						    int j = 0;
						    while (stunCounter[j][0] != buster[0]) {
						        j++;
						    }
						    int stunValue = stunCounter[j][1];
						    // Zmieniony poniższy warunek. Może da się sprytniej
						    if (targetCarryingGhost != -1 && stunValue == 0 && distanceCarryingGhost <= 1760 && distanceCarryingGhost > 900) {
						       /* if (distanceCarryingGhost <= 1760)*/ System.out.println("STUN " + ((int[]) enemyBusters.get(target))[0]);
								//else System.out.println("MOVE " + ((int[]) enemyBusters.get(target))[1] + " " + ((int[]) enemyBusters.get(target))[2]);
						    }
						    else if (smallestDistance <= 1760 && smallestDistance > 900 && stunValue == 0) {
						        System.out.println("STUN " + ((int[]) enemyBusters.get(target))[0]);
						        stunCounter[j][1] = 20;
						    }
						    else if (smallestDistance <= 900 && stunValue <= 1) {
						    	int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);
							    
					    		System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
						    }
						    // Otherwise capture ghost or generate new move
						    else {
						    	if (!ghosts.isEmpty()) {
						    		smallestDistance = 18000;
						    		target = -1;
						    		buster = (int[]) myBusters.get(i);

						    		for(int k = 0; k < ghosts.size(); k++) {
						    			int[] ghost = (int[]) ghosts.get(k);
						    			int distance = (int) Math.sqrt(Math.pow(buster[1] - ghost[1],2) + Math.pow(buster[2] - ghost[2],2));
						    			if (distance < smallestDistance){
						    				smallestDistance = distance;
						    				target = k;
										}
						    		}
								
						    		int[] ghost = (int[]) ghosts.get(target);
										
						    		// Check if ghost is close enough to catch it
						    		if (smallestDistance <= 1760 && smallestDistance > 900) {
							            // Check if ghost is being captured by enemy ghost
							            int l = 0;
							            while (currentCapturedGhosts[l][0] != ghost[0]) l++;
							            int myBustersCapturing = currentCapturedGhosts[l][1];
							            if (myBustersCapturing < ghost[5] && myBustersCapturing > 1) System.out.println("MOVE " + ghost[1] + " " + ghost[2]);
							            else {
							                // Do poprawy system sygnalizowania, czy wciąż łapie tego ducha
							                System.out.println("BUST " + ghost[0]);
							                currentCapturedGhosts[l][1] = currentCapturedGhosts[l][1] + 1;
							            }
							        }       
						    		else if (smallestDistance <= 900) {
						    		    int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);
							    
					    		        System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
						    		}    
						    		else System.out.println("MOVE " + ghost[1] + " " + ghost[2]);
								}
						    	// If no ghost is visible and buster can't STUN enemy buster generate new move
						    	else {
						    	    // If searching for ghosts left on the map is not promissing, go to the enemy base to steal their ghosts
						            /*if (ghostCount - (2*myCapturedGhosts) + 2 <= 3) {
							            // !!!!!!!!!!!!Do rozbudowania!!!!!!!!
							            if (myTeamId == 0) {
								            System.out.println("MOVE 14400 7200"); 
							            }
							            else {
								            System.out.println("MOVE 1600 1600");
							            }
						            }
						            else {*/
						    		    int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);
				    
						    		    System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
								    //}
								}
						    }
						}
						else if (!ghosts.isEmpty()) {
							int smallestDistance = 18000;
							int target = -1;
							int[] buster = (int[]) myBusters.get(i);

							for(int j = 0; j < ghosts.size(); j++) {
								int[] ghost = (int[]) ghosts.get(j);
								int distance = (int) Math.sqrt(Math.pow(buster[1] - ghost[1],2) + Math.pow(buster[2] - ghost[2],2));
								if (distance < smallestDistance){
									smallestDistance = distance;
									target = j;
								}
							}
								
							int[] ghost = (int[]) ghosts.get(target);
										
							// Check if ghost is close enough to catch it
							if (smallestDistance <= 1760 && smallestDistance > 900) {
							    // Check if ghost is being captured by enemy ghost
							    int l = 0;
							    while (currentCapturedGhosts[l][0] != ghost[0]) {
							        System.err.println("Array: " + currentCapturedGhosts[l][0] + ", ghost: " + ghost[0]); 
							        l++;
							    }
							    int myBustersCapturing = currentCapturedGhosts[l][1];
							    if (myBustersCapturing < ghost[5] && myBustersCapturing > 1) System.out.println("MOVE " + ghost[1] + " " + ghost[2]);
							    else {
							        // Do poprawy system sygnalizowania, czy wciąż łapie tego ducha
							        System.out.println("BUST " + ghost[0]);
							        currentCapturedGhosts[l][1] = currentCapturedGhosts[l][1] + 1;
							    }
							}
							else if (smallestDistance <= 900) {
								int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);
							    
					    		System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
							}
							else System.out.println("MOVE " + ghost[1] + " " + ghost[2]);
						}
					}
					// Buster is STUNed
					else if (((int[]) myBusters.get(i))[4] == 2) {
					       int[] buster = (int[]) myBusters.get(i);
					       int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);   
					    	System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
					}
					// Buster is carrying ghost
					else {
					    int[] buster = (int[]) myBusters.get(i);
						    
						// Check the range betwen buster and enemyBuster
						int smallestDistance = 18000;
						int target = 0;
						for (int j = 0; j < enemyBusters.size(); j++) {
						    int[] enemyBuster = (int[]) enemyBusters.get(j);
						    int distance = (int) Math.sqrt(Math.pow(buster[1] - enemyBuster[1],2) + Math.pow(buster[2] - enemyBuster[2],2));
						    
						    if (distance < smallestDistance) {
						        smallestDistance = distance;
						        target = j;
						    }
						}
						    
						// If enemy buster is in the range stun him
						int j = 0;
						while (stunCounter[j][0] != buster[0]) {
						    j++;
						}
						int stunValue = stunCounter[j][1];
						if (smallestDistance <= 1760 && stunValue == 0) {
						    System.out.println("STUN " + ((int[]) enemyBusters.get(target))[0]);
						    stunCounter[j][1] = 20;
						}
						/*else if (smallestDistance < 900 && stunValue <= 1) {
							int[] newMoves = generateMoves(buster[0], buster[1], buster[2]);
						    
				    		System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
						}*/
						else {
						    if (myTeamId == 0) {
							    if (Math.sqrt(Math.pow(buster[1],2) + Math.pow(buster[2], 2)) <= 1600) {
							    	System.out.println("RELEASE");
							    	myCapturedGhosts++;
							    	System.err.println("Captured ghosts: " + myCapturedGhosts);
							    }
							    else System.out.println("MOVE 0 0");
						    }
						    else {
						    	if (Math.sqrt(Math.pow(16001 - buster[1],2) + Math.pow(9001 - buster[2], 2)) <= 1600) {
						    		System.out.println("RELEASE");
						    		myCapturedGhosts++;
						    		System.err.println("Captured ghosts: " + myCapturedGhosts);
						    	}
							    else System.out.println("MOVE 16001 9001");
						    }	
					    }
					}
				}
            }
        }
    }
}
