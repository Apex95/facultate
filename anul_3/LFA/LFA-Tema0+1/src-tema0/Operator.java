

public abstract class Operator extends Expression {

	protected abstract String preArguments();
	
	protected String inArgument(Expression argument) {
		String pre = "", post = "";
		if(argument.getPriority() < this.getPriority()) {
			pre = "(";
			post = ")";
		}
		return pre + argument.inOrder() + post;
	}
	protected String preArgument(Expression argument) {
		return " " + argument.preOrder();
	}
	
	@Override
	public String preOrder() {
		return "(" + this.getName() + this.preArguments() + ")";
	}

}
