// Fill out your copyright notice in the Description page of Project Settings.


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
