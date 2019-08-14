package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod ce trateaza comparatiile de forma a < b

public class CompareNode extends OpNode
{

	public CompareNode(Context c) {
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
		return leftChild.evaluate() < rightChild.evaluate() ? 1 : 0;
	}

}
