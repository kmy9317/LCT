#include "LCAssetManager.h"

#include "Character/LCCharacterDefinition.h"

ULCAssetManager& ULCAssetManager::Get()
{
	ULCAssetManager* Singleton = Cast<ULCAssetManager>(GEngine->AssetManager.Get());
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogLoad, Fatal, TEXT("Asset Manager Needs to be of the type LCAssetMaanger"));
	return (*NewObject<ULCAssetManager>());
}

void ULCAssetManager::LoadCharacterDefinitions(const FStreamableDelegate& LoadFinishedCallback)
{
	LoadPrimaryAssetsWithType(ULCCharacterDefinition::GetCharacterDefinitionAssetType(), TArray<FName>(), LoadFinishedCallback);
}

bool ULCAssetManager::GetLoadedCharacterDefinitions(TArray<ULCCharacterDefinition*>& LoadedCharacterDefinations) const
{
	TArray<UObject*> LoadedObjects;
	bool bLoaded = GetPrimaryAssetObjectList(ULCCharacterDefinition::GetCharacterDefinitionAssetType(), LoadedObjects);
	if (bLoaded)
	{
		for (UObject* LoadedObject : LoadedObjects)
		{
			LoadedCharacterDefinations.Add(Cast<ULCCharacterDefinition>(LoadedObject));
		}
	}

	return bLoaded;
}