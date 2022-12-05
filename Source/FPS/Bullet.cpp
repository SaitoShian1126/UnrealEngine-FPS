// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sphereの生成
	OurSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	// 半径の設定
	OurSphereComponent->InitSphereRadius(15);

	// RootComponentにOurSphereComponentを設定
	RootComponent = OurSphereComponent;

	// Projectの生成
	OurProjectComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComponent"));

	// SetUpdatedComponent()にOurSphereComponentを設定
	OurProjectComponent->SetUpdatedComponent(OurSphereComponent);

	// InitialSpeed,MaxSpeedを設定
	OurProjectComponent->InitialSpeed = 3000.0f;
	OurProjectComponent->MaxSpeed = 3000.0f;

	// bRotationFollowsVelocity,bShouldBounceのフラグをON
	OurProjectComponent->bRotationFollowsVelocity = true;
	OurProjectComponent->bShouldBounce = true;

	// Bouncinessを0.3fにする
	OurProjectComponent->Bounciness = 0.3f;

	// ProjectileGravityScaleを0.0fにする
	OurProjectComponent->ProjectileGravityScale = 0.0f;

	// Staticの生成
	OurStaticComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticComponent"));

	// UStaticMeshの検索と設定
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Mesh/Sphere.Sphere'"));
	if (Mesh.Succeeded())
	{
		OurStaticComponent->SetStaticMesh(Mesh.Object);

		// RootComponentを親に親子付け
		OurStaticComponent->SetupAttachment(RootComponent);
	}

	//インポートしたMaterialを検索
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Material/Fbx_Default_Material.Fbx_Default_Material'"));

	if (Material.Succeeded())
	{
		//UMaterialInstanceDynamicを生成
		OurMaterial = UMaterialInstanceDynamic::Create(Material.Object, OurStaticComponent);
		OurStaticComponent->SetMaterial(0, OurMaterial);
		OurStaticComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	}

	OurSphereComponent->BodyInstance.SetCollisionProfileName(TEXT("BulletProfile"));
}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	OurProjectComponent->Velocity = ShootDirection * OurProjectComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
		if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
		{
			OtherComponent->AddImpulseAtLocation(OurProjectComponent->Velocity * 100.0f, Hit.ImpactPoint);
		}

	if (OtherActor->ActorHasTag("Stage"))
	{

	}
	else
	{
		Destroy();
	}
}

void ABullet::EnemyHit(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InitialLifeSpan--;
	if (InitialLifeSpan <= 0)
	{
		Destroy();
		InitialLifeSpan = 100;
	}
}

