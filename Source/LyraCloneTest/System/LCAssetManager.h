// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LCAssetManager.generated.h"

class ULCCharacterDefinition;

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static ULCAssetManager& Get();
	void LoadCharacterDefinitions(const FStreamableDelegate& LoadFinishedCallback);
	bool GetLoadedCharacterDefinitions(TArray<ULCCharacterDefinition*>& LoadedCharacterDefinations) const;
	
};
