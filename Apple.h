// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction.h" 
#include "Apple.generated.h"

UCLASS()
class FIRSTLAB_API AApple : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	AApple();
	virtual void Tick(float DeltaTime) override;


	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;


	UFUNCTION(BlueprintCallable)
	void ActivateApple();
	UFUNCTION(BlueprintCallable)
	void DeactivateApple();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* AppleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AppleMaxDistance;
	
	
	//spawn->on visibility, phys->collapse, offphys->newlocation ->spawn;

private:

	
	// Set apple location
	void SetLocation();

	FVector StartPosition;

};
