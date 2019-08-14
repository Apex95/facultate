package Main;

/**Nod ce retine un operator
 * @author Dan
 *
 */
public class Operator extends Nod
{
	// retin daca expresia e incadrata intre paranteze - altfel acestea se pierd la afisarea arborelui
	boolean areParanteze = false;
	
	/**
	 * @param info - operatorul sub forma de string
	 * @param areParanteze - daca expresia e incadrata sau nu in paranteze
	 * @param nodStanga - operandul din stanga
	 * @param nodDreapta - operandul din dreapta
	 */
	public Operator(String info, boolean areParanteze, Nod nodStanga, Nod nodDreapta)
	{
		this.info = info;
		
		this.areParanteze = areParanteze;
		
		this.nodStanga = nodStanga;
		this.nodDreapta = nodDreapta;
	}
}
