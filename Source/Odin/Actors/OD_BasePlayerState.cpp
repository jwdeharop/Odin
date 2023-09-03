#include "Actors/OD_BasePlayerState.h"
#include "Components/OD_AbilitySystemComponent.h"
#include "Components/OD_CompInventory.h"
#include "Libraries/OD_NetLibrary.h"

AOD_BasePlayerState::AOD_BasePlayerState() : Super()
{
    // Create ability system component, and set it to be explicitly replicated.
    AbilitySystemComponent = CreateDefaultSubobject<UOD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    CompInventory = CreateDefaultSubobject<UOD_CompInventory>(TEXT("CompInventory"));

    constexpr bool bIsReplicated = true;
    AbilitySystemComponent->SetIsReplicated(bIsReplicated);
    CompInventory->SetIsReplicated(bIsReplicated);
    SetReplicates(bIsReplicated);
}

void AOD_BasePlayerState::Initialize()
{
    if (CompInventory)
    {
        CompInventory->Initialize();
    }
}

UAbilitySystemComponent* AOD_BasePlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UOD_CompInventory* AOD_BasePlayerState::GetCompInventory()
{
    return CompInventory;
}

const UOD_CompInventory* AOD_BasePlayerState::GetCompInventory() const
{
    return CompInventory;
}

void AOD_BasePlayerState::OnRep_Owner()
{
    Super::OnRep_Owner();
}

void AOD_BasePlayerState::BeginPlay()
{
    Super::BeginPlay();
}

