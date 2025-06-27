// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BirdToLaunch.generated.h"

UCLASS()
class FIRSTLAB_API ABirdToLaunch : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABirdToLaunch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
