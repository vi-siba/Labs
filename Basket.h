// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Basket.generated.h"

UCLASS()
class FIRSTLAB_API ABasket : public APawn
{
	GENERATED_BODY()

public:
	ABasket();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasketMaxDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasketSpeed;

protected:
	virtual void BeginPlay() override;
	void MoveLeftRight(float move);


private:
	FVector ActorLocationStart;


};
