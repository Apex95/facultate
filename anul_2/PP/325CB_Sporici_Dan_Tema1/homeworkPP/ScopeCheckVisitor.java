package homeworkPP;

import homeworkPP.nodes.*;

// visitor pt verificarea variabilelor, daca sunt in scope

public class ScopeCheckVisitor extends Visitor
{
	private boolean isInScope = true;
	
	public boolean check(Node root)
	{
		root.leftChild.accept(this);
		root.rightChild.accept(this);
		
		return isInScope;
	}
	
	@Override
	public void visit(AttribNode node) 
	{
		// daca am prins ceva ce nu e in scope, nu mai continui
		if (!isInScope)
			return;
		
		node.rightChild.accept(this);
		
		// daca ce e in dreapta e in scope (fie numar fie variabila), fac o evaluare a atribuirii ca sa actualizez contextul
		// si sa marchez noua variabila ca fiind in scope
		if (isInScope)
			node.evaluate();
	}

	@Override
	public void visit(CompareNode node) 
	{
		if (!isInScope)
			return;
		
		// verific in ambele parti
		node.leftChild.accept(this);
		node.rightChild.accept(this);
		
	}

	@Override
	public void visit(EqualNode node) 
	{
		if (!isInScope)
			return;
		
		node.leftChild.accept(this);
		node.rightChild.accept(this);
		
	}

	@Override
	public void visit(IfNode node) 
	{	
		if (!isInScope)
			return;
	
		// salvez contextul initial
		node.getContext().saveVariablesTable();
		
		node.conditionChild.accept(this);
		node.leftChild.accept(this);
		node.rightChild.accept(this);
		
		// restaurez contextul initial + modificari de variabile
		node.getContext().updateVariablesTable();
		
	}

	@Override
	public void visit(ProductNode node) 
	{
		if (!isInScope)
			return;
		
		node.leftChild.accept(this);
		node.rightChild.accept(this);
	}

	@Override
	public void visit(ReturnNode node) 
	{
		if (!isInScope)
			return;
		
		// return are un singur nod child, cel din stanga
		node.leftChild.accept(this);
		
	}

	@Override
	public void visit(SemicolonNode node) 
	{
		if (!isInScope)
			return;
		
		node.leftChild.accept(this);
		node.rightChild.accept(this);
		
	}

	@Override
	public void visit(SumNode node) 
	{
		if (!isInScope)
			return;
		
		node.leftChild.accept(this);
		node.rightChild.accept(this);
	}

	@Override
	public void visit(SymbolNode node) 
	{
		if (!isInScope)
			return;
		
		// daca nu e in contextul actual => nu e in scope
		try 
		{
			node.getContext().valueOf(node.getName()); // asta lanseaza exceptie daca nu e in context
			isInScope = true;
		} 
		catch (Exception e) 
		{
			isInScope = false;
			//e.printStackTrace();
		}
	}

	@Override
	public void visit(ValueNode node) 
	{
		if (!isInScope)
			return;
		
		isInScope = true; // o valoare numerica e mereu in scope
		
	}

	@Override
	public void visit(WhileNode node) 
	{
		if (!isInScope)
			return;
		
		// salvez contextul initial
		node.getContext().saveVariablesTable();
		
		node.conditionChild.accept(this);
		node.leftChild.accept(this);
		
		// restaurez contextul initial + modificari de variabile
		node.getContext().updateVariablesTable();
	}

}
