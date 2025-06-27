// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"


// Sets default values
AApple::AApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AppleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AppleMesh"));
	//RootComponent = AppleMesh;
	//AppleMesh->SetupAttachment(RootComponent);
	//AppleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//AppleMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//AppleMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	//AppleMesh->OnComponentBeginOverlap.AddDynamic(this, &AApple::OnBeginOverlap);

	AppleMaxDistance = 700.0f;


}

// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();
	

	if (AppleMesh)
	{
		AApple::DeactivateApple();
		StartPosition = GetActorLocation();
		AApple::SetLocation();
		AppleMesh->OnComponentBeginOverlap.AddDynamic(this, &AApple::OnBeginOverlap);
		AppleMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));


	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Apple mesh doesn't exist"));

	}

	

}

void AApple::Activate_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Activate"));

	AApple::ActivateApple();
}
void AApple::Deactivate_Implementation()
{
	AApple::DeactivateApple();

}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AApple::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AApple::DeactivateApple();
	AApple::SetLocation();
	AApple::ActivateApple();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hitted!"));

}

//spawn->on visibility, phys->collapse, offphys, vis->spawn;

void AApple::SetLocation()
{

	float RandLocationY = FMath::RandRange(-AppleMaxDistance, AppleMaxDistance);
	FVector LocationSet(StartPosition.X, StartPosition.Y+ RandLocationY, StartPosition.Z);
	SetActorLocation((LocationSet), false);
	

}

void AApple::ActivateApple()
{

	if (AppleMesh)
	{
		AppleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AppleMesh->SetSimulatePhysics(true);
		AppleMesh->SetVisibility(true);

	}
}

void AApple::DeactivateApple()
{
	if (AppleMesh)
	{

		AppleMesh->SetSimulatePhysics(false);
		AppleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AppleMesh->SetVisibility(false);
	}
}


/*
if (OtherActor && OtherActor != this)
	{

		APawn* Pawn = Cast<APawn>(OtherActor);
		if (Pawn && Pawn->IsPlayerControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped with the player!"));
		}
		else
		{

		}
		//Destroy();
		AActor::Destroy();

	}

*/