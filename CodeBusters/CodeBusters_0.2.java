import java.util.*;
import java.io.*;
import java.math.*;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Send your busters out into the fog to trap ghosts and bring them home!
 **/
class Player {

    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int bustersPerPlayer = in.nextInt(); // the amount of busters you control
        int ghostCount = in.nextInt(); // the amount of ghosts on the map
        int myTeamId = in.nextInt(); // if this is 0, your base is on the top left of the map, if it is one, on the bottom right
				int[] bustersTargets = new int[bustersPerPlayer];
        Arrays.fill(bustersTargets, -1);

        // game loop
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
								}
								else if (entity[3] != myTeamId && entity[3] != -1) {
									enemyBusters.add(entity);
								}
								else {
									ghosts.add(entity);
								}
            }

						// Write an action using System.out.println()
            // To debug: System.err.println("Debug messages...");
            for (int i = 0; i < bustersPerPlayer; i++) {

							// No ghosts or other busters visible
							if (numberOfEntities == bustersPerPlayer) {
								System.out.println("MOVE " + (2*((int[]) myBusters.get(i))[1]) + " " + (2*((int[]) myBusters.get(i))[2]));

							}
							else {
								if (bustersTargets[i] == -1) {
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
										bustersTargets[i] = ghost[0];
										
										// Check if ghost is close enough to catch it
										if (smallestDistance <= 1760) System.out.println("BUST " + ghost[0]);
										else System.out.println("MOVE " + ghost[1] + " " + ghost[2]);
									}
									// Other visible target is enemy buster
									else { 
										System.out.println("MOVE " + (2*((int[])myBusters.get(i))[1]) + " " + (2*((int[]) myBusters.get(i))[2]));
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

