#include "Clipboard.h"

void 
Win32_Clipboard::copy(const utpImage& image)
{
	if (!image.isValid())
		return;

	Win32_ClipboardScope	clipboardScope;
	Win32_MemoryScope		memScope {sizeof(BITMAPV5HEADER) + image.bytesPerRow() * image.height(), false};		// pad row size to a multiple of 4 bytes

	bool isSuccess = createBitmapFormat(memScope.lock<BITMAPV5HEADER>(), image);
	memScope.setAutoFree(!isSuccess);

	EmptyClipboard();
	SetClipboardData(CF_DIBV5, memScope.handle());

	// png
	#if 0
	{
		auto clipFormat = RegisterClipboardFormat(TEXT("PNG"));

		TArray<uint8> png;
		isSuccess = image.serializeTo_Png(png, ColorType::RGBA);
		if (isSuccess)
		{
			Win32_MemoryScope pngMemScope {StaticCast<size_t>(png.Num()), false};		// pad row size to a multiple of 4 bytes
			auto* p = pngMemScope.lock<uint8*>();
			if (p)
			{
				memcpy(p, png.GetData(), png.Num());
				SetClipboardData(clipFormat, pngMemScope.handle());
			}
		}
	}
	#endif // 0
}

bool 
Win32_Clipboard::createBitmapFormat(BITMAPV5HEADER* out, const utpImage& image)
{
	using u8	= uint8;
	using u32	= uint32;

	check(out);

	auto width	= image.width();
	auto height = image.height();

	if (width <= 0 || height <= 0)
		return false;

	if (image.format() != ColorType::RGBA)
	{
		log("bitmap only support RGBA");
		return false;
	}

	/*auto rMask		= 0x00ff0000;
	auto gMask		= 0x0000ff00;
	auto bMask		= 0x000000ff;
	auto aMask		= 0xff000000;*/

	auto rMask		= 0x000000ff;
	auto gMask		= 0x0000ff00;
	auto bMask		= 0x00ff0000;
	auto aMask		= 0xff000000;

	auto rShift		= 0;
	auto gShift		= 8;
	auto bShift		= 16;
	auto aShift		= 24;

	auto padding		= 0;	// pad row size to a multiple of 4 bytes
	auto byteSize		= ColorTypeUtil::getByteSize(image.format());
	auto bytesPerRow	= width * byteSize + padding;

	auto& o = *out;
	o.bV5Size			= sizeof(BITMAPV5HEADER);
	o.bV5Width			= width;
	o.bV5Height			= height;
	o.bV5Planes			= 1;
	o.bV5BitCount		= static_cast<WORD>(byteSize * 8);
	o.bV5Compression	= BI_RGB;
	o.bV5SizeImage		= bytesPerRow * height;

	// only works for BI_BITFIELDS
	/*o.bV5RedMask		= rMask;
	o.bV5GreenMask		= gMask;
	o.bV5BlueMask		= bMask;
	o.bV5AlphaMask		= aMask;*/

	o.bV5CSType			= LCS_WINDOWS_COLOR_SPACE;
	o.bV5Intent			= LCS_GM_GRAPHICS;
	o.bV5ClrUsed		= 0;

	const u8*	src = (image.data());
	u8*			dst = ((reinterpret_cast<u8*>(&o) + o.bV5Size) + (height - 1) * bytesPerRow);

	for (int y = height - 1; y >= 0; --y)
	{
		auto* srcRow = reinterpret_cast<const u32*>(src);
		auto* dstRow = reinterpret_cast<u32*>(dst);

		for (int x = 0; x < width; ++x)
		{
			// pre mulitply alpha
			u32 a = ((*srcRow & aMask) >> aShift);

			u32 r = ((*srcRow & rMask) >> rShift) * a / 255;
			u32 g = ((*srcRow & gMask) >> gShift) * a / 255;
			u32 b = ((*srcRow & bMask) >> bShift) * a / 255;

			#if 0
			#if UTP_CPU_ENDIAN_LITTLE
			*dstRow = (b << 0) | (g << 8) | (r << 16) | (a << 24);
			#else
			*dstRow = (r << 0) | (g << 8) | (b << 16) | (a << 24);
			#endif // RDS_CPU_ENDIAN_LITTLE
			#else
			*dstRow = (r << 0) | (g << 8) | (b << 16) | (a << 24);		// since FColor is not raw data
			#endif // 0

			srcRow++;
			dstRow++;
		}

		src += bytesPerRow;
		dst -= bytesPerRow;
	}

	return true;
}

