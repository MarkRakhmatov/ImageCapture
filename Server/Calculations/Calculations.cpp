#include "Calculations.h"
#include "CodeGeneration.h"
#include "SettingsHandler.h"

#include <iostream>
#include <errno.h>
#include <cmath>

namespace ServerSide
{
	const int zonesRowCount = 3;
	const int zonesColumnCount = 3;

	template<typename T>
	T sqr(T num)
	{
		return num*num;
	}

	int GetZoneBrihtness(ImageBuffer<unsigned char> &img, int column, int row, int zoneSize)
	{
		int brightness = 0;
		const int rows = row + zoneSize;
		const int columns = column + zoneSize;
		for (int i = column; i < columns; ++i)
		{
			for (int j = row; j < rows; ++j)
			{
				brightness += *img.GetElement(i, j);
			}
		}
		return brightness/(zoneSize*zoneSize);
	}

	ECalculationsStatus GetAnglesByPoints(ImageBuffer<unsigned char> &img, Point pointA, Point pointC, Point pointB, float& fi, float& gamma)
	{
		std::cout << "point A.x = " << pointA.x << std::endl;
		std::cout << "point A.y = " << pointA.y << std::endl;

		std::cout << "point C.x = " << pointC.x << std::endl;
		std::cout << "point C.y = " << pointC.y << std::endl;

		std::cout << "point B.x = " << pointB.x << std::endl;
		std::cout << "point B.y = " << pointB.y << std::endl;

		int R = 6370;
		int h = 408;
		Point pointE;
		Point pointD;
		RET_ON_TRUE((pointB.y - pointC.y) == 0, ECalculationsStatus::FAIL);
		float A1 = static_cast<float>(pointB.x - pointA.x)
											/ (pointB.y - pointA.y);

		pointE.y = (pointA.y + pointC.y) / 2;
		pointE.x = (pointA.x + pointC.x) / 2;

		RET_ON_TRUE((pointB.y - pointC.y) == 0, ECalculationsStatus::FAIL);
		float B1 =  pointE.y +
				pointE.x*(pointC.x - pointA.x) / (pointB.y - pointC.y);

		float A2 = static_cast<float>(pointB.x - pointC.x) / (pointB.y - pointC.y);

		pointD.y = (pointC.y + pointB.y) / 2;
		pointD.x = (pointC.x + pointB.x) / 2;

		float B2 = pointD.y + pointD.x*(pointB.x - pointC.x) / (pointB.y - pointC.y);

		RET_ON_TRUE((A2 - A1) == 0, ECalculationsStatus::FAIL);
		Point pointO;
		pointO.x = (B2 - B1) / (A2 - A1);
		pointO.y = B1 - A1 * (B2 - B1) / (A2 - A1);
		float r = sqrt(sqr(pointA.x - pointO.x) + sqr(pointA.y - pointO.y));
		float rf = (R*sqrt(1 - sqr(R / (R + h))));
		float n = rf / r;
		float hr = R * (1 / (sqrt(1 / sqr(r / R))) - 1);

		RET_ON_TRUE((pointB.x - pointA.x) == 0, ECalculationsStatus::FAIL);
		fi = atan((pointB.y - pointA.y) / (pointB.x - pointA.x)) * 180 / 3.14;
		RET_ON_TRUE(r > R, ECalculationsStatus::FAIL);
		float gammaRadian = asinf(sqrt(1 - sqr(r / R)));
		gamma =  gammaRadian * 180 / 3.14;

		std::cout << "R " << R << std::endl;
		std::cout << "h " << h << std::endl;
		std::cout << "hr " << hr << std::endl;
		std::cout << "A1 " << A1 << std::endl;
		std::cout << "B1 " << B1 << std::endl;
		std::cout << "A2 " << A2 << std::endl;
		std::cout << "B2 " << B2 << std::endl;
		std::cout << "X0 " << pointO.x << std::endl;
		std::cout << "Y0 " << pointO.y << std::endl;
		std::cout << "r " << r << std::endl;
		std::cout << "n " << n << std::endl;
		std::cout << "fi " << fi << std::endl;
		std::cout << "gamma " << gamma << std::endl;

		return ECalculationsStatus::SUCCESS;
	}

	ECalculationsStatus PreAnalysis(ImageBuffer<unsigned char> &img, int arrBright[zonesColumnCount][zonesRowCount])
	{
		int width = img.GetWidth();
		int height = img.GetHeight();

		const int32_t zoneSize = SettingsHandler::Get().GetPointDetectionWindfowSize();
		const int32_t brightnessLowerBound = SettingsHandler::Get().GetBrightnessLowerBound();
		const int columnStep = (height -zoneSize)/(zonesColumnCount-1);
		const int rowStep = (width - zoneSize)/(zonesRowCount-1);

		for(int i = 0; i < zonesColumnCount; ++i)
		{
			for(int j = 0; j < zonesRowCount; ++j)
			{
				arrBright[i][j] =
						GetZoneBrihtness(img, i*columnStep, j*rowStep, zoneSize);
			}
		}

		int counter = 0, counterI = 0, brightSum = 0, brightSumI = 0;

		int temp{};
		for(int i = 0; i < zonesColumnCount; ++i)
		{
			for(int j = 0; j < zonesRowCount; ++j)
			{
				temp = arrBright[i][j];
				std::cout << "arrBright[" << i << "][" << j << "] = " << temp << std::endl;
				if (temp < brightnessLowerBound)
				{
					++counter;
					brightSum += temp;
				}
				else
				{
					++counterI;
					brightSumI += temp;
				}
			}
		}

		if (counterI < 1)
		{
			std::cout << "There is no Earth on the photo" << std::endl;
			return ECalculationsStatus::FAIL;
		}

		RET_ON_TRUE(counter == 0, ECalculationsStatus::FAIL);

		int AvrBright = brightSum / counter;
		int AvrBrightI = brightSumI / counterI;

		std::cout << "\nSummary brightness " << brightSum << std::endl;
		std::cout << "Number of zones " << counter << std::endl;
		std::cout << "average brightness " << AvrBright << std::endl;

		std::cout << "\nSummary brightnessI " << brightSumI << std::endl;
		std::cout << "Number of Izones " << counterI << std::endl;
		std::cout << "average brightnessI " << AvrBrightI << std::endl;

		return ECalculationsStatus::SUCCESS;
	}

