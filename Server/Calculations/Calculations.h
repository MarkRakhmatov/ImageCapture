#pragma once
#include "ImageProcessing/ImageProcessor.h"

enum class ECalculationsStatus
{
	SUCCESS,
	FAIL
};

ECalculationsStatus GetAngles(ImageBuffer<unsigned char>& img, int32_t& x, int32_t& y);
