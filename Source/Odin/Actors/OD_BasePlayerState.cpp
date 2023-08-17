#include "Actors/OD_BasePlayerState.h"
#include "Components/OD_AbilitySystemComponent.h"

AOD_BasePlayerState::AOD_BasePlayerState() : Super()
{
    // Create ability system component, and set it to be explicitly replicated.
    AbilitySystemComponent = CreateDefaultSubobject<UOD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    constexpr bool bIsReplicated = true;
    AbilitySystemComponent->SetIsReplicated(bIsReplicated);
}

UAbilitySystemComponent* AOD_BasePlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

