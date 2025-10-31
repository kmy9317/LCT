#include "LCHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "LCCharacter.h"
#include "LCGameplayTags.h"
#include "LCLogChannels.h"
#include "LCPawnData.h"
#include "LCPawnExtensionComponent.h"
#include "AbilitySystem/LCAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/LCInputComponent.h"
#include "Input/LCInputConfig.h"
#include "Input/LCInputGameplayTags.h"
#include "Player/LCPlayerController.h"
#include "Player/LCPlayerState.h"
#include "UserSettings/EnhancedInputUserSettings.h"

const FName ULCHeroComponent::NAME_ActorFeatureName("Hero");

ULCHeroComponent::ULCHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//AbilityCameraMode = nullptr;
}

void ULCHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogLC, Error, TEXT("[ULCHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool ULCHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == LCGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == LCGameplayTags::InitState_Spawned && DesiredState == LCGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<ALCPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ALCPlayerController* LCPC = GetController<ALCPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !LCPC || !LCPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == LCGameplayTags::InitState_DataAvailable && DesiredState == LCGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ALCPlayerState* LCPS = GetPlayerState<ALCPlayerState>();

		return LCPS && Manager->HasFeatureReachedInitState(Pawn, ULCPawnExtensionComponent::NAME_ActorFeatureName, LCGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == LCGameplayTags::InitState_DataInitialized && DesiredState == LCGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void ULCHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == LCGameplayTags::InitState_DataAvailable && DesiredState == LCGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALCPlayerState* LCPS = GetPlayerState<ALCPlayerState>();
		if (!ensure(Pawn && LCPS))
		{
			return;
		}

		const ULCPawnData* PawnData = nullptr;

		if (ULCPawnExtensionComponent* PawnExtComp = ULCPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULCPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(LCPS->GetLCAbilitySystemComponent(), LCPS);
		}

		if (ALCPlayerController* LCPC = GetController<ALCPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		// if (PawnData)
		// {
		// 	if (ULCCameraComponent* CameraComponent = ULCCameraComponent::FindCameraComponent(Pawn))
		// 	{
		// 		CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
		// 	}
		// }
	}
}

void ULCHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == ULCPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == LCGameplayTags::InitState_DataInitialized)
		{
			// LCPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, LCHeroComponent도 DataInitialized 상태로 변경
			// - CanChangeInitState 확인
			CheckDefaultInitialization();
		}
	}
}

void ULCHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { LCGameplayTags::InitState_Spawned, LCGameplayTags::InitState_DataAvailable, LCGameplayTags::InitState_DataInitialized, LCGameplayTags::InitState_GameplayReady };

	ContinueInitStateChain(StateChain);
}

void ULCHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
	BindOnActorInitStateChanged(ULCPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(LCGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void ULCHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULCHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const ULCPawnExtensionComponent* PawnExtComp = ULCPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULCPawnData* PawnData = PawnExtComp->GetPawnData<ULCPawnData>())
		{
			if (const ULCInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FLCInputMappingContextAndPriority& Mapping : InputConfig->InputMappingContexts)
				{
					if (UInputMappingContext* IMC = Mapping.MappingContexts)
					{
						if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
						{
							Settings->RegisterInputMappingContext(IMC);
						}
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
					}
				}
				
				ULCInputComponent* LCIC = Cast<ULCInputComponent>(PlayerInputComponent);
				if (ensureMsgf(LCIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULCInputComponent or a subclass of it.")))
				{
					TArray<uint32> BindHandles;
					LCIC->BindAbilityActions(InputConfig, this,&ThisClass::Input_AbilityInputTagStarted, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					LCIC->BindNativeAction(InputConfig, LCGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					LCIC->BindNativeAction(InputConfig, LCGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					LCIC->BindNativeAction(InputConfig, LCGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}
}

void ULCHeroComponent::Input_AbilityInputTagStarted(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULCPawnExtensionComponent* PawnExtComp = ULCPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULCAbilitySystemComponent* LCASC = PawnExtComp->GetLCAbilitySystemComponent())
			{
				LCASC->AbilityInputTagStarted(InputTag);
			}
		}
	}
}

void ULCHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULCPawnExtensionComponent* PawnExtComp = ULCPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULCAbilitySystemComponent* LCASC = PawnExtComp->GetLCAbilitySystemComponent())
			{
				LCASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void ULCHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const ULCPawnExtensionComponent* PawnExtComp = ULCPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (ULCAbilitySystemComponent* LCASC = PawnExtComp->GetLCAbilitySystemComponent())
		{
			LCASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void ULCHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULCHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void ULCHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ALCCharacter* Character = GetPawn<ALCCharacter>())
	{
		Character->ToggleCrouch();
	}
}

