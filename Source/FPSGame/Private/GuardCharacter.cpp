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
    if(!HasState())
        GuardState = NewObject<UGuardIdleState>(this, TEXT("GuardIdleState"));
    else if(GuardState->GetType() == NextGuardState)
        return;
    else if (EGuardState::Idle == NextGuardState)
        GuardState = NewObject<UGuardIdleState>(this, TEXT("GuardIdleState"));
    else if(EGuardState::Alerted == NextGuardState)
        GuardState = NewObject<UGuardAlertedState>(this, TEXT("GuarddAlertedState"));
    else if(EGuardState::Suspicious == NextGuardState)
        GuardState = NewObject<UGuardSuspiciousState>(this, TEXT("GuardSuspiciousState"));

    UE_LOG(LogTemp, Warning, TEXT("Change to %s state!"), *GuardState->GetName());

    this->OnStateChanged(NextGuardState);
}

void AGuardCharacter::OnHearNoiseEvent(
    APawn *PawnInstigator,
    const FVector &Location,
    float Volume
) {
    if(PawnInstigator == nullptr) return;
    ExecTrans([&] { return GuardState->OnHearNoiseEvent(this, PawnInstigator, Location); });
}

void AGuardCharacter::OnSeePawnEvent(APawn *SeePawn) { 
    if(SeePawn == nullptr) return;
    ExecTrans([&] { return GuardState->OnSeePawnEvent(this, SeePawn); });
}

void AGuardCharacter::ResetOrientation() {
    ExecTrans([&] { return GuardState->ResetOrientation(this); });
}

void AGuardCharacter::ExecTrans(std::function<EGuardState()> eval) {
    if(HasState()) SetState(eval());
}
bool AGuardCharacter::HasState() {return GetState() != nullptr; }
IGuardState* AGuardCharacter::GetState() { return GuardState; }
// ----------------------------------------------------------------------------