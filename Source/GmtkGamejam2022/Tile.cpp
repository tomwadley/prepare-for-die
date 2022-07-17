#include "Tile.h"

#include "GmtkGamejam2022GameModeBase.h"


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATile::Init(int32 C, int32 R, bool BT)
{
	Column = C;
	Row = R;
	IsBoardTile = BT;
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	
	if (!IsBoardTile)
	{
		return;
	}
	
	AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->PlaceFence(Column, Row);
		
	UE_LOG(LogTemp, Warning, TEXT("Tile clicked: %d, %d"), Column, Row);
}

void ATile::FenceHitByDie_Implementation()
{
	Destroy();
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	const AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsBoardTile && Resource != None)
	{
		GetWorldTimerManager().SetTimer(ResourceTimer, this, &ATile::ResourceTimerCallback, GameMode->ResourceMineRate, true);	
	}
}

void ATile::ResourceTimerCallback() const
{
	AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());

	switch (Resource)
	{
	case One: GameMode->MineResource1();
		break;
	case Two: GameMode->MineResource2();
		break;
	default: ;
	}
	
	const EResource R = Resource.GetValue();
	UE_LOG(LogTemp, Warning, TEXT("Got resource %d from tile %d, %d"), R, Column, Row);
}
