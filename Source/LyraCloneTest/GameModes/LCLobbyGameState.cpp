// Fill out your copyright notice in the Description page of Project Settings.


#include "LCLobbyGameState.h"

#include "Net/UnrealNetwork.h"

void ALCLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PlayerSelectionArray, COND_None, REPNOTIFY_Always);
}

void ALCLobbyGameState::RequestPlayerSelectionChange(const APlayerState* RequestingPlayer, uint8 DesiredSlot)
{
	if (!HasAuthority() || IsSlotOccupied(DesiredSlot))
	{
		return;
	}

	FLCPlayerSelectionInfo* PlayerSelectionPtr = PlayerSelectionArray.FindByPredicate([&](const FLCPlayerSelectionInfo& PlayerSelection)
		{
			return PlayerSelection.IsForPlayer(RequestingPlayer);
		}
	);

	if (PlayerSelectionPtr)
	{
		PlayerSelectionPtr->SetSlot(DesiredSlot);
	}
	else
	{
		PlayerSelectionArray.Add(FLCPlayerSelectionInfo(DesiredSlot, RequestingPlayer));
	}

	OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
}

void ALCLobbyGameState::SetCharacterSelected(const APlayerState* SelectingPlayer, ULCCharacterDefinition* SelectedDefinition)
{
	if (IsDefinitionSelected(SelectedDefinition))
	{
		return;
	}

	FLCPlayerSelectionInfo* FoundPlayerSelection = PlayerSelectionArray.FindByPredicate([&](const FLCPlayerSelectionInfo& PlayerSelection)
		{
			return PlayerSelection.IsForPlayer(SelectingPlayer);
		}
	);

	if (FoundPlayerSelection)
	{
		FoundPlayerSelection->SetCharacterDefinition(SelectedDefinition);
		OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
	}
}

bool ALCLobbyGameState::IsSlotOccupied(uint8 SlotId) const
{
	for (const FLCPlayerSelectionInfo& PlayerSelection : PlayerSelectionArray)
	{
		if (PlayerSelection.GetPlayerSlot() == SlotId)
		{
			return true;
		}
	}

	return false;
}

bool ALCLobbyGameState::IsDefinitionSelected(const ULCCharacterDefinition* Definition) const
{
	const FLCPlayerSelectionInfo* FoundPlayerSelection = PlayerSelectionArray.FindByPredicate([&](const FLCPlayerSelectionInfo& PlayerSelection)
		{
			return PlayerSelection.GetCharacterDefinition() == Definition;
		}
	);

	return FoundPlayerSelection != nullptr;

}

void ALCLobbyGameState::SetCharacterDeselected(const ULCCharacterDefinition* DefinitionToDeselect)
{
	if (!DefinitionToDeselect)
	{
		return;
	}
	
	FLCPlayerSelectionInfo* FoundPlayerSelection = PlayerSelectionArray.FindByPredicate([&](const FLCPlayerSelectionInfo& PlayerSelection)
		{
			return PlayerSelection.GetCharacterDefinition() == DefinitionToDeselect;
		}
	);

	if (FoundPlayerSelection)
	{
		FoundPlayerSelection->SetCharacterDefinition(nullptr);
		OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
	}
}

const TArray<FLCPlayerSelectionInfo>& ALCLobbyGameState::GetPlayerSelection() const
{
	return PlayerSelectionArray;
}

bool ALCLobbyGameState::CanStartHeroSelection() const
{
	return PlayerSelectionArray.Num() == PlayerArray.Num();
}

void ALCLobbyGameState::OnRep_PlayerSelectionArray()
{
	OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
}
