#pragma once

#include "common/upt_common.h"
#include "GameFramework/PlayerController.h"

#include "ClipboardPlayerController.generated.h"

class UClipboardTestUiWdg;

UCLASS()
class PTEST_VARADISE_API AClipboardPlayerController : public APlayerController 
{
	GENERATED_BODY()
public:
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	TObjectPtr<UInputAction> InputMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	TObjectPtr<UInputAction> InputLook;
	UPROPERTY(EditAnywhere, Category="Input")		TSoftObjectPtr<UInputMappingContext> InputMapping;*/

	UPROPERTY(EditAnywhere)				TSubclassOf<UClipboardTestUiWdg>	uiWidgetClass;
	UPROPERTY(EditAnywhere, Transient)	TObjectPtr<UClipboardTestUiWdg>		uiWidget = nullptr;

public:
	AClipboardPlayerController();

	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	void createUiWidget();
};