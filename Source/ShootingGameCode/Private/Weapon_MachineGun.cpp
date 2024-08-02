// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_MachineGun.h"
#include "GameFramework/Character.h"

void AWeapon_MachineGun::EventTrigger_Implementation(bool IsPress)
{
	if (IsValid(ShootMontage) == false)
		return;

	if (IsCanShoot() == false)
		return;

	if (IsPress)
	{
		LoopShooting();
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.SetTimer(th_LoopShooting, this, &AWeapon_MachineGun::LoopShooting, 0.2f, true);
	}
	else
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(th_LoopShooting);
	}
}

void AWeapon_MachineGun::LoopShooting()
{
	if (IsValid(ShootMontage) == false)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(th_LoopShooting);
		return;
	}

	if (IsCanShoot() == false)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(th_LoopShooting);
		return;
	}

	m_pChar->PlayAnimMontage(ShootMontage);
}
