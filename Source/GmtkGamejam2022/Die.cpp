#include "Die.h"

#include "GameFramework/RotatingMovementComponent.h"


ADie::ADie()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADie::Init(Board* B, int32 C, int32 R, ADie::ERollDirection D)
{
	_Board = B;
	Column = C;
	Row = R;
	InitRollDirection = D;
}

void ADie::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	RollRotationTimeline.TickTimeline(DeltaTime);
}

void ADie::Destroyed()
{
	Super::Destroyed();
	
	for (ATile* Tile : Tiles)
	{
		Tile->Destroy();
	}
	if (PivotPoint != nullptr)
	{
		PivotPoint->Destroy();	
	}
}

void ADie::SetRollDirection(const ERollDirection Direction)
{
	for (ATile* Tile : Tiles)
	{
		Tile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	RollDirection = Direction;
	SetPivotPointLocation();

	for (ATile* Tile : Tiles)
	{
		Tile->AttachToActor(PivotPoint, FAttachmentTransformRules::KeepWorldTransform);
	}
}

int32 ADie::GetColumn() const
{
	return Column;
}

int32 ADie::GetRow() const
{
	return Row;
}

void ADie::BeginPlay()
{
	Super::BeginPlay();

	InitPivotPoint();

	TArray TileTransforms = {
		FTransform(FRotator3d(0, 0, 0), FVector(0.f, 0.f, ATile::HalfLength)),
		FTransform(FRotator3d(0, 0, -90.f), FVector(0.f, -ATile::HalfLength, 0.f)),
		FTransform(FRotator3d(90.f, 0, 0), FVector(-ATile::HalfLength, 0.f, 0.f)),
		FTransform(FRotator3d(-90.f, 0, 0), FVector(ATile::HalfLength, 0.f, 0.f)),
		FTransform(FRotator3d(0, 0, 90.f), FVector(0.f, ATile::HalfLength, 0.f)),
		FTransform(FRotator3d(180.f, 0, 0), FVector(0.f, 0.f, -ATile::HalfLength)),
	};

	for (FTransform Transform : TileTransforms)
	{
		ATile* Tile = GetWorld()->SpawnActorDeferred<ATile>(_Board->RandomTileClass(), Transform);
		Tile->Init(-1, -1, false);
		Tile->FinishSpawning(Transform);
		
		Tiles.Add(Tile);
		Tile->AttachToActor(PivotPoint, FAttachmentTransformRules::KeepRelativeTransform);
	}

	FOnTimelineFloat RollRotationCallback;
	RollRotationCallback.BindUFunction(this, FName("RollRotationCallback"));

	if (RollRotationCurve)
	{
		RollRotationTimeline.AddInterpFloat(RollRotationCurve, RollRotationCallback);
	}
	
	FOnTimelineEvent RollRotationFinished;
	RollRotationFinished.BindUFunction(this, FName("RollRotationFinished"));
	RollRotationTimeline.SetTimelineFinishedFunc(RollRotationFinished);
	
	SetRollDirection(InitRollDirection);
	RollRotationTimeline.Play();
}

void ADie::RollRotationCallback(float Value) const
{
	FRotator Rotator;

	switch (RollDirection)
	{
	case North: Rotator = FRotator(0.f, 0.f, -Value);
		break;
	case West: Rotator = FRotator(Value, 0.f, 0.f);
		break;
	case South: Rotator = FRotator(0.f, 0.f, Value);
		break;
	case East: Rotator = FRotator(-Value, 0.f, 0.f);
		break;
	}
	
	PivotPoint->SetActorRotation(Rotator);
}

void ADie::RollRotationFinished()
{
	for (ATile* Tile : Tiles)
	{
		Tile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	FVector LocationOffset;
	int32 ColumnOffset = GetColumnOffset(RollDirection);
	int32 RowOffset = GetRowOffset(RollDirection);
	
	switch (RollDirection)
	{
	case North:
		LocationOffset = FVector(0.f, -ATile::Length, 0.f);
		break;
	case West:
		LocationOffset = FVector(-ATile::Length, 0.f, 0.f);
		break;
	case South:
		LocationOffset = FVector(0.f, ATile::Length, 0.f);
		break;
	case East:
		LocationOffset = FVector(ATile::Length, 0.f, 0.f);
		break;
	}
	
	SetActorLocation(GetActorLocation() + LocationOffset);
	Column += ColumnOffset;
	Row += RowOffset;
	
	InitPivotPoint();
	SetPivotPointLocation();

	for (ATile* Tile : Tiles)
	{
		Tile->AttachToActor(PivotPoint, FAttachmentTransformRules::KeepWorldTransform);
	}
	
	const UINT BottomTileIndex = GetBottomTileIndex();
	UClass* BottomTileClass = Tiles[BottomTileIndex]->GetClass();
	const bool OnBoard = _Board->UpdateCell(Column, Row, BottomTileClass);

	DieLanded(OnBoard);

	int RemainingAttempts = 16;
	while (RemainingAttempts > 0 && _Board->ContainsFence(Column + ColumnOffset, Row + RowOffset))
	{
		_Board->HitFence(Column + ColumnOffset, Row + RowOffset);
		
		switch (RollDirection)
		{
		case North: SetRollDirection(East);
			break;
		case West: SetRollDirection(North);
			break;
		case South: SetRollDirection(West);
			break;
		case East: SetRollDirection(South);
			break;
		}
		ColumnOffset = GetColumnOffset(RollDirection);
		RowOffset = GetRowOffset(RollDirection);
		RemainingAttempts--;
	}

	RollRotationTimeline.PlayFromStart();
}

void ADie::SetPivotPointLocation() const
{
	FVector RelativePivotPoint;

	switch (RollDirection)
	{
	case North: RelativePivotPoint = FVector(0.f, -ATile::HalfLength, -ATile::HalfLength);
		break;
	case West: RelativePivotPoint = FVector(-ATile::HalfLength, 0.f, -ATile::HalfLength);
		break;
	case South: RelativePivotPoint = FVector(0.f, ATile::HalfLength, -ATile::HalfLength);
		break;
	case East: RelativePivotPoint = FVector(ATile::HalfLength, 0.f, -ATile::HalfLength);
		break;
	}
	
	PivotPoint->SetActorLocation(GetActorLocation() + RelativePivotPoint);
}

void ADie::InitPivotPoint()
{
	if (PivotPoint != nullptr)
	{
		PivotPoint->Destroy();	
	}
	PivotPoint = GetWorld()->SpawnActor(AActor::StaticClass());
	PivotPoint->SetRootComponent(NewObject<USceneComponent>(PivotPoint));
	PivotPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

UINT ADie::GetBottomTileIndex()
{
	UINT Result = 0;
	double LowestZ = TNumericLimits<double>::Max();
	
	for (int i = 0; i < Tiles.Num(); i++)
	{
		const ATile* Tile = Tiles[i];
		if (const double Z = Tile->GetTransform().GetLocation().Z; Z < LowestZ)
		{
			Result = i;
			LowestZ = Z;
		}
	}

	return Result;
}

int32 ADie::GetColumnOffset(const ERollDirection RollDirection)
{
	switch (RollDirection)
	{
	case West:
		return -1;
	case East:
		return 1;
	default:
		return 0;
	}
}

int32 ADie::GetRowOffset(const ERollDirection RollDirection)
{
	switch (RollDirection)
	{
	case North:
		return -1;
	case South:
		return 1;
	default:
		return 0;
	}
}
