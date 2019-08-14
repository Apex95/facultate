package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod pentru valori numerice
public class ValueNode extends Node
{
	private int value = 0;
	
	public ValueNode(int value, Context context)
	{
		super(context);
		
		this.value = value;
	}
	
	public void accept(Visitor v)
	{
		v.visit(this);
	}

	@Override
	public int evaluate() 
	{
		return value;
	}

}
