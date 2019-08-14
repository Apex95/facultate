import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.Scanner;

public class Main 
{
	public static void main(String[] args)
	{
		Scanner scanner = null;
		try 
		{
			scanner = new Scanner(new File("test.in"));
		} 
		catch (FileNotFoundException e) 
		{
			e.printStackTrace();
			return;
		}
		
		
		int v = scanner.nextInt();
		int e = scanner.nextInt();
		
		int[][] E = new int[v+1][v+1];
		
		int n1 = 0, n2 = 0;
		
		String SAT = "";
		
		for (int i = 0; i < e; i++)
		{
			n1 = scanner.nextInt();
			n2 = scanner.nextInt();
			
			E[n1][n2] = 1;
		}
		
		
		// 1 - O(v^2)
		// construiesc v clauze cu toate locatiile posibile pentru fiecare nod (pt ca lantul trece prin toate nodurile)
		// astfel pentru x(i,j) daca nodul j se afla pe pozitia i, clauza este valida (true)
		SAT +="(";
		for (int i = 1; i <= v; i++)
		{
			for (int j = 1; j <= v; j++)
			{
				SAT += "x" + Integer.toString((j - 1) * v + i);
				//SAT += "x" + Integer.toString(j) + Integer.toString(i);
			
				if (j != v)
					SAT += "V";
			}
			
			if (i != v)
				SAT += ")^(";
		}
		SAT += ")^";
		
		//SAT += "\n";
		
		// 2 - O(v^3)
		// construiesc clauze cu cate 2 literali de forma (~x(j,i) V ~x(k,i))
		// care vor fi false doar daca ambii literali sunt true (deci nodul i ocupa 2 pozitii (j si k)) 
		for (int i = 1; i <= v; i++)
			for (int j = 1; j <= v; j++)
				for (int k = j+1; k <= v; k++)
//					if (j != k)
					{
						//SAT += "(~x" + Integer.toString(j) + Integer.toString(i) + "V~x" + Integer.toString(k) + Integer.toString(i) + ")";
						SAT += "(~x" + Integer.toString((j - 1) * v + i) + "V~x" + Integer.toString((k - 1) * v + i) + ")";
						
						if (i != v || j != v || k != v)
							SAT += "^";
					}

		//SAT += "\n";
		
		SAT += "("; 
		// 3 - O(v^2) 
		// exact ca la 1) dar inversez indicii ca sa fortez conditia
		// ca fiecare pozitie din lant sa contina un nod
		for (int i = 1; i <= v; i++)
		{
			for (int j = 1; j <= v; j++)
			{
				//SAT += "x" + Integer.toString(i) + Integer.toString(j);
				SAT += "x" + Integer.toString((i - 1) * v + j);
				
				if (j != v)
					SAT += "V";
			}
		
			if (i != v)
				SAT += ")^(";
		}
		
		SAT += ")^";
		
		//SAT += "\n";
		// 4 - O(v^3)
		// verific ca 2 noduri sa nu fie simultan pe aceeasi pozitie in 
		// lantul hamiltonian - pe pozitia i sa nu existe si un nod j si unul k
		// daca exista, o clauza va fii falsa => toata expresia va fii falsa
		for (int i = 1; i <= v; i++)
			for (int j = 1; j <= v; j++)
				for (int k = j+1; k <= v; k++)
//					if (j != k)
					{
						//SAT += "(~x" + Integer.toString(i) + Integer.toString(j) + "V~x" + Integer.toString(i) + Integer.toString(k) + ")";
						SAT += "(~x" + Integer.toString((i - 1) * v + j) + "V~x" + Integer.toString((i - 1) * v + k) + ")";
						
						if (i != v || j != v || k != v)
							SAT += "^";
					}
		
		//SAT += "\n";
		// 5 - O(v^3)
		// conditia de verificare ca intre 2 noduri aflate in lantul hamiltonian
		// sa existe si o muchie in graful original: (~x(i,j) V ~x(i+1, k)) 
		// clauza anterioara e falsa daca exista 2 noduri (j si k) aflate pe pozitii succesive (i si i+1)
		// intre care nu exista o muchie (cost 0 in matricea de adiacenta)
		for (int i = 1; i <= v-1; i++)
			for (int j = 1; j <= v; j++)
				for (int k = 1; k <= v; k++)
					if (E[k][j] == 0 && E[j][k] == 0)//&& k != j)
					{
						//SAT += "(~x" + Integer.toString(i) + Integer.toString(j) + "V~x" + Integer.toString(i+1) + Integer.toString(k) + ")";
						
						SAT += "(~x" + Integer.toString((i - 1) * v + j) + "V~x" + Integer.toString(i * v + k) + ")";
						
						
						if (i != v-1 || j != v || k != v)
							SAT += "^";
					}
		
		if (SAT.substring(SAT.length()-1).contains("^"))
			SAT = SAT.substring(0, SAT.length()-1);
		
		
		try 
		{
			Files.write(Paths.get("test.out"), SAT.getBytes(), StandardOpenOption.CREATE);
		} 
		catch (IOException e1) 
		{
			e1.printStackTrace();
		}
		finally
		{
			if (scanner != null)
				scanner.close();
		}
	}
}
