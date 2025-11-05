// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LCPawnData.generated.h"

class ULCInputConfig;
class ULCAbilitySet;

/**
 * ULCPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "LC Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class LYRACLONETEST_API ULCPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static FPrimaryAssetType GetPawnAssetType() { return FPrimaryAssetType(TEXT("PawnData")); }

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(GetPawnAssetType(), GetFName());
	}

	ULCPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from ALCPawn or ALCCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LC|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LC|Abilities")
	TArray<TObjectPtr<ULCAbilitySet>> AbilitySets;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LC|Input")
	TObjectPtr<ULCInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LC|Camera")
	// TSubclassOf<ULCCameraMode> DefaultCameraMode;
};
