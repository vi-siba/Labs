// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <coroutine>
#include "Interaction.h" 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppleSpawner.generated.h"


UCLASS(Blueprintable)
class FIRSTLAB_API AAppleSpawner : public AActor, public IInteraction
{
	GENERATED_BODY()

public:	
	AAppleSpawner();
	virtual void Tick(float DeltaTime) override;
	virtual void Activate_Implementation();
	virtual void Deactivate_Implementation();



protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AActor> ObjectToSpawn;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int NumberOfApples;

private:

	void ActivateSpawner();
	void SpawnApples();
	void ActivateApples();
	void DeactivateApples();


	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	int SpawnCount = 3;

	TArray<AActor*> ObjectToSpawnArray;

	FTimerHandle SpawnTimerHandle;

};

