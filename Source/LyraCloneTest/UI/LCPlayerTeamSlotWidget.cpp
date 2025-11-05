#include "LCPlayerTeamSlotWidget.h"

#include "Character/LCCharacterDefinition.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULCPlayerTeamSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacterIcon->GetDynamicMaterial()->SetScalarParameterValue(CharacterEmptyMatParamName, 1);
	CachedCharacterNameStr = "";
}

void ULCPlayerTeamSlotWidget::UpdateSlot(const FString& PlayerName, const ULCCharacterDefinition* CharacterDefinition)
{
	CachedPlayerNameStr = PlayerName;

	if (CharacterDefinition)
	{
		PlayerCharacterIcon->GetDynamicMaterial()->SetTextureParameterValue(CharacterIconMatParamName, CharacterDefinition->LoadIcon());
		PlayerCharacterIcon->GetDynamicMaterial()->SetScalarParameterValue(CharacterEmptyMatParamName, 0);
		CachedCharacterNameStr = CharacterDefinition->GetCharacterDisplayName();
	}
	else
	{
		PlayerCharacterIcon->GetDynamicMaterial()->SetScalarParameterValue(CharacterEmptyMatParamName, 1);
		CachedCharacterNameStr = "";
	}

	UpdateNameText();
}

void ULCPlayerTeamSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	NameText->SetText(FText::FromString(CachedCharacterNameStr));
	PlayAnimationForward(HoverAnim);
}

void ULCPlayerTeamSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	NameText->SetText(FText::FromString(CachedPlayerNameStr));
	PlayAnimationReverse(HoverAnim);
}

void ULCPlayerTeamSlotWidget::UpdateNameText()
{
	if (IsHovered())
	{
		NameText->SetText(FText::FromString(CachedCharacterNameStr));
	}
	else
	{
		NameText->SetText(FText::FromString(CachedPlayerNameStr));
	}
}