#include "GMFourthLab.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

AGMFourthLab::AGMFourthLab()
{
    PrimaryActorTick.bCanEverTick = false;
    GameState = EBlackjackState::Betting;
    PlayerMoney = InitialPlayerMoney;
    CurrentBet = 0;
    bPlayerHasBlackjack = false;
    bDealerHasBlackjack = false;
    bDealerCardsRevealed = false;
    DealerHiddenCardActor = nullptr;
}

void AGMFourthLab::BeginPlay()
{
    Super::BeginPlay();

    for (int32 i = 0; i < 12; i++)
    {
        if (CardActorClass)
        {
            ACardActor* Card = GetWorld()->SpawnActor<ACardActor>(CardActorClass);
            Card->SetActorHiddenInGame(true);
            ActiveCards.Add(Card);
        }
    }
}

void AGMFourthLab::InitDeck()
{
    Deck.Empty();

    for (int32 value = 2; value <= 14; ++value)
    {
        for (int32 suit = 0; suit < 4; ++suit)
        {
            Deck.Add({ value, static_cast<ESuit>(suit) });
        }
    }

    const int32 LastIndex = Deck.Num() - 1;
    for (int32 i = 0; i <= LastIndex; ++i)
    {
        const int32 SwapIndex = FMath::RandRange(i, LastIndex);
        Deck.Swap(i, SwapIndex);
    }
}

void AGMFourthLab::ReshuffleDeckIfNeeded()
{
    if (Deck.Num() < MinDeckCardsBeforeReshuffle)
    {
        InitDeck();
    }
}

FCardData AGMFourthLab::DrawCard()
{
    if (Deck.Num() == 0)
    {
        InitDeck();
    }
    return Deck.Pop();
}

ACardActor* AGMFourthLab::GetInactiveCardActor()
{
    for (ACardActor* Card : ActiveCards)
    {
        if (Card && Card->IsHidden())
        {
            return Card;
        }
    }

    if (CardActorClass)
    {
        ACardActor* NewCard = GetWorld()->SpawnActor<ACardActor>(CardActorClass);
        ActiveCards.Add(NewCard);
        return NewCard;
    }
    return nullptr;
}

void AGMFourthLab::SpawnCard(const FCardData& Card, const FVector& Position, bool bFaceUp)
{
    if (ACardActor* CardActor = GetInactiveCardActor())
    {
        CardActor->SetActorHiddenInGame(false);
        CardActor->SetActorLocation(Position);
        CardActor->CardValue = Card.Value;
        CardActor->CardSuit = Card.Suit;
        CardActor->SetFaceUp(bFaceUp);
        CardActor->UpdateCardVisual();

        if (!bFaceUp)
        {
            DealerHiddenCardActor = CardActor;
        }
    }
}

void AGMFourthLab::PlaceBet(int32 Amount)
{
    if (GameState != EBlackjackState::Betting || Amount <= 0 || Amount > PlayerMoney)
        return;

    CurrentBet = Amount;
    PlayerMoney -= Amount;
    StartGame();
}

void AGMFourthLab::StartGame()
{
    ResetActiveCards();
    PlayerHand.Empty();
    DealerHand.Empty();
    bPlayerHasBlackjack = false;
    bDealerHasBlackjack = false;
    bDealerCardsRevealed = false;
    DealerHiddenCardActor = nullptr;
    WinnerText = FString();

    InitDeck();
    
    PlayerHand.Add(DrawCard());
    PlayerHand.Add(DrawCard());
    
    DealerHand.Add(DrawCard());
    DealerHand.Add(DrawCard());

    for (int32 i = 0; i < PlayerHand.Num(); ++i)
    {
        const FVector SpawnPos = PlayerCardsBasePosition + PlayerCardsOffset * i;
        SpawnCard(PlayerHand[i], SpawnPos, true);
    }

    SpawnCard(DealerHand[0], DealerCardsBasePosition, true);
    SpawnCard(DealerHand[1], DealerCardsBasePosition + DealerCardsOffset, false);

    CheckBlackjacks();

    if (bPlayerHasBlackjack && bDealerHasBlackjack)
    {
        HandleBothBlackjack();
    }
    else if (bPlayerHasBlackjack)
    {
        HandlePlayerBlackjack();
    }
    else if (bDealerHasBlackjack)
    {
        GameState = EBlackjackState::PlayerTurn;
    }
    else
    {
        GameState = EBlackjackState::PlayerTurn;
    }

    OnDealerScoreUpdated();
}

