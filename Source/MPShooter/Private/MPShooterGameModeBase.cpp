// Fill out your copyright notice in the Description page of Project Settings.

#include "MPShooterGameModeBase.h"
#include "ShooterPawn.h"

AMPShooterGameModeBase::AMPShooterGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	PlayerColors.Add(FLinearColor(0.30f, 0.02f, 0.02f));
	PlayerColors.Add(FLinearColor(0.05, 0.02f, 0.02f));
	PlayerColors.Add(FLinearColor(0.02f, 0.30f, 0.02f));
	PlayerColors.Add(FLinearColor(0.02f, 0.02f, 0.7f));
	LastPlayerColorIndex = -1;

	DefaultPawnClass = AShooterPawn::StaticClass();
}

void AMPShooterGameModeBase::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);

	AShooterPawn* ShooterPawn = Cast<AShooterPawn>(PlayerPawn);
	if (ShooterPawn) {

		const int32 PlayerColorIndex = (LastPlayerColorIndex + 1) % PlayerColors.Num();
		if (PlayerColors.IsValidIndex(PlayerColorIndex)) {
			ShooterPawn->AuthSetColor(PlayerColors[PlayerColorIndex]);
			LastPlayerColorIndex = PlayerColorIndex;
		}
	}
}