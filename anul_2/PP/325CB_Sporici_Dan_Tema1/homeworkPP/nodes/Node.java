package homeworkPP.nodes;

import java.util.regex.Pattern;

import homeworkPP.Context;
import homeworkPP.Visitor;

// clasa abstracta pt toate nodurile din arbore

public abstract class Node 
{
	public abstract void accept(Visitor v);
	public abstract int evaluate();
	protected Context context;
	
	// marchez daca evaluarea acestui nod intoarce un rezultat util
	// ce trebuie propagat spre radacina (ex: nodurile de return, suma si produs)
	public boolean returns = false;
	
	public Node leftChild = null;
	public Node rightChild = null;
	public Node conditionChild = null;
	
	public Node(Context context)
	{
		this.context = context;
	}
	
	// verificari ca sa pot determina un tip de nod, fiind dat un string
	
	private static boolean isValue(String exp)
	{
		return Pattern.matches("^\\d+$", exp);
	}
	
	private static boolean isSymbol(String exp)
	{
		return Pattern.matches("^[a-zA-Z]+$", exp);
	}
	
	private static boolean isOp(String exp)
	{
		return Pattern.matches("^(\\+|\\*|==|<)$", exp);
	}
	
	private static boolean isIf(String exp)
	{
		return Pattern.matches("^if$", exp);
	}
	
	private static boolean isAttrib(String exp)
	{
		return Pattern.matches("^=$", exp);
	}
	
	private static boolean isSemicolon(String exp)
	{
		return Pattern.matches("^;$", exp);
	}
	
	private static boolean isReturn(String exp)
	{
		return Pattern.matches("^return$", exp);
	}
	
	private static boolean isWhile(String exp)
	{
		return Pattern.matches("^while$", exp);
	}
	
	public Context getContext()
	{
		return context;
	}
	
	// factory: intorc nodul corespunzator in functie de ce string se da
	
	public static Node newNode(String content, Context c)
	{		
		// daca e valoare (nr)
		if (isValue(content))
			return new ValueNode(Integer.parseInt(content), c);
		else
			if (isOp(content))
				return OpNode.newNode(content, c); // daca e Op (semn)
			else
				if (isIf(content))
					return new IfNode(c);
				else
					if (isAttrib(content))
						return new AttribNode(c);
					else
						if (isSemicolon(content))
							return new SemicolonNode(c);
						else
							if (isReturn(content))
								return new ReturnNode(c);
							else
								if (isWhile(content))
									return new WhileNode(c);
								else
									if (isSymbol(content)) // daca e simbol (variabila)
										return new SymbolNode(content, c);
								
		
		
		// ceva neasteptat ?
		return null;
	}	
}
