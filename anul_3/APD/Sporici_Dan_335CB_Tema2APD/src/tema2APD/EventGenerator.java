package tema2APD;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

import tema2APD.Event.EventTypes;


public class EventGenerator extends Thread
{
	String fileName = null;
	int eventsInFile;
	
	ArrayBlockingQueue<Event> eventsQueue = null;
	
	ArrayList<Integer> outputsPrime = new ArrayList<Integer>();
	ArrayList<Integer> outputsFact = new ArrayList<Integer>();
	ArrayList<Integer> outputsSquare = new ArrayList<Integer>();
	ArrayList<Integer> outputsFib = new ArrayList<Integer>();
	
	
	AtomicInteger finishedGenerators;
	
	public EventGenerator(String fileName, ArrayBlockingQueue<Event> eventsQueue, AtomicInteger finishedGenerators, int eventsInFile)
	{
		this.fileName = fileName;
		this.eventsQueue = eventsQueue;
		this.finishedGenerators = finishedGenerators;
		this.eventsInFile = eventsInFile;
	}
	
	public ArrayList<Integer> getOutputsPrime()
	{
		return outputsPrime;
	}
	
	public ArrayList<Integer> getOutputsFact()
	{
		return outputsFact;
	}
	
	public ArrayList<Integer> getOutputsSquare()
	{
		return outputsSquare;
	}
	
	public ArrayList<Integer> getOutputsFib()
	{
		return outputsFib;
	}
	
	
	@Override
	public void run() 
	{
		try  
		{
			BufferedReader inputBuffer = new BufferedReader(new FileReader(fileName));
		    
			String crtLine;
		    String[] tokens;
		    Event crtEvent = null;
		    
		    for (int i = 0; i < eventsInFile; i++)
		    {
		    	crtLine = inputBuffer.readLine();
		    	
		    	crtLine = crtLine.replace(" ", "");
		    	tokens = crtLine.split(",");
		    	
		    	Thread.sleep(Integer.parseInt(tokens[0]));
		    
		    	if (tokens[1].equals("PRIME"))
		    		crtEvent = new Event(EventTypes.PRIME, Integer.parseInt(tokens[2]), outputsPrime) ;
		    	else
		    		if (tokens[1].equals("FACT"))
			    		crtEvent = new Event(EventTypes.FACT, Integer.parseInt(tokens[2]), outputsFact);
		    		else
		    			if (tokens[1].equals("SQUARE"))
				    		crtEvent = new Event(EventTypes.SQUARE, Integer.parseInt(tokens[2]), outputsSquare);
		    			else
		    				if (tokens[1].equals("FIB"))
					    		crtEvent = new Event(EventTypes.FIB, Integer.parseInt(tokens[2]), outputsFib);
		    	
		    	if (crtEvent != null)
		    		eventsQueue.put(crtEvent);
		    }
		    
		    finishedGenerators.incrementAndGet();
		    
		    inputBuffer.close();
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}

}
