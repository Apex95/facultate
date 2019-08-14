#include "filter.h"

#include <stdio.h>
#include <climits>

Filter::Filter()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			smoothWeights[i][j] = 1;

			sharpWeights[i][j] = (i + j == 1) || (i + j == 3) ? -2 : 0;
			sharpWeights[i][j] = (i == j && i + j == 2) ? 11 : sharpWeights[i][j];


			blurWeights[i][j] = (i + j == 1) || (i + j == 3) ? 2 : 1;
			blurWeights[i][j] = (i == j && i + j == 2) ? 4 : blurWeights[i][j];

			meanRemoveWeights[i][j] = -1;
			meanRemoveWeights[i][j] = (i == j && i + j == 2) ? 9 : meanRemoveWeights[i][j]; 

		}
	}

	smoothFactor = 9.0f;
	sharpFactor = 3.0f;
	blurFactor = 16.0f;
	meanRemoveFactor = 1.0f;

}

Filter::~Filter()
{
	
}



void Filter::applyFilter(Command cmd, std::vector<std::vector<unsigned char> > * image)
{
	int crtPixelValue = 0;

	std::vector<std::vector<unsigned char> > outputImage;


	for (int i = 0; i < (*image).size(); i++)
	{
		std::vector<unsigned char> row;

		if (i == 0 || i == (*image).size() - 1)
		{
			outputImage.push_back((*image)[i]);
			continue;
		}

		


		for (int j = 0; j < (*image)[i].size(); j++)
		{
			if (j == 0 || j == (*image)[i].size() - 1)
			{
				row.push_back((*image)[i][j]);
				continue;
			}

			if (cmd == SMOOTHEN)
			{
				crtPixelValue = ((*image)[i-1][j-1] * smoothWeights[0][0] +
						(*image)[i-1][j] * smoothWeights[0][1] + 
						(*image)[i-1][j+1] * smoothWeights[0][2] +

						(*image)[i][j-1] * smoothWeights[1][0] +
						(*image)[i][j] * smoothWeights[1][1] +
						(*image)[i][j+1] * smoothWeights[1][2] +

						(*image)[i+1][j-1] * smoothWeights[2][0] +
						(*image)[i+1][j] * smoothWeights[2][1] +
						(*image)[i+1][j+1] * smoothWeights[2][2]) / smoothFactor;				
				
				

			}


			if (cmd == SHARPEN)
			{
				crtPixelValue = ((*image)[i-1][j-1] * sharpWeights[0][0] +
						(*image)[i-1][j] * sharpWeights[0][1] + 
						(*image)[i-1][j+1] * sharpWeights[0][2] +

						(*image)[i][j-1] * sharpWeights[1][0] +
						(*image)[i][j] * sharpWeights[1][1] +
						(*image)[i][j+1] * sharpWeights[1][2] +

						(*image)[i+1][j-1] * sharpWeights[2][0] +
						(*image)[i+1][j] * sharpWeights[2][1] +
						(*image)[i+1][j+1] * sharpWeights[2][2]) / sharpFactor;
				
			}

			if (cmd == BLUR)
			{
				crtPixelValue = ((*image)[i-1][j-1] * blurWeights[0][0] +
						(*image)[i-1][j] * blurWeights[0][1] + 
						(*image)[i-1][j+1] * blurWeights[0][2] +

						(*image)[i][j-1] * blurWeights[1][0] +
						(*image)[i][j] * blurWeights[1][1] +
						(*image)[i][j+1] * blurWeights[1][2] +

						(*image)[i+1][j-1] * blurWeights[2][0] +
						(*image)[i+1][j] * blurWeights[2][1] +
						(*image)[i+1][j+1] * blurWeights[2][2]) / blurFactor;
			
			}

			if (cmd == MEAN_REMOVE)
			{
				crtPixelValue = ((*image)[i-1][j-1] * meanRemoveWeights[0][0] +
						(*image)[i-1][j] * meanRemoveWeights[0][1] + 
						(*image)[i-1][j+1] * meanRemoveWeights[0][2] +

						(*image)[i][j-1] * meanRemoveWeights[1][0] +
						(*image)[i][j] * meanRemoveWeights[1][1] +
						(*image)[i][j+1] * meanRemoveWeights[1][2] +

						(*image)[i+1][j-1] * meanRemoveWeights[2][0] +
						(*image)[i+1][j] * meanRemoveWeights[2][1] +
						(*image)[i+1][j+1] * meanRemoveWeights[2][2]) / meanRemoveFactor;

			}

			

			if (crtPixelValue > 255)
				crtPixelValue = 255;

			if (crtPixelValue < 0)
				crtPixelValue = 0;
			
			row.push_back(crtPixelValue);
				
		}
		outputImage.push_back(row);

	}


	(*image).assign(outputImage.begin(), outputImage.end());
	
}
