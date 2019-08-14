package tema2PA;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class P1 
{
	// pt citirea int-urilor din fisier
	private static int readInt(BufferedInputStream inputStream) throws IOException
	{
		int c = 0;
		int n = 0;
		
		while ((c = inputStream.read()) != -1 && c >= '0' && c <= '9')
			n = n * 10 + (c - '0');
		
		return n;
	}
	
	
	public static void main(String[] args) throws IOException
	{

		BufferedInputStream inputStream = new BufferedInputStream(new FileInputStream("kim.in"));
		BufferedOutputStream outputStream = new BufferedOutputStream(new FileOutputStream("kim.out"));
		
		
		int n = readInt(inputStream);
		int m = readInt(inputStream);
		int q = readInt(inputStream);
		
		Edge[] edges = new Edge[m];
		
		
		int node1, node2; long cost;
		Edge _e = null;
		
		int offset = 0;
		
		for (int i = 0; i < m; i++)
		{
			node1 = readInt(inputStream);
			node2 = readInt(inputStream);
			cost = readInt(inputStream);
			
			_e = new Edge(node1, node2, cost);
			edges[i+offset] = _e;
		}
		
		
		Edge[] sortedEdges = new Edge[m];
		System.arraycopy(edges, 0, sortedEdges, 0, m);
		Arrays.sort(sortedEdges);
		
		int[] parentsTemplate = new int[n+1];
		
		int[] parents = new int[n+1];

		
		// initial fiecare nod e propriul parinte
		for (int j = 0; j <= n; j++)
		{
			parentsTemplate[j] = j;
		}
		
		// aici retin costul calculat in functie de o muchie impusa
		long[] costs = new long[m+1];
		
		long totalCost = 0;
		Edge e = null;
		int k = 0;
		int crtEdges = 0;
		int requiredEdgeIndex = 0;
		
		for (int i = 0; i <= q; i++)
		{
			requiredEdgeIndex = 0;
			
			if (i != 0)
			{
				requiredEdgeIndex = readInt(inputStream);
				if (costs[requiredEdgeIndex] != 0) // mi se cere iar arbore cu aceeasi muchie
				{
					// il am deja calculat
					outputStream.write((Long.toString(costs[requiredEdgeIndex]) + "\n").getBytes());
					continue;
				}
				
			}
			
			System.arraycopy(parentsTemplate, 0, parents, 0, n+1);

			
			totalCost = 0;
			e = null;
			
			k = 0;
			crtEdges = 0;
			
			if (i != 0)
			{
				// se cere o anumita muchie in arbore
	
				e = edges[requiredEdgeIndex-1]; 
				parents[e.node1] = e.node2;
				
				crtEdges++;
				
				totalCost += e.cost;

			}
			
			// imi trebuie n-1 muchii (pt arbore)
			while (crtEdges < n-1 && k < m)
			{
				e = sortedEdges[k];
				
				
				node1 = e.node1;
				node2 = e.node2;

				//caut seturile din care fac parte e.node1 si e.node2
				while ((node1 != parents[node1]) || (node2 != parents[node2]))
				{
					// comprim putin path-ul, ca sa ajung mai repede la root data viitoare
					parents[node1] = parents[parents[node1]];
					parents[node2] = parents[parents[node2]];
					
					node1 = parents[node1]; 
					node2 = parents[node2];
				}
				
				// actualizez parintii celor 2 noduri ca fiind direct radacinile seturilor
				parents[e.node1] = node1;
				parents[e.node2] = node2;
				
				// daca nu sunt in acelasi set (altfel s-ar forma ciclu)
				if (node1 != node2)
				{
					// fac merge intre cei 2 arbori
					crtEdges++;
					
					parents[node1] = node2;
					
					totalCost += e.cost;

				}
				
				// iau muchia urmatoare
				k++;
			}
			
			// memorez costul pt muchia asta - in caz ca se cere iar aceeasi muchie
			costs[requiredEdgeIndex] = totalCost;

			outputStream.write((Long.toString(totalCost) + "\n").getBytes());
		}
		
		
		outputStream.close();
		
		
	}
}
