package Main;

import java.util.ArrayList;
import java.util.List;

/**Analizeaza semantic o expresie folosind arborele generat de TextParser
 * 
 * @author Dan
 *
 */
public class SemanticAnalyzer 
{
	// template-uri pentru mesajele de eroare
	private final String msgMembrulNuEVar = "Membrul stang nu e variabila la linia {linie} coloana {coloana}\n";
	private final String msgVarNedeclarata = "{var} nedeclarata la linia {linie} coloana {coloana}\n";
	private final String msgOK = "Ok!\n";
	
	public SemanticAnalyzer()
	{
	}
	
	List<String>variabileInitializate = new ArrayList<String>();

	// verifica daca variabilele sunt initializate - totodata genereaza o lista cu cele neinitializate
	private boolean verificaInitializareVariabile(Nod n, List<String> varNeinitializate)
	{	
		if (n.eFrunza())
			if (n.eVariabila())
			{
				boolean raspuns = variabileInitializate.contains(n.getInfo());
				if (raspuns == false)
					varNeinitializate.add(n.getInfo());
				
				return raspuns;
			}
			else
				return true;
			
		
		return verificaInitializareVariabile(n.getNodStanga(), varNeinitializate) 
				&& verificaInitializareVariabile(n.getNodDreapta(), varNeinitializate);
	}
	
	// creeaza mesajul folosind un template de mai sus
	private String formateazaMesaj(Expresie expr, String mesaj, String var)
	{
		// folosita pentru calculul coloanei la care se afla eroarea
		String _exp = expr.getInfo().substring(expr.getInfo().indexOf('='));
		
		if (var != null)
		{
			mesaj = mesaj.replace("{var}", var);
			mesaj = mesaj.replace("{coloana}", Integer.toString(1+_exp.indexOf(var) + expr.getInfo().length() - _exp.length()));
		}
		else
			mesaj = mesaj.replace("{coloana}", "1");
		
		
		mesaj = mesaj.replace("{linie}", Integer.toString(expr.getNrLinie()));
			
		
		return mesaj;
	}
	
	/**Verifica semantic o expresie data
	 * @param expr - expresia ce trebuie verificata (ce contine legatura catre tree)
	 * @return - rezultatul verificarii (eroare / ok)
	 */
	public String verificaExpresie(Expresie expr)
	{
		Nod n = expr.getTree();
		
		if (n == null || n.getNodDreapta() == null || n.getNodStanga() == null)
			return null;
	
		// primul operator dupa care se face impartirea e egal, deci n-are rost sa caut recursiv
		if (n.getInfo().equals("="))
		{	
			// expresie = ceva / const = ceva
			if (!n.getNodStanga().eVariabila())
				return formateazaMesaj(expr, msgMembrulNuEVar, null);
			
			// var = ceva; verific daca ceva nu contine variabile neinitializate
			List<String> varNeinitializate = new ArrayList<String>();
			
			if (verificaInitializareVariabile(n.getNodDreapta(), varNeinitializate))
			{
				variabileInitializate.add(n.getNodStanga().getInfo());
				return msgOK;
			}
			else
				return formateazaMesaj(expr, msgVarNedeclarata, varNeinitializate.get(0));
			
		}
		else
			return msgOK;  // din enunt, doar 2 tipuri de erori pot exista si astea sunt tratate mai sus
		
			
	}
}
