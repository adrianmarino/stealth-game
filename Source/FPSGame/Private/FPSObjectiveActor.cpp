#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

AFPSObjectiveActor::AFPSObjectiveActor() {
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetupAttachment(MeshComponent);

	SetReplicates(true);
}

void AFPSObjectiveActor::BeginPlay() {
	Super::BeginPlay();
	PlayEffects();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

void AFPSObjectiveActor::PlayEffects() {
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if(Character) {
		Character->SetObjectiveCarried(true);
		PlayEffects();
		UE_LOG(LogTemp, Warning, TEXT("Objective Carried by character!"));
	}
}