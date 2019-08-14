package homeworkPP.nodes;

import homeworkPP.Context;

// clasa abstracta pentru toate nodurile ce implica operatii

public abstract class OpNode extends Node
{	
	public OpNode(Context c) {
		super(c);
		// TODO Auto-generated constructor stub
	}

	// la fel ca la Node - tot factory, cu care returnez nodul corespunzator stringului
	// obs: SumNode si ProductNode intorc la evaluare valori ce vor fi necesare pt a continua evaluarea
	// deci trebuie propagate spre radacina
	public static OpNode newNode(String op, Context c)
	{
		if (op.contains("+"))
			return new SumNode(c) {{ this.returns = true; }};
		else
			if (op.contains("*"))
				return new ProductNode(c) {{ this.returns = true; }};
			else
				if (op.contains("=="))
					return new EqualNode(c);
				else
					return new CompareNode(c);
	}
	


}
