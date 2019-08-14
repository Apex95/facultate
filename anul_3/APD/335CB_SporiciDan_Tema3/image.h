#pragma once

#include "graph.h"
#include "commands.h"
#include "filter.h"
#include <stdio.h>
#include <vector>
#include <mpi.h>

class Image
{
public:
	Image(Graph * graph);
	~Image();

	bool readAndSend();
	void recvChunkFromParent();
	void processImage();

	void recvChunksFromAdjacents();
	void sendChunkToParent();

	void writeToFile();

	void sendNofLinesToParent();
	void recvNofLinesFromAdjacents();

protected:

private:
	void readPGM();

	int nOfImages, crtWidth, crtHeight, crtMaxValue;
	char * crtInputName, crtOutputName[100];

	int nOfProcessedLines;

	Command crtCommand;

	std::vector<std::vector<unsigned char> > image;

	Graph * graph;
	Filter filter;

	FILE * f = NULL;
};
