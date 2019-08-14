#pragma once

#include "commands.h"
#include <stdio.h>
#include <vector>
#include <mpi.h>

class Graph
{
public:
	Graph(int rank);
	~Graph();

	void readInput();
	void debugShow();
	int sendProbes();
	void recvProbe();
	void recvEcho();

	void addPathToParent();

	void sendEcho();

	void sendTree();
	void recvTree();

	void sendChunksToAdjacents(Command * crtCommand, std::vector<std::vector<unsigned char> > image);

	void recvChunkFromParent(Command * crtCommand, std::vector<std::vector<unsigned char> > * image);
	void sendChunkToParent(Command * crtCommand, std::vector<std::vector<unsigned char> > image);

	void recvChunksFromAdjacents(std::vector<std::vector<unsigned char> > * image);

	bool isLeaf();
	int getRank();

	void wakeUp();
	void goSleep();
	bool isAwake();

	void sendAwakeStatus();
	void recvAwakeStatus();

	void sendNofLinesToParent(int nOfLines);
	void recvNofLines(std::vector<int> * processedLines);

	int getNofLeaves();

protected:

private:
	std::vector<std::vector<int> > graph;
	std::vector<std::vector<int> > tree;

	int nOfNodes, nOfAdjacents;
	int parent;
	int rank;

	int imgOffset;

	MPI_Status status;

	bool _isLeaf;
	char _isAwake;
};
