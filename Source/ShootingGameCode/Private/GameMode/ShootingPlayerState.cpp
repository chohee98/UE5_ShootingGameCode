// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h"// Replicated ó������ DOREPLIFETIME ����� ������ �ִ� ���̺귯��

AShootingPlayerState::AShootingPlayerState():m_CurHp(100), m_Mag(0)
{
}

// GetLifetimeReplicatedProps : Replicated ������ �ڵ带 �����ϴ� ����
void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingPlayerState, m_CurHp);
	DOREPLIFETIME(AShootingPlayerState, m_Mag);
}

void AShootingPlayerState::AddDamage(float Damage)
{
	m_CurHp = m_CurHp - Damage;
	m_CurHp = FMath::Clamp(m_CurHp, 0, 100);
	OnRep_CurHp();
}

void AShootingPlayerState::AddMag()
{
	m_Mag = m_Mag + 1;
	OnRep_Mag();
}

void AShootingPlayerState::UseMag()
{
	if (IsCanReload() == false)
		return;

	m_Mag = m_Mag - 1;
	OnRep_Mag();
}

void AShootingPlayerState::OnRep_CurHp()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, 
		FString::Printf(TEXT("OnRep_CurHp CurHp=%f"), m_CurHp));

	if (Event_Dele_UpdateHp.IsBound())
		Event_Dele_UpdateHp.Broadcast(m_CurHp, 100);
}

void AShootingPlayerState::OnRep_Mag()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green,
		FString::Printf(TEXT("OnRep_Mag Mag=%d"), m_Mag));

	if (Event_Dele_UpdateMag.IsBound())
		Event_Dele_UpdateMag.Broadcast(m_Mag);
}

bool AShootingPlayerState::IsCanReload()
{
	if (m_Mag <= 0)
		return false;

	return true;
}
