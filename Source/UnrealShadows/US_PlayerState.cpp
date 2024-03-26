// Fill out your copyright notice in the Description page of Project Settings.


#include "US_PlayerState.h"
#include "US_Character.h"
#include "US_CharacterStats.h"
#include "Net/UnrealNetwork.h"

void AUS_PlayerState::OnRep_XP(int32 OldValue) const
{
	OnXPChanged.Broadcast(XP);
}

void AUS_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}

void AUS_PlayerState::AddXP(int32 Value)
{
	XP += Value;
	OnXPChanged.Broadcast(XP);

	if (const auto Character = Cast<AUS_Character>(GetPawn()))
	{
		if (Character->GetCharacterStats()->NextLevelXP < XP)
		{
			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelUp.Broadcast(CharacterLevel);
		}
	}
}

void AUS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AUS_PlayerState, XP, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AUS_PlayerState, CharacterLevel, COND_OwnerOnly);
}
