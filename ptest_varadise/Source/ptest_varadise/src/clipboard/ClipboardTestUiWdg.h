#pragma once

#include <Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ListView.h>
#include <Components/CanvasPanel.h>

#include "ClipboardTestUiWdg.generated.h"

UCLASS()
class PTEST_VARADISE_API UClipboardTestUiWdg : public UUserWidget 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<UCanvasPanel>		cpnlImages = nullptr;
	UPROPERTY(EditAnywhere, Transient)						TArray<TObjectPtr<UButton> >	btnCopyImages;

public:
	UClipboardTestUiWdg(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() void OnButtonPressed();

private:
	UButton* getFocusedImageButton();
};