

public class Union extends BinaryOperator {

	public Union(Expression left, Expression right) {
		super(left, right);
		this.priority = Priorities.UNION_PRIORITY;
	}

	@Override
	protected String getName() {
		return "union";
	}

	@Override
	protected String getSymbol() {
		return "|";
	}

}
