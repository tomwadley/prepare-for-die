#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Tile.h"
#include "Die.generated.h"

UCLASS()
class GMTKGAMEJAM2022_API ADie : public AActor
{
	GENERATED_BODY()

public:
	ADie();

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

	FTimeline RollRotationTimeline;
	
	ERollDirection RollDirection;

	void SetPivotPointLocation() const;

	void InitPivotPoint();
};
