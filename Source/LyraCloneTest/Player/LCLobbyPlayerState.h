#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LCLobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONETEST_API ALCLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
