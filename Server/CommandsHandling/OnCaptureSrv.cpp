#include "OnCaptureSrv.h"
#include "SwapChain.h"
#include "Calculations.h"
#include "JpegHelper.h"
#include "SettingsHandler.h"
#include "CommunicationUtils.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>

namespace ServerSide
{
	using namespace Communication;

	OnCaptureSrv::OnCaptureSrv(std::unique_ptr<IImageSource<unsigned char>>& imageSource)
	: mImageSource(imageSource)
	{
	}

	long long int GetCurrentTime()
	{
	  auto now = std::chrono::system_clock::now();
	  return std::chrono::time_point_cast<std::chrono::milliseconds>(now)
	  	  .time_since_epoch().count();
	}

	void MarkPoint(ImageBuffer<unsigned char> &img, uint32_t column, uint32_t row)
	{
		uint32_t width = img.GetWidth();
		uint32_t height = img.GetHeight();
		std::cout << "width " << width <<std::endl;
		std::cout << "height " << height <<std::endl;
		std::cout << "column " << column <<std::endl;
		std::cout << "row " << row <<std::endl;
		unsigned char* pixel;
		const int32_t zoneSize = SettingsHandler::Get().GetPointDetectionWindfowSize();
		for(uint32_t i = row; i < row + zoneSize && i < height; ++i)
		{
			for(uint32_t j = column; j < column + zoneSize && j < width; ++j)
			{
				pixel = img.GetElement(i, j);
				pixel[0] = 255;
				pixel[1] = 0;
				pixel[2] = 0;
			}
		}
	}

	void MarkHorizonPoints(ImageBuffer<unsigned char>& buffer, const std::vector<Point>& points)
	{
		for(Point point : points)
		{
			MarkPoint(buffer, point.x, point.y);
			std::cout<< "Point x = "<< point.x << " y = " << point.y << std::endl;
		}
	}
	bool SobelPreproc(SwapChain<ImageBuffer<unsigned char>>& chain)
	{
		auto& settingsHandler = SettingsHandler::Get();
		auto& kernels = settingsHandler.GetSobelKernels();
		ConvOperatorHandler<unsigned char> convHandler
					(
							kernels[0], kernels[1]
					);
		auto& originalBuff = *chain.GetActive();
			chain.Swap();
			auto& processedBuff = *chain.GetActive();
			ImageProcessor::Convolution(
					originalBuff,
					processedBuff,
					convHandler);
		return true;
	}
	bool PrewittPreproc(SwapChain<ImageBuffer<unsigned char>>& chain)
	{
		auto& settingsHandler = SettingsHandler::Get();
		auto& kernels = settingsHandler.GetPrewittKernels();
		ConvOperatorHandler<unsigned char> convHandler
					(
							kernels[0], kernels[1]
					);
		auto& originalBuff = *chain.GetActive();
		chain.Swap();
		auto& processedBuff = *chain.GetActive();
		ImageProcessor::Convolution(
				originalBuff,
				processedBuff,
				convHandler);

		return true;
	}
	bool CustomPreproc(SwapChain<ImageBuffer<unsigned char>>& chain)
	{
		auto& settingsHandler = SettingsHandler::Get();
		auto& kernels = settingsHandler.GetCustomKernels();
		for(size_t i = 0; i < kernels.size(); ++i)
		{
			ConvHandler<unsigned char> convHandler
			(
					kernels[i]
			);
			auto& originalBuff = *chain.GetActive();
			chain.Swap();
			auto& processedBuff = *chain.GetActive();
			ImageProcessor::Convolution(
					originalBuff,
					processedBuff,
					convHandler);
		}
		return true;
	}
	bool PreprocessImage(ImageBuffer<unsigned char>& originalImgBuffer, ImageBuffer<unsigned char>& resultBuffer)
	{
		auto& settingsHandler = SettingsHandler::Get();

		float r{};
		float g{};
		float b{};
		settingsHandler.GetRgb(r, g, b);
		auto decomprBuffer = RgbToGrayscale<unsigned char>(originalImgBuffer, r, g, b);
		auto width = decomprBuffer.GetWidth();
		auto height = decomprBuffer.GetHeight();
		if(!width || !height)
		{
			std::cout<<"Failed to get buffer!"<<std::endl;
			return false;
		}
		ImageBuffer<unsigned char> secondBuffer(width, height, decomprBuffer.GetPixelType());
		SwapChain<ImageBuffer<unsigned char>> chain(&decomprBuffer, &secondBuffer);
		std::string preprocName = settingsHandler.GetPreprocessingAlgoName();
		bool res= false;
		if(preprocName == "Sobel")
		{
			res = SobelPreproc(chain);
		}
		else if(preprocName == "Prewitt")
		{
			res = PrewittPreproc(chain);
		}
		else
		{
			res = CustomPreproc(chain);
		}

		resultBuffer = std::move(*chain.GetActive());

		return res;
	}
	EConnectionStatus ServerSide::OnCaptureSrv::Handle(Socket& sock)
	{
		EConnectionStatus response = EConnectionStatus::FAIL;
		bool res = false;
		do
		{
			std::string fileName("Images/img_");
			if(!mImageSource)
			{
				std::cout<<"Invalid image source!"<<std::endl;
				break;
			}
			auto image = mImageSource->GetImage();
			auto originalImgBuffer = JpegHelper::Decompress(image.data(), image.size(), TJPF::TJPF_RGB);

			auto time = GetCurrentTime();
			ImageBuffer<unsigned char> preprocessedImgBuffer;
			res = PreprocessImage(originalImgBuffer, preprocessedImgBuffer);
			auto compressedBuffer = JpegHelper::Compress(preprocessedImgBuffer);
			JpegHelper::WriteBufferToFile(compressedBuffer, fileName + std::to_string(time) + "_processed.jpeg");

			HorizonPointsInfo pointsInfo;
			ECalculationsStatus status = GetHorizonPointsInfo(preprocessedImgBuffer, pointsInfo);

			MarkHorizonPoints(originalImgBuffer, pointsInfo.points);

			auto compressedMarkedImage = JpegHelper::Compress(originalImgBuffer);
			JpegHelper::WriteBufferToFile(compressedMarkedImage, fileName + std::to_string(time) + "_points.jpeg");

			if(status == ECalculationsStatus::FAIL)
			{
				break;
			}
			response = EConnectionStatus::SUCCESS;
			res = sock.SendData(&response);
			RET_ON_FALSE(res, EConnectionStatus::FAIL);

			std::string fiStr = std::to_string(pointsInfo.fi);
			res = Communication::SendString(sock, fiStr);
			RET_ON_FALSE(res, EConnectionStatus::FAIL);

			std::string gammsStr = std::to_string(pointsInfo.gamma);
			res = Communication::SendString(sock, gammsStr);
			RET_ON_FALSE(res, EConnectionStatus::FAIL);

			return EConnectionStatus::SUCCESS;
		}
		while(false);

		sock.SendData(&response);
		std::cout<<"Failed to send response!"<<std::endl;
		return EConnectionStatus::FAIL;
	}
}
