#include "LCCharacter.h"

#include "LCPawnExtensionComponent.h"
#include "AbilitySystem/LCAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ALCCharacter::ALCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// LCPawnExtensionComponent만 native c++로 설정해 주는 이유는 이 컴포넌트가 다른 컴포넌트들의
	// 초기화를 담당하는 컴포넌트이기 떄문에 가장 먼저 생성해 주어야 한다. 엔진에서는  native c++의 컴포넌트가 먼저 생성되고 
	// 이후에 블루프린트에서 만든 컴포넌트들이 생성되도록 구현되어 있다. 
	PawnExtComponent = CreateDefaultSubobject<ULCPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
}

void ALCCharacter::OnAbilitySystemInitialized()
{
	
	
}

void ALCCharacter::OnAbilitySystemUninitialized()
{
	
}

UAbilitySystemComponent* ALCCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetLCAbilitySystemComponent();
}

void ALCCharacter::ToggleCrouch()
{
	const UCharacterMovementComponent* MoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());

	if (IsCrouched() || MoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (MoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void ALCCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void ALCCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void ALCCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void ALCCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}

void ALCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

