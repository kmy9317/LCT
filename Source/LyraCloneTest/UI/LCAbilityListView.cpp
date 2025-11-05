// Fill out your copyright notice in the Description page of Project Settings.


#include "LCAbilityListView.h"

#include "Abilities/GameplayAbility.h"

void ULCAbilityListView::ConfigureAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	for (TSubclassOf<UGameplayAbility>& Ability : Abilities)
	{
		AddItem(Ability);
	}
}

void ULCAbilityListView::AbilityGaugeGenerated(UUserWidget& Widget)
{
	
}

const FAbilityWidgetData* ULCAbilityListView::FindWidgetDataForAbility(const TSubclassOf<UGameplayAbility>& AbilityClass) const
{
	if (!AbilityDataTable)
		return nullptr;

	for (auto& AbilityWidgetDataPair : AbilityDataTable->GetRowMap())
	{
		const FAbilityWidgetData* WidgetData = AbilityDataTable->FindRow<FAbilityWidgetData>(AbilityWidgetDataPair.Key, "");
		if (WidgetData->AbilityClass == AbilityClass)
		{
			return WidgetData;
		}
	}

	return nullptr;
}
