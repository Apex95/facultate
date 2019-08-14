package Main;

import java.io.FileWriter;
import java.io.IOException;

/**
 * Retine expresia data (originala) si executa operatii asupra ei.
 * @author Dan
 */
public class Expresie extends Nod 
{
	private TextParser parser;
	private SemanticAnalyzer analyzer;
	private ExpressionEvaluator evaluator;
	
	private Nod tree = null;
	private int nrLinie;
	
	
	/**
	 * @param linie		- linia citita din fisier
	 * @param nrLinie	- numarul liniei curente
	 * @param parser	- parserul folosit pentru generarea acelui tree
	 * @param analyzer	- analizatorul semantic
	 * @param evaluator	- evaluatorul de expresii
	 */
	public Expresie(String linie, int nrLinie, TextParser parser, SemanticAnalyzer analyzer, ExpressionEvaluator evaluator) 
	{
		this.info = linie;
		this.nrLinie = nrLinie;
		
		this.parser = parser;
		this.analyzer = analyzer;
		this.evaluator = evaluator;
		
		Nod n = this.parser.parse(linie);
		
		// Expresie va contine atat expresia parsata cat si legatura catre tree-ul corespunzator
		this.nodStanga = n;
		this.nodDreapta = n;
		tree = n;
	}
	
	// afisare pe nivele (primul exercitiu)
	private boolean afiseazaNivel(Nod n, int nivelCurent, int nivel, FileWriter fptWriter) throws IOException
	{
		// daca ajung aici, s-a afisat tot pe ramura respectiva => true
		if (n == null || nivelCurent > nivel)
			return true;
		
		if (n instanceof Operator && nivelCurent < nivel)
			if (((Operator)n).areParanteze)
				fptWriter.write("(");

		
		boolean rStanga = afiseazaNivel(n.getNodStanga(), nivelCurent+1, nivel, fptWriter);
		
		if (nivelCurent == nivel || n.eFrunza())
		{	
			if (n instanceof Termen)
				fptWriter.write("T");

			if (n instanceof Factor)
				fptWriter.write("F");

			if (n instanceof Operator)
				fptWriter.write("E");

			if (n instanceof Ternar)
				fptWriter.write("N");

		}
		else if (nivelCurent < nivel)
				fptWriter.write(n.info);

		
		boolean rDreapta = afiseazaNivel(n.getNodDreapta(), nivelCurent+1, nivel, fptWriter);
		
		if (n instanceof Operator && nivelCurent < nivel)
			if (((Operator)n).areParanteze)
				fptWriter.write(")");

		// cat timp functia asta returneaza true, inseamna ca mai sunt nivele de afisat (nu s-a ajuns numai pe frunze)
		if (n.eFrunza())
			return false;
		else
			return rStanga || rDreapta;
	}
	
	/**
	 * Scrie fiecare nivel din tree-ul obtinut in stream-ul dat.
	 * @param fptWriter	- streamul in care sa afiseze tree-ul
	 * @throws IOException
	 */
	public void afiseazaTree(FileWriter fptWriter) throws IOException
	{
		int nivel = 0;
		while (afiseazaNivel(this.tree, 0, nivel, fptWriter))
		{
			nivel++;
			fptWriter.write("\n");
		}
		
		fptWriter.write("\n");
	}
	
	
	/**
	 * Valideaza (semantic) expresia curenta
	 * @param fsaWriter	- streamul in care e afisat rezultatul analizei semantice
	 * @throws IOException
	 */
	public void valideazaExpresie(FileWriter fsaWriter) throws IOException
	{
		fsaWriter.write(analyzer.verificaExpresie(this));
	}
	
	
	/**
	 * Evalueaza rezultatul expresiei curente
	 * @param feeWriter - streamul in care e afisat rezultatul evaluarii
	 * @throws IOException
	 */
	public void evalueazaExpresie(FileWriter feeWriter) throws IOException
	{
		evaluator.evalueaza(this, feeWriter);
	}
	
	/**
	 * @return	arborele de sintaxa atasat expresiei curente
	 */
	public Nod getTree() 
	{
		return tree;
	}

	
	/**
	 * @return nr liniei curente
	 */
	public int getNrLinie() 
	{
		return nrLinie;
	}

	
}
