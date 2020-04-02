#include "Calculations.h"


int GetZoneBrihtness(ImageBuffer<unsigned char> &img, int column, int row, int zoneSize)
{
	int brightness = 0;
	int rows = row + zoneSize;
	int columns = column + zoneSize;
	for (int j = row; j < rows; ++j)
	{
		for (int i = column; i < columns; ++i)
		{
			brightness += *img.GetElement(i, j);
		}
	}
	return brightness/(zoneSize*zoneSize);
}

ECalculationsStatus
GetAngles(ImageBuffer<unsigned char> &img, int32_t& x, int32_t& y)
{
	int zoneSize = 30;
	const int zonesRowCount = 3;
	const int zonesColumnCount = 3;

	int width = img.GetWidth();
	int height = img.GetHeight();

	int arrBright[zonesColumnCount][zonesRowCount]
						{{0, 0, 0},
						{0, 0, 0},
						{0, 0, 0 }};
	const int columnStep = (height -zoneSize)/(zonesColumnCount-1);
	const int rowStep = (width - zoneSize)/(zonesRowCount-1);
	for(int i = 0; i < zonesColumnCount; ++i)
	{
		for(int j = 0; j < zonesRowCount; ++j)
		{
			arrBright[i][j] =
					GetZoneBrihtness(img, i*columnStep, j*rowStep,zoneSize);
		}
	}
	return ECalculationsStatus::SUCCESS;
}
