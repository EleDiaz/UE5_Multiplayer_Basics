#pragma once

#include "CoreMinimal.h"
#include "PlayerKey.h"
#include "GameFramework/Character.h"
#include "ShooterPawn.generated.h"

UCLASS()
class MPSHOOTER_API AShooterPawn : public ACharacter, public IPlayerKey
{
private:
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class UMaterialInstanceDynamic* MeshMID;

	UPROPERTY(ReplicatedUsing = OnRep_Color, Transient, BlueprintReadOnly, Category = "Player")
	FLinearColor Color;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* WeaponHandle;

	UPROPERTY(ReplicatedUsing = OnRep_Weapon, Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AimTraceDistance;

	virtual void Tick(float DeltaTime) override;

	virtual bool HasKey_Implementation(int id) override;

	virtual void PostInitializeComponents() override; // Called when every component is created and initialized.
	
	void AuthSetColor(const FLinearColor& InColor);

private:
	UFUNCTION()
	void OnRep_Color();

public:
	// Sets default values for this character's properties
	AShooterPawn(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnRep_Weapon();
	UFUNCTION()
	void OnFire();
	UFUNCTION()
	void OnMoveForward(float Axis);
	UFUNCTION()
	void OnMoveRight(float Axis);
	UFUNCTION()
	void OnMoveUp(float Axis);
	UFUNCTION()
	void OnLookRight(float Axis);
	UFUNCTION()
	void OnLookUp(float Axis);
};
