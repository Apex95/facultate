package temapa;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;

import javax.sound.midi.SysexMessage;

public class p1 
{	
	public static void solve() throws IOException
	{
		BufferedInputStream inputStream = new BufferedInputStream(new FileInputStream("joc.in"));
		BufferedOutputStream outputStream = new BufferedOutputStream(new FileOutputStream("joc.out"));
		
		int n = 0;
		int c;
		
		
		// primul numar din fisier e n (int)
		while ((c = inputStream.read()) != -1 && c >= '0' && c <= '9')
		{
			n = n * 10 + (c - '0');
		}
		
		//inputStream.read(); // newline
		
		int moves;
		int len;
		
		int[] word = new int[351];
		int[] letterOccurences = null;
		
		boolean canBePalindrome = true;
	
		for (int i = 0; i < n; i++)
		{
			moves = 0;
			len = 0;
			canBePalindrome = true;
			
			// 26 de caractere in alfabetul folosit
			letterOccurences = new int[26];
			
			
			// citesc cuvintele...asa cum sunt date
			while ((c = inputStream.read()) != -1)
			{
				if (c == '\r')
					continue;
				
				if (c == '\n')
					break;
				
				word[len] = c;
				letterOccurences[c - 97]++;
				len++;
			}
			
			// --- debug
//			for (int k = 0; k < len; k++)
//				System.err.print((char)word[k]);
//			System.err.println();
			
			
			// nu pot avea palindrom cu 2 aparitii impare
			// pt litere diferite
			
			int odds = 0;
			
			for (int k = 0; k < 26; k++)
			{
				if (letterOccurences[k] % 2 == 1)
					odds++;
				
				if (odds > 1)
				{
					canBePalindrome = false;
					break;
				}
			}
			
			if (!canBePalindrome)
			{
				outputStream.write("-1\n".getBytes());
				continue;
			}
			
			// caz in care am o singura litera de un tip
			boolean oddCase = false;
			int oddIndex = -1;
			
			
			for (int k = 0; k < len/2; k++)
				if (word[k] != word[len-k-1])
				{
					oddCase = false;
					
					for (int k2 = k+1; k2 < len-k-1; k2++)
					{
						if (word[len-k-1] == word[k2])
						{
							oddCase = true;
							moves += k2 - k;
							for (int k3 = k2; k3 > k; k3--)
							{
								int aux = word[k3];
								word[k3] = word[k3-1];
								word[k3-1] = aux;
								
							}
							break;
						}
					}
					
					
					// cazul "iaian" cand compar i cu n si nu gasesc caractere pt interschimare (nu mai exista un n)
					// dar daca mut acum acel n pe mijloc, risc ca la o viitoare interschimbare, n sa se deplaseze
					// si sa fac mutarile astea in plus
					
					if (!oddCase)
					{
						len--;
						k--;
						
						oddIndex = len-k-1;
					}
					
				}
			
			// daca exista o litera care ar trebui sa vina pe centru
			if (oddIndex != -1)
			{
				len++; // refac len (scurtat cand am ignorat litera) 
				
				moves += oddIndex - len/2;
				for (int k3 = oddIndex; k3 > len/2; k3--)
				{
					int aux = word[k3];
					word[k3] = word[k3-1];
					word[k3-1] = aux;
					
				}
			}
			outputStream.write((Integer.toString(moves) + " ").getBytes());
				
				
		}
		
		inputStream.close();
		outputStream.close();
	}
	
	
}
