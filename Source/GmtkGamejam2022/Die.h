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

	enum ERollDirection { North, West, South, East};

	void Init(Board* B, int32 C, int32 R, ERollDirection D);

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* RollRotationCurve;

	int32 GetColumn() const;
	int32 GetRow() const;

	UFUNCTION(BlueprintImplementableEvent)
	void DieLanded(bool OnBoard);

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
	ERollDirection InitRollDirection;

	void SetRollDirection(const ERollDirection Direction);

	void SetPivotPointLocation() const;

	void InitPivotPoint();

	UINT GetBottomTileIndex();

	int32 Column;
	int32 Row;

	static int32 GetColumnOffset(ERollDirection RollDirection);
	static int32 GetRowOffset(ERollDirection RollDirection);
};
