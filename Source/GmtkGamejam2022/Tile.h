#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class GMTKGAMEJAM2022_API ATile : public AActor
{
	GENERATED_BODY()

public:
	inline constexpr static double Length = 100.f;
	inline constexpr static double HalfLength = Length / 2.f;
	
	ATile();
};
