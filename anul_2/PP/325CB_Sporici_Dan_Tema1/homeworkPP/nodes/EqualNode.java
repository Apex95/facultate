package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod ce trateaza egalitatea a == b

public class EqualNode extends OpNode 
{

	public EqualNode(Context c) 
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
		return leftChild.evaluate() == rightChild.evaluate() ? 1 : 0;
	}
	

}
