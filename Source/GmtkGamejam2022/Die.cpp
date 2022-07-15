#include "Die.h"

#include "GmtkGamejam2022GameModeBase.h"


ADie::ADie()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADie::BeginPlay()
{
	Super::BeginPlay();

	const AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());
	
	const FTransform Transform1 = FTransform(FRotator3d(0, 0, 0), FVector(0.f, 0.f, 50.f));
	const FTransform Transform6 = FTransform(FRotator3d(180.f, 0, 0), FVector(0.f, 0.f, -50.f));
	const FTransform Transform2 = FTransform(FRotator3d(-90.f, 0, 0), FVector(50.f, 0.f, 0.f));
	const FTransform Transform5 = FTransform(FRotator3d(90.f, 0, 0), FVector(-50.f, 0.f, 0.f));
	const FTransform Transform3 = FTransform(FRotator3d(0, 0, 90.f), FVector(0.f, 50.f, 0.f));
	const FTransform Transform4 = FTransform(FRotator3d(0, 0, -90.f), FVector(0.f, -50.f, 0.f));
	
	Side1 = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform1));
	Side1->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	Side2 = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform2));
	Side2->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	Side3 = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform3));
	Side3->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	Side4 = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform4));
	Side4->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	Side5 = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform5));
	Side5->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	
	Side6 = Cast<ATile>(GetWorld()->SpawnActor(GameMode->EmptyTileClass, &Transform6));
	Side6->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}
