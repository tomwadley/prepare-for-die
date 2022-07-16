#include "GmtkGamejam2022GameModeBase.h"

TSubclassOf<ATile> AGmtkGamejam2022GameModeBase::RandomTileClass()
{
	const int32 Index = FMath::RandRange(0, TileClasses.Num() - 1);
	return TileClasses[Index];
}

void AGmtkGamejam2022GameModeBase::UpdateCell(const int32 Column, const int32 Row, const TSubclassOf<ATile> TileClass)
{
	if (Column < 0 || Column >= Columns || Row < 0 || Row >= Rows)
	{
		return;
	}
	
	ATile* OldTile = Tiles[Column][Row];
	OldTile->Destroy();
	
	const FVector Location = GetCellLocation(Column, Row);
	ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(TileClass, &Location));
	Tiles[Column][Row] = NewTile;
}

void AGmtkGamejam2022GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TileClasses.Add(EmptyTileClass);
	if (Resource1TileClass != nullptr)
	{
		TileClasses.Add(Resource1TileClass);		
	}
	if (Resource2TileClass != nullptr)
	{
		TileClasses.Add(Resource2TileClass);	
	}
	
	for (int i = 0; i < Columns; i++)
	{
		Tiles.Add(TArray<ATile*>());
		for (int j = 0; j < Rows; j++)
		{
			FVector Location = GetCellLocation(i, j);
			ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(RandomTileClass(), &Location));
			Tiles[i].Add(NewTile);
		}
	}

	SpawnDie(Columns, 1);
}

void AGmtkGamejam2022GameModeBase::SpawnDie(const int32 Column, const int32 Row)
{
	const FVector DieSpawnLocation = GetCellLocation(Column, Row) + FVector(0.f, 0.f, ATile::HalfLength);
	ADie* Die = GetWorld()->SpawnActorDeferred<ADie>(DieClass, FTransform(DieSpawnLocation));
	Die->Init(this, Column, Row);
	Die->FinishSpawning(FTransform(DieSpawnLocation));
	Dies.Add(Die);
}

FVector AGmtkGamejam2022GameModeBase::GetCellLocation(const int X, const int Y) const
{
	const double XOffset = (Columns - 1) * -ATile::HalfLength;
	const double YOffset = (Rows - 1) * -ATile::HalfLength;

	const double XLoc = XOffset + X * ATile::Length;
	const double YLoc = YOffset + Y * ATile::Length;

	return FVector(XLoc, YLoc, 0.f);
}
