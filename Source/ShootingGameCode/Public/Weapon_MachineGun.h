// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API AWeapon_MachineGun : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void EventTrigger_Implementation(bool IsPress) override;

	UFUNCTION()
	void LoopShooting();

public:
	FTimerHandle th_LoopShooting;
};
