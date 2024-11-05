#include "ClipboardPlayerController.h"
#include "ClipboardTestUiWdg.h"

AClipboardPlayerController::AClipboardPlayerController()
{
	{
		static ConstructorHelpers::FClassFinder<UClipboardTestUiWdg> cls{TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/clipboard/ui/ClipboardTestUiWdg'")};
		uiWidgetClass = cls.Class;
	}
}

void 
AClipboardPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	createUiWidget();
}

void 
AClipboardPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!uiWidget)
	{
		createUiWidget();
	}
}

void 
AClipboardPlayerController::createUiWidget()
{
	if (!uiWidgetClass)
	{
		log("uiWidgetClass is null");
		return;
	}

	uiWidget = CreateWidget<UClipboardTestUiWdg>(this, uiWidgetClass);
	if (!uiWidget)
	{
		log("createUiWidget failed");
		return;
	}

	uiWidget->AddToViewport();
	//uiWidget->SetKeyboardFocus();
}
