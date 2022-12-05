// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class FPS_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* OurSphereComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* OurProjectComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* OurStaticComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UMaterialInstanceDynamic* OurMaterial;

	int InitialLifeSpan = 100;

	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void EnemyHit(AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
