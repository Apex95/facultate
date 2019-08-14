package trie;

/** 
 * Clasa TrieWord1 ce implementeaza primul tip de elemente (ignora case)
 * 
 * Obs: toCharArray() va incerca mereu sa intoarca un cuvant dar daca nu este posibil (null)
 * va intoarce un caracter. Cuvantul returnat se va termina mereu cu caracterul '1' (ElementInfo.HAS_WORD)
 * iar char-ul cu caracterul '0' (ElementInfo.HAS_CHAR).
 *
 *@author Dan
 */
public class TrieWord1 implements TrieElement 
{
	private String word = null;
	private char c[] = new char[2];

	
	/**
	 * @param c           caracterul(ascii) specific elementului
	 * @param word        cuvantul reprezentat de element (null daca nu exista)
	 */
	public TrieWord1(char c, String word)
	{
		this.c[0] = c;
		this.c[1] = ElementInfo.HAS_CHAR;
		this.word = word;
	}
	
	@Override
	public char[] toCharArray() 
	{	
		c[0] = Character.toLowerCase(c[0]);
		
		// returneaza cuvantul din element; daca nu exista, returneaza caracterul
		return (word == null) ? c : (word + ElementInfo.HAS_WORD).toLowerCase().toCharArray();
	}

	/**
	 * @return           cuvantul original (case sensitive) retinut in element
	 */
	public String getWord() 
	{
		return word;
	}

}
