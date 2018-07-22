#include "GuardAlertedState.h"

UGuardAlertedState::UGuardAlertedState() {}

EGuardState UGuardAlertedState::GetType() { return EGuardState::Alerted; }