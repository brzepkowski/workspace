import org.jgrapht.UndirectedGraph;
import org.jgrapht.graph.AbstractBaseGraph;
import org.jgrapht.graph.ClassBasedEdgeFactory;
import org.jgrapht.graph.DefaultEdge;

class MyGraph extends AbstractBaseGraph<Integer, DefaultEdge> implements UndirectedGraph<Integer, DefaultEdge> {
    MyGraph() {
        super(new ClassBasedEdgeFactory<>(DefaultEdge.class), true, true);
    }
}
