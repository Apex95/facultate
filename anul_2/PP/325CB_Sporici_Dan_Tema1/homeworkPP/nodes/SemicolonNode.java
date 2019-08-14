package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod pentru secventiere (;)
public class SemicolonNode extends Node 
{

	public SemicolonNode(Context context)
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
		int result = leftChild.evaluate();
		
		// daca fiul din stanga e return sau alt nod de secventiere => trebuie sa propage rezultatul inapoi
		if (leftChild instanceof ReturnNode || leftChild instanceof SemicolonNode && leftChild.returns)
		{
			this.returns = true;
			return result;
		}

		result = rightChild.evaluate();
		
		// la fel si pentru nodul din dreapta
		if (rightChild instanceof ReturnNode || rightChild instanceof SemicolonNode && rightChild.returns)
		{
			this.returns = true;
			return result;
		}
		
		return 0;
	}
}
