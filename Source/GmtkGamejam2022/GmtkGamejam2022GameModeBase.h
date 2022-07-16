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

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> Resource1TileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> Resource2TileClass;

	TSubclassOf<ATile> RandomTileClass();
	
protected:
	virtual void BeginPlay() override;

	void SpawnDie();

private:
	TArray<TSubclassOf<ATile>> TileClasses;
	
	TArray<ADie*> Dies;
	TArray<TArray<ATile*>> Tiles;

	FVector GetCellLocation(int X, int Y) const;
};
