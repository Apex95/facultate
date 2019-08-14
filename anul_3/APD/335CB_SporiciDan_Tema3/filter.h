#pragma once

#include <vector>
#include "commands.h"

class Filter
{
public:
	Filter();
	~Filter();

	
	void applyFilter(Command cmd, std::vector<std::vector<unsigned char> > * image);
protected:
private:

	int smoothWeights[3][3];
	float smoothFactor;

	int sharpWeights[3][3];
	float sharpFactor;

	int blurWeights[3][3];
	float blurFactor;

	int meanRemoveWeights[3][3];
	float meanRemoveFactor;
	
};
