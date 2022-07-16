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

void ADie::BeginPlay()
{
	Super::BeginPlay();

	const AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());

	PivotPoint = GetWorld()->SpawnActor(AActor::StaticClass());
	PivotPoint->SetRootComponent(NewObject<USceneComponent>(PivotPoint));
	PivotPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	TArray<FTransform> TileTransforms = {
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

	//// demo below of setting the pivot point and rotating:
	
	for (ATile* Tile : Tiles)
	{
		Tile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	PivotPoint->SetActorLocation(GetActorLocation() + FVector(-50.f, -50.f, -50.f));

	for (ATile* Tile : Tiles)
	{
		Tile->AttachToActor(PivotPoint, FAttachmentTransformRules::KeepWorldTransform);
	}

	RollRotationTimeline.Play();
}

void ADie::RollRotationCallback(float Value) const
{
	const FRotator Rotator = FRotator(Value, 0.f, 0.f);
	PivotPoint->SetActorRotation(Rotator);
}

void ADie::RollRotationFinished()
{
}
