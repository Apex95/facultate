package Main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Main 
{
	
	public static void main(String[] args)
	{
		if (args.length == 0)
		{
			System.out.println("Si fisieru' ?");
			return;
		}
		
		String fisierIntrare = args[0];
		
		FileReader fReader = null;
		BufferedReader bReader = null;
		
		// clasele folosite penru parsare, evaluare, etc.
		final TextParser parser = new TextParser();
		final SemanticAnalyzer analyzer = new SemanticAnalyzer();
		final ExpressionEvaluator evaluator = new ExpressionEvaluator();
		
		// streamurile de iesire
		FileWriter fptWriter = null;
		FileWriter fsaWriter = null;
		FileWriter feeWriter = null;
		
		try 
		{
			fReader = new FileReader(fisierIntrare);
			bReader = new BufferedReader(fReader);
			
			String linie;
			int nrLinie = 0;
			
			
			fptWriter = new FileWriter(fisierIntrare + "_pt");
			fsaWriter = new FileWriter(fisierIntrare + "_sa");
			feeWriter = new FileWriter(fisierIntrare + "_ee");
			
			// citire linie cu linie
			while ((linie = bReader.readLine()) != null)
			{
				nrLinie++;
				
				Expresie treeSintaxa = new Expresie(linie, nrLinie, parser, analyzer, evaluator);
				
				// cele 3 operatii
				treeSintaxa.afiseazaTree(fptWriter);
				treeSintaxa.valideazaExpresie(fsaWriter);
				treeSintaxa.evalueazaExpresie(feeWriter);
			}
			
			fReader.close();
			bReader.close();
			
		} 
		catch (FileNotFoundException e)
		{
			// fisierul nu exista
			return;
		}
		catch (IOException e) 
		{
			// fisierul exista dar nu-l pot accesa
			return;
		}
		finally
		{
			try 
			{
				// inchide toate streamurile
				if (fReader != null)
					fReader.close();
				
				if (bReader != null)
					bReader.close();
				
				if (fptWriter != null)
					fptWriter.close();
				
				if (fsaWriter != null)
					fsaWriter.close();
				
				if (feeWriter != null)
					feeWriter.close();
				
			}
			catch (IOException e) 
			{
				// alte exceptii la inchiderea streamurilor ?
			}
		}
	}
}
