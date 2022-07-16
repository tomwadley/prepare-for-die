#include "GmtkGamejam2022GameModeBase.h"

void AGmtkGamejam2022GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < Rows; i++)
	{
		Tiles.Add(TArray<ATile*>());
		for (int j = 0; j < Columns; j++)
		{
			const double X = i * ATile::Length;
			const double Y = j * ATile::Length;
			const FVector Location = FVector(X, Y, 0.f);
			ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(EmptyTileClass, &Location));
			Tiles[i].Add(NewTile);
		}
	}

	SpawnDie();
}

void AGmtkGamejam2022GameModeBase::SpawnDie()
{
	const FVector DieLocation = FVector(200.f, 200.f, 400.f);
	Dies.Add(Cast<ADie>(GetWorld()->SpawnActor(DieClass, &DieLocation)));
}
