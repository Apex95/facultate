

public class Optional extends PostfixOperator {

	public Optional(Expression operand) {
		super(operand);
		this.priority = Priorities.OPTIONAL_PRIORITY;
	}
	
	@Override
	protected String getName() {
		return "optional";
	}
	
	@Override
	protected String getSymbol() {
		return "?";
	}

}
