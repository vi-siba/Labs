#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ESuit.h"
#include "CardActor.h"
#include "GMFourthLab.generated.h"

UENUM(BlueprintType)
enum class EBlackjackState : uint8
{
    Betting,
    PlayerTurn,
    DealerTurn,
    GameOver
};

USTRUCT(BlueprintType)
struct FCardData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Value;

    UPROPERTY(BlueprintReadWrite)
    ESuit Suit;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEvent);

UCLASS()
class FIRSTLAB_API AGMFourthLab : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGMFourthLab();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ACardActor> CardActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    int32 InitialPlayerMoney = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    int32 MinDeckCardsBeforeReshuffle = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FVector PlayerCardsOffset = FVector(200, 0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FVector DealerCardsOffset = FVector(200, 0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FVector PlayerCardsBasePosition = FVector(0, 0, 100);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FVector DealerCardsBasePosition = FVector(0, 0, 300);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float DealerActionDelay = 1.0f;

    UPROPERTY(BlueprintAssignable, Category = "Blackjack")
    FGameEvent OnBettingStarted;

    UFUNCTION(BlueprintCallable)
    void StartGame();

    UFUNCTION(BlueprintCallable)
    void RestartGame();

    UFUNCTION(BlueprintCallable)
    void PlayerHit();

    UFUNCTION(BlueprintCallable)
    void PlayerStand();

    UFUNCTION(BlueprintCallable)
    void PlaceBet(int32 Amount);

    UFUNCTION(BlueprintCallable)
    void FlipDealerHiddenCard();

    UFUNCTION(BlueprintCallable)
    void CompleteGame();

    UFUNCTION(BlueprintPure)
    int32 GetPlayerHandValue() const;

    UFUNCTION(BlueprintPure)
    int32 GetDealerHandValue() const;

    UFUNCTION(BlueprintPure)
    FString GetWinnerText() const;

    UFUNCTION(BlueprintPure)
    FString GetGameStatusText() const;

    UFUNCTION(BlueprintPure)
    EBlackjackState GetGameState() const { return GameState; }

    UFUNCTION(BlueprintPure)
    int32 GetPlayerMoney() const { return PlayerMoney; }

    UFUNCTION(BlueprintPure)
    int32 GetCurrentBet() const { return CurrentBet; }

    UFUNCTION(BlueprintPure)
    bool HasPlayerBlackjack() const { return bPlayerHasBlackjack; }

    UFUNCTION(BlueprintPure)
    bool HasDealerBlackjack() const { return bDealerHasBlackjack; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintNativeEvent, Category = "Dealer")
    void OnDealerScoreUpdated();
    virtual void OnDealerScoreUpdated_Implementation();

private:
    TArray<FCardData> Deck;
    TArray<FCardData> PlayerHand;
    TArray<FCardData> DealerHand;
    TArray<ACardActor*> ActiveCards;

    EBlackjackState GameState;
    FString WinnerText;
    int32 PlayerMoney;
    int32 CurrentBet;
    bool bPlayerHasBlackjack;
    bool bDealerCardsRevealed;
    bool bDealerHasBlackjack;
    ACardActor* DealerHiddenCardActor;

    FTimerHandle DealerTimerHandle;
    int32 DealerCardIndex;

    void InitDeck();
    void ReshuffleDeckIfNeeded();
    FCardData DrawCard();
    void SpawnCard(const FCardData& Card, const FVector& Position, bool bFaceUp);
    ACardActor* GetInactiveCardActor();
    void ResetActiveCards();
    int32 GetHandValue(const TArray<FCardData>& Hand) const;
    void CheckBlackjacks();
    void HandleBothBlackjack();
    void HandlePlayerBlackjack();
    void HandleDealerBlackjack();
    void DealerTakeCard();
    void DetermineWinner();
    void EndGame(const FString& Result, bool bRevealDealer);
    void UpdateMoneyAfterGame();
    void StartDealerTurn();
};