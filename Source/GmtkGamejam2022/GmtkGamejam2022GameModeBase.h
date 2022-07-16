#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Die.h"
#include "Board.h"
#include "GameFramework/GameModeBase.h"
#include "GmtkGamejam2022GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GMTKGAMEJAM2022_API AGmtkGamejam2022GameModeBase : public AGameModeBase, public Board
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int Rows;

	UPROPERTY(EditAnywhere)
	int Columns;

	UPROPERTY(EditAnywhere)
	int OutOfBoundSize;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADie> DieClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> EmptyTileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> Resource1TileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> Resource2TileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> FenceTileClass;

	virtual TSubclassOf<ATile> RandomTileClass() override;
	
	virtual void UpdateCell(int32 Column, int32 Row, TSubclassOf<ATile> TileClass) override;

	virtual bool ContainsFence(int32 Column, int32 Row) override;

protected:
	virtual void BeginPlay() override;

	void SpawnDie(const int32 Column, const int32 Row, const ADie::ERollDirection RollDirection);

private:
	TArray<TSubclassOf<ATile>> TileClasses;
	
	ADie* Die;
	TArray<TArray<ATile*>> Tiles;
	TArray<TArray<ATile*>> Fences;

	FVector GetCellLocation(int X, int Y) const;

	void DieLifecycleMgmt();
};
