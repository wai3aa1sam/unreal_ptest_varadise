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