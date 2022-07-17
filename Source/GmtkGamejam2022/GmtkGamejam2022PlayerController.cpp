#include "GmtkGamejam2022PlayerController.h"

void AGmtkGamejam2022PlayerController::EnterPlaceFenceMode()
{
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
