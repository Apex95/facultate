package homeworkPP.nodes;

import homeworkPP.Context;
import homeworkPP.Visitor;

public class IfNode extends Node 
{
	public IfNode(Context c) 
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
		// salvez contextul initial ca sa pot verifica daca blocul IF imi adauga variabile noi
		// pe care oricum va trebui sa le scot din context la finalul blocului
		context.saveVariablesTable();
		
		int result = conditionChild.evaluate();
		
		if (result == 1)
			leftChild.evaluate();
		else
			if (rightChild != null)
				rightChild.evaluate(); // evaluez ramura else, daca exista
		
		context.updateVariablesTable();
		// actualizez contextul anterior luand in calcul doar modificari ale variabilelor deja existente
		
		return 0;
	}
	
}
