#include "LCLobbyWidget.h"

#include "CharacterEntryWidget.h"
#include "LCAbilityListView.h"
#include "LCPlayerTeamLayoutWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "LCTeamSelectionWidget.h"
#include "Actors/LCCharacterDisplay.h"
#include "Character/LCCharacterDefinition.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerStart.h"
#include "GameModes/LCLobbyGameState.h"
#include "Kismet/GameplayStatics.h"
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
	StartMatchButton->SetIsEnabled(false);
	StartMatchButton->OnClicked.AddDynamic(this, &ThisClass::StartMatchButtonClicked);
	
	ULCAssetManager::Get().LoadCharacterDefinitions(FStreamableDelegate::CreateUObject(this, &ThisClass::CharacterDefinitionLoaded));

	if (CharacterSelectionTileView)
	{
		CharacterSelectionTileView->OnItemSelectionChanged().AddUObject(this, &ULCLobbyWidget::CharacterSelected);
	}

	SpawnCharacterDisplay();
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

		// TODO : 다른 플레이어의 캐릭터도 보여주도록 제거 or 수정
		if (PlayerSelection.IsForPlayer(GetOwningPlayerState()))
		{
			UpdateCharacterDisplay(PlayerSelection);
		}
	}

	if (LCGameState)
	{
		StartHeroSelectionButton->SetIsEnabled(LCGameState->CanStartHeroSelection());
		StartMatchButton->SetIsEnabled(LCGameState->CanStartMatch());
	}

	if (PlayerTeamLayoutWidget)
	{
		PlayerTeamLayoutWidget->UpdatePlayerSelection(PlayerSelections);
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

void ULCLobbyWidget::SpawnCharacterDisplay()
{
	if (CharacterDisplay)
	{
		return;
	}
	if (!CharacterDisplayClass)
	{
		return;
	}
	
	FTransform CharacterDisplayTransform = FTransform::Identity;

	// TODO : 접속한 플레이어별 PlayerStart 위치 지정 필요
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	if (PlayerStart)
	{
		CharacterDisplayTransform = PlayerStart->GetActorTransform();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CharacterDisplay = GetWorld()->SpawnActor<ALCCharacterDisplay>(CharacterDisplayClass, CharacterDisplayTransform, SpawnParams);
	GetOwningPlayer()->SetViewTarget(CharacterDisplay);
}

void ULCLobbyWidget::UpdateCharacterDisplay(const FLCPlayerSelectionInfo& PlayerSelectionInfo)
{
	if (!PlayerSelectionInfo.GetCharacterDefinition())
	{
		return;
	}

	CharacterDisplay->ConfigureWithCharacterDefination(PlayerSelectionInfo.GetCharacterDefinition());

	// TODO : CharacterInfo or PawnData 에서 캐릭터에 부여되는 Ability를 가져와서 AbilityListView에 적용되도록 구조 생각
	//AbilityListView->ConfigureAbilities();
}

void ULCLobbyWidget::StartMatchButtonClicked()
{
	if (LobbyPlayerController)
	{
		LobbyPlayerController->Server_RequestStartMatch();
	}
}
