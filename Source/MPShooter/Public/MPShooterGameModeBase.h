// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MPShooterGameModeBase.generated.h"

/**
 * No se replica, se encuentra únicamente en el servidor. (GameMode)
 */
UCLASS()
class MPSHOOTER_API AMPShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	TArray<FLinearColor> PlayerColors;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Players")
	int32 LastPlayerColorIndex;

public:
	AMPShooterGameModeBase(const FObjectInitializer& ObjectInitializer);

	// El método se ejecuta por cada jugador que se conecte.
	virtual void SetPlayerDefaults(class APawn* PlayerPawn) override;
};
