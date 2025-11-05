#include "LCGameInstance.h"

#include "Components/GameFrameworkComponentManager.h"
#include "LyraCloneTest/LCGameplayTags.h"

void ULCGameInstance::Init()
{
	Super::Init();

	// InitState를 GFCM에 등록
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(LCGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(LCGameplayTags::InitState_DataAvailable, false, LCGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(LCGameplayTags::InitState_DataInitialized, false, LCGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(LCGameplayTags::InitState_GameplayReady, false, LCGameplayTags::InitState_DataInitialized);
	}
}

void ULCGameInstance::StartMatch()
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer || GetWorld()->GetNetMode() == ENetMode::NM_ListenServer)
	{
		LoadLevelAndListen(GameLevel);
	}
}

void ULCGameInstance::LoadLevelAndListen(TSoftObjectPtr<UWorld> Level)
{
	const FName LevelURL = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));

	if (LevelURL != "")
	{
		GetWorld()->ServerTravel(LevelURL.ToString() + "?listen");
	}
}
