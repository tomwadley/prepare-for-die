#include "Die.h"

#include "GmtkGamejam2022GameModeBase.h"
#include "GameFramework/RotatingMovementComponent.h"


ADie::ADie()
{
	PrimaryActorTick.bCanEverTick = false;
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

	URotatingMovementComponent* Rotator = NewObject<URotatingMovementComponent>(PivotPoint);
	Rotator->RotationRate = FRotator(30.f, 0.f, 0.f);
	Rotator->RegisterComponent();
	Rotator->Activate();
	PivotPoint->AddInstanceComponent(Rotator);
}
