// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

#include "HasTrigger.h"
#include "PlayerKey.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"


// Sets default values
APressurePlate::APressurePlate(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialMesh(TEXT("/Script/Engine.Material'/Game/Materials/M_Platform.M_Platform'"));

	bReplicates = true;
	
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent, TEXT("Plate"));
	//MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Plate"));
	TriggerBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent, TEXT("TriggerBox"));
	//TriggerBox->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("TriggerBox"));

	if (MeshFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshFinder.Object);
		MeshComponent->SetMaterial(0, MaterialMesh.Object);
		MeshComponent->SetRelativeScale3D(FVector(1.0,1.0,0.1));
		TriggerBox->SetBoxExtent(MeshFinder.Object->GetBoundingBox().GetExtent());
	}
	
}

void APressurePlate::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshDynamicMat = MeshComponent->CreateDynamicMaterialInstance(0);
}

void APressurePlate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APressurePlate, ActualColor)
}

void APressurePlate::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Cast<IPlayerKey>(OtherActor))
	{
		if (IPlayerKey::Execute_HasKey(OtherActor, TriggerKeyId))
		{
			PlayerActivating++;
		}

		if (PlayerActivating >= NumPlayerToActivate)
		{
			if (TriggerableActor != nullptr)
			{
				if (Cast<IHasTrigger>(TriggerableActor))
				{
					IHasTrigger::Execute_Dispatch(TriggerableActor.Get());
				}
			}
		}
	}
	UpdateColor();
}

void APressurePlate::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (Cast<IPlayerKey>(OtherActor))
	{
		if (IPlayerKey::Execute_HasKey(OtherActor, TriggerKeyId))
		{
			PlayerActivating--;
		}
	}
	UpdateColor();
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void APressurePlate::UpdateColor()
{
	if (PlayerActivating >= NumPlayerToActivate)
	{
		ActualColor = PressedColor;
	}
	else if (PlayerActivating == 0)
	{
		ActualColor = UnpressedColor;
	}
	else
	{
		ActualColor = NotEnoughPressureColor;
	}
	SetActorTickEnabled(true);
	OnRep_Color();
}

void APressurePlate::OnRep_Color()
{
	if (ensure(MeshDynamicMat)) {

		PreviousColor = MeshDynamicMat->K2_GetVectorParameterValue(TEXT("Color"));
		SetActorTickEnabled(true);
	}
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float ColorChangeElapsed = CurrentTime - LastColorChangeTime;
	const float ColorChangeAlpha = ColorChangeDuration < UE_KINDA_SMALL_NUMBER ? 1.0f : FMath::Min(1.0f, ColorChangeElapsed / ColorChangeDuration);
	const FLinearColor TargetColor = ActualColor * FMath::Lerp(10.0f, 1.0f, ColorChangeAlpha);
	const FLinearColor NewColor = FLinearColor::LerpUsingHSV(PreviousColor, TargetColor, ColorChangeAlpha);
	if (MeshDynamicMat)
	{
		MeshDynamicMat->SetVectorParameterValue(TEXT("Color"), NewColor);
	}

	if (ColorChangeAlpha >= 1.0f)
	{
		SetActorTickEnabled(false);
	}
	
}



