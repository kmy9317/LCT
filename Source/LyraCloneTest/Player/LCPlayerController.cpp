// Fill out your copyright notice in the Description page of Project Settings.


#include "LCPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "LCPlayerState.h"
#include "AbilitySystem/LCAbilitySystemComponent.h"

ALCPlayerController::ALCPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ALCPlayerState* ALCPlayerController::GetLCPlayerState() const
{
	return CastChecked<ALCPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ULCAbilitySystemComponent* ALCPlayerController::GetLCAbilitySystemComponent() const
{
	const ALCPlayerState* LCPS = GetLCPlayerState();
	return (LCPS ? LCPS->GetLCAbilitySystemComponent() : nullptr);
}

void ALCPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

void ALCPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트에서 PlayerController가 늦게 복제된 경우
	if (GetWorld()->IsNetMode(NM_Client))
	{
		if (ALCPlayerState* LCPS = GetPlayerState<ALCPlayerState>())
		{
			if (ULCAbilitySystemComponent* LCASC = LCPS->GetLCAbilitySystemComponent())
			{
				LCASC->RefreshAbilityActorInfo();
				LCASC->TryActivateAbilitiesOnSpawn();
			}
		}
	}
}

void ALCPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ULCAbilitySystemComponent* CYASC = GetLCAbilitySystemComponent())
	{
		CYASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);

}
