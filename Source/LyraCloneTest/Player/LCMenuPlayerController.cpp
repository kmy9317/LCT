// Fill out your copyright notice in the Description page of Project Settings.


#include "LCMenuPlayerController.h"

#include "Blueprint/UserWidget.h"

void ALCMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);

	// for listen server
	if (HasAuthority() && IsLocalPlayerController())
	{
		SpawnWidget();
	}
}

void ALCMenuPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsLocalPlayerController())
	{
		SpawnWidget();
	}
}

void ALCMenuPlayerController::SpawnWidget()
{
	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}
