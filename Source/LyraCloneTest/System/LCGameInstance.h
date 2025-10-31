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

protected:
	virtual void Init() override;
};
