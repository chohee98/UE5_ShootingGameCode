// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMag.h"
#include "ItemInteraction.h"

// Sets default values
AItemMag::AItemMag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	SetRootComponent(Scene);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	StaticMesh->SetCollisionProfileName("OverlapAllDynamic");

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AItemMag::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AItemMag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemMag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemMag::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IItemInteraction* pInterface = Cast<IItemInteraction>(OtherActor);
	if (pInterface == nullptr)
		return;

	pInterface->Execute_BeginOverlapItemMag(OtherActor);

	Destroy();
}

