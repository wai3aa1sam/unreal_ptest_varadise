#include "ClipboardTestUiWdg.h"

#include "Clipboard.h"

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
	if (!tex)
	{
		log("no texture found");
		return;
	}

	OsUtil::CopyImageToClipboard(tex);
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

