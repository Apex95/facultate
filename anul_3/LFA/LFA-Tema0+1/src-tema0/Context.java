

import java.util.LinkedList;

public class Context {

	public LinkedList<Expression> operands;
	public LinkedList<Expression.Priorities> operators;
    public int state;
	private static LinkedList<Context> contexts = new LinkedList<Context>();

	public Context() {
		this.operands = new LinkedList<Expression>();
		this.operators = new LinkedList<Expression.Priorities>();
	}

	public Expression getResult() {
		this.colllapse();
		return this.operands.pop();
	}

	public Context enter(int state) {
        this.state = state;
		Context.contexts.push(this);
		return new Context();
	}

	public Context exit() {
		Expression result = this.getResult();
		Context.contexts.peek().operands.push(result);
		return Context.contexts.pop();
	}

	public void force() {
		Expression.Priorities operator = this.operators.pop();
		Expression operand1 = this.operands.pop();
		Expression operand2;
		switch(operator) {
		case KLEENE_PRIORITY:
			this.operands.push(new KleeneStar(operand1));
			return;
		case UNION_PRIORITY:
			operand2 = this.operands.pop();
			this.operands.push(new Union(operand2, operand1));
			return;
		case PLUS_PRIORITY:
			this.operands.push(new Plus(operand1));
			return;
		case OPTIONAL_PRIORITY:
			this.operands.push(new Optional(operand1));
			return;
		case CONCAT_PRIORITY:
			operand2 = this.operands.pop();
			this.operands.push(new Concatenation(operand2, operand1));
			return;
		default:
			return;
		}
	}

	public void colllapse() {
		while(!this.operators.isEmpty()) {
			this.force();
		}
	}
	@Override
	public String toString() {
		return " " +this.operands.toString() + " " + this.operators.toString();
	}
}
