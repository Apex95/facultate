#include "image.h"


Image::Image(Graph * graph)
{
	this->graph = graph;

	nOfProcessedLines = 0;
}

Image::~Image()
{
}

bool Image::readAndSend()
{
	if (f == NULL)
	{
		f = fopen("imagini.in", "r");
		fscanf(f, "%d\n", &nOfImages);
	}

	if (nOfImages <= 0)
	{
		fclose(f);
		return false;
	}

	char buffer[512];
	
	fgets(buffer, 512, f);
		
	char * _cmd = strtok(buffer, " ");

	if (strstr(_cmd, "blur") != NULL)
		crtCommand = BLUR;

	if (strstr(_cmd, "sharpen") != NULL)
		crtCommand = SHARPEN;

	if (strstr(_cmd, "smooth") != NULL)
		crtCommand = SMOOTHEN;

	if (strstr(_cmd, "mean_removal") != NULL)
		crtCommand = MEAN_REMOVE;

	crtInputName = strtok(NULL, " ");

	memset(crtOutputName, 0, sizeof(crtOutputName));
	strcpy(crtOutputName, strtok(NULL, " "));


	if (crtOutputName[strlen(crtOutputName)-1] == '\n' || crtOutputName[strlen(crtOutputName)-1] == '\r'
		|| crtOutputName[strlen(crtOutputName)-1] == ' ')
	     crtOutputName[strlen(crtOutputName)-1] = '\0';

		
	processImage();
		

	nOfImages--;

	if (nOfImages <= 0)
		graph->goSleep();

	return true;
}

void Image::readPGM()
{
	image.clear();

	FILE * f = fopen(crtInputName, "r");

	char buffer[512];
	
	fgets(buffer, 512, f);
	char * magicNumber = strtok(buffer, " ");
	
	if (magicNumber[strlen(magicNumber)-1] == '\n' || magicNumber[strlen(magicNumber)-1] == '\r'
		    || magicNumber[strlen(magicNumber)-1] == ' ')
			magicNumber[strlen(magicNumber)-1] = '\0';

	if (strcmp(magicNumber, "P2"))
	{
		fclose(f);  // nu e plain PGM
		return;
	}

	fgets(buffer, 512, f);
	
	while (strstr(buffer, "#") != NULL)
		fgets(buffer, 512, f);

	sscanf(buffer, "%d %d\n", &crtWidth, &crtHeight);
	
	fscanf(f, "%d", &crtMaxValue);

	// -- bordare	
	std::vector<unsigned char> _emptyRow;
	for (int i = 0; i < crtWidth + 2; i++)
		_emptyRow.push_back(0);

	// bordare - linia de sus
	image.push_back(_emptyRow);

	int color;
	for (int i = 0; i < crtHeight; i++)
	{
		std::vector<unsigned char> row;

		// bordare
		row.push_back(0);		

		for (int j = 0; j < crtWidth; j++)
		{
			fscanf(f, "%d", &color);
			row.push_back((unsigned char)color);
		}
		
		// bordare
		row.push_back(0);

		image.push_back(row);
	}

	// bordare - linia de jos
	image.push_back(_emptyRow);

	printf("%d %d\n", crtWidth, crtHeight);


	fclose(f);

	
}



void Image::processImage()
{
	// rank 0 face si citirea din fisier
	if (graph->getRank() == 0)
		readPGM();

	

	if (!graph->isLeaf())
	{
		graph->sendChunksToAdjacents(&crtCommand, image);
	}
	else
	{
		filter.applyFilter(crtCommand, &image);
		nOfProcessedLines += image.size() - 2;  // fara liniile folosite la bordare

		graph->sendChunkToParent(&crtCommand, image);
	}
}

void Image::recvChunkFromParent()
{
	graph->recvChunkFromParent(&crtCommand, &image);
	
}

void Image::recvChunksFromAdjacents()
{
	if (graph->isLeaf())
		return;

	graph->recvChunksFromAdjacents(&image);
}

void Image::sendChunkToParent()
{
	if (graph->isLeaf())
		return;

	graph->sendChunkToParent(&crtCommand, image);
}

void Image::writeToFile()
{
	FILE * g = fopen(crtOutputName, "w");

	fprintf(g, "P2\n%ld %ld\n%d\n", image[0].size()-2, image.size(), crtMaxValue);
	
	for (std::vector<std::vector<unsigned char> >::iterator i = image.begin(); i != image.end(); i++)
	{
		for (int j = 1; j < (*i).size() - 1; j++)
		{
			fprintf(g, "%d\n", (*i)[j]);
		}
	}
	fclose(g);
}

void Image::sendNofLinesToParent()
{
	graph->sendNofLinesToParent(nOfProcessedLines);
}

void Image::recvNofLinesFromAdjacents()
{
	std::vector<int> processedLines;
	graph->recvNofLines(&processedLines);

	FILE * g = fopen("statistica.out", "w");


	for (int i = 0; i < processedLines.size(); i++)
		fprintf(g, "%d: %d\n", i, processedLines[i]);

	fclose(g);
}




