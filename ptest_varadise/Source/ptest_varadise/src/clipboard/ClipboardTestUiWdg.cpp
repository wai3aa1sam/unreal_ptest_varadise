#include "ClipboardTestUiWdg.h"
#include "Clipboard.h"
#include "Components/CanvasPanelSlot.h"

UClipboardTestUiWdg::UClipboardTestUiWdg(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void
UClipboardTestUiWdg::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	auto children = cpnlImages->GetAllChildren();
	for (auto* e : children)
	{
		auto* btn = Cast<UButton>(e);
		if (btn)
			btn->OnPressed.AddDynamic(this, &ThisClass::OnButtonPressed);
	}
}

void 
UClipboardTestUiWdg::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) 
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void
UClipboardTestUiWdg::OnButtonPressed() 
{
	UButton* btn = Cast<UButton>(getFocusedImageButton());
	
	if (!btn)
	{
		log("no button found");
		return;
	}

	auto* tex = Cast<UTexture2D>(btn->WidgetStyle.Normal.GetResourceObject());
	if (tex)
	{
		OsUtil::CopyImageToClipboard(tex, getCopyRegion(tex->GetSizeX(), tex->GetSizeY()));
	}
	else
	{
		auto size = 256;
		TArray<FColor> arr;
		arr.SetNum(size * size);
		for (auto& e : arr)
		{
			e = btn->BackgroundColor.ToFColor(true);
		}
		OsUtil::CopyImageToClipboard(size, size, arr, getCopyRegion(size, size));
	}
}

UButton* 
UClipboardTestUiWdg::getFocusedImageButton()
{
	/*TSharedPtr<SWidget> FocusedSlateWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
	if (!cpnlImages || !FocusedSlateWidget.IsValid())
	{
		log("No focused Slate widget found");
		return nullptr;
	}*/

	auto children = cpnlImages->GetAllChildren();
	for (UWidget* e : children)
	{
		auto* btn = Cast<UButton>(e);
		if (btn && btn->IsPressed())
			return btn;
	}

	return nullptr;
}

Rect2i 
UClipboardTestUiWdg::getCopyRegion(uint32 w, uint32 h) const
{
	Rect2i o;
	o.x = cpyImg_sld_offset_x->GetValue() * w;
	o.y = cpyImg_sld_offset_y->GetValue() * h;
	o.w = cpyImg_sld_resolution_x->GetValue() * w;
	o.h = cpyImg_sld_resolution_y->GetValue() * h;

	#if UE_BUILD_DEVELOPMENT
	{
		FStringFormatOrderedArguments Args;
		Args.Add(o.x);
		Args.Add(o.y);
		Args.Add(o.w);
		Args.Add(o.h);
		FString str = FString::Format(TEXT("Rect2i {0} {1} {2} {3}"), Args);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
	}
	#endif // UE_BUILD_DEVELOPMENT

	return o;
}
