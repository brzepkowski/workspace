import org.jgrapht.*;
import org.jgrapht.graph.*;

import java.util.Arrays;
import java.util.Random;

public final class Main {

    private static final int n = 25;

    public static void main(String[] args) {
        UndirectedGraph<Integer, DefaultEdge> graph = createGraph(n);

        System.out.println(graph.toString());

        Random randomGenerator = new Random();
        boolean[] dep = new boolean[n];
        for (int i = 0; i < n; i++) dep[i] = randomGenerator.nextBoolean();
        System.out.println(Arrays.toString(dep));

        for (int t = 0; t < 20; t++) {
            for (int i = 0; i < n; i++) {
                new MISThread(graph, dep, i).run();
            }
        }

        System.out.println(Arrays.toString(dep));

        int counter = 0;
        for (boolean aDep : dep) if (!aDep) counter++;

        System.out.println(counter);
    }

    private static UndirectedGraph<Integer, DefaultEdge> createGraph(int vertexNumber) {
        UndirectedGraph<Integer, DefaultEdge> g = new MyGraph();

        for (int i = 0; i < vertexNumber; i++) g.addVertex(i);

        for (int i = 0; i <= 20; i+= 5) {
            for (int j = 0; j < 4; j++) {
                g.addEdge(j + i, j + i + 1);
            }
        }

        for (int i = 0; i <= 19; i+=5) {
            for (int j = 0; j <= 4; j++) {
                g.addEdge(i + j, j + 5);
            }
        }

        for (int i = 0; i <= 4; i++) {
            g.addEdge(i, i + 20);
        }

        for (int i = 0; i <= 20; i+= 5) {
            g.addEdge(i, i + 4);
        }

        return g;
    }
}

