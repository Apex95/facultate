package lfatema1;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;

public class SymbolsSet 
{
	private HashMap<Character, Symbol> symbolsSet;
	private HashSet<Terminal> terminalsSet;
	private HashSet<NonTerminal> nonTerminalsSet;
	
	public SymbolsSet()
	{
		symbolsSet = new HashMap<Character, Symbol>();
		terminalsSet = new HashSet<Terminal>();
		nonTerminalsSet = new HashSet<NonTerminal>();
	}
	
	public void addTerminal(Terminal t)
	{
		terminalsSet.add(t);
		symbolsSet.put(t.getChar(), t);
	}
	
	public void addNonTerminal(NonTerminal nt)
	{
		nonTerminalsSet.add(nt);
		symbolsSet.put(nt.getChar(), nt);
	}
	

	
	public Iterator<NonTerminal> getNonTerminalsIterator()
	{
		return nonTerminalsSet.iterator();
	}
	
	public Iterator<Terminal> getTerminalsIterator()
	{
		return terminalsSet.iterator();
	}
	
	public int getSize()
	{
		return terminalsSet.size() + nonTerminalsSet.size();
	}
	
	public boolean terminalsSetEquals(TerminalsSet terminalsSet)
	{
		return this.terminalsSet.equals(terminalsSet.getTerminalsSet());
	}
	
	public void removeSymbol(Symbol s)
	{
		if (nonTerminalsSet.contains(s))
			nonTerminalsSet.remove(s);
		
		if (terminalsSet.contains(s))
			terminalsSet.remove(s);
		
		if (symbolsSet.containsKey(s.getChar()))
			symbolsSet.remove(s.getChar(), s);
	}
	
	public Symbol getSymbolByChar(Character c)
	{
		return symbolsSet.get(c);
	}
	
//	public Symbol findSymbol(Symbol s)
//	{
//		
//		if (s instanceof Terminal)
//		{
//			Iterator<Terminal> it = terminalsSet.iterator();
//			Terminal crtTerminal;
//			
//			while (it.hasNext())
//			{
//				crtTerminal = it.next();
//				
//				if (crtTerminal.equals(s))
//					return crtTerminal;
//				
//			}
//		}
//		
//		
//		if (s instanceof NonTerminal)
//		{
//			Iterator<NonTerminal> it = nonTerminalsSet.iterator();
//			NonTerminal crtNonTerminal;
//			
//			while (it.hasNext())
//			{
//				crtNonTerminal = it.next();
//				
//				if (crtNonTerminal.equals(s))
//					return crtNonTerminal;
//				
//			}
//		}
//		
//		return null;
//		
//	}
	
	public boolean containsNonTerminal(NonTerminal nt)
	{		
		return nonTerminalsSet.contains(nt);
	}
	
	public boolean containsTerminal(Terminal t)
	{
		return terminalsSet.contains(t);
	}
	
	public boolean contains(Symbol s)
	{
		return symbolsSet.containsKey(s.getChar());
		
	}
	
}
