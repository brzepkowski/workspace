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

    public static int[] horizontal = new int[] {1600, 4800, 8000, 11200, 14400}; // Pozioma
    public static int[] vertical = new int[] {1600, 4500, 7400}; // Pionowa
    public static int[][] previousTarget;
    public static boolean previousTargetArrayFilled = false;
    
    public static int[] generateMoves(int id, int x, int y) {
        
        Random random = new Random();
        // Top-left square
        if (x <= 8000 && y <= 4500) {
            // Generate new moves only when buster is close to the wall
            if (x <= 1600 || y <= 1600) {
                while (x <= 8000 && y <= 4500) {
                    x = horizontal[random.nextInt(5)];
                    y = vertical[random.nextInt(3)];
                }
                // Set new target
                int i = 0;
                while (previousTarget[i][0] != id) i++;
                previousTarget[i][1] = x;
                previousTarget[i][2] = y;
            }
        }
        // Top-right square
        else if (x >= 8000 && y <= 4500) {
            // Generate new moves only when buster is close to the wall
            if (x >= 14400 || y <= 1600) {
                while (x >= 8000 && y <= 4500) {
                    x = horizontal[random.nextInt(5)];
                    y = vertical[random.nextInt(3)];
                }
                // Set new target
                int i = 0;
                while (previousTarget[i][0] != id) i++;
                previousTarget[i][1] = x;
                previousTarget[i][2] = y;
            }
        }
        // Bottom-left square
        else if (x < 8000 && y > 4500) {
            // Generate new moves only when buster is close to the wall
            if (x <= 1600 || y >= 7400) {
                while (x < 8000 && y > 4500) {
                    x = horizontal[random.nextInt(5)];
                    y = vertical[random.nextInt(3)];
                }
                // Set new target
                int i = 0;
                while (previousTarget[i][0] != id) i++;
                previousTarget[i][1] = x;
                previousTarget[i][2] = y;
            }
        }
        // Bottom-right square
        else if (x > 8000 && y > 4500) {
            // Generate new moves only when buster is close to the wall
            if (x >= 14400 || y >= 7400) {
                while (x > 8000 && y > 4500) {
                    x = horizontal[random.nextInt(5)];
                    y = vertical[random.nextInt(3)];
                }
                // Set new target
                int i = 0;
                while (previousTarget[i][0] != id) i++;
                previousTarget[i][1] = x;
                previousTarget[i][2] = y;
            }
        }
        else {
          int i = 0;
          while (previousTarget[i][0] != id) i++;
          
          x = previousTarget[i][1];
          y = previousTarget[i][2];
        }
        
        int[] newMoves = {x, y};
        
        System.err.println("I am going to give moves: " + newMoves[0] + ", " + newMoves[1]);
        return newMoves;

    }
    
    public static void main(String args[]) {
        
        Scanner in = new Scanner(System.in);
        int bustersPerPlayer = in.nextInt(); // the amount of busters you control
        int ghostCount = in.nextInt(); // the amount of ghosts on the map
        int myTeamId = in.nextInt(); // if this is 0, your base is on the top left of the map, if it is one, on the bottom right
        previousTarget = new int[bustersPerPlayer][3];
        int previousTargetArrayIterator = 0;

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
					// Array of previous targets has to be initiated with ids of busters
					if (previousTargetArrayFilled == false) {
					  previousTarget[previousTargetArrayIterator ][0] = entity[0];
					  previousTarget[previousTargetArrayIterator ][1] = -1;
					  previousTarget[previousTargetArrayIterator ][2] = -1;
					  previousTargetArrayIterator++;
					}
				}
				else if (entity[3] != myTeamId && entity[3] != -1) {
					enemyBusters.add(entity);
				}
				else {
					ghosts.add(entity);
				}
            }
            
            // At this point array of previous target has t obe filled with ids
            previousTargetArrayFilled = true;


			// Write an action using System.out.println()
            // To debug: System.err.println("Debug messages...");
            for (int i = 0; i < bustersPerPlayer; i++) {

			    // No ghosts or other busters visible and buster not carrying any ghost
				if (numberOfEntities == bustersPerPlayer && ((int[]) myBusters.get(i))[4] == 0) {
				    int[] buster = (int[]) myBusters.get(i);
					int id = buster[0];
					int x = buster[1];
				    int y = buster[2];
				    int[] newMoves = generateMoves(id, x, y);
				    
                    System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
			    }
				else {
					if (((int[]) myBusters.get(i))[4] == 0) {
						if (!ghosts.isEmpty()) {

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
							if (smallestDistance <= 1760) System.out.println("BUST " + ghost[0]);
							else System.out.println("MOVE " + ghost[1] + " " + ghost[2]);
						}
						// Other visible target is enemy buster
						else { 
						    // !!!!!!Check if enemy buster is in the range of stun. If so, stun him
						    
						    
	
						    int[] buster = (int[]) myBusters.get(i);
					        int id = buster[0];
					        int x = buster[1];
				            int y = buster[2];
				            int[] newMoves = generateMoves(id, x, y);
				    
                            System.out.println("MOVE " + newMoves[0] + " " + newMoves[1]);
						}
					}
					// Buster is carrying ghost
					else {
						int[] buster = (int[]) myBusters.get(i);
						if (myTeamId == 0) {
							if (Math.sqrt(Math.pow(buster[1],2) + Math.pow(buster[2], 2)) <= 1600) System.out.println("RELEASE");
							else System.out.println("MOVE 0 0");
						}
						else {
							if (Math.sqrt(Math.pow(16001 - buster[1],2) + Math.pow(9001 - buster[2], 2)) <= 1600) System.out.println("RELEASE");
							else System.out.println("MOVE 16001 9001");
						}								
					}
				}
            }
        }
    }
    
}

