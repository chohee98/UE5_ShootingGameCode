// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"// Replicated 처리에서 DOREPLIFETIME 기능을 가지고 있는 라이브러리
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/ShootingHUD.h"
#include "GameMode/ShootingPlayerState.h"

// Sets default values
AWeapon::AWeapon():m_Ammo(30)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetCollisionProfileName("OverlapAllDynamic");

	bReplicates = true;
	SetReplicateMovement(true);

	SetRootComponent(WeaponMesh);
}

// GetLifetimeReplicatedProps : Replicated 변수의 코드를 연결하는 영역
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, m_pChar);
	DOREPLIFETIME(AWeapon, m_Ammo);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation(bool IsPress)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green,
		FString::Printf( TEXT("EventTrigger_Implementation IsPressr=%d"), IsPress));

	if (IsValid(ShootMontage) == false)
		return;

	if (IsPress == false)
		return;

	if (IsCanShoot() == false)
		return;

	m_pChar->PlayAnimMontage(ShootMontage);
}

void AWeapon::EventShoot_Implementation()
{
	if (FireEffect == nullptr)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect,
		WeaponMesh->GetSocketLocation("muzzle"),
		WeaponMesh->GetSocketRotation("muzzle"),
		FVector(0.2f, 0.2f, 0.2f));

	ACharacter* pPlayer0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (pPlayer0 != m_pChar)
		return;

	FVector vStart;
	FVector vEnd;
	CalcShootStartEndPos(vStart, vEnd);
	// 여기 아래 부터는 vStart, vEnd 값이 계산되서 들어가 있다.

	// GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("(Client)ReqApplyDamage"));
	ReqApplyDamage(vStart, vEnd);
}

void AWeapon::EventPickup_Implementation(ACharacter* pChar)
{
	// AttachToActor;
	// AttachToComponent;
	m_pChar = pChar;
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(pChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	UpdateHUD_MyAmmo(m_Ammo);
}

void AWeapon::EventDrop_Implementation(ACharacter* pChar)
{
	UpdateHUD_MyAmmo(0);
	m_pChar = nullptr;
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(true);
	SetOwner(nullptr);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false)
		return;

	m_pChar->PlayAnimMontage(ReloadMontage);
}

void AWeapon::EventReload_Complate_Implementation()
{
	if (IsValid(m_pChar) == false)
		return;

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (IsValid(pPlayer0) == false)
		return;

	AController* pOwner = m_pChar->GetController();
	if (pPlayer0 != pOwner)
		return;

	ReqReload();
}


void AWeapon::ReqApplyDamage_Implementation(FVector vStart, FVector vEnd)
{
	// GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("(Server)ReqApplyDamage"));
	UseAmmo();

	FCollisionObjectQueryParams collisionObjs;
	collisionObjs.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjs.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjs.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjs.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjs.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjs.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams collisionQuery; // 컬리전 채크 추가 옵션
	collisionQuery.AddIgnoredActor(m_pChar); // 히트 채크시 무시할 액터 추가 

	FHitResult result;
	bool itHit = GetWorld()->LineTraceSingleByObjectType(result, vStart, vEnd, collisionObjs, collisionQuery);
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Red, false, 5.0f);
	if (itHit == false)
		return;

	ACharacter* pHitChar = Cast<ACharacter>(result.GetActor());
	if (pHitChar == nullptr)// pHitChar가 nullptr 이라는 건 result.GetActor()가 캐릭터가 아니다.
		return;

	UGameplayStatics::ApplyDamage(pHitChar, 10.0f, m_pChar->GetController(), this, UDamageType::StaticClass());
}

void AWeapon::ReqReload_Implementation()
{
	AShootingPlayerState* pPS = Cast<AShootingPlayerState>(m_pChar->GetPlayerState());

	if (IsValid(pPS) == false)
		return;

	pPS->UseMag();
}

void AWeapon::OnRep_Ammo()
{
	UpdateHUD_MyAmmo(m_Ammo);
}

void AWeapon::CalcShootStartEndPos(FVector& vStart, FVector& vEnd)
{
	if (IsValid(m_pChar) == false)
		return;

	USpringArmComponent* pArm = Cast<USpringArmComponent>(m_pChar->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (IsValid(pArm) == false)
		return;

	APlayerController* pPlayer = GetWorld()->GetFirstPlayerController();// Player index 0번
	FVector vCameraLoc = pPlayer->PlayerCameraManager->GetCameraLocation();
	FVector vCameraForward = pPlayer->PlayerCameraManager->GetActorForwardVector();

	vStart = (vCameraForward * (pArm->TargetArmLength + 100)) + vCameraLoc;
	vEnd = (vCameraForward * 10000) + vCameraLoc;
}

void AWeapon::UseAmmo()
{
	m_Ammo -= 1;
	m_Ammo = FMath::Clamp(m_Ammo, 0, 30);
	OnRep_Ammo();
}

void AWeapon::UpdateHUD_MyAmmo(int Ammo)
{
	if (IsValid(m_pChar) == false)
		return;

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (IsValid(pPlayer0) == false)
		return;

	AController* pOwner = m_pChar->GetController();
	if (pPlayer0 != pOwner)
		return;

	AShootingHUD* pHud = Cast<AShootingHUD>(pPlayer0->GetHUD());
	if (IsValid(pHud) == false)
		return;

	pHud->OnUpdateMyAmmo(Ammo);
}

bool AWeapon::IsCanShoot()
{
	if (m_Ammo <= 0)
		return false;

	return true;
}

