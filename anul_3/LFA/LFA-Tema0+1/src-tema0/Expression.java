

public abstract class Expression {

	protected Priorities priority;
	public abstract String preOrder();
	public abstract String inOrder();
	protected abstract String getName();
	protected abstract String getSymbol();
	public int getPriority() {
		return this.priority.getPriority();
	}
	@Override
	public String toString() {
		return this.inOrder();
	}

	public static void main(String [] argz) {
		Concatenation concat = new Concatenation(new VoidExpression(),
				new Union(new NullExpression(), new SymbolExpression('a')));
		System.out.println(concat.inOrder());
		System.out.println(concat.preOrder());
	}

	public enum Priorities {
		UNION_PRIORITY(0),
		CONCAT_PRIORITY(1),
		KLEENE_PRIORITY(2),
		PLUS_PRIORITY(2),
		OPTIONAL_PRIORITY(2),
		NULL_PRIORITY(3),
		VOID_PRIORITY(3),
		SYMBOL_PRIORITY(3);

		private final int priority;
		private Priorities(int priority) {
			this.priority = priority;
		}
		public int getPriority()
		{
			return this.priority;
		}
	}
}
