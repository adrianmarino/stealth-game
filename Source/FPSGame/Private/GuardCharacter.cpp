// ----------------------------------------------------------------------------
// Headers
// ----------------------------------------------------------------------------
#include "GuardCharacter.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GuardState.h"
#include "GuardIdleWalkingState.h"
#include "GuardAlertedState.h"
#include "GuardSuspiciousState.h"
#include "VectorUtils.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
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
    SetState(EGuardState::IdleWalking);
    Super::BeginPlay();
    OriginalRotator = GetActorRotation();
    MoveToNextPatrolPoint();
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

void AGuardCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    GuardState->Tick(this, DeltaTime);
}
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// Patrol
// ----------------------------------------------------------------------------
float AGuardCharacter::DistanceToPoint(AActor* Point) {
    return VectorUtils::DistanceBetween(GetActorLocation(), Point->GetActorLocation()); 
}

void AGuardCharacter::TurnPatrolDirection() {
    if(!enablePatrol) return;

    if(DistanceToPoint(NextPatrolPoint) >= 100) return;

    if(NextPatrolPoint == SecondPatrolPoint) {
        NextPatrolPoint = FirstPatrolPoint;
    } else if(NextPatrolPoint == FirstPatrolPoint) {
        NextPatrolPoint = SecondPatrolPoint;
    }

    UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), NextPatrolPoint);
}

void AGuardCharacter::MoveToNextPatrolPoint() {
    if(!enablePatrol) return;
    if(NextPatrolPoint == nullptr) NextPatrolPoint = FirstPatrolPoint;

    UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), NextPatrolPoint);
}

void AGuardCharacter::Play() { MoveToNextPatrolPoint(); }
void AGuardCharacter::Pause() { Controller->StopMovement(); }
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// State maquine
// ----------------------------------------------------------------------------
void AGuardCharacter::SetState(EGuardState NextGuardState) {
    if (EGuardState::IdleWalking == NextGuardState)
        GuardState = NewObject<UGuardIdleWalkingState>(this, TEXT("IdleWalking"));
    else if(EGuardState::Alerted == NextGuardState)
         GuardState = NewObject<UGuardAlertedState>(this, TEXT("Alerted"));
    else if(EGuardState::Suspicious == NextGuardState)
        GuardState = NewObject<UGuardSuspiciousState>(this, TEXT("Suspicious"));

    UE_LOG(LogTemp, Warning, TEXT("Change to %s state!"), *GuardState->GetName());

    this->OnStateChanged(NextGuardState);
}

void AGuardCharacter::OnHearNoiseEvent(
    APawn *PawnInstigator,
    const FVector &Location,
    float Volume
) {
    if(PawnInstigator == nullptr) return;
    GuardState->OnHearNoiseEvent(this, PawnInstigator, Location);
}

void AGuardCharacter::OnSeePawnEvent(APawn *SeePawn) { 
    if(SeePawn == nullptr) return;
    GuardState->OnSeePawnEvent(this, SeePawn);
}

void AGuardCharacter::ResetOrientation() { GuardState->ResetOrientation(this); }
// ----------------------------------------------------------------------------