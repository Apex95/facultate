package Main;

import java.util.regex.Pattern;


/**Parseaza o expresie si generaza un arbore.
 * @author Dan
 *
 */
public class TextParser 
{
	public TextParser()
	{
	}

	// imparte o expresie dupa operatorii dati si apeleaza _parse pentru cele 2 expresii obtinute
	private Nod imparteExpresie(String linie, char[] operatori, boolean areParanteze, Class<?> T)
	{
		int ratieParanteze = 0;
		
		for (int i = 0; i < linie.length(); i++)
			if (linie.charAt(i) == '(')
				ratieParanteze++;
			else if (linie.charAt(i) == ')')
					ratieParanteze--;
			else 
				for (int j = 0; j < operatori.length; j++)
					if (linie.charAt(i) == operatori[j] && ratieParanteze == 0)
						return new Operator(Character.toString(linie.charAt(i)), areParanteze, _parse(linie.substring(0, i), T), _parse(linie.substring(i+1, linie.length()), T));
	
		// nu pot sa impart asta dupa operatorii dati
		return null;
	}
	
	
	private Nod _parse(String linie, Class<?> T)
	{
		boolean areParanteze = false;
		
		// sterge parantezele redundante
		if (linie.charAt(0) == '(' && linie.charAt(linie.length()-1) == ')')
		{
			int parantezeStanga = 1;
			int parantezeDreapta = 0;
			
			int i = 1;
			
			for (i = 1; i < linie.length()-1; i++)
			{
				if (linie.charAt(i) == '(')
					parantezeStanga++;
				
				if (linie.charAt(i) == ')')
					parantezeDreapta++;
				
				// exclud ultima paranteza din dreapta, daca totusi sunt egale la un moment dat, nu se pot elimina paranteze
				if (parantezeDreapta == parantezeStanga)
					break;
			}
			
			// daca sunt egale, parantezele respecta configuratia, deci se pot elimina
			if (i == linie.length()-1)
			{
				linie = linie.substring(1, linie.length()-1);
				areParanteze = true;

			}
		}
		
		
		Nod n = null;
		
		// pentru atribuiri
		if (Pattern.matches(".+?=.+?", linie))
			n = imparteExpresie(linie, new char[] { '=' }, areParanteze, T);
		
		if (n != null)
			return n;
	
		
		// tratez conditionarea prescurtata, pentru ':'
		if (Pattern.matches(".+?\\?.+?\\:.+?", linie))
			n = imparteExpresie(linie, new char[] { ':' }, areParanteze, Ternar.class);
				
		if (n != null)
			return n;
				
		// continua tratarea pt '?'  doar daca linia provine dintr-un operator ternar
		if (Pattern.matches(".+?\\?.+?", linie) && T == Ternar.class)
			n = imparteExpresie(linie, new char[] { '?' }, areParanteze, Ternar.class);
				
		if (n != null)
			return n;
		
		
		// tratez cazul pentru + si - => termeni
		if (Pattern.matches(".+?(:?\\+|\\-).+?", linie))
			n = imparteExpresie(linie, new char[] { '+', '-' }, areParanteze, Termen.class);
		
		if (n != null)
			return n;
		
		// cazul pentru * si / => tot expresie...dar cu factori
		if (Pattern.matches(".+?(:?\\*|\\/).+?", linie))
			n = imparteExpresie(linie, new char[] { '*', '/' }, areParanteze, Factor.class);
		
		if (n != null)
			return n;
		
		
		// trateaza expresii de forma a>b, a<b
		if (Pattern.matches("[^<>]+[<>]{1}[^<>]+", linie))
			n = imparteExpresie(linie, new char[] { '<', '>' }, areParanteze, Ternar.class);
		
		if (n != null)
			return n;
		
		
		// aleg tipul nodului in functie de operatorul precedent
		if (T == Termen.class)
			return new Termen(linie);
		
		if (T == Factor.class)
			return new Factor(linie);
			
		if (T == Ternar.class)
			return new Ternar(linie);	
		
		return null; // sper ca nu ajunge aici
	}
	
	private String formateazaInput(String linie)
	{
		// sterg spatiile din expresie
		return linie.replace(" ", "");
	}
	
	/** Parseaza o linie citita si genereaza arborele
	 * @param linie - linia ce trebuie parsata
	 * @return - arborele corespunzator expresiei
	 */
	public Nod parse(String linie)
	{
		linie = formateazaInput(linie);
		return _parse(linie, Termen.class);
	}

}
