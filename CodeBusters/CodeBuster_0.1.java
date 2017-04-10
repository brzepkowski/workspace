import java.util.*;
import java.io.*;
import java.math.*;

/**
 * Send your busters out into the fog to trap ghosts and bring them home!
 **/
class Player {

    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int bustersPerPlayer = in.nextInt(); // the amount of busters you control
        int ghostCount = in.nextInt(); // the amount of ghosts on the map
        int myTeamId = in.nextInt(); // if this is 0, your base is on the top left of the map, if it is one, on the bottom right

        // game loop
        while (true) {

            int numberOfEntities = in.nextInt(); // the number of busters and ghosts visible to you
            int[][] entities = new int[numberOfEntities][6];
						int[][] myBusters = new int[bustersPerPlayer][6];
						int[][] enemyBusters = new int[bustersPerPlayer][6];
						// !!!! Do poprawy
						int[][] ghosts = new int[ghostCount][6];

						// amount of: j - myBusters / k - enemyBusters / l - ghosts
						int j = 0;
						int k = 0;
						int l = 0;

            for (int i = 0; i < numberOfEntities; i++) {
                // entityId - buster id or ghost id
                entities[i][0] = in.nextInt();
                // x and y - position of this buster / ghost
                entities[i][1] = in.nextInt();
                entities[i][2] = in.nextInt();
                // entityType - the team id if it is a buster, -1 if it is a ghost.
                entities[i][3] = in.nextInt();
                // state - For busters: 0=idle, 1=carrying a ghost.
                entities[i][4] = in.nextInt();
                // value - For busters: Ghost id being carried. For ghosts: number of busters attempting to trap this ghost.
                entities[i][5] = in.nextInt();


								// Copy data to adequate array
								if (entities[i][3] == myTeamId) {
									for (int x = 0; x < 6; x++) {
										myBusters[j][x] = entities[i][x];
									}
									j++;
								}
								else if (entities[i][3] != myTeamId && entities[i][3] != -1) {
									for (int x = 0; x < 6; x++) {
										enemyBusters[k][x] = entities[i][x];
									}
									k++;
								}
								else {
									for (int x = 0; x < 6; x++) {
										ghosts[l][x] = entities[i][x];
									}
									l++;
								}

            }

						// Write an action using System.out.println()
            // To debug: System.err.println("Debug messages...");
            for (int i = 0; i < bustersPerPlayer; i++) {

								// If no ghost is visible                
                if (numberOfEntities == bustersPerPlayer) {

                    System.out.println("MOVE " + (myBusters[i][1] + 300) + " " + (myBusters[i][2] + 300));
                }
                else {

								// If this buster doesn't carry any ghost
									if (myBusters[i][5] == 0) {

									int closestGhost = 0;
									int closestDistance = 19000;
									int distance = 0;

									for (int z = 0; z < (numberOfEntities - (2*bustersPerPlayer)); z++) {
										distance = (int) Math.sqrt(Math.pow(ghosts[z][1] - myBusters[i][1], 2) + Math.pow(ghosts[z][2] - myBusters[i][2], 2));
										if (distance < closestDistance) {
											closestGhost = z;
											closestDistance = distance;
										}
									}

									if (distance <= 1760 && distance > 900) {
										System.out.println("BUST " + ghosts[closestGhost][0]);
									}
									// Buster has to stay in the same place
									else if (distance < 900) {
										System.out.println("MOVE " + myBusters[i][1] + " " + myBusters[i][2]);
									}
								}
								// Go to the base to release the ghost
								else {
									// TOP LEFT CORNER - TEAM 0
									if (myTeamId == 0) {

										int distanceFromTheBase = (int) Math.sqrt(Math.pow(myBusters[i][1], 2) + Math.pow(myBusters[i][2], 2));
										if (distanceFromTheBase <= 1600) {
											System.out.println("RELEASE");
										}
										else {
											System.out.println("MOVE 0 0");
										}
									}
									// BOTTOM RIGHT CORNER - TEAM 1
									else {

										int distanceFromTheBase = (int) Math.sqrt(Math.pow(16001 - myBusters[i][1], 2) + Math.pow(9001 - myBusters[i][2], 2));
										if (distanceFromTheBase <= 1600) {
											System.out.println("RELEASE");
										}
										else {
											System.out.println("MOVE 16001 9001");
										}
									}
								}
                }
            }
        }
    }
}

