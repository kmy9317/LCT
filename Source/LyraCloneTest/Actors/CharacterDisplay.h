// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterDisplay.generated.h"

class UCameraComponent;
class ULCCharacterDefinition;

UCLASS()
class LYRACLONETEST_API ACharacterDisplay : public AActor
{
	GENERATED_BODY()
public:	

	ACharacterDisplay();
	void ConfigureWithCharacterDefination(const ULCCharacterDefinition* CharacterDefination);

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Character Display")
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Character Display")
	TObjectPtr<UCameraComponent> ViewCameraComponent;
};
