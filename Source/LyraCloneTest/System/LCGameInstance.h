// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void StartMatch();
	
protected:
	virtual void Init() override;

private:
	void LoadLevelAndListen(TSoftObjectPtr<UWorld> Level);

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> LobbyLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> GameLevel;
};
