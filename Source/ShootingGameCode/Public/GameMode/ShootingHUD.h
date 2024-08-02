// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API AShootingHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void BindMyPlayerState();

	// BlueprintNativeEvent : �������Ʈ���� �̺�Ʈ ���·� ����
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyHp(float CurHp, float MaxHp);

	void OnUpdateMyHp_Implementation(float CurHp, float MaxHp);

	// BlueprintNativeEvent : �������Ʈ���� �̺�Ʈ ���·� ����
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyAmmo(int Ammo);

	void OnUpdateMyAmmo_Implementation(int Ammo);

	// BlueprintNativeEvent : �������Ʈ���� �̺�Ʈ ���·� ����
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyMag(int Mag);

	void OnUpdateMyMag_Implementation(int Mag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HudWidget;

	FTimerHandle th_BindMyPlayerState;
};
