package lfatema1;

import java.util.ArrayList;

public class Rule 
{
	private NonTerminal left;
	private ArrayList<Symbol> right = null;
	private int nOfRNonTerminals = 0;
	private int nOfRTerminals = 0;
	private boolean hasVoid = false;
	
	public Rule()
	{
		this.right = new ArrayList<Symbol>();
	}
	
	public void setLeft(NonTerminal left)
	{
		this.left = left;
	}
	
	public boolean hasVoid()
	{
		return hasVoid;
	}
	
	
	public int getNOfRNonTerminals()
	{
		return nOfRNonTerminals;
	}
	
	public int getNOfRTerminals()
	{
		return nOfRTerminals;
	}
	
	public void addRight(Symbol s)
	{
		if (s.getChar() == 'e')
			hasVoid = true;
		else
		{
			if (Character.isLowerCase(s.getChar()))
				nOfRTerminals++;
		
			if (Character.isUpperCase(s.getChar()))
				nOfRNonTerminals++;
		}
		
		right.add(s);
	}
	
	public ArrayList<Symbol> getRight()
	{
		return right;
	}
	
	public NonTerminal getLeft()
	{
		return left;
	}
	

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((left == null) ? 0 : left.hashCode());
		result = prime * result + ((right == null) ? 0 : right.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Rule other = (Rule) obj;
		if (left == null) {
			if (other.left != null)
				return false;
		} else if (!left.equals(other.left))
			return false;
		if (right == null) {
			if (other.right != null)
				return false;
		} else if (!right.equals(other.right))
			return false;
		return true;
	}
	
	
}
