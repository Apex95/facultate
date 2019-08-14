#include "graph.h"
#include "image.h"
#include "mpi.h"
#include <unistd.h>




// TODO: functie care sa intoarca vector cu vecinii unui nod (ajutatoare)

int main(int argc, char * argv[]) 
{
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	Graph graph(rank);
	Image image(&graph);

	if (rank == 0)
	{
		graph.readInput();
		
		// trimitere probe
		graph.sendProbes();

		// primeste ecou
		graph.recvEcho();


		// aici trimit arborele de acoperire
		graph.debugShow();
		graph.sendTree();

		graph.wakeUp();

		// citesc si trimit imagini
		while (image.readAndSend())
		{
			image.recvChunksFromAdjacents();
			image.writeToFile();			

			graph.sendAwakeStatus();
			
		}

		//primesc nr de linii de la fiecare nod adiacent
		image.recvNofLinesFromAdjacents();
		

	}
	else
	{
		graph.readInput();

		// astept o proba
		graph.recvProbe();

		// trimit probe la restul, exceptand parintele
		graph.sendProbes();

		// primesc fie echo fie probe de la un nod cu care se forma o bucla in graf
		graph.recvEcho();		

		// aici trimite ecou spre parinte
		graph.sendEcho();
				
		// primesc si trimit arborele de acoperire
		graph.recvTree();

		// trimit arborele primit anterior si la ceilalti vecini
		graph.sendTree();


		graph.wakeUp();
		
		while (graph.isAwake())
		{
			// astept sa primesc dinspre initiator o parte din imagine
			image.recvChunkFromParent();

			// trimit mai departe sau inapoi la parinte (daca e frunza)
			image.processImage();
		

			// un nod != frunza astepta rezultatul de la nodurile adiacente
			image.recvChunksFromAdjacents();
	
			// tot pt nod != frunza, propaga rezultatul spre parinte
			image.sendChunkToParent();

			graph.recvAwakeStatus();
			graph.sendAwakeStatus();
		}


		// trimit nr de linii procesate
		image.sendNofLinesToParent();

		
		
	}


	MPI_Finalize();
	return 0;
}
