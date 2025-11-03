#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Player/LCPlayerInfoTypes.h"
#include "LCLobbyGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSelectionUpdated, const TArray<FLCPlayerSelectionInfo>& /*NewPlayerSelection*/);

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ALCLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void RequestPlayerSelectionChange(const APlayerState* RequestingPlayer, uint8 DesiredSlot);
	bool IsSlotOccupied(uint8 SlotId) const;

	const TArray<FLCPlayerSelectionInfo>& GetPlayerSelection() const;

	bool CanStartHeroSelection() const;
private:
	UFUNCTION()
	void OnRep_PlayerSelectionArray();

public:
	FOnPlayerSelectionUpdated OnPlayerSelectionUpdated;

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerSelectionArray)
	TArray<FLCPlayerSelectionInfo> PlayerSelectionArray;
};
