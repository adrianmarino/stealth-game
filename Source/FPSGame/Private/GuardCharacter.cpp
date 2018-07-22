// ----------------------------------------------------------------------------
// Headers
// ----------------------------------------------------------------------------
#include "GuardCharacter.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GuardState.h"
#include "GuardIdleState.h"
#include "GuardAlertedState.h"
#include "GuardSuspiciousState.h"
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
AGuardCharacter::AGuardCharacter() {
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(
        TEXT("Perception"));
    PawnSensingComponent->OnSeePawn.AddDynamic(
        this, &AGuardCharacter::OnSeePawnEvent);
    PawnSensingComponent->OnHearNoise.AddDynamic(
        this, &AGuardCharacter::OnHearNoiseEvent);
}
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
void AGuardCharacter::BeginPlay() {
    SetState(EGuardState::Idle);
    Super::BeginPlay();
    OriginalRotator = GetActorRotation();
}

void AGuardCharacter::SetupOriginalOrientation() { 
    SetActorRotation(OriginalRotator);
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
    FRotator LookAtTo = UKismetMathLibrary::FindLookAtRotation(
        GetActorLocation(), 
        Location
    );
    LookAtTo.Pitch = 0;
    LookAtTo.Roll = 0;
    SetActorRotation(LookAtTo);
}

void AGuardCharacter::CallCompleteMission(APawn* Pawn, bool Success) {
    AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if(GameMode) GameMode->CompleteMission(Pawn, Success);
}
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// State maquine
// ----------------------------------------------------------------------------
void AGuardCharacter::SetState(EGuardState NextGuardState) {
    if(GuardState == nullptr) {
        GuardState = NewObject<UGuardIdleState>(this, TEXT("GuardIdleState"));
        UE_LOG(LogTemp, Warning, TEXT("Change to IDLE State!"));
    
    } else if(GuardState->GetType() == NextGuardState) {
        return;

    } else if (EGuardState::Idle == NextGuardState) { 
        GuardState = NewObject<UGuardIdleState>(this, TEXT("GuardIdleState"));
        UE_LOG(LogTemp, Warning, TEXT("Change to IDLE State!"));
    
    } else if(EGuardState::Alerted == NextGuardState) {
        GuardState = NewObject<UGuardAlertedState>(this, TEXT("GuarddAlertedState"));
        UE_LOG(LogTemp, Warning, TEXT("Change to ALERTED State!"));
    
    } else if(EGuardState::Suspicious == NextGuardState) {
        GuardState = NewObject<UGuardSuspiciousState>(this, TEXT("GuardSuspiciousState"));
        UE_LOG(LogTemp, Warning, TEXT("Change to SUSPICIOUS State!"));
    }
    this->OnStateChanged(NextGuardState);
}

void AGuardCharacter::OnHearNoiseEvent(
    APawn *PawnInstigator,
    const FVector &Location,
    float Volume
) {
    if(GuardState == nullptr || PawnInstigator == nullptr) return;
    
    EGuardState Next = GuardState->OnHearNoiseEvent(this, PawnInstigator, Location);
    SetState(Next);
}

void AGuardCharacter::OnSeePawnEvent(APawn *SeePawn) { 
    if(GuardState == nullptr || SeePawn == nullptr) return;
    EGuardState Next = GuardState->OnSeePawnEvent(this, SeePawn);
    SetState(Next);
}

void AGuardCharacter::ResetOrientation() {
    if(GuardState == nullptr) return;
    EGuardState Next = GuardState->ResetOrientation(this);
    SetState(Next);
}
// ----------------------------------------------------------------------------