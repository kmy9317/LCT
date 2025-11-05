#include "LCPlayerInfoTypes.h"

#include "GameFramework/PlayerState.h"
#include "Network/LCNetStatics.h"

FLCPlayerSelectionInfo::FLCPlayerSelectionInfo()
	: Slot(GetInvalidSlot()), PlayerUniqueId(FUniqueNetIdRepl::Invalid()), PlayerNickname{}, CharacterDefinition(nullptr)
{
}

FLCPlayerSelectionInfo::FLCPlayerSelectionInfo(uint8 InSlot, const APlayerState* InPlayerState)
	: Slot(InSlot), CharacterDefinition(nullptr)
{
	if (InPlayerState)
	{
		PlayerUniqueId = InPlayerState->GetUniqueId();
		PlayerNickname = InPlayerState->GetPlayerName();
	}
}

bool FLCPlayerSelectionInfo::IsForPlayer(const APlayerState* PlayerState) const
{
	if (!PlayerState)
	{
		return false;
	}

#if WITH_EDITOR
	return PlayerState->GetPlayerName() == PlayerNickname;
#else
	return PlayerState->GetUniqueId() == GetPlayerUniqueId();
#endif
}

bool FLCPlayerSelectionInfo::IsValid() const
{
#if WITH_EDITOR
	return true;
#else
	if (!PlayerUniqueId.IsValid())
	{
		return false;
	}
	if (Slot == GetInvalidSlot())
	{
		return false;
	}
	if (Slot >= LCNetStatics::GetPlayerCountPerTeam() * 2)
	{
		return false;
	}
	return true;
#endif
}

uint8 FLCPlayerSelectionInfo::GetInvalidSlot()
{
	return uint8(255);
}
