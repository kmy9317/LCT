#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LCHeroComponent.generated.h"

class ULCInputConfig;
struct FGameplayAbilitySpecHandle;
struct FInputActionValue;

/**
 * component that sets up input and camera handling for player controlled pawns (or bots that simulate players)
 * - this depends on a PawnExtensionComponent to coordinate initialization
 *
 * 카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class LYRACLONETEST_API ULCHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:

	ULCHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "LC|Hero")
	static ULCHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULCHeroComponent>() : nullptr); }
	
	/** Overrides the camera from an active gameplay ability */
	//void SetAbilityCameraMode(TSubclassOf<ULCCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);
	
	/** Clears the camera override if it is set */
	//void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);
	
	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;
	
	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;
	
	//~ Begin IGameFrameworkInitStateInterface interface
	 virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	 virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	 virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	 virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	 virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_AbilityInputTagStarted(FGameplayTag InputTag);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

	//TSubclassOf<ULCCameraMode> DetermineCameraMode() const;

protected:
	/** Camera mode set by an ability. */
	// UPROPERTY()
	// TSubclassOf<ULCCameraMode> AbilityCameraMode;

	/** Spec handle for the last ability to set a camera mode. */
	//FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;

};
