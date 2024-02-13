// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"




UCLASS()
class MPSHOOTER_API APressurePlate : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMaterialInstanceDynamic> MeshDynamicMat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(EditInstanceOnly, Category= "Components")
	TWeakObjectPtr<AActor> TriggerableActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	float ColorChangeDuration = 0.9;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Plate Properties")
	float LastColorChangeTime;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Plate Properties")
	FLinearColor PreviousColor;

	UPROPERTY(ReplicatedUsing = OnRep_Color, EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	FLinearColor ActualColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	FLinearColor UnpressedColor = FLinearColor(0.8,0.8,0.8);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	FLinearColor NotEnoughPressureColor = FLinearColor(0.9, 0.3, 0.3);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	FLinearColor PressedColor = FLinearColor(0.3, 0.9, 0.3);
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Plate Properties")
	int PlayerActivating = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	int NumPlayerToActivate = 2;

	/**
	 * Each pressure plate could have a key associate to enable their trigger. This would be like having some power.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Properties")
	int TriggerKeyId = 0;

	// Sets default values for this actor's properties
	APressurePlate(const FObjectInitializer& ObjectInitializer);
	

	virtual void PostInitializeComponents() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateColor();
	
	UFUNCTION()
	void OnRep_Color();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
