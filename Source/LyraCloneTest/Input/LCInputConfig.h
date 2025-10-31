// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LCInputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FLCInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContexts;
	
	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority = 0;
};

USTRUCT(BlueprintType)
struct FLCInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	ULCInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLCInputMappingContextAndPriority> InputMappingContexts;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLCInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLCInputAction> AbilityInputActions;
};
