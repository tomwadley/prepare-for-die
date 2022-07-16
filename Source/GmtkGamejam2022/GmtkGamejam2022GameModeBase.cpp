#include "GmtkGamejam2022GameModeBase.h"

void AGmtkGamejam2022GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < Columns; i++)
	{
		Tiles.Add(TArray<ATile*>());
		for (int j = 0; j < Rows; j++)
		{
			FVector Location = GetCellLocation(i, j);
			ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(EmptyTileClass, &Location));
			Tiles[i].Add(NewTile);
		}
	}

	SpawnDie();
}

void AGmtkGamejam2022GameModeBase::SpawnDie()
{
	const FVector DieSpawnLocation = GetCellLocation(Columns, 1) + FVector(0.f, 0.f, ATile::HalfLength);
	Dies.Add(Cast<ADie>(GetWorld()->SpawnActor(DieClass, &DieSpawnLocation)));
}

FVector AGmtkGamejam2022GameModeBase::GetCellLocation(const int X, const int Y) const
{
	const double XOffset = (Columns - 1) * -ATile::HalfLength;
	const double YOffset = (Rows - 1) * -ATile::HalfLength;

	const double XLoc = XOffset + X * ATile::Length;
	const double YLoc = YOffset + Y * ATile::Length;

	return FVector(XLoc, YLoc, 0.f);
}
