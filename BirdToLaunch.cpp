// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdToLaunch.h"

// Sets default values
ABirdToLaunch::ABirdToLaunch()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABirdToLaunch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABirdToLaunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABirdToLaunch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



// get LaunchVector (Xl,Yl) = ((MousePos(Xm,Ym) - BirdSpawnPlace(X,Y));
// 
// if Xl^2+Yl^2 >= float SlingshotLenght^2 => 
// VectorSqrt = sqrt(Xl^2+Yl^2)
// SetPosition  (SlingshotLenght*LaunchVector (Xl,Yl)/VectorSqrt+BirdSpawnPlace(X,Y))
// 
// if Xl^2+Yl^2 < float SlingshotLenght^2 => 
// SetPosition MousePos(Xm,Ym);
// 
// 
// 
// Yl/Xl*SlingshotLenght;
		

