#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "ESuit.h"
#include "CardActor.generated.h"

UCLASS()
class FIRSTLAB_API ACardActor : public AActor
{
    GENERATED_BODY()

public:
    ACardActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    int32 CardValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    ESuit CardSuit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
    UPaperSpriteComponent* SuitSprite;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
    UPaperSpriteComponent* RankSprite;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
    UPaperSpriteComponent* BackSprite;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Card")
    void UpdateCardVisual();
    virtual void UpdateCardVisual_Implementation();

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void FlipCard();

    UFUNCTION(BlueprintCallable)
    void SetFaceUp(bool bNewFaceUp);

    UPROPERTY(BlueprintReadWrite)
    bool bFaceUp;
};