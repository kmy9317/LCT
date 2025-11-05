#include "LCCharacterDefinition.h"
#include "LCCharacter.h"

UTexture2D* ULCCharacterDefinition::LoadIcon() const
{
	CharacterIcon.LoadSynchronous();
	if (CharacterIcon.IsValid())
		return CharacterIcon.Get();

	return nullptr;
}

TSubclassOf<ALCCharacter> ULCCharacterDefinition::LoadCharacterClass() const
{
	CharacterClass.LoadSynchronous();
	if (CharacterClass.IsValid())
		return CharacterClass.Get();
	
	return TSubclassOf<ALCCharacter>();

}

TSubclassOf<UAnimInstance> ULCCharacterDefinition::LoadDisplayAnimationBP() const
{
	DisplayAnimBP.LoadSynchronous();
	if (DisplayAnimBP.IsValid())
		return DisplayAnimBP.Get();

	return TSubclassOf<UAnimInstance>();
}

USkeletalMesh* ULCCharacterDefinition::LoadDisplayMesh() const
{
	TSubclassOf<ALCCharacter> LoadedCharaterClass = LoadCharacterClass();
	if (!LoadedCharaterClass)
		return nullptr;

	ACharacter* Character = Cast<ACharacter>(LoadedCharaterClass.GetDefaultObject());
	if (!Character)
		return nullptr;

	return Character->GetMesh()->GetSkeletalMeshAsset();
}
