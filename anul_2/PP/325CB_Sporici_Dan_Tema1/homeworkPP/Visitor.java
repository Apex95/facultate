package homeworkPP;

import homeworkPP.nodes.*;

// clasa initiala pt tot ceea ce inseamna visitor (pt scope si pt context)

public abstract class Visitor 
{
	public abstract void visit(AttribNode node);  // verif daca pt a = b, b e in scope
	public abstract void visit(CompareNode node); // verif a < b, a si b sunt in scope
	public abstract void visit(EqualNode node); // a == b, ambele sunt in scope ?
	public abstract void visit(IfNode node);  // verif daca toate var din conditie si ramuri sunt in scope
	public abstract void visit(ProductNode node); // a * b - daca sunt in scope ambele
	public abstract void visit(ReturnNode node);  // return x - daca x e declarata undeva
	public abstract void visit(SemicolonNode node); // aici verif daca exista un return in program
	public abstract void visit(SumNode node);  // a + b, ambele trebuie sa fie in scope
	public abstract void visit(SymbolNode node);
	public abstract void visit(ValueNode node);
	public abstract void visit(WhileNode node); // verif conditiile si continutul daca au toate variabilele in scope
}
