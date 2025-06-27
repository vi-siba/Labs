// Fill out your copyright notice in the Description page of Project Settings.


#include "AppleSpawner.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AAppleSpawner::AAppleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NumberOfApples = 2;

	SpawnInterval = 0.6f;
}

// Called when the game starts or when spawned
void AAppleSpawner::BeginPlay()
{
	Super::BeginPlay();

	AAppleSpawner::ActivateSpawner();

}

void AAppleSpawner::Activate_Implementation()
{
	AAppleSpawner::ActivateSpawner();
}

void AAppleSpawner::ActivateSpawner()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectToSpawn, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ObjectToSpawnArray.Add(Actor);
	}

	for (int i = FoundActors.Num(); i < NumberOfApples; i++)
	{
		AAppleSpawner::SpawnApples();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Spawn apple"));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ActivateSpawner"));

	AAppleSpawner::ActivateApples();

}

void AAppleSpawner::Deactivate_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Deactivate"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectToSpawn, FoundActors);

	for (AActor* Actor : FoundActors)	
	{
		ObjectToSpawnArray.Add(Actor);
	}

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AAppleSpawner::DeactivateApples();
	}


}

void AAppleSpawner::DeactivateApples()
{
	if (!ObjectToSpawnArray.IsEmpty())
	{

		AActor* Actor = ObjectToSpawnArray[ObjectToSpawnArray.Num() - 1];

		if (Actor && Actor->GetClass()->ImplementsInterface(UInteraction::StaticClass()))
		{
			IInteraction::Execute_Deactivate(Actor);
			ObjectToSpawnArray.RemoveSingle(Actor);

		}
	}

}

void AAppleSpawner::SpawnApples()
{

	if (ObjectToSpawn)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ObjectToSpawn, GetActorLocation(), GetActorRotation());

			if (SpawnedActor) 
			{
				ObjectToSpawnArray.Add(SpawnedActor);
			}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ObjectToSpawn is not setted"));
	}
}

void AAppleSpawner::ActivateApples()
{
	

	if (!ObjectToSpawnArray.IsEmpty())
	{

	AActor* Actor = ObjectToSpawnArray[ObjectToSpawnArray.Num() - 1];

	if (Actor && Actor->GetClass()->ImplementsInterface(UInteraction::StaticClass()))
	{
		IInteraction::Execute_Activate(Actor);
	}

	/*
		if (Actor->GetClass()->ImplementsInterface(UInteraction::StaticClass()))
		{
			IInteraction* Interaction = Cast<IInteraction>(Actor);
			if (Interaction)
			{
				Interaction->Execute_Activate(Actor);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ActivateApples"));
			}
		}
	*/

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hasnt interface"));
		}

	ObjectToSpawnArray.RemoveSingle(Actor);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AAppleSpawner::ActivateApples, SpawnInterval, true);
	}

	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Empty"));
	}
	
}

// Called every frame
void AAppleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

