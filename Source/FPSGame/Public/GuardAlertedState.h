#pragma once

#include "CoreMinimal.h"
#include "GuardState.h"
#include "UObject/NoExportTypes.h"
#include "GuardAlertedState.generated.h"

UCLASS()
class FPSGAME_API UGuardAlertedState : public UObject, public IGuardState
{
	GENERATED_BODY()

	UGuardAlertedState();

	virtual EGuardState GetType() override;

	virtual FString GetName() override;
};