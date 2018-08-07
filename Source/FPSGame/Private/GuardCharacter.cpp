// ----------------------------------------------------------------------------
// Headers
// ----------------------------------------------------------------------------
#include "GuardCharacter.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "VectorUtils.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
AGuardCharacter::AGuardCharacter() {
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Perception"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGuardCharacter::OnSeePawnEvent);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AGuardCharacter::OnHearNoiseEvent);

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

    CurrentState = EGuardState::IdleWalking;
}
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
void AGuardCharacter::BeginPlay() {
    Super::BeginPlay();
    OriginalRotator = GetActorRotation();
    MoveToNextPatrolPoint();
}

void AGuardCharacter::SetupOriginalOrientation() { SetActorRotation(OriginalRotator); }

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

void AGuardCharacter::CallCompleteMission(APawn* Pawn, bool Success) {
    AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if(GameMode) GameMode->CompleteMission(Pawn, Success);
}

void AGuardCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    TurnPatrolDirection();
}

void AGuardCharacter::SetState(EGuardState NextGuardState) {
    CurrentState = NextGuardState;
    OnRep_GuardState();
}

void AGuardCharacter::OnHearNoiseEvent(
    APawn *PawnInstigator,
    const FVector &Location,
    float Volume
) {
    if(PawnInstigator == nullptr) return;

    RotateTo(Location);
    StartResetOrientation();

    if(EGuardState::IdleWalking == CurrentState) {
        Pause();
        SetState(EGuardState::Suspicious);
    }
}

void AGuardCharacter::OnSeePawnEvent(APawn *SeePawn) {
    if(
        SeePawn != nullptr && 
        (
            EGuardState::Alerted == CurrentState ||
            EGuardState::IdleWalking == CurrentState
        )
    ) {
        SetState(EGuardState::Alerted);
        Pause();
        CallCompleteMission(SeePawn, false);
    }
}

void AGuardCharacter::ResetOrientation() { 
    SetupOriginalOrientation();
    Play();
    SetState(EGuardState::IdleWalking);
}

void AGuardCharacter::OnRep_GuardState() {
    OnStateChanged(CurrentState);
}

void AGuardCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGuardCharacter, CurrentState);
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