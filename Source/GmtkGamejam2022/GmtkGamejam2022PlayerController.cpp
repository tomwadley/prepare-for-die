#include "GmtkGamejam2022PlayerController.h"

#include "GmtkGamejam2022GameModeBase.h"

bool AGmtkGamejam2022PlayerController::CanEnterPlaceFenceMode()
{
	if (PlaceFenceMode)
	{
		return false;
	}

	const AGmtkGamejam2022GameModeBase* GameMode = Cast<AGmtkGamejam2022GameModeBase>(GetWorld()->GetAuthGameMode());
	
	return GameMode->GetResource1() >= GameMode->Resource1FenceCost;
}

void AGmtkGamejam2022PlayerController::EnterPlaceFenceMode()
{
	if (!CanEnterPlaceFenceMode())
	{
		return;
	}
	
	PlaceFenceMode = true;
	EnteredPlaceFenceMode();
}

void AGmtkGamejam2022PlayerController::LeavePlaceFenceMode()
{
	PlaceFenceMode = false;
	LeftPlaceFenceMode();
}

bool AGmtkGamejam2022PlayerController::IsPlaceFenceMode()
{
	return PlaceFenceMode;
}
