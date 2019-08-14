package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod ce trateaza while-uri
public class WhileNode extends Node
{
	public WhileNode(Context context) 
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
		// la fel ca la IF, ca sa nu mentin in contextul general variabile declarate in blocul WHILE
		context.saveVariablesTable();
		
		while (conditionChild.evaluate() == 1)
			leftChild.evaluate();
		
		context.updateVariablesTable();
		return 0;
	}

}
