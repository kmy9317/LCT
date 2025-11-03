#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCLobbyWidget.generated.h"

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

	UPROPERTY()
	TObjectPtr<ALCLobbyPlayerController> LobbyPlayerController;

	UPROPERTY()
	TObjectPtr<ALCLobbyGameState> LCGameState;

	FTimerHandle ConfigureGameStateTimerHandle;
};
