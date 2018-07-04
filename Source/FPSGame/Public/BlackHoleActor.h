#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHoleActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class FPSGAME_API ABlackHoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlackHoleActor();

protected:

	UPROPERTY(EditAnywhere, Category = "Settings")
	float ForceStrength = 2000;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* InnerSphere;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OuterSphere;

	UFUNCTION()
	void OverlapInnerSphereEvent(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:

	virtual void Tick(float DeltaTime) override;
};
