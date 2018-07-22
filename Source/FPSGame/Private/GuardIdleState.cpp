#include "GuardIdleState.h"
#include "GuardCharacter.h"
#include "GuardStateEnum.h"
#include "FPSGameMode.h"
#include "DebugUtils.h"

UGuardIdleState::UGuardIdleState() {}

EGuardState UGuardIdleState::GetType() { return EGuardState::Idle; }

FString UGuardIdleState::GetName() { return GetClass()->GetFName().ToString(); }

EGuardState UGuardIdleState::OnSeePawnEvent(
    AGuardCharacter* Character,
    APawn* SeePawn
) {
    DebugUtils::ShowSphereIn(
        Character->GetWorld(), 
        SeePawn->GetActorLocation(),
        FColor::Red
    );
    Character->CallCompleteMission(SeePawn, false);
   return EGuardState::Alerted;
}

EGuardState UGuardIdleState::OnHearNoiseEvent(
    AGuardCharacter* Character, 
    APawn* PawnInstigator, 
    const FVector& Location
) {
    DebugUtils::ShowSphereIn(Character->GetWorld(), Location, FColor::Blue);
    Character->RotateTo(Location);
    Character->StartResetOrientation();
    return EGuardState::Suspicious;
}