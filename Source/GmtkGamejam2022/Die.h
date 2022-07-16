#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Die.generated.h"

UCLASS()
class GMTKGAMEJAM2022_API ADie : public AActor
{
	GENERATED_BODY()

public:
	ADie();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AActor* PivotPoint;

	UPROPERTY()
	TArray<ATile*> Tiles;
};
