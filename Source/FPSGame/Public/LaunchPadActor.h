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

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (MakeEditWidget = true))
	FVector Velocity;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
