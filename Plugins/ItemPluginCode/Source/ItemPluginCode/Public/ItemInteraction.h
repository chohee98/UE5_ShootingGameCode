// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInteraction.generated.h"

/**
 *
 */
UINTERFACE(Blueprintable)
class UItemInteraction : public UInterface
{
	GENERATED_BODY()
};

class ITEMPLUGINCODE_API IItemInteraction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginOverlapItemMag();
};
