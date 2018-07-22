#include "GuardAlertedState.h"

UGuardAlertedState::UGuardAlertedState() {}

EGuardState UGuardAlertedState::GetType() { return EGuardState::Alerted; }

FString UGuardAlertedState::GetName() { return GetClass()->GetFName().ToString(); }