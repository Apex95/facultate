

public class NullExpression extends BasicExpression{
	
	public NullExpression() {
		this.priority = Priorities.NULL_PRIORITY;
	}
	@Override
	protected String getSymbol() {
		return "e";
	}
	
	@Override
	protected String getName() {
		return "epsilon";
	}
}