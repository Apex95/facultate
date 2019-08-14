package lfatema1;

import java.util.HashSet;
import java.util.Iterator;

public class RulesSet 
{
	HashSet<Rule> rulesSet;
	
	public RulesSet()
	{
		rulesSet = new HashSet<Rule>();
	}
	
	public void addRule(Rule r)
	{
		rulesSet.add(r);
	}
	
	public Iterator<Rule> getIterator()
	{
		return rulesSet.iterator();
	}
}
