

public abstract class PostfixOperator extends UnaryOperator {

	public PostfixOperator(Expression operand) {
		super(operand);
	}
	@Override
	public String inOrder() {
		return this.inArgument(this.operand) + this.getSymbol();
	}

}
