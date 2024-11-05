#include "utpImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <external/stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <external/stb/stb_image_write.h>

int 
ColorTypeUtil::getByteSize(ColorType v)
{
	switch (v)
	{
		case ColorType::RGBA: { return 4; } break;
		default: { check(false); return 0; }break;
	}
}

void 
utpImage::create(int width, int height, const uint8* data, size_t n, ColorType format)
{
	create(width, height, format);
	
	resizeBuffer(n);
	::memcpy(_data.GetData(), data, n);
}

void 
utpImage::create(int width, int height, ColorType format)
{
	_width	= width;
	_height = height;
	_format = format;
	resizeBuffer(width * height * ColorTypeUtil::getByteSize(format));
}

void 
utpImage::create(int width, int height, const TArray<uint8>& data, ColorType format)
{
	create(width, height, data.GetData(), data.Num(), format);
}

void 
utpImage::create(int width, int height, const TArray<FColor>& data)
{
	create(width, height, reinterpret_cast<const uint8*>(data.GetData()), data.Num() * sizeof(FColor), ColorType::RGBA);
}

void 
utpImage::resizeBuffer(size_t nBytes)
{
	_data.Empty();
	_data.SetNum(nBytes);
}

bool 
utpImage::serializeTo_Png(TArray<uint8>& data, ColorType format) const
{
	int width = this->width();

	if (width <= 0 || height() <= 0)
		return false;

	auto pixelByteSize = ColorTypeUtil::getByteSize(format);
	auto strideInBytes = pixelByteSize * width;

	check(format == _format);
	{
		int len;
		unsigned char *png = stbi_write_png_to_mem((const unsigned char *) _data.GetData(), strideInBytes, width, height(), pixelByteSize, &len);
		if (png == NULL)
		{
			log("serialze to png failed");
			return false;
		}

		data.SetNum(len);
		memcpy(data.GetData(), png, len);

		STBIW_FREE(png);
	}
	
	TArray<uint8> a;
	a.SetNum(width * height() * pixelByteSize);

	auto* src		= reinterpret_cast<uint32*>(a.GetData());
	const auto* p	= reinterpret_cast<const FColor*>(_data.GetData());

	{
		FStringFormatOrderedArguments Args;
		Args.Add(p->R);
		Args.Add(p->G);
		Args.Add(p->B);
		Args.Add(p->A);
		FString str = FString::Format(TEXT("RGBA: {0} {1} {2} {3}"), Args);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
	}

	for (size_t i = 0; i < width * height(); i++)
	{
		//e = FColor::Red;
		*src = (*p).ToPackedBGRA();
		//*src = FColor::Red.ToPackedRGBA();
		
		src++;
		++p;
	}

	const char* filename = "W:/game_develop/repo/unreal/programming_test/unreal_ptest_varadise/ptest_varadise/out.png";
	stbi_write_png(filename, width, height(), pixelByteSize, reinterpret_cast<const void*>(a.GetData()), strideInBytes);

	return true;
}
