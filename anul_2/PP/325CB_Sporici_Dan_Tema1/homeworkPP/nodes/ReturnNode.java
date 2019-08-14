package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod pt instructiunea return

public class ReturnNode extends Node 
{

	public ReturnNode(Context context) 
	{
		super(context);
		this.returns = true; // rezultatul trebuie propagat spre radacina
	}

	@Override
	public void accept(Visitor v) 
	{
		v.visit(this);
	}

	@Override
	public int evaluate()
	{
		
		return leftChild.evaluate();
	}
	
}
