#include "FPSExtractionZoneActor.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

AFPSExtractionZoneActor::AFPSExtractionZoneActor() {
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Pawn, 
		ECollisionResponse::ECR_Overlap
	);
	OverlapComponent->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComponent;

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&AFPSExtractionZoneActor::HandleOverlapEvent
	);
	OverlapComponent->SetHiddenInGame(false);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->RelativeLocation = FVector::ZeroVector;
	DecalComponent->DecalSize = FVector(200.f);
}

void AFPSExtractionZoneActor::HandleOverlapEvent(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	) {
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if(!(Character && Character->GetObjectiveCarried())) return;

	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode) GameMode->CompleteMission(Character);
}
