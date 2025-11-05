#include "LCGameMode.h"

#include "Player/LCPlayerState.h"

void ALCGameMode::StartPlay()
{
	Super::StartPlay();
}

APlayerController* ALCGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewPlayerController = Super::SpawnPlayerController(InRemoteRole, Options);
	return NewPlayerController;
}

UClass* ALCGameMode::GetDefaultPawnClassForController_Implementation(AController* Controller)
{
	ALCPlayerState* LCPlayerState = Controller->GetPlayerState<ALCPlayerState>();
	if (LCPlayerState && LCPlayerState->GetSelectedPawnClass())
	{
		return LCPlayerState->GetSelectedPawnClass();
	}
	
	return BackupPawnClass;
}

APawn* ALCGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	// TODO : 커스텀 PlayerStart 등을 활용해 Spawn 위치 지정 가능 
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
