#include "GuardState.h"

EGuardState IGuardState::ResetOrientation(AGuardCharacter* Character) {
    return GetType();
}

EGuardState IGuardState::OnSeePawnEvent(AGuardCharacter* Character, APawn* Pawn)  {
    return GetType();
}

EGuardState IGuardState::OnHearNoiseEvent(AGuardCharacter* Character, APawn* PawnInstigator, const FVector& Location)  {
    return GetType();
}