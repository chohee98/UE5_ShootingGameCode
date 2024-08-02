// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemInteraction.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShootingGameCodeCharacter.generated.h"


UCLASS(config=Game)
class AShootingGameCodeCharacter : public ACharacter, public IItemInteraction
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Test Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction;

	/** Test Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** PressF Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PressFAction;

	/** PressR Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PressRAction;

public:
	AShootingGameCodeCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for Test input */
	void Test(const FInputActionValue& Value);

	/** Called for Reload input */
	void Reload(const FInputActionValue& Value);

	/** Called for Shoot input */
	void ShootPress(const FInputActionValue& Value);

	/** Called for Shoot input */
	void ShootRelease(const FInputActionValue& Value);

	/** Called for PressF input */
	void PressF(const FInputActionValue& Value);

	/** Called for PressF input */
	void PressR(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Apply damage to this actor.
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// https://docs.unrealengine.com/4.27/ko/InteractiveExperiences/Networking/Actors/RPCs/
	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqTestMsg();

	// NetMulticast : 모두에서 실행
	UFUNCTION(NetMulticast, Reliable)
	void ResTestMsg();

	// Client : 소유중인 클라이언트에서 실행
	UFUNCTION(Client, Reliable)
	void ResTestMsgToOwner();

	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqReload();

	// NetMulticast : 모두에서 실행
	UFUNCTION(NetMulticast, Reliable)
	void ResReload();

	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqShoot(bool isPress);

	// NetMulticast : 모두에서 실행
	UFUNCTION(NetMulticast, Reliable)
	void ResShoot(bool isPress);

	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqPressF();

	// NetMulticast : 모두에서 실행
	UFUNCTION(NetMulticast, Reliable)
	void ResPressF(AActor* EquipActor);

	// Server : 서버에서 실행
	// Reliable : 신뢰성
	UFUNCTION(Server, Reliable)
	void ReqPressR();

	// NetMulticast : 모두에서 실행
	UFUNCTION(NetMulticast, Reliable)
	void ResPressR();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginOverlapItemMag();

	virtual void BeginOverlapItemMag_Implementation() override;

public:
	UFUNCTION(BlueprintCallable)
	void TestWeaponSpawn(TSubclassOf<class AWeapon> WeaponClass);

	void UpdateBindTestWeapon();

	AActor* GetNearestActor();

	void EventEquip(AActor* pActor);

	void EventUnEquip();

public:
	UPROPERTY(Replicated)
	FRotator m_LookAtRotation;

	UFUNCTION(BlueprintPure)
	FRotator GetLookAtRotation() { return m_LookAtRotation; };

	UPROPERTY(Replicated, BlueprintReadWrite)
	AActor* m_pEquipWeapon;

	FTimerHandle th_BindTestWeapon;
};

