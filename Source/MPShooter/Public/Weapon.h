// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponFirePacket
{
	GENERATED_BODY();

	UPROPERTY()
	float ServerFireTime;

	UPROPERTY()
	bool bCausedDamage;

	UPROPERTY()
	FVector_NetQuantize ImpactPoint;

	UPROPERTY()
	FVector_NetQuantizeNormal ImpactNormal;
};


UCLASS()
class MPSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	float AimInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	float DropInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	FRotator DropRotation;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Aiming|State")
	FVector AimLocation;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Aiming|State")
	bool bAimLocationIsValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
	float FireCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
	float LastFireTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
	class UNiagaraSystem* FireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
	class UNiagaraSystem* ImpactEffect;

	UPROPERTY(ReplicatedUsing = OnRep_LastFirePacket, VisibleAnywhere, BlueprintReadOnly, Category = "Firing|State")
	FWeaponFirePacket LastFirePacket;

	void HandleFireInput();

	// como un evento en el blueprint de ejecución en el servidor
	UFUNCTION(Server, Reliable)
	void Server_TryFire(const FVector& MuzzleLocation, const FVector& Direction);

	UFUNCTION()
	void OnRep_LastFirePacket();

private:
	void PlayFireEffects();
	void PlayImpactEffects(const FVector& ImpactPoint, const FVector& ImpactNormal, bool bCausedDamage);

	bool RunFireTrace(FHitResult& OutHit);

public:

	void UpdateAimLocation(const FVector& InWorldAimLocation, const FVector& InViewAimLocation);

public:
	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
