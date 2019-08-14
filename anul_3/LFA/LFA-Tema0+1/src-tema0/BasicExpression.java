

public abstract class BasicExpression extends Expression {

	@Override
	public String preOrder() {
		return this.getName();
	}

	@Override
	public String inOrder() {
		return this.getSymbol();
	}

}
