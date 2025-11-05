// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCAbilityTooltip.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCAbilityTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAbilityInfo(const FName& AbilityName, UTexture2D* AbilityTexture, const FText& AbilityDescription, float AbilityCooldown, float AbilityCost);

private:	
	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UTextBlock> AbilityNameText;

	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UImage> AbilityIcon;

	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UTextBlock> AbilityDescriptionText;

	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UTextBlock> AbilityCooldownText;

	UPROPERTY(meta=(BindWidget))	
	TObjectPtr<UTextBlock> AbilityCostText;
};
