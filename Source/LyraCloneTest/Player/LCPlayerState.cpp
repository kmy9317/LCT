#include "LCPlayerState.h"

#include "LCLogChannels.h"
#include "LCPlayerController.h"
#include "AbilitySystem/LCAbilitySet.h"
#include "AbilitySystem/LCAbilitySystemComponent.h"
#include "Character/LCPawnData.h"
#include "Character/LCPawnExtensionComponent.h"
#include "Net/UnrealNetwork.h"

ALCPlayerState::ALCPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULCAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	
	SetNetUpdateFrequency(100.f);
}

void ALCPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
}

void ALCPlayerState::Reset()
{
	Super::Reset();
}

void ALCPlayerState::ClientInitialize(AController* C)
{
	// PlayerController에 PlayerState 설정 시점에 호출되는 함수(OnRep_PlayerState)
	Super::ClientInitialize(C);
	if (ULCPawnExtensionComponent* PawnExtensionComponent = ULCPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		UE_LOG(LogLC, Warning, TEXT("client init"));
		PawnExtensionComponent->CheckDefaultInitialization();
	}
}

void ALCPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void ALCPlayerState::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
	case ELCPlayerConnectionType::Player:
	case ELCPlayerConnectionType::InactivePlayer:
		//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
		// (e.g., for long running servers where they might build up if lots of players cycle through)
		bDestroyDeactivatedPlayerState = true;
		break;
	default:
		bDestroyDeactivatedPlayerState = true;
		break;
	}
	
	SetPlayerConnectionType(ELCPlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void ALCPlayerState::OnReactivated()
{
	if (GetPlayerConnectionType() == ELCPlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(ELCPlayerConnectionType::Player);
	}
}

ALCPlayerController* ALCPlayerState::GetLCPlayerController() const
{
	return Cast<ALCPlayerController>(GetOwner());
}

ULCAbilitySystemComponent* ALCPlayerState::GetLCAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* ALCPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALCPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	// TODO : PawnData등의 로드 완료 Delegate 바인딩
}

void ALCPlayerState::SetPawnData(const ULCPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLC, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// PawnData의 AbilitySet을 순회하며, ASC에 Ability를 할당
	// - 이 과정에서 ASC의 ActivatableAbilities에 추가됨
	for (const ULCAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	ForceNetUpdate();
}

void ALCPlayerState::SetPlayerConnectionType(ELCPlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}




