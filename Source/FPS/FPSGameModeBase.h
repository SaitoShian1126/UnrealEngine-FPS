// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameModeBase();
	virtual void StartPlay() override;

	UUserWidget* pWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget>Widget;
};
