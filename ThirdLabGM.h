#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdLabGM.generated.h"

UENUM(BlueprintType)
enum class ELetterState : uint8
{
    Green,
    Yellow,
    Red
};

UCLASS()
class FIRSTLAB_API AThirdLabGM : public AGameModeBase
{
    GENERATED_BODY()

public:
    AThirdLabGM();

    UFUNCTION(BlueprintCallable, Category = "Word Game")
    void StartNewGame();

    UFUNCTION(BlueprintCallable, Category = "Word Game")
    TArray<ELetterState> CheckPlayerGuess(const FString& PlayerInput);

    UFUNCTION(BlueprintCallable, Category = "Word Game")
    FString GetTargetWord() const { return TargetWord; }

    UFUNCTION(BlueprintCallable, Category = "Word Game")
    bool IsGameWon() const { return bGameWon; }

    UFUNCTION(BlueprintCallable, Category = "Word Game")
    void RestartGame();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Word Game")
    TArray<FString> WordDatabase;

    UPROPERTY(BlueprintReadWrite, Category = "Word Game")
    int32 RemainingAttempts;


private:
    FString TargetWord;
    bool bGameWon;
    int32 MaxAttempts;

    bool IsGameActive() const;

    void SelectRandomWord();
};