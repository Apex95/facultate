package Main;

/**Retine operanzi ce provin din expresia ternara
 * 
 * @author Dan
 *
 */
public class Ternar extends Nod 
{
	/**
	 * @param info - informatia retinuta in nod
	 */
	public Ternar(String info) 
	{
		this.info = info;
		
		this.nodStanga = null;
		this.nodDreapta = null;
	}
}
