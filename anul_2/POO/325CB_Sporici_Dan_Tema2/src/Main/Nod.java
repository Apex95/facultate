package Main;

import java.util.regex.Pattern;

public abstract class Nod
{
	protected Nod nodStanga = null;
	protected Nod nodDreapta = null;
	
	protected String info;
	
	
	public Nod()
	{
	}

	
	/**
	 * @return - fiul stang al nodului curnet
	 */
	public Nod getNodStanga() {
		return nodStanga;
	}

	/**
	 * @param nodStanga - seteaza fiul stang al nodului curent
	 */
	public void setNodStanga(Nod nodStanga) {
		this.nodStanga = nodStanga;
	}

	/**
	 * @return - fiul drept al nodului curent
	 */
	public Nod getNodDreapta() {
		return nodDreapta;
	}

	/**
	 * @param nodDreapta - seteaza fiul drept al nodului curent
	 */
	public void setNodDreapta(Nod nodDreapta) {
		this.nodDreapta = nodDreapta;
	}
	
	/**
	 * @return - returneaza informatia aflata intr-un nod (operator, numar, variabila)
	 */
	public String getInfo() {
		return info;
	}

	
	/**
	 * @param info - informatia ce trebuie setata pentru un nod
	 */
	public void setInfo(String info) {
		this.info = info;
	}


	/**
	 * @return - TRUE daca nodul nu mai are fii / FALSE altfel
	 */
	public boolean eFrunza()
	{
		return nodStanga == null && nodDreapta == null;
	}
	
	/**
	 * @return - TRUE daca informatia din nodul curent este o variabila / FALSE altfel
	 */
	public boolean eVariabila()
	{
		return Pattern.matches("^([a-zA-Z][a-zA-Z0-9_-]*)$", info);
	}
}
