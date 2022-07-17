#include "GmtkGamejam2022GameModeBase.h"

#include "GmtkGamejam2022PlayerController.h"

TSubclassOf<ATile> AGmtkGamejam2022GameModeBase::RandomTileClass()
{
	TSubclassOf<ATile> TileClass;
	
	float R = FMath::RandRange(0.f, 1.f);
	if (R < Resource1TileProbability)
	{
		TileClass = Resource1TileClass;
	}
	else if (R >= Resource1TileProbability && R < Resource1TileProbability + Resource2TileProbability)
	{
		TileClass = Resource2TileClass;
	}
	else
	{
		TileClass = EmptyTileClass;
	}

	return TileClass;
}

bool AGmtkGamejam2022GameModeBase::UpdateCell(const int32 Column, const int32 Row, const TSubclassOf<ATile> TileClass)
{
	bool Result = false;
	
	if (Column >= 0 && Column < Columns && Row >= 0 && Row < Rows)
	{
		ATile* OldTile = Tiles[Column][Row];
		OldTile->Destroy();

		const FVector Location = GetCellLocation(Column, Row);
		
		ATile* NewTile = GetWorld()->SpawnActorDeferred<ATile>(TileClass, FTransform(Location));
		NewTile->Init(Column, Row, true);
		NewTile->FinishSpawning(FTransform(Location));
		
		Tiles[Column][Row] = NewTile;

		Result = true;
	}
	
	DieLifecycleMgmt();
	return Result;
}

bool AGmtkGamejam2022GameModeBase::ContainsFence(const int32 Column, const int32 Row)
{
	if (Column >= 0 && Column < Columns && Row >= 0 && Row < Rows)
	{
		return Fences[Column][Row] != nullptr;
	}
	return false;
}

void AGmtkGamejam2022GameModeBase::HitFence(int32 Column, int32 Row)
{
	if (Column >= 0 && Column < Columns && Row >= 0 && Row < Rows)
	{
		if (ATile* Fence = Fences[Column][Row]; Fence != nullptr)
		{
			Fence->FenceHitByDie();
			Fences[Column][Row] = nullptr;
		}
	}
}

void AGmtkGamejam2022GameModeBase::BuyFence(int32 Column, int32 Row)
{
	if (Column < 0 || Column >= Columns || Row < 0 || Row >= Rows)
	{
		return;
	}
	if (Fences[Column][Row] != nullptr)
	{
		return;
	}
	if (Resource1 < Resource1FenceCost)
	{
		return;
	}

	Resource1 -= Resource1FenceCost;

	const FVector Location = GetCellLocation(Column, Row);
	ATile* FenceTile = Cast<ATile>(GetWorld()->SpawnActor(FenceTileClass, &Location));
	Fences[Column][Row] = FenceTile;

	AGmtkGamejam2022PlayerController* PlayerController = Cast<AGmtkGamejam2022PlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->LeavePlaceFenceMode();
}

void AGmtkGamejam2022GameModeBase::MineResource1()
{
	Resource1 += ResourceMineQty;
}

void AGmtkGamejam2022GameModeBase::MineResource2()
{
	Resource2 += ResourceMineQty;
	if (Resource2 >= Resource2WinCost)
	{
		AGmtkGamejam2022PlayerController* PlayerController = Cast<AGmtkGamejam2022PlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->GameWon();
	}
}

int32 AGmtkGamejam2022GameModeBase::GetResource1() const
{
	return Resource1;
}

int32 AGmtkGamejam2022GameModeBase::GetResource2() const
{
	return Resource2;
}

void AGmtkGamejam2022GameModeBase::StartGame()
{
	DieLifecycleMgmt();
}

void AGmtkGamejam2022GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < Columns; i++)
	{
		Tiles.Add(TArray<ATile*>());
		Fences.Add(TArray<ATile*>());
		for (int j = 0; j < Rows; j++)
		{
			FVector Location = GetCellLocation(i, j);
			
			ATile* NewTile = GetWorld()->SpawnActorDeferred<ATile>(RandomTileClass(), FTransform(Location));
			NewTile->Init(i, j, true);
			NewTile->FinishSpawning(FTransform(Location));
			
			Tiles[i].Add(NewTile);

			ATile* FenceTile = nullptr;
			if (FMath::RandRange(0, 4) == 0) // 20% chance for a fence
			{
				FenceTile = Cast<ATile>(GetWorld()->SpawnActor(FenceTileClass, &Location));
			}
			Fences[i].Add(FenceTile);
		}
	}
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

	if (Resource2 >= Resource2WinCost)
	{
		return;
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
