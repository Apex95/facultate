package tema2APD;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Main 
{
	public static void main(String[] args)
	{
		int queueSize = Integer.parseInt(args[0]);
		int eventsInFile = Integer.parseInt(args[1]);
		
		
		ArrayList<EventGenerator> generatorsList = new ArrayList<EventGenerator>();
		ArrayBlockingQueue<Event> eventsQueue = new ArrayBlockingQueue<>(queueSize);
		
		AtomicInteger finishedGenerators = new AtomicInteger(0);
		
		ExecutorService executor = Executors.newFixedThreadPool(8);
		
		EventGenerator _crtGenerator;
		for (int i = 2; i < args.length; i++)
		{
			_crtGenerator = new EventGenerator(args[i], eventsQueue, finishedGenerators, eventsInFile);
			_crtGenerator.start();
			
			generatorsList.add(_crtGenerator);
		}

		
		while (finishedGenerators.get() < generatorsList.size() || !eventsQueue.isEmpty())
		{
			if (eventsQueue.isEmpty())
				continue;
			
			try 
			{
				executor.submit(eventsQueue.take());
			} 
			catch (InterruptedException e) 
			{
				e.printStackTrace();
			}
		}
		
		// ajunge aici -> eventsQueue e empty, pot da shutdown
		executor.shutdown();
		
		// astept termminarea taskurilor deja trimise
		try 
		{
			executor.awaitTermination(1, TimeUnit.MINUTES);
		} 
		catch (InterruptedException e) 
		{
			e.printStackTrace();
		}
		
		ArrayList<Integer> outputsPrime = new ArrayList<Integer>();
		ArrayList<Integer> outputsFact = new ArrayList<Integer>();
		ArrayList<Integer> outputsSquare = new ArrayList<Integer>();
		ArrayList<Integer> outputsFib = new ArrayList<Integer>();

		
		for (int i = 0; i < generatorsList.size(); i++)
		{
			outputsPrime.addAll(generatorsList.get(i).getOutputsPrime());
			outputsFact.addAll(generatorsList.get(i).getOutputsFact());
			outputsSquare.addAll(generatorsList.get(i).getOutputsSquare());
			outputsFib.addAll(generatorsList.get(i).getOutputsFib());
		}
		
		// sortare
		Collections.sort(outputsPrime);
		Collections.sort(outputsFact);
		Collections.sort(outputsSquare);
		Collections.sort(outputsFib);
		
		
		BufferedWriter writer;
		try 
		{
			writer = new BufferedWriter(new FileWriter("PRIME.out"));
			for (int i = 0; i < outputsPrime.size(); i++)
				writer.write(outputsPrime.get(i) + "\n");
			writer.close();
			
			writer = new BufferedWriter(new FileWriter("FACT.out"));
			for (int i = 0; i < outputsFact.size(); i++)
				writer.write(outputsFact.get(i) + "\n");
			writer.close();
			
			writer = new BufferedWriter(new FileWriter("SQUARE.out"));
			for (int i = 0; i < outputsSquare.size(); i++)
				writer.write(outputsSquare.get(i) + "\n");
			writer.close();
			
			writer = new BufferedWriter(new FileWriter("FIB.out"));
			for (int i = 0; i < outputsFib.size(); i++)
				writer.write(outputsFib.get(i) + "\n");
			writer.close();
		} 
		catch (IOException e1) {
			e1.printStackTrace();
		}
		
		

	}
}
