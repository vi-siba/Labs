#include "ThirdLabGM.h"
#include "Math/UnrealMathUtility.h"

AThirdLabGM::AThirdLabGM()
{
    PrimaryActorTick.bCanEverTick = true;

    RemainingAttempts = MaxAttempts;
    bGameWon = false;
    MaxAttempts = 5;
}

void AThirdLabGM::RestartGame()
{
    StartNewGame();
}

void AThirdLabGM::StartNewGame()
{
    RemainingAttempts = MaxAttempts;
    bGameWon = false;
    SelectRandomWord();
}

void AThirdLabGM::SelectRandomWord()
{
    TArray<FString> ValidWords;

    for (const FString& Word : WordDatabase)
    {
        if (Word.Len() == 5)
        {
            ValidWords.Add(Word);
        }
    }

    const int32 RandomIndex = FMath::Rand() % ValidWords.Num();
    TargetWord = ValidWords[RandomIndex].ToLower();
}

TArray<ELetterState> AThirdLabGM::CheckPlayerGuess(const FString& PlayerInput)
{
    TArray<ELetterState> Result;
    Result.Init(ELetterState::Red, 5);

    if (!IsGameActive() || PlayerInput.Len() != 5)
    {
        return Result;
    }

    FString Input = PlayerInput.ToLower();
    TMap<TCHAR, int32> LetterCounts;

    for (TCHAR c : TargetWord)
    {
        LetterCounts.FindOrAdd(c)++;
    }
    
    bGameWon = true;
    for (int32 i = 0; i < 5; ++i)
    {
        if (Input[i] == TargetWord[i])
        {
            Result[i] = ELetterState::Green;
            LetterCounts[Input[i]]--;
        }
        else
        {
            bGameWon = false;
        }
    }

    for (int32 i = 0; i < 5; ++i)
    {
        if (Result[i] == ELetterState::Green) continue;

        if (LetterCounts.Contains(Input[i]) && LetterCounts[Input[i]] > 0)
        {
            Result[i] = ELetterState::Yellow;
            LetterCounts[Input[i]]--;
        }
    }

    if (bGameWon)
    {
        UE_LOG(LogTemp, Display, TEXT("Right, guessed letter: \"%s\""), *TargetWord);
    }
    else
    {
        RemainingAttempts--;
        if (RemainingAttempts <= 0)
        {
            UE_LOG(LogTemp, Display, TEXT("Wrong, guessed letter: \"%s\""), *TargetWord);
        }
    }

    return Result;
}

bool AThirdLabGM::IsGameActive() const
{
    return RemainingAttempts > 0 && !bGameWon;
}