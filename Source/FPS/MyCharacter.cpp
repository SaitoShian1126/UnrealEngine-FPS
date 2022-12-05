// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OurComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SetCamera"));
	USceneComponent* SetScene = CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent());
	OurComponent->SetupAttachment(SetScene);

	OurComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	OurComponent->bUsePawnControlRotation = true;

	SkeletaComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SetSkeleta"));
	SkeletaComponent->SetupAttachment(OurComponent);
	SkeletaComponent->SetOnlyOwnerSee(true);

	SkeletaComponent->bCastDynamicShadow = false;
	SkeletaComponent->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	// USoundCueの検索と設定
	static ConstructorHelpers::FObjectFinder<USoundCue>Sound(TEXT("'/Game/Audio/SE_Shot_Cue.SE_Shot_Cue'"));

	if (Sound.Succeeded())
	{
		ShotSound = Sound.Object;
	}

	// Audioの生成
	AudioCmp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	// AudioComponentを親に親子付け
	AudioCmp->SetupAttachment(RootComponent);
	AudioCmp->SetSound(ShotSound);
}

void AMyCharacter::MoveForward(float AxisValue)
{
		//Controller->GetControlRotation()でcharacterのRotatorで取得
		FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	//FRotatorを元に回転行列を作成し、行列の軸(X,Y,Z)を取得
	AddMovementInput(vector, AxisValue);
}

void AMyCharacter::MoveRight(float AxisValue)
{
		//Controller->GetControlRotation()でcharacterのRotatorで取得
		FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	//FRotatorを元に回転行列を作成し、行列の軸(X,Y,Z)を取得
	AddMovementInput(vector, AxisValue);
}

void AMyCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AMyCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AMyCharacter::Fire()
{
	if (ProjectileClass)
	{
		// 1,GetActorEyesViewPoint(&FVector,&FRotater)で視点位置と視点回転を取得
		FVector Location;
		FRotator Rotation;
		GetActorEyesViewPoint(Location, Rotation);

		// 2,作成した弾発射位置用メンバ変数に(X:100,Y:0,Z:0)を設定
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 3,UWorld* World = GetWorld();でワールド情報を取得
		UWorld* World = GetWorld();

		if (World)
		{
			// 4,ワールド情報->SpawnActor<生成したいクラスの型>(・・・);関数を呼び出す。
			// 返り値:生成されたクラスのインスタンス
			// 5,4の第一引数:生成したいクラスを指定
			// TSubclassOf・・・で宣言済みの設定したクラスを渡す。
			// 4の第二引数:Location(生成位置)
			// 視点位置 + FTransform(視点回転).TransformVector(オフセット);で作成。
			// 4の第三引数:Rotation(回転)
			// 視点回転量を設定
			// 4の第四引数:FActorSpawnParameters(生成時パラメータ情報を設定して渡す)
			// Ownerに自身のポインタ。InstigatorにGetInstigator()を設定。
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ABullet* pBullet = World->SpawnActor<ABullet>(ProjectileClass, Location, Rotation, SpawnParams);

			if (pBullet)
			{
				// 7,FireInDirection()をコールし、視点回転量.Vector()を渡す。
				FVector LaunchDirection = Rotation.Vector();
				pBullet->FireInDirection(LaunchDirection);
				AudioCmp->Play(0.0f);
			}
		}
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//MoveXの入力値をMoveForward関数へバインドする
	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);

	//MoveYの入力値をMoveForward関数へバインドする
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	//MoveXの入力値をTurn関数へバインドする
	InputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);

	//MoveYの入力値をLookUp関数へバインドする
	InputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);

	InputComponent->BindAction("Jump",EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::Jump()
{
	bPressedJump = true;
}
