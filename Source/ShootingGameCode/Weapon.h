// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SHOOTINGGAMECODE_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger(bool IsPress);

	virtual void EventTrigger_Implementation(bool IsPress) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();

	virtual void EventShoot_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickup(ACharacter* pChar);

	virtual void EventPickup_Implementation(ACharacter* pChar) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pChar);

	virtual void EventDrop_Implementation(ACharacter* pChar) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	virtual void EventReload_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload_Complate();

	virtual void EventReload_Complate_Implementation() override;

public:
	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqApplyDamage(FVector vStart, FVector vEnd);

	UFUNCTION()
	void OnRep_Ammo();

	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqReload();

public:
	void CalcShootStartEndPos(FVector& vStart, FVector& vEnd);

	void UseAmmo();

	void UpdateHUD_MyAmmo(int Ammo);

	bool IsCanShoot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* FireEffect;

	UPROPERTY(Replicated, BlueprintReadWrite)
	ACharacter* m_pChar;

	UPROPERTY(ReplicatedUsing = OnRep_Ammo)
	int m_Ammo;
};