void AGMFourthLab::CheckBlackjacks()
{
    bPlayerHasBlackjack = (PlayerHand.Num() == 2 && GetHandValue(PlayerHand) == 21);
    bDealerHasBlackjack = (DealerHand.Num() == 2 && GetHandValue(DealerHand) == 21);
}

void AGMFourthLab::PlayerHit()
{
    if (GameState != EBlackjackState::PlayerTurn) return;

    const FCardData NewCard = DrawCard();
    PlayerHand.Add(NewCard);

    const FVector SpawnPos = PlayerCardsBasePosition + PlayerCardsOffset * (PlayerHand.Num() - 1);
    SpawnCard(NewCard, SpawnPos, true);

    if (GetHandValue(PlayerHand) > 21)
    {
        EndGame("Player bust! Dealer wins", false);
    }
}

void AGMFourthLab::PlayerStand()
{
    if (GameState != EBlackjackState::PlayerTurn) return;

    if (bDealerHasBlackjack && !bDealerCardsRevealed)
    {
        HandleDealerBlackjack();
        return;
    }

    StartDealerTurn();
}

void AGMFourthLab::FlipDealerHiddenCard()
{
    if (DealerHiddenCardActor && !bDealerCardsRevealed)
    {
        DealerHiddenCardActor->SetFaceUp(true);
        DealerHiddenCardActor->UpdateCardVisual();
        bDealerCardsRevealed = true;
        OnDealerScoreUpdated();
    }
}

int32 AGMFourthLab::GetHandValue(const TArray<FCardData>& Hand) const
{
    int32 totalValue = 0;
    int32 aceCount = 0;

    for (const FCardData& Card : Hand)
    {
        if (Card.Value >= 2 && Card.Value <= 10)
        {
            totalValue += Card.Value;
        }
        else if (Card.Value >= 11 && Card.Value <= 13) // J, Q, K
        {
            totalValue += 10;
        }
        else if (Card.Value == 14) // Ace
        {
            totalValue += 11;
            aceCount++;
        }
    }
    
    while (totalValue > 21 && aceCount > 0)
    {
        totalValue -= 10;
        aceCount--;
    }

    return totalValue;
}

void AGMFourthLab::StartDealerTurn()
{
    GameState = EBlackjackState::DealerTurn;
    FlipDealerHiddenCard();

    DealerCardIndex = DealerHand.Num();
    GetWorld()->GetTimerManager().SetTimer(
        DealerTimerHandle,
        this,
        &AGMFourthLab::DealerTakeCard,
        DealerActionDelay,
        false
    );
}

void AGMFourthLab::DealerTakeCard()
{
    const int32 DealerScore = GetHandValue(DealerHand);
    const int32 PlayerScore = GetHandValue(PlayerHand);

    if (DealerScore > 21 || (DealerScore > PlayerScore && PlayerScore <= 21))
    {
        DetermineWinner();
        return;
    }

    const FCardData NewCard = DrawCard();
    DealerHand.Add(NewCard);

    const FVector SpawnPos = DealerCardsBasePosition + DealerCardsOffset * DealerCardIndex++;
    SpawnCard(NewCard, SpawnPos, true);

    GetWorld()->GetTimerManager().SetTimer(
        DealerTimerHandle,
        this,
        &AGMFourthLab::DealerTakeCard,
        DealerActionDelay,
        false
    );

    OnDealerScoreUpdated();
}

