package Main;

/**
 * Nod ce retine un termen (operand al unei sume / diferente)
 * @author Dan
 *
 */
public class Termen extends Nod 
{
	/**
	 * @param info - valoarea operandului
	 */
	public Termen(String info) 
	{
		this.info = info;
		
		this.nodStanga = null;
		this.nodDreapta = null;
	}
	
}
