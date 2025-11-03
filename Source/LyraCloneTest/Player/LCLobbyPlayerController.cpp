// Fill out your copyright notice in the Description page of Project Settings.


#include "LCLobbyPlayerController.h"

#include "LCPlayerState.h"
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
	LCGameState->RequestPlayerSelectionChange(GetPlayerState<ALCPlayerState>(), NewSlotID);
}

bool ALCLobbyPlayerController::Server_RequestPlayerSelectionChange_Validate(uint8 NewSlotID)
{
	return true;
}
