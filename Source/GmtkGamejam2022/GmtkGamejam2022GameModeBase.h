#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Die.h"
#include "GameFramework/GameModeBase.h"
#include "GmtkGamejam2022GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GMTKGAMEJAM2022_API AGmtkGamejam2022GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int Rows;

	UPROPERTY(EditAnywhere)
	int Columns;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADie> DieClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> EmptyTileClass;
	
protected:
	virtual void BeginPlay() override;

	void SpawnDie();

private:
	TArray<ADie*> Dies;
	TArray<TArray<ATile*>> Tiles;
};
