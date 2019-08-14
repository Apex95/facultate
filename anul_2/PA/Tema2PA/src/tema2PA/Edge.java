package tema2PA;

public class Edge implements Comparable<Edge>
{
	int node1 = -1;
	int node2 = -1;
	long cost = -1;
	
	public Edge(int node1, int node2, long cost)
	{
		this.node1 = node1;
		this.node2 = node2;
		this.cost = cost;
	}

	@Override
	public int compareTo(Edge o) 
	{
		return Long.compare(this.cost, o.cost);
	}
	
	
}
