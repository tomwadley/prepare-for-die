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
	float Resource1TileProbability;

	UPROPERTY(EditAnywhere)
	float Resource2TileProbability;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> FenceTileClass;

	UPROPERTY(EditAnywhere)
	float ResourceMineRate;

	UPROPERTY(EditAnywhere)
	int32 ResourceMineQty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Resource2WinCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Resource1FenceCost;

	virtual TSubclassOf<ATile> RandomTileClass() override;
	
	virtual bool UpdateCell(int32 Column, int32 Row, TSubclassOf<ATile> TileClass) override;

	virtual bool ContainsFence(int32 Column, int32 Row) override;

	virtual void HitFence(int32 Column, int32 Row) override;

	void BuyFence(int32 Column, int32 Row);
	
	void MineResource1();
	void MineResource2();

	UFUNCTION(BlueprintCallable)
	int32 GetResource1() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetResource2() const;

protected:
	virtual void BeginPlay() override;

	void SpawnDie(const int32 Column, const int32 Row, const ADie::ERollDirection RollDirection);

private:
	ADie* Die;
	TArray<TArray<ATile*>> Tiles;
	TArray<TArray<ATile*>> Fences;
	
	int32 Resource1;
	int32 Resource2;

	FVector GetCellLocation(int X, int Y) const;

	void DieLifecycleMgmt();
};
