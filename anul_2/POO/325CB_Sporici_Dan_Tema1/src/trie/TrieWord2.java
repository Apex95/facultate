package trie;


/**
 * Clasa TrieWord2 - implementeaza al 2-lea tip de elemente (ce ignora '_-()')
 * 
 * Obs: toCharArray() va incerca mereu sa intoarca un cuvant dar daca nu este posibil (null)
 * va intoarce un caracter. Cuvantul returnat se va termina mereu cu caracterul '1' (ElementInfo.HAS_WORD)
 * iar char-ul cu caracterul '0' (ElementInfo.HAS_CHAR).
 * 
 * @author Dan
 */
public class TrieWord2 implements TrieElement 
{
	private String word = null;
	private char c[] = new char[2];


	/**
	 * @param c           caracterul(ascii) specific elementului
	 * @param word        cuvantul reprezentat de element (null daca nu exista)
	 */
	public TrieWord2(char c, String word)
	{
		this.c[0] = c;
		this.c[1] = ElementInfo.HAS_CHAR;
		this.word = word;
	}
	
	private String filterSymbols(String word)
	{
		// black magic
		return word.replaceAll("\\(|\\)|\\-|\\_", "");
	}
	
	@Override
	public char[] toCharArray() 
	{
		// returneaza cuvantul din element; daca nu exista, returneaza caracterul
		return (word == null) ? c : (filterSymbols(word) + ElementInfo.HAS_WORD).toCharArray();
	}
	
	/**
	 * @return           cuvantul original (cu tot cu simbolurile neacceptate) retinut in element
	 */
	public String getWord() 
	{
		return word;
	}
}
