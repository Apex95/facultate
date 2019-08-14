package Main;

public class Factor extends Nod 
{
	/** Nod ce retine un factor (rezultat al produsului / impartirii)
	 * @param info - factorul
	 */
	public Factor(String info) 
	{
		this.info = info;
		
		this.nodStanga = null;
		this.nodDreapta = null;
	}
	
}
