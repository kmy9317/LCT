// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCPlayerTeamLayoutWidget.generated.h"

struct FLCPlayerSelectionInfo;
class UHorizontalBox;
class ULCPlayerTeamSlotWidget;
/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ULCPlayerTeamLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdatePlayerSelection(const TArray<FLCPlayerSelectionInfo>& PlayerSelections);
private:	
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	float PlayerTeamWidgetSlotMargin = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TSubclassOf<ULCPlayerTeamSlotWidget> PlayerTeamSlotWidgetClass;

	// TODO : 팀 구별 없이 Layount 용도로만 사용
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> TeamOneLayoutBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> TeamTwoLayoutBox;

	UPROPERTY()
	TArray<ULCPlayerTeamSlotWidget*> TeamSlotWidgets;
};
