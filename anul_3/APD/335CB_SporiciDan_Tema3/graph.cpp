#include "graph.h"
#include <algorithm>
#include <math.h>


#define MIN(x, y) (((x) < (y)) ? (x) : (y))

Graph::Graph(int rank)
{
	this->rank = rank;
	parent = -1;
	nOfAdjacents = 0;

	_isLeaf = false;

	_isAwake = 0;

}

Graph::~Graph()
{
}

void Graph::readInput()
{
	FILE * f = fopen("topologie.in", "r");

	int nodeId, adjacentId;
	char * token = NULL, * _token = NULL;
	
	std::vector<std::vector<int> >::iterator i;

	char buffer[512];

	while (fgets(buffer, 512, f))
	{
		i = graph.begin();

		std::vector<int> adjacents;
		std::vector<int> _empty;

		token = strtok(buffer, ":");
		sscanf(token, "%d", &nodeId);
		
		_token = strtok(NULL, ":");		
		

		while (token = strtok(_token, " "))
		{
			if (sscanf(token, "%d", &adjacentId) == 1)
			{
				adjacents.push_back(adjacentId);

				if (nodeId == rank)
					nOfAdjacents++;			
			}
			_token = NULL;
		}

		tree.insert(tree.begin() + nodeId, _empty); 
		graph.insert(i+nodeId, adjacents);
		
	}
	fclose(f);

	

	nOfNodes = graph.size();
}

void Graph::debugShow()
{
	for (std::vector<std::vector<int> >::iterator i = tree.begin(); i != tree.end(); i++)
		{
			printf("%ld: ", i - tree.begin());
			for (std::vector<int>::iterator j = (*i).begin(); j != (*i).end(); j++)
			{
				printf("%d ", (*j));
			}
			printf("\n");
		}
}



int Graph::sendProbes()
{
	int nOfProbes = 0;

	std::vector<std::vector<int> >::iterator i = graph.begin() + rank;

	// trimit probe la fiecare vecin
	for (std::vector<int>::iterator j = (*i).begin(); j != (*i).end(); j++)
	{
		if ((*j) == parent)
			continue;

		printf("[%d] probe sent to %d\n", rank, (*j));
		MPI_Send(&(*j), 1, MPI_INT, (*j), 0, MPI_COMM_WORLD);

		nOfProbes++;
	}
	
	return nOfProbes;
}