void AGMFourthLab::DetermineWinner()
{
    const int32 PlayerScore = GetHandValue(PlayerHand);
    const int32 DealerScore = GetHandValue(DealerHand);

    if (PlayerScore > 21)
    {
        EndGame("Player bust! Dealer wins", true);
    }
    else if (DealerScore > 21)
    {
        EndGame("Dealer bust! Player wins", true);
    }
    else if (PlayerScore > DealerScore)
    {
        EndGame("Player wins", true);
    }
    else if (DealerScore > PlayerScore)
    {
        EndGame("Dealer wins", true);
    }
    else
    {
        EndGame("Draw", true);
    }
}

void AGMFourthLab::EndGame(const FString& Result, bool bRevealDealer)
{
    GameState = EBlackjackState::GameOver;
    WinnerText = Result;

    if (bRevealDealer && !bDealerCardsRevealed)
    {
        FlipDealerHiddenCard();
    }

    UpdateMoneyAfterGame();
    OnDealerScoreUpdated();
}

void AGMFourthLab::UpdateMoneyAfterGame()
{
    if (WinnerText.Contains("Player wins"))
    {
        PlayerMoney += CurrentBet * 2;
    }
    else if (WinnerText.Contains("Draw"))
    {
        PlayerMoney += CurrentBet;
    }
    CurrentBet = 0;
}

void AGMFourthLab::ResetActiveCards()
{
    for (ACardActor* Card : ActiveCards)
    {
        if (Card)
        {
            Card->SetActorHiddenInGame(true);
            Card->SetFaceUp(true);
        }
    }
}

void AGMFourthLab::CompleteGame()
{
    GameState = EBlackjackState::Betting;
    CurrentBet = 0;
    WinnerText = FString();
    bDealerCardsRevealed = false;
    ResetActiveCards();
    OnBettingStarted.Broadcast();
}

void AGMFourthLab::RestartGame()
{
    PlayerMoney = InitialPlayerMoney;
    CompleteGame();
}

int32 AGMFourthLab::GetPlayerHandValue() const
{
    return GetHandValue(PlayerHand);
}

int32 AGMFourthLab::GetDealerHandValue() const
{
    if (!bDealerCardsRevealed && DealerHand.Num() > 0)
    {
        return DealerHand[0].Value == 14 ? 11 :
            (DealerHand[0].Value >= 11 ? 10 : DealerHand[0].Value);
    }
    return GetHandValue(DealerHand);
}

FString AGMFourthLab::GetWinnerText() const
{
    return WinnerText;
}

FString AGMFourthLab::GetGameStatusText() const
{
    FString StateInfo;
    switch (GameState)
    {
    case EBlackjackState::Betting:
        StateInfo = FString::Printf(TEXT("Balance: $%d | Place your bet"), PlayerMoney);
        break;
    case EBlackjackState::PlayerTurn:
        StateInfo = FString::Printf(TEXT("Player: %d | Dealer: %d"),
            GetPlayerHandValue(),
            GetDealerHandValue());
        break;
    case EBlackjackState::DealerTurn:
        StateInfo = TEXT("Dealer is playing...");
        break;
    case EBlackjackState::GameOver:
        StateInfo = FString::Printf(TEXT("%s | Player: %d | Dealer: %d"),
            *WinnerText,
            GetPlayerHandValue(),
            GetDealerHandValue());
        break;
    }
    return StateInfo;
}

void AGMFourthLab::OnDealerScoreUpdated_Implementation()
{
}

void AGMFourthLab::HandleBothBlackjack()
{
    FlipDealerHiddenCard();
    GameState = EBlackjackState::GameOver;
    PlayerMoney += CurrentBet;
    WinnerText = "Draw: Both have blackjack";
}

void AGMFourthLab::HandlePlayerBlackjack()
{
    FlipDealerHiddenCard();
    GameState = EBlackjackState::GameOver;
    PlayerMoney += FMath::RoundToInt(CurrentBet * 2.5f);
    WinnerText = "Player wins with blackjack!";
}

void AGMFourthLab::HandleDealerBlackjack()
{
    FlipDealerHiddenCard();
    GameState = EBlackjackState::GameOver;
    WinnerText = "Dealer wins with blackjack!";
}