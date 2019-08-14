package lfatema1;

public class Symbol 
{
	protected Character c;
	private boolean canDerivateVoid = false;
	private boolean isUseful = false;
	
	public Symbol(Character c)
	{
		this.c = c;
		if (c == 'e')
		{
			//canDerivateVoid = true;
			//isUseful = true;
		}
	}
	
	public Character getChar()
	{
		return c;
	}
	
	public boolean isVoid()
	{
		return c == 'e';
	}
	
	public boolean canDerivateVoid()
	{
		return this.canDerivateVoid;
	}
	
	public boolean canDerivateVoid(boolean v)
	{
		this.canDerivateVoid = v;
		return this.canDerivateVoid;
	}
	
	public boolean isUseful()
	{
		return this.isUseful;
	}
	
	public boolean isUseful(boolean v)
	{
		this.isUseful = v;
		return this.isUseful;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((c == null) ? 0 : c.hashCode());
		result = prime * result + (canDerivateVoid ? 1231 : 1237);
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;

		Symbol other = (Symbol) obj;
		if (c == null) {
			if (other.c != null)
				return false;
		} else if (!c.equals(other.c))
			return false;
		if (canDerivateVoid != other.canDerivateVoid)
			return false;
		return true;
	}
	
	
}
