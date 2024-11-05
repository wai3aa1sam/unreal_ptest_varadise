#pragma once

#include "common/upt_common.h"

UENUM()
enum class ColorType
{
	None = 0,
	RGBA,
};
ENUM_CLASS_FLAGS(ColorType)

struct ColorTypeUtil
{
	static int getByteSize(ColorType);
};

class utpImage
{
public:
	void create(int width, int height, const uint8* data, size_t n, ColorType format);
	void create(int width, int height, const TArray<uint8>& data, ColorType format);
	void create(int width, int height, const TArray<FColor>& data);
	void create(int width, int height, ColorType format);

	bool serializeTo_Png(TArray<uint8>& data, ColorType format) const;

public:
	int				width()			const { return _width; }
	int				height()		const { return _height; }

	bool			isValid()		const { return _width > 0 && _height > 0; }

	int				bytesPerRow()	const { return width() * ColorTypeUtil::getByteSize(format()); }

	ColorType		format()		const { return _format; }

			uint8*	data()				  { return _data.GetData(); }
	const	uint8*	data()			const { return _data.GetData(); }

private:
	int				_width	= 0;
	int				_height = 0;
	ColorType		_format = ColorType::RGBA;
	TArray<uint8>	_data;
};
