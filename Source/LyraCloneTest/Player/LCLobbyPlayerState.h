#pragma once

#include "CoreMinimal.h"
#include "LCPlayerInfoTypes.h"
#include "GameFramework/PlayerState.h"
#include "LCLobbyPlayerState.generated.h"

class ALCLobbyGameState;
class ULCCharacterDefinition;
/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ALCLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ALCLobbyPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSelectedCharacterDefinition(ULCCharacterDefinition* NewDefinition);

private:
	void PlayerSelectionUpdated(const TArray<FLCPlayerSelectionInfo>& NewPlayerSelections);
	
private:
	UPROPERTY(Replicated)
	FLCPlayerSelectionInfo PlayerSelection;

	UPROPERTY()
	TObjectPtr<ALCLobbyGameState> LobbyGameState;
};
