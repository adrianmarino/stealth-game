#include "GuardCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AGuardCharacter::AGuardCharacter() {
    PrimaryActorTick.bCanEverTick = true;
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Perception"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGuardCharacter::OnSeePawnEvent);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AGuardCharacter::OnHearNoiseEvent);
}

void AGuardCharacter::BeginPlay() {
    Super::BeginPlay();
    OriginalRotator = GetActorRotation();
}

void AGuardCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AGuardCharacter::OnSeePawnEvent(APawn *SeePawn) {
    if (SeePawn == nullptr) return;
    ShowSphereIn(SeePawn->GetActorLocation(), FColor::Red);
}

void AGuardCharacter::OnHearNoiseEvent(
        APawn *PawnInstigator,
        const FVector &Location,
        float Volume
) {
    if (PawnInstigator == nullptr) return;
    ShowSphereIn(Location, FColor::Blue);
    this->RotateTo(Location);

    GetWorldTimerManager().ClearTimer(ResetRotationTimer);
    GetWorldTimerManager().SetTimer(
        ResetRotationTimer, 
        this,
        &AGuardCharacter::ResetOrientation,
        3
    );
}

void AGuardCharacter::ResetOrientation() {
    SetActorRotation(OriginalRotator);
}

void AGuardCharacter::ShowSphereIn(FVector Location, FColor Color) {
        DrawDebugSphere(
            GetWorld(),
            Location,
            32.0f,
            30,
            Color,
            false,
            10.0f
    );
}

void AGuardCharacter::RotateTo(FVector Location) {
    FRotator LookAtTo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location);
    LookAtTo.Pitch = 0;
    LookAtTo.Roll = 0;
    SetActorRotation(LookAtTo);
}