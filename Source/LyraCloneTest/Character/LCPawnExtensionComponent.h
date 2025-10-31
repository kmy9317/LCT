// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"

#include "LCPawnExtensionComponent.generated.h"

class ULCAbilitySystemComponent;
class ULCPawnData;
/**
 * 초기화 전반을 조정하는 컴포넌트
 */
UCLASS()
class LYRACLONETEST_API ULCPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:

	ULCPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "LC|Pawn")
	static ULCPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULCPawnExtensionComponent>() : nullptr); }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULCPawnData* InPawnData);

	/** Gets the current ability system component, which may be owned by a different actor */
	UFUNCTION(BlueprintPure, Category = "LC|Pawn")
	ULCAbilitySystemComponent* GetLCAbilitySystemComponent() const { return AbilitySystemComponent; }

	/** AbilitySystemComponent의 AvatorActor 대상 초기화 or 해제 호출 */
	void InitializeAbilitySystem(ULCAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();

	/** Should be called by the owning pawn when the pawn's controller changes. */
	void HandleControllerChanged();

	/** Should be called by the owning pawn when the player state has been replicated. */
	void HandlePlayerStateReplicated();

	/** Should be called by the owning pawn when the input component is setup. */
	void SetupPlayerInputComponent();

	/** OnAbilitySystem[Initialized|Uninitialized] Delegate에 추가: */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

protected:

	/**
	* UPawnComponent interfaces
	*/
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnRep_PawnData();

	/** ASC Init과 Uninit의 Delegate 추가 */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

	/**
	 * Pawn을 생성한 데이터를 캐싱
	 */
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "LC|Pawn")
	TObjectPtr<const ULCPawnData> PawnData;

	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY(Transient)
	TObjectPtr<ULCAbilitySystemComponent> AbilitySystemComponent;
};
