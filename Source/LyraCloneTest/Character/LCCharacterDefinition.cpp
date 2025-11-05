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
	DisplayMesh.LoadSynchronous();
	if (DisplayMesh.IsValid())
		return DisplayMesh.Get();

	return nullptr;
}
