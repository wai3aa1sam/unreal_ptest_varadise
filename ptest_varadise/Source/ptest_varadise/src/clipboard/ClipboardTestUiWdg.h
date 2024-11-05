#pragma once

#include "common/upt_common.h"

#include <Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ListView.h>
#include <Components/CanvasPanel.h>
#include <Components/Slider.h>

#include "ClipboardTestUiWdg.generated.h"

#if 0
UCLASS()
class PTEST_VARADISE_API ClipboardUiWdgContext : public UObject
{
public:
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<UCanvasPanel>	cpnlImages				= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_resolution_x = nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_resolution_y = nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_offset_x		= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_offset_x		= nullptr;
};
#endif // 0

UCLASS()
class PTEST_VARADISE_API UClipboardTestUiWdg : public UUserWidget 
{
	GENERATED_BODY()
public:
	//UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<UCanvasPanel>		cpnlImages = nullptr;
	//UPROPERTY(EditAnywhere, Transient)						TArray<TObjectPtr<UButton> >	btnCopyImages;
	//UPROPERTY() TObjectPtr<ClipboardUiWdgContext> uiWdgCtx;

	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<UCanvasPanel>	cpnlImages				= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_resolution_x = nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_resolution_y = nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_offset_x		= nullptr;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))	TObjectPtr<USlider>			cpyImg_sld_offset_y		= nullptr;

public:
	UClipboardTestUiWdg(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() void OnButtonPressed();

	Rect2i getCopyRegion(uint32 w, uint32 h) const;

private:
	UButton* getFocusedImageButton();
};