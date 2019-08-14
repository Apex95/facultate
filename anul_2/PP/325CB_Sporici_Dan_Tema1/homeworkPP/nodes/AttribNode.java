package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

// nod ce se ocupa de atribuiri (a = b)

public class AttribNode extends Node
{

	public AttribNode(Context c) 
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
		// pot face atribuire doar in symbol (variabila), altfel e gresita sintaxa
		String symbolName = ((SymbolNode)leftChild).getName();
		Context context = ((SymbolNode)leftChild).getContext();
		
		// adaug atribuirea in contextul curent
		context.add(symbolName, rightChild.evaluate());
		
		return 0; // nu returneaza nimic important
	}
	
}
