#pragma once

#include "CoreMinimal.h"
#include "GmtkGamejam2022PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GMTKGAMEJAM2022_API AGmtkGamejam2022PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool CanEnterPlaceFenceMode();
	
	UFUNCTION(BlueprintCallable)
	void EnterPlaceFenceMode();

	UFUNCTION(BlueprintCallable)
	void LeavePlaceFenceMode();

	UFUNCTION(BlueprintImplementableEvent)
	void EnteredPlaceFenceMode();

	UFUNCTION(BlueprintImplementableEvent)
	void LeftPlaceFenceMode();

	UFUNCTION(BlueprintCallable)
	bool IsPlaceFenceMode();

private:
	bool PlaceFenceMode;
};
