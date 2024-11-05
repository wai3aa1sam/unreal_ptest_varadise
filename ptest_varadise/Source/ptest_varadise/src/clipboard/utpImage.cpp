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
	
	_data.SetNum(n);
	::memcpy(_data.GetData(), data, n);
}

void 
utpImage::create(int width, int height, ColorType format)
{
	_width	= width;
	_height = height;
	_format = format;
	_data.SetNum(width * height * ColorTypeUtil::getByteSize(format));
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

	return true;
}
