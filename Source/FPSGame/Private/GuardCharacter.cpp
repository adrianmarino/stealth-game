#include "GuardCharacter.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AGuardCharacter::AGuardCharacter() {
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Perception"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGuardCharacter::OnSeePawnEvent);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AGuardCharacter::OnHearNoiseEvent);

    State = EGuardState::Idel;
}

void AGuardCharacter::SetState(EGuardState NextState) {
    if(NextState == State) return;
    State = NextState;
    this->OnStateChanged(State);
}

void AGuardCharacter::BeginPlay() {
    Super::BeginPlay();
    OriginalRotator = GetActorRotation();
}

void AGuardCharacter::OnSeePawnEvent(APawn *SeePawn) {
    if (SeePawn == nullptr) return;
    this->ShowSphereIn(SeePawn->GetActorLocation(), FColor::Red);

    AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if(GameMode) GameMode->CompleteMission(SeePawn, false);

    this->SetState(EGuardState::Alerted);
}

void AGuardCharacter::OnHearNoiseEvent(
    APawn *PawnInstigator,
    const FVector &Location,
    float Volume
) {
    if(State == EGuardState::Alerted) return;

    if (PawnInstigator == nullptr) return;
    this->ShowSphereIn(Location, FColor::Blue);
    this->RotateTo(Location);
    this->StartResetOrientation();

    this->SetState(EGuardState::Suspicious);
}

void AGuardCharacter::ResetOrientation() {
    if(State == EGuardState::Alerted) return;
    SetActorRotation(OriginalRotator);
    this->SetState(EGuardState::Idel);    
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

void AGuardCharacter::StartResetOrientation() {
    GetWorldTimerManager().ClearTimer(ResetRotationTimer);
    GetWorldTimerManager().SetTimer(
        ResetRotationTimer, 
        this,
        &AGuardCharacter::ResetOrientation,
        3
    );
}

void AGuardCharacter::RotateTo(FVector Location) {
    FRotator LookAtTo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location);
    LookAtTo.Pitch = 0;
    LookAtTo.Roll = 0;
    SetActorRotation(LookAtTo);
}