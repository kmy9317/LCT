// Fill out your copyright notice in the Description page of Project Settings.


#include "LCTeamSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void ULCTeamSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this, &ThisClass::SelectButtonClicked);
}

void ULCTeamSelectionWidget::SetSlotID(uint8 NewSlotID)
{
	SlotID = NewSlotID;
}

void ULCTeamSelectionWidget::UpdateSlotInfo(const FString& PlayerNickname)
{
	InfoText->SetText(FText::FromString(PlayerNickname));
}

void ULCTeamSelectionWidget::SelectButtonClicked()
{
	OnSlotClicked.Broadcast(SlotID);
}
