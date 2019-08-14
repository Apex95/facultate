package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod pt multiplicarea a * b

public class ProductNode extends OpNode 
{
	public ProductNode(Context c) 
	{
		super(c);
	}

	@Override
	public void accept(Visitor v) 
	{
		v.visit(this);
	}

	@Override
	public int evaluate()
	{
		return leftChild.evaluate() * rightChild.evaluate();
	}
	
}
