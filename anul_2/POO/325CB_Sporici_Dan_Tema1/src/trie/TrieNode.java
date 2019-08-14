package trie;

import java.util.ArrayList;
import java.util.List;

/**
 * Clasa pt nodurile ce formeaza Trie-ul
 * 
 * @author Dan
 */
public class TrieNode 
{
	private List<TrieElement> elements = new ArrayList<TrieElement>();
	private List<TrieNode> nextNodes = new ArrayList<TrieNode>();
	
	private char nodeChar; // caracterul din nod
	

	/**
	 * @param element           elementul ce corespunde nodului 
	 */
	public TrieNode(TrieElement element)
	{
		if (element == null)
			return;
		
		elements.add(element);
		nodeChar = element.toCharArray()[element.toCharArray().length - 2];
	}
	

	/**
	 * @return           lista de descendenti (TrieNode)
	 */
	public List<TrieNode> getNextNodes() 
	{
		return nextNodes;
	}

	/**
	 * @return           primul element din lista (daca nu exista, intoarce null)
	 */
	public TrieElement getElement() 
	{
		return elements.size() > 0 ? elements.get(0) : null; // lucrez doar cu primul element (cuvant), celelalte difera doar prin case
	}
	
	public void removeElement()
	{
		if (elements.size() > 0)
			elements.remove(0);
	}
	
	/**
	 * @return           intreaga lista de elemente din nod
	 */
	public List<TrieElement> getAllElements()
	{
		return new ArrayList<TrieElement>(elements);
	}
	
	/**
	 * Adauga un element nou in lista si inlatura primul element (ce continea doar un caracter)
	 * 
	 * @param element           elementul ce trebuie adaugat in lista
	 */
	public void addElement(TrieElement element) 
	{
		// elimina elementul adaugat initial (ce continea doar un caracter) pt a face loc altora ce pot contine cuvinte
		if (elements.size() == 1 && elements.get(0).toCharArray()[elements.get(0).toCharArray().length - 1] == ElementInfo.HAS_CHAR)
			removeElement();
		
		elements.add(element);
	}
	
	/**
	 * @return           caracterul corespunzator nodului (preluat din primul element)
	 */
	public char getNodeChar() 
	{
		return nodeChar;
	}
}
