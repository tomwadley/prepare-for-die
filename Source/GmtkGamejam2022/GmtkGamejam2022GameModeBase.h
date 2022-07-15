#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GameFramework/GameModeBase.h"
#include "GmtkGamejam2022GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GMTKGAMEJAM2022_API AGmtkGamejam2022GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> EmptyTile;

	virtual void BeginPlay() override;

private:
	TArray<TArray<ATile*>> Tiles;
};
