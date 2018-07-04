#include "BlackHoleActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ABlackHoleActor::ABlackHoleActor() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Mesh;

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphere"));
	InnerSphere->SetSphereRadius(100);
	InnerSphere->SetupAttachment(Mesh);
	InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleActor::OverlapInnerSphereEvent);

	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphere"));
	OuterSphere->SetSphereRadius(3000);
	OuterSphere->SetupAttachment(Mesh);
}

void ABlackHoleActor::OverlapInnerSphereEvent(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if(OtherActor) OtherActor->Destroy();
}

void ABlackHoleActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappigComponents;
	OuterSphere->GetOverlappingComponents(OverlappigComponents);

	for (auto& It : OverlappigComponents) {
		if(It && It->IsSimulatingPhysics()) {
			It->AddRadialForce(
				GetActorLocation(), 
				OuterSphere->GetScaledSphereRadius(), 
				-ForceStrength, 
				ERadialImpulseFalloff::RIF_Constant, 
				true
			);
		}
	}
}

