

public class Concatenation extends BinaryOperator{
	
	public Concatenation(Expression left, Expression right) {
		super(left, right);
		this.priority = Priorities.CONCAT_PRIORITY;
	}
	
	@Override
	protected String getName() {
		return "concat";
	}

	@Override
	protected String getSymbol() {
		return "";
	}
}
