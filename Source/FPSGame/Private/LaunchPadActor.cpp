#include "LaunchPadActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

ALaunchPadActor::ALaunchPadActor() {
	MeshComponent = this->InitializeMesh();
	RootComponent = MeshComponent;
	OverlapComponent = this->InitializeOverlap(MeshComponent);

	LaunchPitch = 40;
	LaunchStrength = 1500;
}

UStaticMeshComponent* ALaunchPadActor::InitializeMesh() {
	UStaticMeshComponent* Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	return Component;
}

UBoxComponent* ALaunchPadActor::InitializeOverlap(UStaticMeshComponent* Mesh) {
	UBoxComponent* Component = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap"));
	Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap
	);
	Component->SetupAttachment(Mesh);
	return Component;
}

void ALaunchPadActor::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	if(!OtherActor) return;

	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if(!Character) return;

	Character->LaunchCharacter(GetLaunchVelocity(), true, true);
}

FVector ALaunchPadActor::GetLaunchVelocity() {
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch = LaunchPitch;
	
	return LaunchDirection.Vector() * LaunchStrength;
}
