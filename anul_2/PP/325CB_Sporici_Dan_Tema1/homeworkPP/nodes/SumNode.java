package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;


// nod pt suma 
public class SumNode extends OpNode 
{	
	public SumNode(Context context) 
	{
		super(context);	
	}

	@Override
	public void accept(Visitor v) 
	{
		v.visit(this);
	}

	@Override
	public int evaluate()
	{
		return leftChild.evaluate() + rightChild.evaluate();
	}
}
