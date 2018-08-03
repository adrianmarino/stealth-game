#include "GuardState.h"

void IGuardState::Tick(AGuardCharacter* Character, float DeltaTime) {
}

void IGuardState::ResetOrientation(AGuardCharacter* Character) {
}

void IGuardState::OnSeePawnEvent(AGuardCharacter* Character, APawn* Pawn)  {
}

void IGuardState::OnHearNoiseEvent(AGuardCharacter* Character, APawn* PawnInstigator, const FVector& Location)  {
}