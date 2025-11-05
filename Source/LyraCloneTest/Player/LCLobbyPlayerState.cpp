#include "LCLobbyPlayerState.h"

#include "GameModes/LCLobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ALCLobbyPlayerState::ALCLobbyPlayerState()
{
	bReplicates = true;
	SetNetUpdateFrequency(100.f);
}

void ALCLobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALCLobbyPlayerState, PlayerSelection);
}

void ALCLobbyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	LobbyGameState = Cast<ALCLobbyGameState>(UGameplayStatics::GetGameState(this));

	if (LobbyGameState)
	{
		LobbyGameState->OnPlayerSelectionUpdated.AddUObject(this, &ThisClass::PlayerSelectionUpdated);
	}
}

void ALCLobbyPlayerState::Server_SetSelectedCharacterDefinition_Implementation(ULCCharacterDefinition* NewDefinition)
{
	if (!LobbyGameState)
	{
		return;
	}
	if (!NewDefinition)
	{
		return;
	}

	if (LobbyGameState->IsDefinitionSelected(NewDefinition))
	{
		return;
	}

	if (PlayerSelection.GetCharacterDefinition())
	{
		LobbyGameState->SetCharacterDeselected(PlayerSelection.GetCharacterDefinition());
	}
	PlayerSelection.SetCharacterDefinition(NewDefinition);
	LobbyGameState->SetCharacterSelected(this, NewDefinition);
}

bool ALCLobbyPlayerState::Server_SetSelectedCharacterDefinition_Validate(ULCCharacterDefinition* NewDefinition)
{
	return true;
}

void ALCLobbyPlayerState::PlayerSelectionUpdated(const TArray<FLCPlayerSelectionInfo>& NewPlayerSelections)
{
	for (const FLCPlayerSelectionInfo& NewPlayerSelection : NewPlayerSelections)
	{
		if (NewPlayerSelection.IsForPlayer(this))
		{
			PlayerSelection = NewPlayerSelection;
			return;
		}
	}
}
