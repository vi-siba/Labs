#include "CardActor.h"
#include "PaperSprite.h"
#include "UObject/ConstructorHelpers.h"

ACardActor::ACardActor()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    SuitSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SuitSprite"));
    SuitSprite->SetupAttachment(RootComponent);
    SuitSprite->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

    RankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RankSprite"));
    RankSprite->SetupAttachment(RootComponent);
    RankSprite->SetRelativeLocation(FVector(0.f, 0.f, -30.f));

    BackSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackSprite"));
    BackSprite->SetupAttachment(RootComponent);
    BackSprite->SetRelativeLocation(FVector::ZeroVector);


    static ConstructorHelpers::FObjectFinder<UPaperSprite> BackSpriteAsset(TEXT("PaperSprite'/Game/Blueprints/Blueprints/FourthLab/Back.Back'"));
    if (BackSpriteAsset.Succeeded())
    {
        BackSprite->SetSprite(BackSpriteAsset.Object);
    }

    bFaceUp = true;
}

void ACardActor::UpdateCardVisual_Implementation()
{
    const bool bShowFront = bFaceUp;

    if (SuitSprite) SuitSprite->SetHiddenInGame(!bShowFront);
    if (RankSprite) RankSprite->SetHiddenInGame(!bShowFront);
    if (BackSprite) BackSprite->SetHiddenInGame(bShowFront);

    if (bFaceUp)
    {
        FString SuitName;
        switch (CardSuit)
        {
        case ESuit::Hearts:   SuitName = TEXT("Hearts");   break;
        case ESuit::Diamonds: SuitName = TEXT("Diamonds"); break;
        case ESuit::Clubs:    SuitName = TEXT("Clubs");    break;
        case ESuit::Spades:   SuitName = TEXT("Spades");   break;
        default: return;
        }

        FString SuitPath = FString::Printf(TEXT("/Game/Blueprints/Blueprints/FourthLab/Suits/%s.%s"), *SuitName, *SuitName);

        FString ValueName;
        if (CardValue == 14)
            ValueName = TEXT("A");
        else if (CardValue == 11)
            ValueName = TEXT("J");
        else if (CardValue == 12)
            ValueName = TEXT("Q");
        else if (CardValue == 13)
            ValueName = TEXT("K");
        else
            ValueName = FString::FromInt(CardValue);

        FString ValuePath = FString::Printf(TEXT("/Game/Blueprints/Blueprints/FourthLab/Ranks/%s.%s"), *ValueName, *ValueName);

        UPaperSprite* SuitSpriteAsset = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, *SuitPath));
        UPaperSprite* RankSpriteAsset = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, *ValuePath));

        if (SuitSpriteAsset)
            SuitSprite->SetSprite(SuitSpriteAsset);

        if (RankSpriteAsset)
            RankSprite->SetSprite(RankSpriteAsset);
    }
}

void ACardActor::SetFaceUp(bool bNewFaceUp)
{
    bFaceUp = bNewFaceUp;
    UpdateCardVisual();
}