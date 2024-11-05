#pragma once

#include <Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ListView.h>

#include "ClipboardTestUiWdg.generated.h"

UCLASS()
class PTEST_VARADISE_API UClipboardTestUiWdg : public UUserWidget 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget)) TObjectPtr<UButton> btnCopyImage_0	= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget)) TObjectPtr<UButton> btnCopyImage_1	= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget)) TObjectPtr<UButton> btnCopyImage_2	= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget)) TObjectPtr<UButton> btnCopyImage_3	= nullptr;

public:
	UClipboardTestUiWdg(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() void OnButtonPressed();

	int _Count = 0;
};