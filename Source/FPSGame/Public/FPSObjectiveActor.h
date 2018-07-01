#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:
	AFPSObjectiveActor();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* PickupFX;

	virtual void BeginPlay() override;

	void PlayEffects();

public:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};