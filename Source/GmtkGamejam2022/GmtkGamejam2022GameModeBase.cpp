#include "GmtkGamejam2022GameModeBase.h"

void AGmtkGamejam2022GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	const double XOffset = (Columns - 1) * -ATile::HalfLength;
	const double YOffset = (Rows - 1) * -ATile::HalfLength;

	for (int i = 0; i < Columns; i++)
	{
		Tiles.Add(TArray<ATile*>());
		for (int j = 0; j < Rows; j++)
		{
			const double X = XOffset + i * ATile::Length;
			const double Y = YOffset + j * ATile::Length;
			const FVector Location = FVector(X, Y, 0.f);
			ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(EmptyTileClass, &Location));
			Tiles[i].Add(NewTile);
		}
	}

	SpawnDie();
}

void AGmtkGamejam2022GameModeBase::SpawnDie()
{
	const FVector DieLocation = FVector(200.f, 200.f, 200.f);
	Dies.Add(Cast<ADie>(GetWorld()->SpawnActor(DieClass, &DieLocation)));
}
