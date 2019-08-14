

public class VoidExpression extends BasicExpression {

	public VoidExpression() {
		this.priority = Priorities.VOID_PRIORITY;
	}
	@Override
	protected String getName() {
		return "phi";
	}
	@Override
	protected String getSymbol() {
		return "O";
	}
}
