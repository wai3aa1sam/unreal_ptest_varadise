#pragma once

#include "common/upt_common.h"
#include <GameFramework/GameModeBase.h>

#include "ClipboardGameMode.generated.h"


UCLASS()
class PTEST_VARADISE_API AClipboardGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AClipboardGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};