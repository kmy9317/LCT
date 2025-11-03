// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LCMenuPlayerController.h"
#include "LCLobbyPlayerController.generated.h"

DECLARE_DELEGATE(FOnSwitchToHeroSelection);

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ALCLobbyPlayerController : public ALCMenuPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPlayerSelectionChange(uint8 NewSlotID);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartHeroSelection();

	UFUNCTION(Client, Reliable)
	void Client_StartHeroSelection();

public:
	FOnSwitchToHeroSelection OnSwitchToHeroSelection;
	
};
