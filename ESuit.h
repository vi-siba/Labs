#pragma once

#include "ESuit.generated.h"

UENUM(BlueprintType)
enum class ESuit : uint8
{
    Hearts     UMETA(DisplayName = "Hearts"),
    Diamonds   UMETA(DisplayName = "Diamonds"),
    Clubs      UMETA(DisplayName = "Clubs"),
    Spades     UMETA(DisplayName = "Spades")
};
