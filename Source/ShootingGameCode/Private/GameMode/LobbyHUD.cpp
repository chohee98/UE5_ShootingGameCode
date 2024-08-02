// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyHUD.h"
#include "Blueprint/UserWidget.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	//if (IsValid(HudWidgetClass) == false)
	//	return;
	check(HudWidgetClass);// check : check ()안에 객체가 비정상적이면 죽는다.

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();

	APlayerController* player0 = GetWorld()->GetFirstPlayerController();

	if (IsValid(player0) == false)
		return;

	player0->SetInputMode(FInputModeUIOnly());
	player0->bShowMouseCursor = true;
}
