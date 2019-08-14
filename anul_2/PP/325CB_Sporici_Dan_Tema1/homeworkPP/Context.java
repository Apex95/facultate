package homeworkPP;

import java.util.HashMap;
import java.util.Map.Entry;

public class Context 
{
  
	private HashMap<String, Integer> variablesTable = new HashMap<String, Integer>();
	
    public void add (String v, Integer i)
    {
    	variablesTable.put(v, i);
    	
    }
    // Treat undefined variable problem using exceptions
    public Integer valueOf(String v) throws Exception
    {
    	if (variablesTable.containsKey(v))
    		return variablesTable.get(v);
    	else
    		throw new Exception("Variabila nu exista in contextul dat.");
    }
    
    private HashMap<String, Integer> backupVariablesTable = null;
    
    // salvez tabelul de variabile pt cand intru intr-un bloc IF / WHILE
    public void saveVariablesTable()
    {
    	backupVariablesTable = new HashMap<String, Integer>(variablesTable);
    }
    
    // din tabelul obtinut dupa IF / WHILE, trebuie sa pastrez doar modificari de variabile deja existente
    // restul devin out of scope (fiind declarate in blocul IF / WHILE)
    public void updateVariablesTable()
    {
    	for (Entry<String, Integer> entry : variablesTable.entrySet())
    	{
    		// daca o variabila din contextul rezultat deja exista in contextul de backup,
    		// inseamna ca in IF/ WHILE s-a facut o actualizare a acelei variabile => in scope
    		if (backupVariablesTable.containsKey(entry.getKey())) 
    			backupVariablesTable.put(entry.getKey(), entry.getValue());
    	}
    	
    	variablesTable = backupVariablesTable;
    }
}