// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LCCharacterDefinition.generated.h"

class ULCPawnData;
class ALCCharacter;
class USkeletalMesh;

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(GetCharacterDefinitionAssetType(), GetFName());
	}
	static FPrimaryAssetType GetCharacterDefinitionAssetType() { return FPrimaryAssetType(TEXT("CharacterDefinition")); }

	FString GetCharacterDisplayName() const { return CharacterName; }
	UTexture2D* LoadIcon() const;
	TSubclassOf<ALCCharacter> LoadCharacterClass() const;
	TSubclassOf<UAnimInstance> LoadDisplayAnimationBP() const;
	USkeletalMesh* LoadDisplayMesh() const;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FString CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftObjectPtr<UTexture2D> CharacterIcon;

	// TODO : Pawn Data로 통합 예정 
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftClassPtr<ALCCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftObjectPtr<USkeletalMesh> DisplayMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftClassPtr<UAnimInstance> DisplayAnimBP;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftObjectPtr<ULCPawnData> PawnData;
};
