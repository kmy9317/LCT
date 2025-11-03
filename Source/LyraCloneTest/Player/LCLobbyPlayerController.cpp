// Fill out your copyright notice in the Description page of Project Settings.


#include "LCLobbyPlayerController.h"
#include "LCLobbyPlayerState.h"
#include "GameModes/LCLobbyGameState.h"



void ALCLobbyPlayerController::Server_RequestPlayerSelectionChange_Implementation(uint8 NewSlotID)
{
	if (!GetWorld())
	{
		return;
	}

	ALCLobbyGameState* LCGameState = GetWorld()->GetGameState<ALCLobbyGameState>();
	if (!LCGameState)
	{
		return;
	}
	LCGameState->RequestPlayerSelectionChange(GetPlayerState<ALCLobbyPlayerState>(), NewSlotID);
}

bool ALCLobbyPlayerController::Server_RequestPlayerSelectionChange_Validate(uint8 NewSlotID)
{
	return true;
}

void ALCLobbyPlayerController::Server_StartHeroSelection_Implementation()
{
	if (!HasAuthority() || !GetWorld())
	{
		return;
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ALCLobbyPlayerController* PlayerController = Cast<ALCLobbyPlayerController>(*It);
		if (PlayerController)
		{
			PlayerController->Client_StartHeroSelection();
		}
	}
}

bool ALCLobbyPlayerController::Server_StartHeroSelection_Validate()
{
	return true;
}

void ALCLobbyPlayerController::Client_StartHeroSelection_Implementation()
{
	OnSwitchToHeroSelection.ExecuteIfBound();
}
