#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCLobbyWidget.generated.h"

class ULCPlayerTeamLayoutWidget;
class ULCAbilityListView;
class ALCCharacterDisplay;
class ALCLobbyPlayerState;
class UTileView;
struct FLCPlayerSelectionInfo;
class ALCLobbyGameState;
class ALCLobbyPlayerController;
class ULCTeamSelectionWidget;
class UUniformGridPanel;
class UButton;
class UWidgetSwitcher;
class UWidget;

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	void ClearAndPopulateTeamSelectionSlots();
	void SlotSelected(uint8 NewSlotID);

	void ConfigureGameState();

	void UpdatePlayerSelectionDisplay(const TArray<FLCPlayerSelectionInfo>& PlayerSelections);

	UFUNCTION()
	void StartHeroSelectionButtonClicked();
	
	void SwitchToHeroSelection();
	void CharacterDefinitionLoaded();

	void CharacterSelected(UObject* SelectedUObject);

	void SpawnCharacterDisplay();
	void UpdateCharacterDisplay(const FLCPlayerSelectionInfo& PlayerSelectionInfo);

	UFUNCTION()
	void StartMatchButtonClicked();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> MainSwitcher;

	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UWidget> TeamSelectionRoot;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StartHeroSelectionButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> TeamSelectionSlotGridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "TeamSelection")
	TSubclassOf<ULCTeamSelectionWidget> TeamSelectionWidgetClass;

	UPROPERTY()
	TArray<ULCTeamSelectionWidget*> TeamSelectionSlots;

	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UWidget> HeroSelectionRoot;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTileView> CharacterSelectionTileView;

	// TODO : 현재 캐릭터의 Ability 관련 정보 UI
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<ULCAbilityListView> AbilityListView;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULCPlayerTeamLayoutWidget> PlayerTeamLayoutWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StartMatchButton;
	
	UPROPERTY()
	TObjectPtr<ALCLobbyPlayerController> LobbyPlayerController;

	UPROPERTY()
	TObjectPtr<ALCLobbyPlayerState> LobbyPlayerState;

	UPROPERTY()
	TObjectPtr<ALCLobbyGameState> LCGameState;

	UPROPERTY(EditDefaultsOnly, Category = "Character Display")
	TSubclassOf<ALCCharacterDisplay> CharacterDisplayClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character Display")
	TObjectPtr<ALCCharacterDisplay> CharacterDisplay;

	FTimerHandle ConfigureGameStateTimerHandle;
};
