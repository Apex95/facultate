

public class KleeneStar extends PostfixOperator{

	public KleeneStar(Expression operand) {
		super(operand);
		this.priority = Priorities.KLEENE_PRIORITY;
	}
	@Override
	protected String getName() {
		return "kleene";
	}
	@Override
	protected String getSymbol() {
		return "*";
	}
}
