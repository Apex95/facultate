

public class Plus extends PostfixOperator {

	public Plus(Expression operand) {
		super(operand);
		this.priority = Priorities.PLUS_PRIORITY;
	}
	
	@Override
	protected String getName() {
		return "plus";
	}
	
	@Override
	protected String getSymbol() {
		return "+";
	}

}
