#include "Die.h"

#include "GmtkGamejam2022GameModeBase.h"
#include "GameFramework/RotatingMovementComponent.h"


ADie::ADie()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADie::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	RollRotationTimeline.TickTimeline(DeltaTime);
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

void ADie::BeginPlay()
{
	Super::BeginPlay();

	const AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());

	InitPivotPoint();

	TArray TileTransforms = {
		FTransform(FRotator3d(0, 0, 0), FVector(0.f, 0.f, ATile::HalfLength)),
		FTransform(FRotator3d(-90.f, 0, 0), FVector(ATile::HalfLength, 0.f, 0.f)),
		FTransform(FRotator3d(0, 0, 90.f), FVector(0.f, ATile::HalfLength, 0.f)),
		FTransform(FRotator3d(0, 0, -90.f), FVector(0.f, -ATile::HalfLength, 0.f)),
		FTransform(FRotator3d(90.f, 0, 0), FVector(-ATile::HalfLength, 0.f, 0.f)),
		FTransform(FRotator3d(180.f, 0, 0), FVector(0.f, 0.f, -ATile::HalfLength))
	};

	for (FTransform Transform : TileTransforms)
	{
		ATile* Tile = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform));
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

	//// demo below of starting die roll:

	SetRollDirection(West);
	
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
	
	switch (RollDirection)
	{
	case North: LocationOffset = FVector(0.f, -ATile::Length, 0.f);
		break;
	case West: LocationOffset = FVector(-ATile::Length, 0.f, 0.f);
		break;
	case South: LocationOffset = FVector(0.f, ATile::Length, 0.f);
		break;
	case East: LocationOffset = FVector(ATile::Length, 0.f, 0.f);
		break;
	}
	
	SetActorLocation(GetActorLocation() + LocationOffset);
	
	InitPivotPoint();
	SetPivotPointLocation();

	for (ATile* Tile : Tiles)
	{
		Tile->AttachToActor(PivotPoint, FAttachmentTransformRules::KeepWorldTransform);
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
