

public abstract class BinaryOperator extends Operator {

	protected Expression left, right;
	
	public BinaryOperator(Expression left, Expression right) {
		this.left = left;
		this.right = right;
	}

	@Override
	protected String preArguments() {
		String result = "";
		if(this.left.getName().equals(this.getName())) {
			result += ((BinaryOperator) this.left).preArguments();
		} else {
			result += this.preArgument(this.left);
		}
		result += this.preArgument(this.right);
		return result;
	}

	@Override
	public String inOrder() {
		return this.inArgument(this.left) + this.getSymbol() +
				this.inArgument(this.right);
	}

}
