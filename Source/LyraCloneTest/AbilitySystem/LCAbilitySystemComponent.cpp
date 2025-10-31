// Fill out your copyright notice in the Description page of Project Settings.


#include "LCAbilitySystemComponent.h"

#include "Abilities/LCGameplayAbility.h"

ULCAbilitySystemComponent::ULCAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputStartedSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void ULCAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);
	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// TODO : AnimInstance 초기화

		TryActivateAbilitiesOnSpawn();
	}
}

void ULCAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const ULCGameplayAbility* LCAbilityCDO = Cast<ULCGameplayAbility>(AbilitySpec.Ability))
		{
			LCAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}

void ULCAbilitySystemComponent::AbilitySpecInputStarted(FGameplayAbilitySpec& Spec)
{
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, Spec.Handle, OriginalPredictionKey);
	}
}

void ULCAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
}

void ULCAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}

void ULCAbilitySystemComponent::AbilityInputTagStarted(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputStartedSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void ULCAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void ULCAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void ULCAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	// 이번 프레임에 활성화할 어빌리티들을 저장할 정적 배열
	// 정적 변수로 선언하여 매 프레임마다 메모리 할당을 피함
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const ULCGameplayAbility* LCAbilityCDO = Cast<ULCGameplayAbility>(AbilitySpec->Ability);
				// 지속 입력으로 활성화되는 어빌리티들 처리 (WhileInputActive 정책)
				if (LCAbilityCDO && LCAbilityCDO->GetActivationPolicy() == ELCAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	// 입력 시작 이벤트 처리 (Started 이벤트 → GameCustom1 이벤트 발생)
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputStartedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				// 활성화된 어빌리티만 Started 이벤트를 전달
				if (AbilitySpec->IsActive())
				{
					// GameCustom1 Replicated 이벤트 발생 
					AbilitySpecInputStarted(*AbilitySpec);
				}
			}
		}
	}

	// 입력 pressed 이벤트 처리 (Pressed 이벤트 → 활성화 vs 입력 이벤트 분기)
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const ULCGameplayAbility* LCAbilityCDO = Cast<ULCGameplayAbility>(AbilitySpec->Ability);

					if (LCAbilityCDO && LCAbilityCDO->GetActivationPolicy() == ELCAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// 큐잉된 모든 어빌리티들 일괄 활성화
	// Held와 Pressed 입력을 한 번에 처리하여 중복 활성화 방지
	// Held 입력이 어빌리티를 활성화한 후 Pressed 입력이 같은 어빌리티에 입력 이벤트를 보내는 것을 방지
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// Release 입력 이벤트 처리
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				// Ability Spec의 InputPressed 플래그 해제
				AbilitySpec->InputPressed = false;

				// 활성화된 어빌리티만 Release 이벤트를 전달
				if (AbilitySpec->IsActive())
				{
					// InputReleased Replicated 이벤트 발생 
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// 캐시된 입력들 초기화
	// InputHeldSpecHandles는 입력이 Released될 때까지 유지
	InputStartedSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void ULCAbilitySystemComponent::ClearAbilityInput()
{
	InputStartedSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}