void 
OsUtil::CopyImageToClipboard(const utpImage& image)
{
	Clipboard clipboard;
	clipboard.copy(image);
}

void 
OsUtil::CopyImageToClipboard(const int32 InSizeX, const int32 InSizeY, const TArray<FColor>& InImageData)
{
	utpImage image;
	image.create(InSizeX, InSizeY, InImageData);
	CopyImageToClipboard(image);
}

void 
OsUtil::CopyImageToClipboard(UTexture2D* InImageTexture)
{
	if (!InImageTexture)
		return;

	utpImage image;
	utpImage_createTo(&image, InImageTexture);
	
	CopyImageToClipboard(image);
}

void 
utpImage_createTo(utpImage* out, UTexture2D* InImageTexture)
{
	check(out && InImageTexture);

	auto width		= InImageTexture->GetSizeX();
	auto height		= InImageTexture->GetSizeY();

	utpImage tmp;
	tmp.create(width, height, ColorType::RGBA);

	if (!InImageTexture->GetPlatformData()->Mips.IsEmpty())
	{
		auto& bulkData = InImageTexture->GetPlatformData()->Mips[0].BulkData;

		tmp.resizeBuffer(bulkData.GetBulkDataSize());
		void* buf = tmp.data();
		bulkData.GetCopy(&buf);
	}

	*out = std::move(tmp);
}


#if 0

#undef UpdateResource
class ReadTextureScope
{
public:
	ReadTextureScope(utpImage& dst, UTexture2D* tex, int mipLevel = 0) 
	{ 
		if (!tex)
			return;

		auto& mips = tex->GetPlatformData()->Mips;
		if (mips.IsEmpty())
		{
			log("texture has no mips");
			return;
		}

		_tex		= tex;
		_mipLevel	= mipLevel;

		_copyTo(dst);
	}

	~ReadTextureScope()
	{
		if (_tex && mipmap().BulkData.IsLocked())
		{
			/*mipmap().BulkData.Unlock();
			_tex->CompressionSettings	= _oldCompressionSettings;
			_tex->MipGenSettings		= _oldMipGenSettings;
			_tex->SRGB 					= _oldSRGB;
			_tex->UpdateResource();*/

			_tex = nullptr;
		}
	}

	//template<class T> T getReadData() { return StaticCast<T>(_hnd->LockReadOnly()); }

private:
	void _copyTo(utpImage& dst)
	{
		/*
		* reference:
		* ~ https://isaratech.com/ue4-reading-the-pixels-from-a-utexture2d/
		*/

		_oldCompressionSettings	= _tex->CompressionSettings;
		_oldMipGenSettings		= _tex->MipGenSettings;
		_oldSRGB				= _tex->SRGB;

		/*_tex->CompressionSettings		= TextureCompressionSettings::TC_VectorDisplacementmap;
		_tex->MipGenSettings			= TextureMipGenSettings::TMGS_NoMipmaps;
		_tex->SRGB						= false;
		_tex->UpdateResource();*/

		#if 0
		const FColor* formatedImageData = StaticCast<const FColor*>(mipmap().BulkData.LockReadOnly()); // lock.getReadData<const FColor*>();
		if (!formatedImageData)
		{
			log("utpImage_createTo() no bulkData");
			return;
		}

		auto width		= _tex->GetSizeX();
		auto height		= _tex->GetSizeY();

		#if 0
		for (int32 Y = 0; Y < height; Y++)
		{
			for(int32 X = 0; X < width; X++)
			{
				*dst = formatedImageData[Y * width + X];
				dst++;
			}
		}
		#else
		memcpy(dst, formatedImageData, width * height * sizeof(FColor));
		#endif // 0

		#else

		void* buf = nullptr;
		mipmap().BulkData.GetCopy(&buf);
		if (buf)
		{
			memcpy(dst.data(), buf, mipmap().BulkData.GetBulkDataSize());
		}

		#endif // 0


	}

	FTexture2DMipMap& mipmap() { return _tex->GetPlatformData()->Mips[_mipLevel]; }

private:
	UTexture2D*	_tex  = nullptr;

	int _mipLevel = 0;
	TextureCompressionSettings	_oldCompressionSettings; 
	TextureMipGenSettings		_oldMipGenSettings; 
	bool						_oldSRGB;
};

#endif // 0
