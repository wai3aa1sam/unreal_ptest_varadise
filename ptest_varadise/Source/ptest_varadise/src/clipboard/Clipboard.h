#pragma once

#include "common/upt_common.h"
#include "utpImage.h"

#include <Windows.h>

class UTexture2D;

class PTEST_VARADISE_API Win32_Clipboard
{
public:
	void copy(const utpImage& image);

private:
	bool createBitmapFormat(BITMAPV5HEADER* out, const utpImage& image);
};

class PTEST_VARADISE_API OsUtil
{
public:
	static void CopyImageToClipboard(const utpImage& image);
	static void CopyImageToClipboard(const int32 InSizeX, const int32 InSizeY, const TArray<FColor>& InImageData);
	static void CopyImageToClipboard(UTexture2D* InImageTexture);
};

inline void utpImage_createTo(utpImage* out, UTexture2D* InImageTexture);

class Win32_ClipboardScope
{
public:
	Win32_ClipboardScope()
	{
		HWND win = nullptr;
		_isValidClipboard = OpenClipboard(win);
		if (!_isValidClipboard) 
		{
			log("OpenClipboard error");
		}
	}
	~Win32_ClipboardScope()
	{
		if (_isValidClipboard)
		{
			CloseClipboard();
		}
	}

private:
	bool _isValidClipboard = false;
};

class Win32_MemoryScope
{
public:
	Win32_MemoryScope(size_t byteSize, bool isAutoFree)
	{
		_hnd = GlobalAlloc(GHND, byteSize);
		_isAutoFree = isAutoFree;
	}

	~Win32_MemoryScope()
	{
		if (!_hnd)
			return;

		if (_hasLocked)
		{
			GlobalUnlock(_hnd);
		}

		if (_isAutoFree)
		{
			GlobalFree(_hnd);
		}
	}

	void setAutoFree(bool v) { _isAutoFree = v; }

public:
	template<class T> T* lock() 
	{  
		check(!_hasLocked);
		auto* p = static_cast<T*>(GlobalLock(_hnd));
		_hasLocked = !!p;
		setAutoFree(!p);
		return _hnd ? p : nullptr;
	}

	HGLOBAL handle() { return _hnd; }

private:
	HGLOBAL _hnd = INVALID_HANDLE_VALUE;
	bool _hasLocked		= false;
	bool _isAutoFree	= false;
};

#if PLATFORM_WINDOWS
	using Clipboard = Win32_Clipboard;
#else
	#error "only support windows"
#endif // PLATFORM_WINDOWS

