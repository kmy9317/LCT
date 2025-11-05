// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCPlayerTeamSlotWidget.generated.h"

class ULCCharacterDefinition;
class UWidgetAnimation;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCPlayerTeamSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void UpdateSlot(const FString& PlayerName, const ULCCharacterDefinition* CharacterDefinition);

	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;

private:
	void UpdateNameText();

private:	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HoverAnim;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PlayerCharacterIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName CharacterIconMatParamName = "Icon";

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName CharacterEmptyMatParamName = "Empty";

	FString CachedPlayerNameStr;
	FString CachedCharacterNameStr;
	
};
