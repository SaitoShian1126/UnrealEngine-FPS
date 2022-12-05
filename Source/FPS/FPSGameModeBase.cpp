// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSGameModeBase.h"

AFPSGameModeBase::AFPSGameModeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSGameModeBase::StartPlay()
{
	Super::StartPlay();

	GEngine->AddOnScreenDebugMessage(
		(uint64)-1,									//Key
		2.0f,										//TimeToDisplay
		FLinearColor::Red.ToFColor(true),			//DisplayColor
		TEXT("HELLO WORLD"));						//DebugString

	//WidgetÇê∂ê¨
	pWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);

	pWidget->AddToViewport();
}
