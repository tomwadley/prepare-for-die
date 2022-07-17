#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM()
enum EResource { None,One,Two };

UCLASS()
class GMTKGAMEJAM2022_API ATile : public AActor
{
	GENERATED_BODY()

public:
	inline constexpr static double Length = 100.f;
	inline constexpr static double HalfLength = Length / 2.f;
	
	ATile();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EResource> Resource;

	void Init(int32 C, int32 R, bool BT);
	
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ResourceMined();
	
	UFUNCTION(BlueprintNativeEvent)
	void FenceHitByDie();

protected:
	virtual void BeginPlay() override;

private:
	int32 Column;
	int32 Row;
	bool IsBoardTile;

	FTimerHandle ResourceTimer;
	
	void ResourceTimerCallback();
};
