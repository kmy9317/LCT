#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "LCCharacter.generated.h"

class ULCPawnExtensionComponent;

UCLASS()
class LYRACLONETEST_API ALCCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALCCharacter();

	void OnAbilitySystemInitialized();
	void OnAbilitySystemUninitialized();

	/**
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void ToggleCrouch();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hak|Character")
	TObjectPtr<ULCPawnExtensionComponent> PawnExtComponent;
};
