// Fill out your copyright notice in the Description page of Project Settings.


#include "LCCharacterDisplay.h"

#include "Camera/CameraComponent.h"
#include "Character/LCCharacterDefinition.h"

ALCCharacterDisplay::ALCCharacterDisplay()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root Comp"));

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh Component");
	MeshComponent->SetupAttachment(GetRootComponent());
	
	ViewCameraComponent = CreateDefaultSubobject<UCameraComponent>("View Camera Component");
	ViewCameraComponent->SetupAttachment(GetRootComponent());
}

void ALCCharacterDisplay::ConfigureWithCharacterDefination(const ULCCharacterDefinition* CharacterDefination)
{
	if (!CharacterDefination)
	{
		return;
	}
	
	MeshComponent->SetSkeletalMesh(CharacterDefination->LoadDisplayMesh());
	MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	MeshComponent->SetAnimInstanceClass(CharacterDefination->LoadDisplayAnimationBP());
}