	enum class EDirection
	{
		HEAD_TO_TAIL,
		TAIL_LO_HEAD,
		LEFT_TO_RIGHT,
		RIGHT_TO_LEFT
	};

	Point ConvertCoordinates(uint32_t width, uint32_t height, Point inputPoint)
	{
		Point point;
		point.x = inputPoint.x-width/2;
		point.y = height/2 - inputPoint.y;
		return point;
	}

	ECalculationsStatus FindHorizonPoints(ImageBuffer<unsigned char> &img, EDirection direction,
			int trashhold, std::vector<Point>& points)
	{
		uint32_t width = img.GetWidth();
		uint32_t height = img.GetHeight();
		const int zoneSize = 10;
		points.resize(3);

		int tempBrightness = 0;
		uint32_t pointsCounter = 0;
		if(direction == EDirection::HEAD_TO_TAIL)
		{
			for(uint32_t column = zoneSize; column < width - zoneSize && pointsCounter < 3; )
			{
				for(uint32_t row = 3*zoneSize; row < height - zoneSize; ++row)
				{
					tempBrightness = GetZoneBrihtness(img, row, column, zoneSize);
					if(tempBrightness > trashhold)
					{
						points[pointsCounter].x = column+zoneSize/2;
						points[pointsCounter].y = row+zoneSize/2;
						++pointsCounter;
						break;
					}
				}
				if(pointsCounter == 1)
				{
					column+=(width - column)/2;
				}
				else if(pointsCounter == 2)
				{
					column = (width - 3*zoneSize);
				}
				else
				{
					column+=zoneSize;
				}
			}
		}
		if(pointsCounter < 3)
		{
			return ECalculationsStatus::FAIL;
		}
		return ECalculationsStatus::SUCCESS;
	}

	ECalculationsStatus
	GetDirection(int arrBright[zonesColumnCount][zonesRowCount], EDirection& direction)
	{
		const int32_t brightnessLowerBound = SettingsHandler::Get().GetBrightnessLowerBound();
		int lightZonesCount = 0;
		for (int j = 0; j < zonesColumnCount; ++j)
		{
			if(arrBright[0][j] < brightnessLowerBound)
			{
				++lightZonesCount;
			}
		}

		if(lightZonesCount == zonesColumnCount)
		{
			direction = EDirection::HEAD_TO_TAIL;
			return ECalculationsStatus::SUCCESS;
		}

		lightZonesCount = 0;
		for (int i = 0; i < zonesRowCount; ++i)
		{
			if(arrBright[i][0] < brightnessLowerBound)
			{
				++lightZonesCount;
			}
		}

		if(lightZonesCount == zonesColumnCount)
		{
			direction = EDirection::LEFT_TO_RIGHT;
			return ECalculationsStatus::SUCCESS;
		}

		direction = EDirection::HEAD_TO_TAIL;

		return ECalculationsStatus::SUCCESS;
	}

	ECalculationsStatus
	GetHorizonPointsInfo(ImageBuffer<unsigned char> &img, HorizonPointsInfo& pointsInfo)
	{
		const uint32_t width = img.GetWidth();
		const uint32_t height = img.GetHeight();
		int arrBright[zonesColumnCount][zonesRowCount]{};
		ECalculationsStatus res = PreAnalysis(img, arrBright);
		RET_ON_TRUE(res == ECalculationsStatus::FAIL, res);
		EDirection direction = EDirection::HEAD_TO_TAIL;

		res = GetDirection(arrBright, direction);
		RET_ON_TRUE(res == ECalculationsStatus::FAIL, res);

		const int32_t trashhold = SettingsHandler::Get().GetDetectionThreshold();
		res = FindHorizonPoints(img, direction, trashhold, pointsInfo.points);
		RET_ON_TRUE(res == ECalculationsStatus::FAIL, res);
		Point pointAConverted = ConvertCoordinates(width, height, pointsInfo.points[0]);
		Point pointBConverted = ConvertCoordinates(width, height, pointsInfo.points[1]);
		Point pointCConverted = ConvertCoordinates(width, height, pointsInfo.points[2]);
		res = GetAnglesByPoints(img, pointAConverted, pointBConverted, pointCConverted,
				pointsInfo.fi, pointsInfo.gamma);

		return res;
	}

}
