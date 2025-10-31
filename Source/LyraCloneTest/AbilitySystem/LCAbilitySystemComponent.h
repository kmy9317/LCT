// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LCAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRACLONETEST_API ULCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULCAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	void TryActivateAbilitiesOnSpawn();

	void AbilityInputTagStarted(const FGameplayTag& InputTag);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:
	virtual void AbilitySpecInputStarted(FGameplayAbilitySpec& Spec);
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:
	// Handles to abilities that had their input started this frame.
	TArray<FGameplayAbilitySpecHandle> InputStartedSpecHandles;
	
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
