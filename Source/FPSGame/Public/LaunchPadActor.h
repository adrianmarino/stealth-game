#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPadActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
struct FVector;

UCLASS()
class FPSGAME_API ALaunchPadActor : public AActor {
	GENERATED_BODY()

public:	
	ALaunchPadActor();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComponent;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
	float LaunchPitch;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
	float LaunchStrength;

	FVector GetLaunchVelocity();

	UStaticMeshComponent* InitializeMesh(UBoxComponent* Parent);

	UBoxComponent* InitializeOverlap();

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
