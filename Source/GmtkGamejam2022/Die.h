#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Tile.h"
#include "Board.h"
#include "Die.generated.h"

UCLASS()
class GMTKGAMEJAM2022_API ADie : public AActor
{
	GENERATED_BODY()

public:
	ADie();

	void Init(Board* B, int32 C, int32 R);

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* RollRotationCurve;

	enum ERollDirection { North, West, South, East};

	void SetRollDirection(const ERollDirection Direction);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void RollRotationCallback(float Value) const;

	UFUNCTION()
	void RollRotationFinished();
	
	UPROPERTY()
	AActor* PivotPoint;

	UPROPERTY()
	TArray<ATile*> Tiles;

	Board* _Board;

	FTimeline RollRotationTimeline;
	
	ERollDirection RollDirection;

	void SetPivotPointLocation() const;

	void InitPivotPoint();

	UINT GetBottomTileIndex();

	int32 Column;
	int32 Row;
};
