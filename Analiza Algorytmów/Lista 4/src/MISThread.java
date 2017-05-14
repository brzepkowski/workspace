import org.jgrapht.UndirectedGraph;
import org.jgrapht.graph.DefaultEdge;

import org.jgrapht.traverse.ClosestFirstIterator;

public class MISThread implements Runnable {
    private UndirectedGraph<Integer, DefaultEdge> graph;
    private boolean[] dep;
    private int vertexId;

    MISThread(UndirectedGraph<Integer, DefaultEdge> graph, boolean[] dep, int vertexId) {
        this.graph = graph;
        this.dep = dep;
        this.vertexId = vertexId;
    }

    @Override
    public synchronized void run() {
        ClosestFirstIterator iterator = new ClosestFirstIterator(graph, vertexId, 1);
        boolean u = false;
        boolean v = false;
        int vertexDegree = graph.degreeOf(vertexId);

        int counter = 0;
        while (iterator.hasNext()) {
            int vertex = (int) iterator.next();
            if (vertex != vertexId && !dep[vertex]) u = true;
            if (vertex != vertexId && dep[vertex]) counter++;
        }

        if (counter == vertexDegree) v = true;

        if (!dep[vertexId] && u) dep[vertexId] = true;

        if (dep[vertexId] && v) dep[vertexId] = false;
    }
}

