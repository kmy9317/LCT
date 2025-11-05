#pragma once

#include "CoreMinimal.h"
#include "LCPlayerInfoTypes.generated.h"

class APlayerState;
class ULCCharacterDefinition;

USTRUCT()
struct FLCPlayerSelectionInfo
{
	GENERATED_BODY()
public:
	FLCPlayerSelectionInfo() ;
	FLCPlayerSelectionInfo(uint8 InSlot, const APlayerState* InPlayerState);

	FORCEINLINE void SetSlot(uint8 NewSlot) { Slot = NewSlot; }
	FORCEINLINE uint8 GetPlayerSlot() const { return Slot; }
	FORCEINLINE FUniqueNetIdRepl GetPlayerUniqueId() const { return PlayerUniqueId; }
	FORCEINLINE FString GetPlayerNickname() const { return PlayerNickname; }
	FORCEINLINE const ULCCharacterDefinition* GetCharacterDefinition() const { return CharacterDefinition; }
	FORCEINLINE void SetCharacterDefinition(ULCCharacterDefinition* NewDefinition) { CharacterDefinition = NewDefinition; }

	bool IsForPlayer(const APlayerState* PlayerState) const;
	bool IsValid() const;

	static uint8 GetInvalidSlot();
	
private:
	UPROPERTY()
	uint8 Slot;

	UPROPERTY()
	FUniqueNetIdRepl PlayerUniqueId;

	UPROPERTY()
	FString PlayerNickname;

	UPROPERTY()
	TObjectPtr<ULCCharacterDefinition> CharacterDefinition;
};