#pragma once
#include "ImageProcessing/ImageProcessor.h"

namespace ServerSide
{
	enum class ECalculationsStatus
	{
		SUCCESS,
		FAIL
	};


	struct Point
	{
		float x;
		float y;
	};

	struct HorizonPointsInfo
	{
		std::vector<Point> points;
		float fi;
		float gamma;

	};

	ECalculationsStatus GetHorizonPointsInfo(ImageBuffer<unsigned char>& img, HorizonPointsInfo& pointsInfo);
}
