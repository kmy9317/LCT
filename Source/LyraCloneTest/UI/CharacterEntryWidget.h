// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterEntryWidget.generated.h"

class UImage;
class ULCCharacterDefinition;
class UTextBlock;
/**
 * 
 */
UCLASS()
class LYRACLONETEST_API UCharacterEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	const ULCCharacterDefinition* GetCharacterDefinition() const { return CharacterDefinition; }
	void SetSelected(bool bIsSelected);
	
private:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterNameText;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FName IconTextureMatParamName = "Icon";

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FName SaturationMatParamName = "Saturation";

	UPROPERTY()
	TObjectPtr<ULCCharacterDefinition> CharacterDefinition;
};
