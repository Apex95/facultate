package trie;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;


/**
 * Clasa Trie ce implementeaza arborele (generic), bazandu-se pe interfata data (AbstractTrie)
 * @author Dan
 */
public class Trie implements AbstractTrie
{	
	private static class TrieComparator implements Comparator<TrieElement>
	{
		@Override
		
		public int compare(TrieElement arg0, TrieElement arg1) 
		{
			char[] word0 = arg0.toCharArray();
			char[] word1 = arg1.toCharArray();
			
			for (int i = 0; i < Math.min(word0.length-1, word1.length-1); i++)
			{
				if (word0[i] < word1[i])
					return -1;
				
				if (word0[i] > word1[i])
					return 1;
			}
			
			if (word0.length < word1.length)
				return -1;
			
			if (word0.length > word1.length)
				return 1;
			
			return 0;
		}
	}
		
	private TrieNode rootNode;
	private TrieNode lastNode;
	
	
	/**
	 * Creeaza un nou Trie (generic)
	 */
	public Trie()
	{
		rootNode = new TrieNode(null);
		lastNode = rootNode;
	}
	
	
	@Override
	public void add(TrieElement element) 
	{	
		char[] word = element.toCharArray();
		
		boolean charExists = false;
		
		// caut cuvantul ce se cere adaugat in arbore
		for (TrieNode t : lastNode.getNextNodes())
		{	
			// daca char-ul corespunzator nodului ce se cere adaugat este acelasi
			// cu charul unui descendent al lui 'lastNode'
			if (word[word.length - 2] == t.getNodeChar())
			{	
				lastNode = t;   // continui pe ramura asta
				charExists = true;
				break; 
			}
		}
			
		// charExists == false => exista un caracter in elementul ce trebuie adaugat, care nu exista si in trie
		if (!charExists)
		{
			TrieNode newNode = new TrieNode(element);
			lastNode.getNextNodes().add(newNode);
			
			// verific daca elementul contine un cuvant sau doar un char
			if (word[word.length - 1] == ElementInfo.HAS_WORD) 
				lastNode = rootNode;  // restez lastNode inapoi la nodul radacina, pentru a adauga alte cuvinte
			else
				lastNode = newNode; // contine doar un char, deci urmatoarele char-uri vin in continuare pe ramura asta
			
			return;
		}
		
		// daca elementul contine un cuvant si n-a fost adaugat mai devreme => exista deja in trie
		if (word[word.length - 1] == ElementInfo.HAS_WORD) 
		{
			lastNode.addElement(element);
			lastNode = rootNode;
		}
	}

	@Override
	public int count(TrieElement element) 
	{
		lastNode = rootNode;
		
		char[] word = element.toCharArray();
		
		boolean charExists = false;
		for (int i = 0; i < word.length - 1; i++)
		{
			charExists = false;
			for (TrieNode t : lastNode.getNextNodes())
			{
				if (t.getNodeChar() == word[i])
				{
					lastNode = t;
					charExists = true;
					break; 
				}
			}
			
			// daca un caracter nu este gasit, nu exista nici cuvantul
			if (!charExists)
			{
				lastNode = rootNode;
				return 0;
			}
		}


		int _size = lastNode.getAllElements().size();
		
		// tratez cazul in care singurul element aflat in nod este un char (il scad - nu se considera la count)
		if (_size == 1 && lastNode.getElement().toCharArray()[lastNode.getElement().toCharArray().length - 1] == ElementInfo.HAS_CHAR)
			_size--;
		lastNode = rootNode;
		
		return _size;
	}

	@Override
	public void remove(TrieElement element) 
	{
		lastNode = rootNode;
		
		char[] word = element.toCharArray();
		
		// caut nodul in trie...
		boolean charExists = false;
		for (int i = 0; i < word.length-1 ; i++)
		{
			charExists = false;
			
			for (TrieNode t : lastNode.getNextNodes())
			{
				if (t.getNodeChar() == word[i])
				{
					lastNode = t;
					charExists = true;
				}
			}
			
			// nu gaseste cuvantul
			if (!charExists)
			{
				lastNode = rootNode;
				return;
			}
		}
	
		// sterg un TrieElement din nodul gasit
		lastNode.removeElement();
		
		lastNode = rootNode;
	}
	
	private void findRemainingWordParts(TrieNode lastNode, List<TrieElement> words)
	{
		if (lastNode == rootNode)
		{
			for (TrieNode t : lastNode.getNextNodes())
				findRemainingWordParts(t, words);
			
			return;
		}
	
		// daca gasesc un nod care are un element ce contine un cuvant, retin toate elementele din acel nod
		if (lastNode.getElement() != null && lastNode.getElement().toCharArray()[lastNode.getElement().toCharArray().length - 1] == ElementInfo.HAS_WORD) 
			words.addAll(lastNode.getAllElements()); 
		
		// short note: deoarece lucrez cu TrieElement, nu (cred ca) am cum sa verific daca un element e mai mic
		// lexicografic decat un altul fara sa umblu la interfete sau sa fac casturi la TrieWord1 / TrieWord2 
		
		
		// we're outta nodes
		if (lastNode.getNextNodes().size() == 0)
			return;
		
		// caut eventuale cuvinte in toate ramurile ramase
		for (TrieNode t : lastNode.getNextNodes())
			findRemainingWordParts(t, words);
	}
	
	@Override
	public TrieElement[] getSortedElements(TrieElement prefix) 
	{
		lastNode = rootNode;
		
		char[] prefixWord = prefix.toCharArray();
		
		List<TrieElement> elements = new ArrayList<TrieElement>();
		
		boolean charExists = false;
		
		for (int i = 0; i < prefixWord.length - 1; i++)
		{
			charExists = false;
			for (TrieNode t : lastNode.getNextNodes())
			{
				if (t.getNodeChar() == prefixWord[i])
				{
					lastNode = t;
					charExists = true;
				}
			}
			
			
			// n-am gasit nici macar prefixul in trie ?
			if (!charExists)
			{
				lastNode = rootNode;
				return null;
			}
			
		}
		
		// ajunge aici => exista o ramura ce contine prefixul, le parcurg pe toate ce pornesc din 'lastNode'
		findRemainingWordParts(lastNode, elements);
		
		lastNode = rootNode;
		
		// sorting stuff
		Collections.sort(elements, new TrieComparator());
		
		
		return elements.toArray(new TrieElement[elements.size()]);
	}

}
