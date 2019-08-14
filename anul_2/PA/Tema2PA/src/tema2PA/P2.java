package tema2PA;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Stack;

public class P2
{
	private static int readInt(BufferedInputStream inputStream) throws IOException
	{
		int c = 0;
		int n = 0;
		
		while ((c = inputStream.read()) != -1 && c >= '0' && c <= '9')
			n = n * 10 + (c - '0');
		
		return n;
	}
	
	
	private static void dfs(int node, int[] nodeApparitions, int[] nodeIndegree)
	{	
		if (!adjacents.containsKey(node))
			return;
		
		if (adjacents.get(node).size() == 0)
			return;
		
		int nextNode = adjacents.get(node).pop();
		
		adjacents.get(nextNode).removeElement(node);
				
		
		// daca un nod are o singura aparitie si input degree = 0, trebuie sa trag muchie spre el
		// (e doar 1 mod de a ajunge acolo)
		
		// node <- nextNode
		if ((nodeApparitions[node] == 1) && nodeIndegree[node] == 0)
			nodeIndegree[node]++;
		else
			// node -> nextNode
			if ((nodeApparitions[nextNode] == 1) && nodeIndegree[nextNode] == 0)
				nodeIndegree[nextNode]++;
			else
					// daca un nod are input degree 0 iar altul nu
					// duc muchie spre cel cu input degree 0
				
					// node -> nextNode
					if (nodeIndegree[node] != 0 && nodeIndegree[nextNode] == 0)
						nodeIndegree[nextNode]++;
					else
						
						// node <- nextNode
						if (nodeIndegree[nextNode] != 0 && nodeIndegree[node] == 0)
							nodeIndegree[node]++;
						else
							
							// duc de la nodul cu input degree mai mare la cel cu
							// degree mai mic
							if (nodeIndegree[node] < nodeIndegree[nextNode])
								nodeIndegree[node]++; 
							else
								nodeIndegree[nextNode]++;
								// ^ in cazul in care cele 2 noduri au acelasi input degree
								// nu prea mai am cum sa aleg in ce parte sa orientez muchia... => 1 nod in plus in unele cazuri 
		
		// am rezolvat cu muchia asta, scad aparitiile la ambele noduri
		nodeApparitions[node]--;
		nodeApparitions[nextNode]--;
		
		// continui de unde am ramas
		dfs(nextNode, nodeApparitions, nodeIndegree);
		
	}
	
	// reprezentare cu liste de adiacenta <nod, lista_vecini>
	static HashMap<Integer, Stack<Integer>> adjacents = new HashMap<Integer, Stack<Integer>>();
	
	public static void main(String[] args) throws IOException
	{
		BufferedInputStream inputStream = new BufferedInputStream(new FileInputStream("portal.in"));
		BufferedOutputStream outputStream = new BufferedOutputStream(new FileOutputStream("portal.out"));
		
		int n = readInt(inputStream);
		int m = readInt(inputStream);
		
		
		int[] nodeApparitions = new int[n+1];
		int[] nodeIndegree = new int[n+1];
		
		int x = 0;
		int y = 0;
		
		for (int i = 0; i < m; i++)
		{
			x = readInt(inputStream);
			y = readInt(inputStream);
			
			
			if (!adjacents.containsKey(x))
				adjacents.put(x, new Stack<Integer>());
			
			if (!adjacents.containsKey(y))
				adjacents.put(y, new Stack<Integer>());
			
			adjacents.get(x).push(y);
			adjacents.get(y).push(x);
			
			nodeApparitions[x]++;
			nodeApparitions[y]++;
		}
		
		
		int offset = 1;
		
		
		// e de preferat sa incep dintr-un nod cu o singura aparitie 
		for (offset = 1; offset < n + 1; offset++)
			if (nodeApparitions[offset] == 1)
				break;
		
		offset--;
		
		// plecand de la offset (primul nod cu o aparitie)
		// incerc sa construiesc graful
		for (int i = offset; i < n+offset; )
		{	
			if (!adjacents.containsKey(i % (n) + 1))
			{
				i++;
				continue;
			}
			
			dfs(i % (n) + 1, nodeApparitions, nodeIndegree);
			if (adjacents.get(i % (n) + 1).size() == 0)
				i++;
		}
		
		// continui cu nodurile sarite initial 
		for (int i = 1; i < offset; )
		{
			if (!adjacents.containsKey(i))
			{
				i++;
				continue;
			}
			
			dfs(i, nodeApparitions, nodeIndegree);	
			
			if (adjacents.get(i).size() == 0)
				i++;
		}
		
		// numar nodurile care au input degree 0
		int isolatedNodes = 0;
		for (int i = 1; i < n+1; i++)
		{
			if (nodeIndegree[i] == 0)
				isolatedNodes++;
		}

		
		outputStream.write((Integer.toString(isolatedNodes) + "\n").getBytes());
		outputStream.close();
	}
}
