// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Bullet.h"
#include "MyCharacter.generated.h"

UCLASS()
class FPS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UCameraComponent* OurComponent;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* SkeletaComponent;

	UPROPERTY(EditAnywhere,Category = "Sound")
		USoundCue* ShotSound;

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void Turn(float AxisValue);
	void LookUp(float AxisValue);

	void Fire();

	void Jump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class ABullet>ProjectileClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UAudioComponent* AudioCmp;
};
