package lfatema1;

import java.util.HashSet;
import java.util.Iterator;

public class TerminalsSet 
{
	private HashSet<Terminal> terminalsSet;
	
	public TerminalsSet()
	{
		terminalsSet = new HashSet<Terminal>();
	}
	
	public void addTerminal(Terminal t)
	{
		terminalsSet.add(t);
	}
	
	public Iterator<Terminal> getIterator()
	{
		return terminalsSet.iterator();
	}
	
	public HashSet<Terminal> getTerminalsSet()
	{
		return (HashSet<Terminal>) terminalsSet.clone();
	}
}
