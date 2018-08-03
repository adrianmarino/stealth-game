#include "LaunchPadActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

ALaunchPadActor::ALaunchPadActor() {
	OverlapComponent = this->InitializeOverlap();
	RootComponent = OverlapComponent;
	MeshComponent = this->InitializeMesh(OverlapComponent);

	LaunchPitch = 40;
	LaunchStrength = 1500;
}

UStaticMeshComponent* ALaunchPadActor::InitializeMesh(UBoxComponent* Parent) {
	UStaticMeshComponent* Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Component->SetupAttachment(Parent);
	return Component;
}

UBoxComponent* ALaunchPadActor::InitializeOverlap() {
	UBoxComponent* Component = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap"));
	Component->SetBoxExtent(FVector(50, 50, 15));
	Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap
	);
	Component->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPadActor::HandleOverlapEvent);
	return Component;
}

void ALaunchPadActor::HandleOverlapEvent(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	) {
	if(OtherComp && OtherComp->IsSimulatingPhysics()) {
		OtherComp->AddImpulse(GetLaunchVelocity(), NAME_None, true);
	} else if(OtherActor) {
		AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
		if(Character) {
			Character->LaunchCharacter(GetLaunchVelocity(), true, true);
		}
	} 
}

FVector ALaunchPadActor::GetLaunchVelocity() {
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch = LaunchPitch;
	
	return LaunchDirection.Vector() * LaunchStrength;
}
