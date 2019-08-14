package lfatema1;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Iterator;

public class Main 
{
	public static void main(String[] args)
	{
		if (args.length == 0)
		{
			System.err.println("Argument error");
			return;
		}
		
		BufferedReader inputBuffer = null;
		Flexer scanner = null;
		try 
		{
			inputBuffer = new BufferedReader(new FileReader("grammar"));
			scanner = new Flexer(inputBuffer);
			scanner.yylex();

			
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
		
		if (!scanner.isValidSyntax())
		{
			System.err.println("Syntax error");
			return;
		}
		
		// -- ajunge aici => corect sintactic
		Iterator<Rule> rulesIterator = scanner.getRulesSet().getIterator();
		
		
		// toti terminalii sa fie in setul de simboluri
		// toti terminalii din setul de simboluri sa fie in setul de terminali
		if (!scanner.getSymbolsSet().terminalsSetEquals(scanner.getTerminalsSet()))
		{
			System.err.println("Semantic error");
			return;
		}
		
		// simbolul de start in multimea neterminalilor
		if (!scanner.getSymbolsSet().containsNonTerminal(scanner.getStartSymbol()))
		{
			System.err.println("Semantic error");
			return;
		}
		
		Rule crtRule = null;
		

		
		while (rulesIterator.hasNext())
		{
			crtRule = rulesIterator.next();
			
			// partea stanga a regulii in multimea neterminalilor
			if (!scanner.getSymbolsSet().containsNonTerminal(crtRule.getLeft()))
			{
				System.err.println("Semantic error");
				return;
			}

			
			// tot ce e in dreapta regulii sa fie in multimea terminalilor sau neterminalilor
			for (Symbol s : crtRule.getRight())
				if (!scanner.getSymbolsSet().contains(s) && !s.isVoid())
				{
					System.err.println("Semantic error");
					return;
				}
		}
			


		// -- ajung aici => corect semantic
		
		
		if (args[0].equals("--is-void"))
		{
			rulesIterator = scanner.getRulesSet().getIterator();
			
			boolean updateDerivationChain = true;
			boolean isUseful = true;
			
			while (updateDerivationChain)
			{
				updateDerivationChain = false;
				
				rulesIterator = scanner.getRulesSet().getIterator();
				
				// iau o regula
				while (rulesIterator.hasNext())
				{
					isUseful = true;
					
					crtRule = rulesIterator.next();
					
					
					// daca are in dreapta direct cuvantul vid => marchez
					if ((crtRule.getNOfRNonTerminals() == 0 || crtRule.hasVoid()) && 
						!scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful())
					{
						scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful(true);
						
					    updateDerivationChain = true;
					    
					    continue;
					}
					
		
					ArrayList<Symbol> _right = crtRule.getRight();
						
					for (Symbol s : _right)
						if (s instanceof NonTerminal)
							if (!scanner.getSymbolsSet().getSymbolByChar(s.getChar()).isUseful())
								isUseful = false;
					

					if (isUseful && !scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful())
					{
						scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful(true);
						
					    updateDerivationChain = true;
					}
				}
			}
			
			if (!scanner.getSymbolsSet().getSymbolByChar(scanner.getStartSymbol().getChar()).isUseful())
				System.out.println("Yes");
			else
				System.out.println("No");
			
		}
		else
			if (args[0].equals("--has-e"))
			{
				rulesIterator = scanner.getRulesSet().getIterator();
				
				boolean updateDerivationChain = true;
				boolean canDerivateVoid = true;
				
				while (updateDerivationChain)
				{
					updateDerivationChain = false;
					
					rulesIterator = scanner.getRulesSet().getIterator();
					
					// iau o regula
					while (rulesIterator.hasNext())
					{
						canDerivateVoid = true;
						
						crtRule = rulesIterator.next();
						
						// daca are in dreapta direct cuvantul vid => marchez
						if (crtRule.hasVoid() && !scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).canDerivateVoid())
						{
							scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).canDerivateVoid(true);
							
						    updateDerivationChain = true;
						    
						    continue;
						}
						
						
						// verific daca in dreapta are numai neterminali si daca din fiecare pot deriva cuvantul vid 
						if (crtRule.getNOfRTerminals() == 0 && crtRule.getNOfRNonTerminals() > 0)
						{
							ArrayList<Symbol> _right = crtRule.getRight();
							
							for (Symbol s : _right)
								if (s instanceof NonTerminal)
									if (!scanner.getSymbolsSet().getSymbolByChar(s.getChar()).canDerivateVoid())
										canDerivateVoid = false;
						}
						else
							canDerivateVoid = false;
						

						if (canDerivateVoid && !scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).canDerivateVoid())
						{
							scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).canDerivateVoid(true);
							
						    updateDerivationChain = true;
						}
					}
				}
				
				if (!scanner.getSymbolsSet().getSymbolByChar(scanner.getStartSymbol().getChar()).canDerivateVoid())
					System.out.println("No");
				else
					System.out.println("Yes");
			}
			else
				if (args[0].equals("--useless-nonterminals"))
				{
					rulesIterator = scanner.getRulesSet().getIterator();
					
					boolean updateDerivationChain = true;
					boolean isUseful = true;
					
					while (updateDerivationChain)
					{
						updateDerivationChain = false;
						
						rulesIterator = scanner.getRulesSet().getIterator();
						
						// iau o regula
						while (rulesIterator.hasNext())
						{
							isUseful = true;
							
							crtRule = rulesIterator.next();
							
							
							// daca are in dreapta direct cuvantul vid => marchez
							if ((crtRule.getNOfRNonTerminals() == 0 || crtRule.hasVoid()) && 
								!scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful())
							{
								scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful(true);
								
							    updateDerivationChain = true;
							    
							    continue;
							}
							
				
							ArrayList<Symbol> _right = crtRule.getRight();
								
							for (Symbol s : _right)
								if (s instanceof NonTerminal)
									if (!scanner.getSymbolsSet().getSymbolByChar(s.getChar()).isUseful())
									{
										//System.out.println("not useful: " + s.getChar());
										isUseful = false;
									}
							

							if (isUseful && !scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful())
							{
								//System.out.println("useful: " + crtRule.getLeft().getChar());
								scanner.getSymbolsSet().getSymbolByChar(crtRule.getLeft().getChar()).isUseful(true);
								
							    updateDerivationChain = true;
							}
						}
					}
					
					
					Iterator<NonTerminal> nonTerminalIterator = scanner.getSymbolsSet().getNonTerminalsIterator();
					NonTerminal crtNonTerminal;
					
					while (nonTerminalIterator.hasNext())
					{
						crtNonTerminal = nonTerminalIterator.next();
						
						if (!crtNonTerminal.isUseful())
							System.out.println(crtNonTerminal.getChar());
					}
				}
				else
					System.err.println("Argument error");
	}
}
