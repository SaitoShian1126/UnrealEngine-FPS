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

	// USoundCue�̌����Ɛݒ�
	static ConstructorHelpers::FObjectFinder<USoundCue>Sound(TEXT("'/Game/Audio/SE_Shot_Cue.SE_Shot_Cue'"));

	if (Sound.Succeeded())
	{
		ShotSound = Sound.Object;
	}

	// Audio�̐���
	AudioCmp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	// AudioComponent��e�ɐe�q�t��
	AudioCmp->SetupAttachment(RootComponent);
	AudioCmp->SetSound(ShotSound);
}

void AMyCharacter::MoveForward(float AxisValue)
{
		//Controller->GetControlRotation()��character��Rotator�Ŏ擾
		FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	//FRotator�����ɉ�]�s����쐬���A�s��̎�(X,Y,Z)���擾
	AddMovementInput(vector, AxisValue);
}

void AMyCharacter::MoveRight(float AxisValue)
{
		//Controller->GetControlRotation()��character��Rotator�Ŏ擾
		FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	//FRotator�����ɉ�]�s����쐬���A�s��̎�(X,Y,Z)���擾
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
		// 1,GetActorEyesViewPoint(&FVector,&FRotater)�Ŏ��_�ʒu�Ǝ��_��]���擾
		FVector Location;
		FRotator Rotation;
		GetActorEyesViewPoint(Location, Rotation);

		// 2,�쐬�����e���ˈʒu�p�����o�ϐ���(X:100,Y:0,Z:0)��ݒ�
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 3,UWorld* World = GetWorld();�Ń��[���h�����擾
		UWorld* World = GetWorld();

		if (World)
		{
			// 4,���[���h���->SpawnActor<�����������N���X�̌^>(�E�E�E);�֐����Ăяo���B
			// �Ԃ�l:�������ꂽ�N���X�̃C���X�^���X
			// 5,4�̑�����:�����������N���X���w��
			// TSubclassOf�E�E�E�Ő錾�ς݂̐ݒ肵���N���X��n���B
			// 4�̑�����:Location(�����ʒu)
			// ���_�ʒu + FTransform(���_��]).TransformVector(�I�t�Z�b�g);�ō쐬�B
			// 4�̑�O����:Rotation(��])
			// ���_��]�ʂ�ݒ�
			// 4�̑�l����:FActorSpawnParameters(�������p�����[�^����ݒ肵�ēn��)
			// Owner�Ɏ��g�̃|�C���^�BInstigator��GetInstigator()��ݒ�B
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ABullet* pBullet = World->SpawnActor<ABullet>(ProjectileClass, Location, Rotation, SpawnParams);

			if (pBullet)
			{
				// 7,FireInDirection()���R�[�����A���_��]��.Vector()��n���B
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

	//MoveX�̓��͒l��MoveForward�֐��փo�C���h����
	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);

	//MoveY�̓��͒l��MoveForward�֐��փo�C���h����
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	//MoveX�̓��͒l��Turn�֐��փo�C���h����
	InputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);

	//MoveY�̓��͒l��LookUp�֐��փo�C���h����
	InputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);

	InputComponent->BindAction("Jump",EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::Jump()
{
	bPressedJump = true;
}
