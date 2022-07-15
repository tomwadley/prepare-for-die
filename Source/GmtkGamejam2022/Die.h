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
	ATile* Side1;

	UPROPERTY()
	ATile* Side2;

	UPROPERTY()
	ATile* Side3;

	UPROPERTY()
	ATile* Side4;

	UPROPERTY()
	ATile* Side5;

	UPROPERTY()
	ATile* Side6;
};
