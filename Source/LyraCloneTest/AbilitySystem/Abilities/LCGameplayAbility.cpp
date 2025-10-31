// Fill out your copyright notice in the Description page of Project Settings.


#include "LCGameplayAbility.h"

#include "AbilitySystemComponent.h"

ULCGameplayAbility::ULCGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = ELCAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULCGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void ULCGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == ELCAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

