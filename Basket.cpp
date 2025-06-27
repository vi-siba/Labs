// Fill out your copyright notice in the Description page of Project Settings.


#include "Basket.h"

// Sets default values
ABasket::ABasket()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BasketSpeed = 30.0f;

}

// Called when the game starts or when spawned
void ABasket::BeginPlay()
{
	Super::BeginPlay();

	BasketMaxDistance = 700.0f;
	ActorLocationStart = GetActorLocation();


	
}

// Called every frame
void ABasket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasket::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveLeftRight",this, &ABasket::MoveLeftRight);

}



void ABasket::MoveLeftRight(float move)
{
	if (Controller != nullptr && !ActorLocationStart.IsZero())
	{
			if (move > 0.0f)
			{

				if ((GetActorLocation().Y) < ActorLocationStart.Y + BasketMaxDistance)
				{
					FVector LocationSet(GetActorLocation().X, GetActorLocation().Y + BasketSpeed, GetActorLocation().Z);
					SetActorLocation((LocationSet), false);
				}
			}
			if (move < 0.0f)
			{
				if ((GetActorLocation().Y) > ActorLocationStart.Y - BasketMaxDistance)
				{
					FVector LocationSet(GetActorLocation().X, GetActorLocation().Y - BasketSpeed, GetActorLocation().Z);
					SetActorLocation((LocationSet), false);
				}
			}
	}
}