#include "OnCaptureSrv.h"
#include "SwapChain.h"
#include "Calculations.h"
#include "JpegHelper.h"
#include "SettingsHandler.h"

namespace ServerSide
{
	using namespace Communication;

	OnCaptureSrv::OnCaptureSrv(std::unique_ptr<IImageSource<unsigned char>>& imageSource)
	: mImageSource(imageSource)
	{
	}

	EConnectionStatus ServerSide::OnCaptureSrv::Handle(Socket& sock)
	{
		do
		{
			std::string fileName("Images/img_");
			auto decomprBuffer = mImageSource->GetImage();
			EConnectionStatus response = EConnectionStatus::FAIL;
			auto width = decomprBuffer.GetWidth();
			auto height = decomprBuffer.GetHeight();
			if(!width || !height)
			{
				std::cout<<"Failed to get buffer!"<<std::endl;
				sock.SendData(&response);
				break;
			}
			ImageBuffer<unsigned char> resultBuffer(width, height, decomprBuffer.GetPixelType());
			SwapChain<ImageBuffer<unsigned char>> chain(&decomprBuffer, &resultBuffer);
			auto& kernels = SettingsHandler::Get().GetKernels();
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

			auto compressedBuffer = JpegHelper::Compress(*chain.GetActive());
			int32_t x{};
			int32_t y{};
			ECalculationsStatus status = GetAngles(*chain.GetActive(), x, y);
			if(status == ECalculationsStatus::FAIL)
			{
				break;
			}
			response = EConnectionStatus::SUCCESS;
			bool res = sock.SendData(&response);
			if(!res)
			{
				break;
			}
			res = sock.SendData(&x, &y);
			if(!res)
			{
				break;
			}

			JpegHelper::WriteBufferToFile(compressedBuffer, fileName);

			return EConnectionStatus::SUCCESS;
		}
		while(false);

		std::cout<<"Failed to send response!"<<std::endl;
		return EConnectionStatus::FAIL;
	}
}
