package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod pt variabile
public class SymbolNode extends Node
{
	private String name;
	
	public SymbolNode(String name, Context context)
	{
		super(context);
		this.name = name;
	}
	
	public void accept(Visitor v)
	{
		v.visit(this);
	}

	@Override
	public int evaluate() 
	{
		try 
		{
			return context.valueOf(name);
		} 
		catch (Exception e) 
		{
			//e.printStackTrace();
		}
		return 0;
	}
	
	public String getName()
	{
		return name;
	}
}
