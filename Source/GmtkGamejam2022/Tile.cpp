#include "Tile.h"

#include "GmtkGamejam2022GameModeBase.h"


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATile::Init(int32 C, int32 R, bool I)
{
	Column = C;
	Row = R;
	Interactable = I;
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	
	if (!Interactable)
	{
		return;
	}
	
	AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->PlaceFence(Column, Row);
		
	UE_LOG(LogTemp, Warning, TEXT("Tile clicked: %d, %d"), Column, Row);
}
