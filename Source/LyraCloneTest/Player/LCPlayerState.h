#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "LCPlayerInfoTypes.h"
#include "GameFramework/PlayerState.h"
#include "LCPlayerState.generated.h"

class ALCPlayerController;
class ULCPawnData;
class ULCAbilitySystemComponent;

/** Defines the types of client connected */
UENUM()
enum class ELCPlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ALCPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
 
public:
	ALCPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "LC|PlayerState")
	ALCPlayerController* GetLCPlayerController() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	ULCAbilitySystemComponent* GetLCAbilitySystemComponent() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULCPawnData* InPawnData);

	void SetPlayerSelection(const FLCPlayerSelectionInfo& NewPlayerSelection) { PlayerSelection = NewPlayerSelection; }

	//~AActor interface
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface

	void SetPlayerConnectionType(ELCPlayerConnectionType NewType);
	ELCPlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }


	// TODO : 임시 코드로써 삭제 예정
	TSubclassOf<APawn> GetSelectedPawnClass() const;
	
private:
	
	// 어빌리티 시스템 컴포넌트에서 PawnData를 참조해서 능력을 부여하기 위해 캐싱을 해놓음
	UPROPERTY(Replicated)
	TObjectPtr<const ULCPawnData> PawnData;

	UPROPERTY(VisibleAnywhere, Category = "LC|PlayerState")
	TObjectPtr<ULCAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Replicated)
	ELCPlayerConnectionType MyPlayerConnectionType;

	FLCPlayerSelectionInfo PlayerSelection;
};
