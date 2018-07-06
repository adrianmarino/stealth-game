#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZoneActor.generated.h"

class UBoxComponent;
class UDecalComplment;

UCLASS()
class FPSGAME_API AFPSExtractionZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPSExtractionZoneActor();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComponent;

	UFUNCTION()
	void HandleOverlapEvent(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	);
};
