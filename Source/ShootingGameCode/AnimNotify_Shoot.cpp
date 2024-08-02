// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "Public/GameMode/ShootingGameCodeCharacter.h"
#include "WeaponInterface.h"

void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AShootingGameCodeCharacter* pChar = Cast<AShootingGameCodeCharacter>(MeshComp->GetOwner());
	if (IsValid(pChar) == false)
		return;

	IWeaponInterface* pInterface = Cast<IWeaponInterface>(pChar->m_pEquipWeapon);
	if (pInterface == nullptr)
		return;

	pInterface->Execute_EventShoot(pChar->m_pEquipWeapon);
}
