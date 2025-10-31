// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LCGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ELCAbilityActivationPolicy : uint8
{
	/** Input이 Trigger 되었을 경우 (Presssed/Released) */
	OnInputTriggered,
	/** Input이 Held되어 있을 경우 */
	WhileInputActive,
	/** avatar가 생성되었을 경우, 바로 할당(패시브 스킬 등) */
	OnSpawn,
};

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULCGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ELCAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
	//~UGameplayAbility interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//~End of UGameplayAbility interface

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LC|AbilityActivation")
	ELCAbilityActivationPolicy ActivationPolicy;

};
