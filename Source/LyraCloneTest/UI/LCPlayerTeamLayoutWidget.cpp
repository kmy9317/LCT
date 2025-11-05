#include "LCPlayerTeamLayoutWidget.h"

#include "LCPlayerTeamSlotWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Network/LCNetStatics.h"
#include "Character/LCCharacterDefinition.h"
#include "Player/LCPlayerInfoTypes.h"

void ULCPlayerTeamLayoutWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TeamOneLayoutBox->ClearChildren();
	TeamTwoLayoutBox->ClearChildren();
	
	if (!PlayerTeamSlotWidgetClass)
	{
		return;
	}
	
	for (int i = 0; i < ULCNetStatics::GetPlayerCountPerTeam() * 2; ++i)
	{
		ULCPlayerTeamSlotWidget* NewSlotWidget = CreateWidget<ULCPlayerTeamSlotWidget>(GetOwningPlayer(), PlayerTeamSlotWidgetClass);
		TeamSlotWidgets.Add(NewSlotWidget);
		
		UHorizontalBoxSlot* NewSlot;
		if (i < ULCNetStatics::GetPlayerCountPerTeam())
		{
			NewSlot = TeamOneLayoutBox->AddChildToHorizontalBox(NewSlotWidget);
		}
		else
		{
			NewSlot = TeamTwoLayoutBox->AddChildToHorizontalBox(NewSlotWidget);
		}

		NewSlot->SetPadding(FMargin{ PlayerTeamWidgetSlotMargin });
	}
}

void ULCPlayerTeamLayoutWidget::UpdatePlayerSelection(const TArray<FLCPlayerSelectionInfo>& PlayerSelections)
{
	for (ULCPlayerTeamSlotWidget* SlotWidget : TeamSlotWidgets)
	{
		SlotWidget->UpdateSlot("", nullptr);
	}

	for (const FLCPlayerSelectionInfo& PlayerSelection : PlayerSelections)
	{
		if (!PlayerSelection.IsValid())
		{
			continue;
		}

		TeamSlotWidgets[PlayerSelection.GetPlayerSlot()]->UpdateSlot(PlayerSelection.GetPlayerNickname(), PlayerSelection.GetCharacterDefinition());
	}
}