// Fill out your copyright notice in the Description page of Project Settings.


#include "AS/RL_AS_Player.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Interface/RL_Interface_Death.h"

void URL_AS_Player::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{	
	ensure(this != nullptr && !IsUnreachable());
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetExperienceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxExperience());
	}
	if (Attribute == GetLeftAmmosInMagazineAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmoCounts());
	}
}

void URL_AS_Player::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	ensure(this != nullptr && !IsUnreachable());
	Super::PostGameplayEffectExecute(Data);
}

void URL_AS_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Player, MaxExperience, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Player, Experience, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Player, MaxAmmoCounts, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Player, LeftAmmosInMagazine, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(URL_AS_Player, MaxAmmosInMagazine, COND_None, REPNOTIFY_Always);
}

void URL_AS_Player::OnRep_MaxAmmoCounts(const FGameplayAttributeData& OldMaxAmmoCounts)
{
	ensure(this != nullptr && !IsUnreachable());
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Player, MaxAmmoCounts, OldMaxAmmoCounts);
}

void URL_AS_Player::OnRep_LeftAmmosInMagazine(const FGameplayAttributeData& OldLeftAmmosInMagazine)
{
	ensure(this != nullptr && !IsUnreachable());
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Player, LeftAmmosInMagazine, OldLeftAmmosInMagazine);
}

void URL_AS_Player::OnRep_MaxAmmosInMagazine(const FGameplayAttributeData& OldMaxAmmosInMagazine)
{
	ensure(this != nullptr && !IsUnreachable());
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Player, MaxAmmosInMagazine, OldMaxAmmosInMagazine);
}

void URL_AS_Player::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
	ensure(this != nullptr && !IsUnreachable());
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Player, Experience, OldExperience);
}

void URL_AS_Player::OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience)
{
	ensure(this != nullptr && !IsUnreachable());
	GAMEPLAYATTRIBUTE_REPNOTIFY(URL_AS_Player, MaxExperience, OldMaxExperience);
}