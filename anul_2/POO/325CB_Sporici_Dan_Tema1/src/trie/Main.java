package trie;


import java.util.Arrays;
import test.Command;
import test.TestReader;
import test.TestWriter;

public class Main 
{	
	/**
	 * interpreteaza comenzi pentru primul trie
	 * 
	 * @param c           comanda
	 */
	private static void runCommands1(Command c)
	{
		if (c.getType() == Command.ADD)
		{
			String word = c.getWord();

			for (int i = 0; i < word.length(); i++)
			{
				// doar in elementul din ultimul nod retin cuvantul
				if (i + 1 == word.length())
					trie1.add(new TrieWord1(word.charAt(i), word));
				else
					trie1.add(new TrieWord1(word.charAt(i), null));
			}
		}
		
		else
		if (c.getType() == Command.COUNT)
			testWriter.printCount(trie1.count(new TrieWord1('\0', c.getWord())));
		
		else
		if (c.getType() == Command.REMOVE)
		{
			trie1.remove(new TrieWord1('\0', c.getWord()));
		}
		
		else	
		if (c.getType() == Command.LIST)
		{
			
			TrieElement prefix = new TrieWord1('\0', c.getWord());
			TrieElement[] elements = trie1.getSortedElements(prefix);
			
			// nu exista cuvinte cu prefixul dat
			if (elements == null)
			{
				testWriter.printWord("\n");
				return;
			}
			
			for (int i = 0; i < elements.length; i++)
			{
				if (elements[i] == null)
					continue;
				
				String minWordLex = ((TrieWord1)elements[i]).getWord();
				
				for (int j = i + 1; j < elements.length; j++)
				{
					if (elements[j] == null)
						continue;
					
					String crtWord = ((TrieWord1)elements[j]).getWord();
					
					// afisez cel mai mic cuvant din punct de vedere lexicografic si sterg duplicatele
					if (Arrays.equals(elements[i].toCharArray(), elements[j].toCharArray()))
					{
						if (crtWord.compareTo(minWordLex) < 0)
							minWordLex = crtWord;
						elements[j] = null;
					}
					else
						break;  // lista e deja sortata
					
				}
				
				testWriter.printWord(minWordLex + " ");
			}
			
			testWriter.printWord("\n");
		}
	}
	
	/**
	 * interpreteaza comenzi pentru al 2-lea trie
	 * 
	 * @param c           comanda
	 */
	private static void runCommands2(Command c)
	{
		if (c.getType() == Command.ADD)
		{
			String word = c.getWord();

			int i = 0;
			
			// sar primele caractere nevalide
			while ("-_()".indexOf(word.charAt(i)) != -1)
				i++;
			
			int j = word.length()-1;
			
			// caut ultimul caracter valid
			while ("-_()".indexOf(word.charAt(j)) != -1)
				j--;
			
			for (; i <= j; i++)
			{
				// daca e ultimul caracter, memorez si cuvantul
				if (i == j)
					trie2.add(new TrieWord2(word.charAt(j), word));
				else
				{
					if ("-_()".indexOf(word.charAt(i)) == -1)  // daca e caracter valid
						trie2.add(new TrieWord2(word.charAt(i), null)); // creez un element (si implicit un nod)
				}
			}
		}
		
		else
		if (c.getType() == Command.COUNT)
			testWriter.printCount(trie2.count(new TrieWord2('\0', c.getWord())));
		
		else
		if (c.getType() == Command.REMOVE)
			trie2.remove(new TrieWord2('\0', c.getWord()));
		
		else
		if (c.getType() == Command.LIST)
		{
			
			TrieElement prefix = new TrieWord2('\0', c.getWord());
			TrieElement[] elements = trie2.getSortedElements(prefix);
			
			
			
			if (elements == null)
			{
				testWriter.printWord("\n");
				return;
			}
			
			// la fel ca la TrieWord1
			
			for (int i = 0; i < elements.length; i++)
			{
				if (elements[i] == null)
					continue;
				
				String minWordLex = ((TrieWord2)elements[i]).getWord();
				
				for (int j = i + 1; j < elements.length; j++)
				{
					if (elements[j] == null)
						continue;
					
					String crtWord = ((TrieWord2)elements[j]).getWord();
					
					// caut minimul lexicografic si scot duplicatele
					if (Arrays.equals(elements[i].toCharArray(), elements[j].toCharArray()))
					{
						if (crtWord.compareTo(minWordLex) < 0)
							minWordLex = crtWord;
						elements[j] = null;
					}
					else
						break;
				}
				
				testWriter.printWord(minWordLex + " ");
			}
			
			testWriter.printWord("\n");
		}
	}
	
	// trie pt case insensitive
	static Trie trie1 = new Trie();
	
	// trie care ignora -_()
	static Trie trie2 = new Trie();
	
	static TestWriter testWriter = new TestWriter("trie.out");
	
	public static void main(String[] args)
	{
		TestReader testReader = new TestReader("trie.in");
		
		
		// -- inceput trie1 --
		String[] words = testReader.getWords();
		
		
		// adaug cuvintele initiale
		for(String w : words)
			for (int i = 0; i < w.length(); i++)
				if (i + 1 == w.length())
					trie1.add(new TrieWord1(w.charAt(i), w));
				else
					trie1.add(new TrieWord1(w.charAt(i), null));

		
		
		// executa comenzi
		Command[] commands = testReader.getFirstCommands();
		
		for (Command c : commands)
			runCommands1(c);
		
		// -- sfarsit trie1 --
		
		
		// -- inceput trie2 --
		
		
		// adauga cuvintele initiale
		for(String w : words)
		{
			int i = 0;
			
			// sar primele caractere nevalide
			while ("-_()".indexOf(w.charAt(i)) != -1)
				i++;
			
			int j = w.length()-1;
			
			// caut ultimul caracter valid
			while ("-_()".indexOf(w.charAt(j)) != -1)
				j--;
			
			
			for (; i <= j; i++)
			{
				if (i == j)
					trie2.add(new TrieWord2(w.charAt(j), w));
				else
				{
					if ("-_()".indexOf(w.charAt(i)) == -1)  // sunt pe un caracter valid
						trie2.add(new TrieWord2(w.charAt(i), null));
				}
			}
		}
		
		
		// executa comenzi
		commands = testReader.getSecondCommands();
		
		for (Command c : commands)
			runCommands2(c);
		
		
		testWriter.close();
	}
}
