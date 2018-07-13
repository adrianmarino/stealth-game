#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "GuardCharacter.h"

AGuardCharacter::AGuardCharacter() {
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Perception"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGuardCharacter::OnSeePawnEvent);
	PawnSensingComponent->OnHearNoise.AddDynamic(this,  &AGuardCharacter::OnHearNoiseEvent);
}

void AGuardCharacter::BeginPlay() {
	Super::BeginPlay();	
}

void AGuardCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AGuardCharacter::OnSeePawnEvent(APawn* SeePawn) {
	if(SeePawn == nullptr) return;

	DrawDebugSphere(
		GetWorld(),
		SeePawn->GetActorLocation(),
		32,
		12,
		FColor::Green,
		false
	);
}

void AGuardCharacter::OnHearNoiseEvent(
	APawn* PawnInstigator, 
	const FVector& Location, 
	float Volume
) {

}