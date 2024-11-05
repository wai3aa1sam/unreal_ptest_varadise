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

	btnCopyImage_0->OnPressed.AddDynamic(this, &ThisClass::OnButtonPressed);
	btnCopyImage_1->OnPressed.AddDynamic(this, &ThisClass::OnButtonPressed);
	btnCopyImage_2->OnPressed.AddDynamic(this, &ThisClass::OnButtonPressed);
	btnCopyImage_3->OnPressed.AddDynamic(this, &ThisClass::OnButtonPressed);

	
}


void 
UClipboardTestUiWdg::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) 
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	_Count++;

	//FString str = ursFormat("TextBlock {}", _Count);
	//MyTextBlock->SetText(FText::FromString(str));
}

void
UClipboardTestUiWdg::OnButtonPressed() 
{
	FSlateApplication::Get().GetUserFocusedWidget(0);

	UButton* btn = btnCopyImage_0;
	
	if (!btn)
		return;

	auto* tex = Cast<UTexture2D>(btn->WidgetStyle.Normal.GetResourceObject());
	if (!tex)
	{
		log("no texture found");
		return;
	}

	OsUtil::CopyImageToClipboard(tex);
}


UFUNCTION(BlueprintCallable)
static UWidget* GetFocusedUMGWidget()
{
	TSharedPtr<SWidget> FocusedSlateWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
	if (!FocusedSlateWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No focused Slate widget found"));
		return nullptr;
	}
	for (TObjectIterator<UWidget> Itr; Itr; ++Itr)
	{
		UWidget* CandidateUMGWidget = *Itr;
		if (CandidateUMGWidget->GetCachedWidget() == FocusedSlateWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Focused UMG widget found: %s"), *CandidateUMGWidget->GetName());
			return CandidateUMGWidget;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No focused UMG widget found"));
	return nullptr;
}