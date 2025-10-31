// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LCPlayerController.generated.h"

class ALCPlayerState;
class ULCAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ALCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALCPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "LC|PlayerController")
	ALCPlayerState* GetLCPlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "CY|PlayerController")
	ULCAbilitySystemComponent* GetLCAbilitySystemComponent() const;

	//~AController interface
	virtual void OnUnPossess() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
