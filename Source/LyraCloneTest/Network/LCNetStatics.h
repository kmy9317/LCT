// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LCNetStatics.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCNetStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static uint8 GetPlayerCountPerTeam();
};
