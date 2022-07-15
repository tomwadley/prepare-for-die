#include "GmtkGamejam2022GameModeBase.h"

void AGmtkGamejam2022GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 4; i++)
	{
		Tiles.Add(TArray<ATile*>());
		for (int j = 0; j < 3; j++)
		{
			const double X = i * 100.f;
			const double Y = j * 100.f;
			const FVector Location = FVector(X, Y, 0.f);
			ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(EmptyTile, &Location));
			Tiles[i].Add(NewTile);
		}
	}
}
