#include "ClipboardGameMode.h"

#include "ClipboardPlayerController.h"

AClipboardGameMode::AClipboardGameMode()
{
	PlayerControllerClass = AClipboardPlayerController::StaticClass();
}

void 
AClipboardGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}
