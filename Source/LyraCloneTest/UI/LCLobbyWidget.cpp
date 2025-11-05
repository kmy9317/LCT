#include "LCLobbyWidget.h"

#include "CharacterEntryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "LCTeamSelectionWidget.h"
#include "Character/LCCharacterDefinition.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/WidgetSwitcher.h"
#include "GameModes/LCLobbyGameState.h"
#include "Network/LCNetStatics.h"
#include "Player/LCLobbyPlayerController.h"
#include "Player/LCLobbyPlayerState.h"
#include "System/LCAssetManager.h"

void ULCLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ClearAndPopulateTeamSelectionSlots();
	ConfigureGameState();
	LobbyPlayerController = GetOwningPlayer<ALCLobbyPlayerController>();
	if (LobbyPlayerController)
	{
		LobbyPlayerController->OnSwitchToHeroSelection.BindUObject(this, &ULCLobbyWidget::SwitchToHeroSelection);
	}
	StartHeroSelectionButton->SetIsEnabled(false);
	StartHeroSelectionButton->OnClicked.AddDynamic(this, &ThisClass::StartHeroSelectionButtonClicked);
	ULCAssetManager::Get().LoadCharacterDefinitions(FStreamableDelegate::CreateUObject(this, &ThisClass::CharacterDefinitionLoaded));

	if (CharacterSelectionTileView)
	{
		CharacterSelectionTileView->OnItemSelectionChanged().AddUObject(this, &ULCLobbyWidget::CharacterSelected);
	}
}

void ULCLobbyWidget::ClearAndPopulateTeamSelectionSlots()
{
	TeamSelectionSlotGridPanel->ClearChildren();

	for (int i = 0; i < ULCNetStatics::GetPlayerCountPerTeam() * 2; i++)
	{
		ULCTeamSelectionWidget* NewSelectionWidget = CreateWidget<ULCTeamSelectionWidget>(this, TeamSelectionWidgetClass);
		if (NewSelectionWidget)
		{
			NewSelectionWidget->SetSlotID(i);
			UUniformGridSlot* NewGridSlot = TeamSelectionSlotGridPanel->AddChildToUniformGrid(NewSelectionWidget);
			if (NewGridSlot)
			{
				int32 Row = i % ULCNetStatics::GetPlayerCountPerTeam();
				int32 Column = i < ULCNetStatics::GetPlayerCountPerTeam() ? 0 : 1;
				NewGridSlot->SetRow(Row);
				NewGridSlot->SetColumn(Column);
			}
			NewSelectionWidget->OnSlotClicked.AddUObject(this, &ThisClass::SlotSelected);
			TeamSelectionSlots.Add(NewSelectionWidget);
		}
	}
}

void ULCLobbyWidget::SlotSelected(uint8 NewSlotID)
{
	if (LobbyPlayerController)
	{
		LobbyPlayerController->Server_RequestPlayerSelectionChange(NewSlotID);
	}
}

void ULCLobbyWidget::ConfigureGameState()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	LCGameState = World->GetGameState<ALCLobbyGameState>();
	if (!LCGameState)
	{
		World->GetTimerManager().SetTimer(ConfigureGameStateTimerHandle, this, &ThisClass::ConfigureGameState, 1.f);
	}
	else
	{
		LCGameState->OnPlayerSelectionUpdated.AddUObject(this, &ThisClass::UpdatePlayerSelectionDisplay);
		UpdatePlayerSelectionDisplay(LCGameState->GetPlayerSelection());
	}
}

void ULCLobbyWidget::UpdatePlayerSelectionDisplay(const TArray<FLCPlayerSelectionInfo>& PlayerSelections)
{
	for (ULCTeamSelectionWidget* TeamSelectionSlot : TeamSelectionSlots)
	{
		TeamSelectionSlot->UpdateSlotInfo("Empty");
	}

	for (UUserWidget* CharacterEntryAsWidget : CharacterSelectionTileView->GetDisplayedEntryWidgets())
	{
		UCharacterEntryWidget* CharacterEntryWidget = Cast<UCharacterEntryWidget>(CharacterEntryAsWidget);
		if (CharacterEntryWidget)
		{
			CharacterEntryWidget->SetSelected(false);
		}
	}

	for (const FLCPlayerSelectionInfo& PlayerSelection : PlayerSelections)
	{
		if (!PlayerSelection.IsValid())
		{
			continue;
		}
		TeamSelectionSlots[PlayerSelection.GetPlayerSlot()]->UpdateSlotInfo(PlayerSelection.GetPlayerNickname());

		UCharacterEntryWidget* SelectedEntry = CharacterSelectionTileView->GetEntryWidgetFromItem<UCharacterEntryWidget>(PlayerSelection.GetCharacterDefinition());
		if (SelectedEntry)
		{
			SelectedEntry->SetSelected(true);
		}
	}

	if (LCGameState)
	{
		StartHeroSelectionButton->SetIsEnabled(LCGameState->CanStartHeroSelection());
	}
}

void ULCLobbyWidget::StartHeroSelectionButtonClicked()
{
	if (LobbyPlayerController)
	{
		LobbyPlayerController->Server_StartHeroSelection();
	}
}

void ULCLobbyWidget::SwitchToHeroSelection()
{
	MainSwitcher->SetActiveWidget(HeroSelectionRoot);
}

void ULCLobbyWidget::CharacterDefinitionLoaded()
{
	TArray<ULCCharacterDefinition*> LoadedCharacterDefinitions;
	if (ULCAssetManager::Get().GetLoadedCharacterDefinitions(LoadedCharacterDefinitions))
	{
		CharacterSelectionTileView->SetListItems(LoadedCharacterDefinitions);
	}
}

void ULCLobbyWidget::CharacterSelected(UObject* SelectedUObject)
{
	if (!LobbyPlayerState)
	{
		LobbyPlayerState = GetOwningPlayerState<ALCLobbyPlayerState>();
	}

	if (!LobbyPlayerState)
	{
		return;
	}

	if (ULCCharacterDefinition* CharacterDefinition = Cast<ULCCharacterDefinition>(SelectedUObject))
	{
		LobbyPlayerState->Server_SetSelectedCharacterDefinition(CharacterDefinition);
	}
}
