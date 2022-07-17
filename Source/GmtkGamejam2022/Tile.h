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

	void Init(int32 C, int32 R, bool I);

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

private:
	int32 Column;
	int32 Row;
	bool Interactable;
};