void Graph::recvProbe()
{
	int _msg;
	MPI_Recv(&_msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

	
	parent = status.MPI_SOURCE;
	printf("[%d] Recv probe. Parent set: %d\n", rank, parent);
}


void Graph::recvEcho()
{
	int remainingMsgs = rank == 0 ? nOfAdjacents : nOfAdjacents - 1;
	int _msg;

	while (remainingMsgs > 0)
	{
		MPI_Recv(&_msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		if (status.MPI_TAG == 0)
		{
			printf("[%d] Recv probe. Rejected parent: %d\n", rank, status.MPI_SOURCE);
		}

		if (status.MPI_TAG == 1)
		{
			printf("[%d] Recv echo from: %d\n", rank, status.MPI_SOURCE);

			int size = _msg, _size = 0, source = status.MPI_SOURCE;
			
			for (int i = 0; i < size; i++)
			{
				MPI_Recv(&_size, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				
				std::vector<int> adjacents;
				adjacents.resize(_size);
				MPI_Recv(&(adjacents[0]), _size, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				// aici partea de merging
				for (int j = 0; j < _size; j++)
					if (std::find(tree[i].begin(), tree[i].end(), adjacents[j]) == tree[i].end())
					{
						tree[i].push_back(adjacents[j]);
						tree[adjacents[j]].push_back(i);
					}
			}

		}

		remainingMsgs--;
	}

	

}

void Graph::sendEcho()
{
	addPathToParent();
	int size = tree.size(), _size = 0;
	MPI_Send(&size, 1, MPI_INT, parent, 1, MPI_COMM_WORLD);

	printf("[%d] echo sent to %d\n", rank, parent);

	for (int i = 0; i < size; i++)
	{
		_size = tree[i].size();

		MPI_Send(&_size, 1, MPI_INT, parent, 1, MPI_COMM_WORLD);
		MPI_Send(&(tree[i][0]), _size, MPI_INT, parent, 1, MPI_COMM_WORLD);
	}

}

void Graph::addPathToParent()
{
	tree[parent].push_back(rank);
	tree[rank].push_back(parent);
}

void Graph::sendTree()
{
	// trimite arborele
	std::vector<int> adjacents = tree[rank];	

	// stabilesc daca e frunza
	if (adjacents.size() - 1 == 0 && rank != 0)
	{
		_isLeaf = true;
		return;
	}

	int size = tree.size(), _size = 0;
	for (std::vector<int>::iterator i = adjacents.begin(); i != adjacents.end(); i++)
	{
		if ((*i) == parent)
			continue;

		MPI_Send(&size, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD);

		for (std::vector<std::vector<int> >::iterator j = tree.begin(); j != tree.end(); j++)
		{
			_size = (*j).size();
			MPI_Send(&_size, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD);
			MPI_Send(&((*j)[0]), _size, MPI_INT, (*i), 2, MPI_COMM_WORLD);

		}		

	}
	
	
}

void Graph::recvTree()
{
	int size = 0, _size = 0;
	MPI_Recv(&size, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
	
	int source = status.MPI_SOURCE;

	for (int i = 0; i < size; i++)
	{
		MPI_Recv(&_size, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
		tree[i].resize(_size);
		MPI_Recv(&(tree[i][0]), _size, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
		
	}
}

bool Graph::isLeaf()
{
	return _isLeaf;
}

int Graph::getRank()
{
	return rank;
}


void Graph::sendChunksToAdjacents(Command * crtCommand, std::vector<std::vector<unsigned char> > image)
{
	
	std::vector<int> adjacents = tree[rank];
	int splitCoefficient = rank == 0 ? adjacents.size() : adjacents.size() - 1;	

	int remainingRows = image.size() - 2;
	int nOfRows = floor((remainingRows) / (float)splitCoefficient);
	int offset = 0, rowLength = 0;



	int _size;
	for (std::vector<int>::iterator i = adjacents.begin(); i != adjacents.end(); i++)
	{
		if ((*i) == parent)
			continue;

		// trimit comanda
		MPI_Send(crtCommand, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD);

		// trimit offset
		MPI_Send(&offset, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD);

		// trimit dimensiunea unei linii
		rowLength = image[0].size();
		MPI_Send(&rowLength, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD);

		// trimit nr de linii
		if (i - adjacents.begin() == adjacents.size()-1)
			_size = remainingRows;
		else
			_size = nOfRows;

		int _size2 = _size + 2;
		MPI_Send(&_size2, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD);

		
		
		for (int j = 0; j < _size2; j++)
		{
			MPI_Send(&(image[j + offset][0]), rowLength, MPI_UNSIGNED_CHAR, (*i), 2, MPI_COMM_WORLD);
		}
		offset += _size;
		remainingRows -= _size;
	}
}

void Graph::recvChunkFromParent(Command * crtCommand, std::vector<std::vector<unsigned char> > * image)
{
	image->clear();
	int rowLength = 0, nOfRows = 0;

	MPI_Recv(crtCommand, 1, MPI_INT, parent, 2, MPI_COMM_WORLD, &status);
	MPI_Recv(&imgOffset, 1, MPI_INT, parent, 2, MPI_COMM_WORLD, &status);
	MPI_Recv(&rowLength, 1, MPI_INT, parent, 2, MPI_COMM_WORLD, &status);
	MPI_Recv(&nOfRows, 1, MPI_INT, parent, 2, MPI_COMM_WORLD, &status);
	
	printf("[%d] recv[p] cmd: %d | offset: %d | rowlength: %d | nofrows: %d\n", rank, *crtCommand, imgOffset, rowLength, nOfRows);

	(*image).resize(nOfRows);

	for (int i = 0; i < nOfRows; i++)
	{
		(*image)[i].resize(rowLength);
		MPI_Recv(&((*image)[i][0]), rowLength, MPI_UNSIGNED_CHAR, parent, 2, MPI_COMM_WORLD, &status);
	}
}

void Graph::sendChunkToParent(Command * crtCommand, std::vector<std::vector<unsigned char> > image)
{
	int rowLength = 0, nOfRows = 0;

	rowLength = image[0].size();
	nOfRows = image.size();

	int _indexStart = 0, _indexOffset = 0;

	// nu trimit prima si ultima linie (care-s in plus, doar pt aplicarea de efecte)
	if (_isLeaf)
	{
		_indexStart = 1;
		nOfRows -= 2;
		_indexOffset = 1;
	}


	MPI_Send(crtCommand, 1, MPI_INT, parent, 2, MPI_COMM_WORLD);
	MPI_Send(&imgOffset, 1, MPI_INT, parent, 2, MPI_COMM_WORLD);
	MPI_Send(&rowLength, 1, MPI_INT, parent, 2, MPI_COMM_WORLD);
	MPI_Send(&nOfRows, 1, MPI_INT, parent, 2, MPI_COMM_WORLD);

	
	for (int i = _indexStart; i < nOfRows + _indexOffset; i++)
		MPI_Send(&(image[i][0]), rowLength, MPI_UNSIGNED_CHAR, parent, 2, MPI_COMM_WORLD);
}

void Graph::recvChunksFromAdjacents(std::vector<std::vector<unsigned char> > * image)
{
	image->clear();

	std::vector<int> adjacents = tree[rank];

	Command _cmd;
	int offset = 0, rowLength, nOfRows;

	for (std::vector<int>::iterator i = adjacents.begin(); i != adjacents.end(); i++)
	{
		if ((*i) == parent)
			continue;

		MPI_Recv(&_cmd, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD, &status);
		MPI_Recv(&offset, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD, &status);
		MPI_Recv(&rowLength, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD, &status);
		MPI_Recv(&nOfRows, 1, MPI_INT, (*i), 2, MPI_COMM_WORLD, &status);

		image->resize(offset + nOfRows);

		for (int j = 0; j < nOfRows; j++)
		{
			((*image)[j + offset]).resize(rowLength);
			MPI_Recv(&((*image)[j + offset][0]), rowLength, MPI_UNSIGNED_CHAR, (*i), 2, MPI_COMM_WORLD, &status);		
		}
		printf("[%d] recv[adj] cmd: %d | offset: %d | rowlength: %d | nofrows: %d\n", rank, _cmd, offset, rowLength, nOfRows);
	}

	

}

void Graph::sendAwakeStatus()
{
	// trimit starea retelei (awake?) la descendenti
	std::vector<int> adjacents = tree[rank];


	for (std::vector<int>::iterator i = adjacents.begin(); i != adjacents.end(); i++)
	{
		if ((*i) == parent)
			continue;

		MPI_Send(&_isAwake, 1, MPI_CHAR, (*i), 3, MPI_COMM_WORLD);
	}
}

void Graph::recvAwakeStatus()
{
	// primesc statusul de la nodul parinte si actualizez
	MPI_Recv(&_isAwake, 1, MPI_CHAR, parent, 3, MPI_COMM_WORLD, &status);
	
}

bool Graph::isAwake()
{
	return _isAwake != 0;
}

void Graph::wakeUp()
{
	_isAwake = 1;
}

void Graph::goSleep()
{
	_isAwake = 0;
}

int Graph::getNofLeaves()
{
	int nOfLeaves = 0;

	for (std::vector<std::vector<int> >::iterator i = tree.begin(); i != tree.end(); i++)
		if ((*i).size() == 1)
			nOfLeaves++;

	return nOfLeaves;
}

void Graph::sendNofLinesToParent(int nOfLines)
{	
	// fara legatura catre nodul parinte
	int nOfAdjacents = tree[rank].size() - 1;

	int _rank, _size;

	// adun rezultatele de la descendenti
	std::vector<int> processedLines;
	std::vector<int> crtLines;

	processedLines.resize(tree.size());
	crtLines.resize(tree.size());

	_size = processedLines.size();

	// daca e frunza, trimite rank si nr de linii
	if (_isLeaf)
	{	
		processedLines[rank] = nOfLines;

		MPI_Send(&(processedLines[0]), _size, MPI_INT, parent, 5, MPI_COMM_WORLD);

		printf("[%d] Leaf - sent lines array w/ size %d\n", rank, _size);

		return;
	}


	// nu e frunza - va aduna toate liniile procesate de fiecare descendent (frunza) intr-un vector
	for (int i = 0; i < nOfAdjacents; i++)
	{
		// si vectorul respectiv (cu nr de linii de la descendenti)
		MPI_Recv(&(crtLines[0]), _size, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
		
		// merge intre datele primite si ce aveam
		for (int j = 0; j < _size; j++)
			if (processedLines[j] == 0)
				processedLines[j] = crtLines[j];
	}

	// trimite vectorul catre nodul parinte
	
	MPI_Send(&(processedLines[0]), _size, MPI_INT, parent, 5, MPI_COMM_WORLD);
}

void Graph::recvNofLines(std::vector<int> * processedLines)
{
	int _size;
	std::vector<int> crtLines;


	int nOfAdjacents = tree[rank].size();

	(*processedLines).resize(tree.size());

	_size = (*processedLines).size();

	crtLines.resize(_size);

	for (int i = 0; i < nOfAdjacents; i++)
	{	
		MPI_Recv(&(crtLines[0]), _size, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);

		for (int j = 0; j < _size; j++)
			if ((*processedLines)[j] == 0)
				(*processedLines)[j] = crtLines[j];
	}
}

