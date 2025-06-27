// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UInteraction : public UInterface
{
    GENERATED_BODY()

};

class FIRSTLAB_API IInteraction
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Activate();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Deactivate();

};