package homeworkPP;

import homeworkPP.nodes.*;


//verifica daca exista return la finalul programului

public class ReturnCheckVisitor extends Visitor 
{
	private boolean hasReturn = false;
	
	public boolean check(Node root)
	{
		// caut return doar in dreapta - merg spre ultima instructiune
		root.rightChild.accept(this);
		
		return hasReturn;
	}
	
	@Override
	public void visit(AttribNode node) 
	{
	}

	@Override
	public void visit(CompareNode node) 
	{
	}

	@Override
	public void visit(EqualNode node) 
	{
	}

	@Override
	public void visit(IfNode node) 
	{
	}

	@Override
	public void visit(ProductNode node) 
	{
	}

	@Override
	public void visit(ReturnNode node) 
	{
		hasReturn = true; // daca nu ajunge aici, return nu e la final sau nu e deloc => nevalid
	}

	@Override
	public void visit(SemicolonNode node) 
	{
		node.rightChild.accept(this);
		
	}

	@Override
	public void visit(SumNode node) 
	{
	}

	@Override
	public void visit(SymbolNode node) 
	{
	}

	@Override
	public void visit(ValueNode node) 
	{
	}

	@Override
	public void visit(WhileNode node) 
	{
	}
	
}
