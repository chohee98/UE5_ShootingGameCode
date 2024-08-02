// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h"// Replicated 처리에서 DOREPLIFETIME 기능을 가지고 있는 라이브러리

AShootingPlayerState::AShootingPlayerState():m_CurHp(100), m_Mag(0)
{
}

// GetLifetimeReplicatedProps : Replicated 변수의 코드를 연결하는 영역
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
