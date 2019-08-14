

public class SymbolExpression extends BasicExpression {
	
	private char value;
	
	public SymbolExpression(char value) {
		this.value = value;
		this.priority = Priorities.SYMBOL_PRIORITY;
	}
	
	@Override
	public String preOrder() {
		return "(" + this.getName() + " " + this.getSymbol() + ")";
	}

	@Override
	protected String getName() {
		return "symbol";
	}

	@Override
	protected String getSymbol() {
		return "" + this.value;
	}

}
