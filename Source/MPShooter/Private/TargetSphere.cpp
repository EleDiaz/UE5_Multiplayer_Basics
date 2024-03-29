// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSphere.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DamageEvents.h"
#include "ShooterPawn.h"
#include "DamageType_WeaponFire.h"

#include "Runtime/Engine/Public/EngineGlobals.h"

// Sets default values
ATargetSphere::ATargetSphere(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Script/Engine.Material'/Game/Materials/M_Target.M_Target'"));

	bReplicates = true;
	NetCullDistanceSquared = FMath::Square(1500.0f);
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = false;
	ColorChangeDuration = 0.333f;

	SetCanBeDamaged(true);

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MeshComponent"));

	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetStaticMesh(MeshFinder.Object);
	MeshComponent->SetMaterial(0, MaterialFinder.Object);

}

// Called when the game starts or when spawned
void ATargetSphere::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

}

// Called every frame
void ATargetSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float ColorChangeElapsed = CurrentTime - LastColorChangeTime;
	const float ColorChangeAlpha = ColorChangeDuration < KINDA_SMALL_NUMBER ? 1.0f : FMath::Min(1.0f, ColorChangeElapsed / ColorChangeDuration);
	const FLinearColor TargetColor = Color * FMath::Lerp(10.0f, 1.0f, ColorChangeAlpha);
	const FLinearColor NewColor = FLinearColor::LerpUsingHSV(PreviousColor, TargetColor, ColorChangeAlpha);
	const FLinearColor DebugColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (MeshMID)
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, TEXT("Changing color"));
		MeshMID->SetVectorParameterValue(TEXT("Color"), NewColor);
	}

	if (ColorChangeAlpha >= 1.0f)
	{
		SetActorTickEnabled(false);
	}
}

void ATargetSphere::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshMID = MeshComponent->CreateDynamicMaterialInstance(0);
}

float ATargetSphere::InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (PointDamageEvent.DamageTypeClass == UDamageType_WeaponFire::StaticClass())
	{
		const AShooterPawn* Pawn = EventInstigator ? EventInstigator->GetPawn<AShooterPawn>() : nullptr;

		if (Pawn)
		{
			Color = Pawn->Color;
			OnRep_Color();
		}

		return Damage;
	}
	return Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
}

void ATargetSphere::OnRep_Color()
{
	if (MeshMID)
	{
		PreviousColor = MeshMID->K2_GetVectorParameterValue(TEXT("Color"));
		SetActorTickEnabled(true);
	}
}

void ATargetSphere::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATargetSphere, Color);
}
