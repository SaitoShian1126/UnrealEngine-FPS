// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sphere�̐���
	OurSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	// ���a�̐ݒ�
	OurSphereComponent->InitSphereRadius(15);

	// RootComponent��OurSphereComponent��ݒ�
	RootComponent = OurSphereComponent;

	// Project�̐���
	OurProjectComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComponent"));

	// SetUpdatedComponent()��OurSphereComponent��ݒ�
	OurProjectComponent->SetUpdatedComponent(OurSphereComponent);

	// InitialSpeed,MaxSpeed��ݒ�
	OurProjectComponent->InitialSpeed = 3000.0f;
	OurProjectComponent->MaxSpeed = 3000.0f;

	// bRotationFollowsVelocity,bShouldBounce�̃t���O��ON
	OurProjectComponent->bRotationFollowsVelocity = true;
	OurProjectComponent->bShouldBounce = true;

	// Bounciness��0.3f�ɂ���
	OurProjectComponent->Bounciness = 0.3f;

	// ProjectileGravityScale��0.0f�ɂ���
	OurProjectComponent->ProjectileGravityScale = 0.0f;

	// Static�̐���
	OurStaticComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticComponent"));

	// UStaticMesh�̌����Ɛݒ�
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Mesh/Sphere.Sphere'"));
	if (Mesh.Succeeded())
	{
		OurStaticComponent->SetStaticMesh(Mesh.Object);

		// RootComponent��e�ɐe�q�t��
		OurStaticComponent->SetupAttachment(RootComponent);
	}

	//�C���|�[�g����Material������
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Material/Fbx_Default_Material.Fbx_Default_Material'"));

	if (Material.Succeeded())
	{
		//UMaterialInstanceDynamic�𐶐�
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

