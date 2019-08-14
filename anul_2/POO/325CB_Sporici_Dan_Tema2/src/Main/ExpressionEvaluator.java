package Main;

import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;

/**Evaluator pentru expresii (folosind arborele fiecarei expresii)
 * @author Dan
 *
 */
public class ExpressionEvaluator 
{
	public ExpressionEvaluator()
	{
	}
	
	private HashMap<String, String> valoriVariabile = new HashMap<String, String>(); 
	private int _indexRezultatTernar = 1; // indica rezultatul (din stanga sau dreapta) al expresiei ternare
	
	private String executaOperatie(Nod n, Expresie expr)
	{
		// inlocuiesc valoarea din dreapta, daca e variabila declarata in HashMap
		if (n.getNodDreapta().eVariabila())
			if (valoriVariabile.containsKey(n.getNodDreapta().getInfo()))
				n.getNodDreapta().setInfo(valoriVariabile.get(n.getNodDreapta().getInfo()));
			else
				return "error";
		
		// daca e o atribuire, exista sansa sa fie o re-atribuire a unei variabile deja existente
		// atribuire poate sa fie doar daca operatorul este si nod radacina pentru arborele de sintaxa
		if (n.getInfo().equals("=") && n.getNodStanga().eVariabila() && expr.getTree() == n)
		{
			valoriVariabile.put(n.getNodStanga().getInfo(), n.getNodDreapta().getInfo());
			return n.getNodStanga().getInfo() + n.getInfo() + n.getNodDreapta().getInfo();
		}
		
		// daca in stanga e variabila, nu e atribuire si nu e decalarata in tabel => eroare 
		if (n.getNodStanga().eVariabila())
			if (valoriVariabile.containsKey(n.getNodStanga().getInfo()))
				n.getNodStanga().setInfo(valoriVariabile.get(n.getNodStanga().getInfo()));
			else
				return "error";
		
		
		
		if (n.getInfo().equals("?"))
			// aduc primul rezultat in nodul ce continea operatorul '?'
			return n.getNodDreapta().getInfo();
		
		// aleg rezultatul in functie de indexul ternar
		if (n.getInfo().equals(":"))
			return _indexRezultatTernar == 1 ? n.getNodStanga().getInfo() : n.getNodDreapta().getInfo();

			
		int ts = 0;
		int td = 0;
		
		try
		{
			ts = Integer.parseInt(n.getNodStanga().getInfo());
			td = Integer.parseInt(n.getNodDreapta().getInfo());
		}
		catch (NumberFormatException e)
		{
			// daca ajung aici inseamna ca expresia obtinuta nu este una ce se poate interpreta
			return "error";
		}
		if (n.getInfo().equals("+"))
			return Integer.toString((ts + td));

		if (n.getInfo().equals("-"))
			return Integer.toString((ts - td));
		
		if (n.getInfo().equals("*"))
			return Integer.toString((ts * td));
		
		if (n.getInfo().equals("/"))
			return Integer.toString((ts / td));
		
		
		if (n.getInfo().equals(">"))
		{
			_indexRezultatTernar = ts > td ? 1 : 2;
			return n.getInfo(); 
		}
		
		if (n.getInfo().equals("<"))
		{
			_indexRezultatTernar = ts < td ? 1 : 2;
			return n.getInfo(); 
		}
		
		// e comparatie, nu atribuire
		if (n.getInfo().equals("=") && expr.getTree() != n)
		{
			_indexRezultatTernar = ts == td ? 1 : 2;
			return n.getInfo(); 
		}
		
		
		return "error"; // operatie necunoscuta ?
		
	}
	
	/**
	 * Evalueaza o expresie data si afiseaza rezultatul in stream.
	 * @param expr - expresia ce trebuie evaluata
	 * @param feeWriter - streamul de scriere
	 * @throws IOException
	 */
	public void evalueaza(Expresie expr, FileWriter feeWriter) throws IOException
	{
		Nod n = expr.getTree();
		feeWriter.write(_evalueaza(n, expr) + "\n");
	}
	
	private String _evalueaza(Nod n, Expresie expr)
	{
		if (n.getNodStanga() == null || n.getNodDreapta() == null)
			return null;
		
		_evalueaza(n.getNodStanga(), expr);
		_evalueaza(n.getNodDreapta(), expr);
		
		
		n.setInfo(executaOperatie(n, expr));
		
		return n.getInfo();
	}
	
}
