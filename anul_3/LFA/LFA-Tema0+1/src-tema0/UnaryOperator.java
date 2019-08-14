

public abstract class UnaryOperator extends Operator {

	protected Expression operand;
	
	public UnaryOperator(Expression operand) {
		this.operand = operand;
	}
	
	@Override
	protected String preArguments() {
		return this.preArgument(this.operand);
	}

}
