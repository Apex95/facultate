package tema2APD;

import java.util.ArrayList;

public class Event implements Runnable
{
	public enum EventTypes
	{
	    PRIME("PRIME"),
	    FACT("FACT"),
	    SQUARE("SQUARE"),
	    FIB("FIB");
		
		private String type;
		
		private EventTypes(String type)
		{
			this.type = type;
		}
	}
	
	EventTypes type;
	int n;
	ArrayList<Integer> outputs = null;
	
	public Event(EventTypes type, int n, ArrayList<Integer> outputs)
	{
		this.type = type;
		this.n = n;
		this.outputs = outputs;
	}
	

	@Override
	public void run() 
	{
		// -- prime
		if (type.equals(EventTypes.PRIME))
		{
			int nr = n;
			boolean prim = false;

			while (!prim)
			{	
				prim = true;
				for (int d = 2; d < nr/2; d++)
					if (nr % d == 0)
					{
						prim = false;
						break;
					}
				
				if (!prim)
					nr--;
			}
			

			// arraylist.add nu pare thread-safe
			synchronized (outputs) 
			{
				outputs.add(nr);
			}	
		}
		
		
		// -- fact
		if (type.equals(EventTypes.FACT))
		{
			int fact = 1;
			int i;
			
			for (i = 2; fact * i < n; i++)
				fact *= i;
			
			i--;
			
			synchronized (outputs) 
			{
				outputs.add(i);
			}
		}
		
		
		// -- square
		if (type.equals(EventTypes.SQUARE))
		{
			synchronized (outputs) 
			{
				outputs.add((int) Math.sqrt(n));
			}
		}
		
		
		
		// -- fib
		if (type.equals(EventTypes.FIB))
		{
			int f0 = 0, f1 = 1, f2 = f0 + f1;
			int i = 2;
			
			while (f2 <= n)
			{
				f0 = f1;
				f1 = f2;
				f2 = f0 + f1;
				i++;
			}
			
			i--;
			
			synchronized (outputs) 
			{
				outputs.add(i);
			}
		}
				
	}
	
}
