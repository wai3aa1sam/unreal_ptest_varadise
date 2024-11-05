#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#define UTP_CPU_ENDIAN_LITTLE PLATFORM_LITTLE_ENDIAN

inline
void log(const FString& msg)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red, msg);
	}
}

template<class T>
struct Rect2
{
	union
	{
		struct { T x, y, w, h; };
		T data[4];
	};

	Rect2() = default;
	Rect2(T x_, T y_, T w_, T h_) { set(x_, y_, w_, h_); }

	void set(T x_, T y_, T w_, T h_)
	{
		x = x_;
		y = y_;
		w = w_;
		h = h_;
	}
};

using Rect2i = Rect2<int32>;