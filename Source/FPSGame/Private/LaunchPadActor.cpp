#include "LaunchPadActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

ALaunchPadActor::ALaunchPadActor() {
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = MeshComponent;

	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap
	);
	OverlapComponent->SetupAttachment(MeshComponent);
}

void ALaunchPadActor::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	if(!OtherActor) return;

	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if(!Character) return;

	Character->LaunchCharacter(Velocity, false, false);
}
