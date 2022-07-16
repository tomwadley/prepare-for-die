#include "GmtkGamejam2022GameModeBase.h"

TSubclassOf<ATile> AGmtkGamejam2022GameModeBase::RandomTileClass()
{
	const int32 Index = FMath::RandRange(0, TileClasses.Num() - 1);
	return TileClasses[Index];
}

void AGmtkGamejam2022GameModeBase::UpdateCell(const int32 Column, const int32 Row, const TSubclassOf<ATile> TileClass)
{
	if (Column >= 0 && Column < Columns && Row >= 0 && Row < Rows)
	{
		ATile* OldTile = Tiles[Column][Row];
		OldTile->Destroy();

		const FVector Location = GetCellLocation(Column, Row);
		ATile* NewTile = Cast<ATile>(GetWorld()->SpawnActor(TileClass, &Location));
		Tiles[Column][Row] = NewTile;
	}
	
	DieLifecycleMgmt();
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

	DieLifecycleMgmt();
}

void AGmtkGamejam2022GameModeBase::SpawnDie(const int32 Column, const int32 Row, const ADie::ERollDirection RollDirection)
{
	const FVector DieSpawnLocation = GetCellLocation(Column, Row) + FVector(0.f, 0.f, ATile::HalfLength);
	Die = GetWorld()->SpawnActorDeferred<ADie>(DieClass, FTransform(DieSpawnLocation));
	Die->Init(this, Column, Row, RollDirection);
	Die->FinishSpawning(FTransform(DieSpawnLocation));
}

FVector AGmtkGamejam2022GameModeBase::GetCellLocation(const int X, const int Y) const
{
	const double XOffset = (Columns - 1) * -ATile::HalfLength;
	const double YOffset = (Rows - 1) * -ATile::HalfLength;

	const double XLoc = XOffset + X * ATile::Length;
	const double YLoc = YOffset + Y * ATile::Length;

	return FVector(XLoc, YLoc, 0.f);
}

void AGmtkGamejam2022GameModeBase::DieLifecycleMgmt()
{
	if (Die != nullptr &&
		(
			Die->GetColumn() >= Columns + OutOfBoundSize
			|| Die->GetColumn() < -OutOfBoundSize
			|| Die->GetRow() >= Rows + OutOfBoundSize
			|| Die->GetRow() < -OutOfBoundSize))
	{
		Die->Destroy();
		Die = nullptr;
	}

	if (Die == nullptr)
	{
		int32 Column;
		int32 Row;
		ADie::ERollDirection RollDirection;
		
		const bool Vertical = FMath::RandBool();
		const bool Increasing = FMath::RandBool();
		if (Vertical)
		{
			Column = FMath::RandRange(0, Columns - 1);
			if (Increasing)
			{
				Row = -OutOfBoundSize - 1;
				RollDirection = ADie::South;
			}
			else
			{
				Row = Rows + OutOfBoundSize;
				RollDirection = ADie::North;
			}
		}
		else
		{
			Row = FMath::RandRange(0, Rows - 1);
			if (Increasing)
			{
				Column = -OutOfBoundSize - 1;
				RollDirection = ADie::East;
			}
			else
			{
				Column = Columns + OutOfBoundSize;
				RollDirection = ADie::West;
			}
		}
		
		SpawnDie(Column, Row, RollDirection);
	}
}
