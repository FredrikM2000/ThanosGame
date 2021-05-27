// Fill out your copyright notice in the Description page of Project Settings.
//https://www.youtube.com/watch?v=X4gMS3u1ACk&ab_channel=AwesomeTutsAwesomeTuts Setup camera, movement


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);//Set capsule size

	//Forbid character to rotate
	//Allow camera to rotate
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;//Allow character movement to rotate in the direction where it is moving
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);//The rate at which the character rotates
	GetCharacterMovement()->JumpZVelocity = 600.0f;//Jump height
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;//Rotate arm based on controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;//Camera does not rotate relative to the arm

	bDead = false;

	TeleportBall = CreateDefaultSubobject<UStaticMeshComponent>("TeleportBall");
	TeleportBall->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	TeleportBall->SetVisibility(false);


}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("StartTeleportAbility", IE_Pressed, this, &AMainCharacter::StartTeleportAbility);
	PlayerInputComponent->BindAction("Primary", IE_Pressed, this, &AMainCharacter::Primary);
}

void AMainCharacter::MoveForward(float Axis)
{
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//Calculate forward direction of vector
		AddMovementInput(Direction, Axis);
	}

}

void AMainCharacter::MoveRight(float Axis)
{
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//Calculate forward direction of vector
		AddMovementInput(Direction, Axis);
	}
}



void AMainCharacter::Primary()
{
	if (bSpaceStone) {
		Teleport();
	}
}

void AMainCharacter::StartTeleportAbility()
{
	if (!bTeleportationAbility) {
		bTeleportationAbility = true;
		TeleportBall->SetVisibility(true);
	}
	else if (bTeleportationAbility) {
		bTeleportationAbility = false;
		TeleportBall->SetVisibility(false);
	}
}

void AMainCharacter::Teleport()
{
	if (bTeleportationAbility) {
		TeleportBall->SetVisibility(false);
		bTeleportationAbility = false;


		FVector CurrentCharacterLocation = GetMesh()->GetComponentLocation();
		FVector Forward = GetMesh()->GetRightVector();

		FVector NewLocation = CurrentCharacterLocation + Forward * 500 + FVector(0.0f, 0.0f, 20.0f);
		//The +20z is to make sure the character does not get stuck in the ground because he is falling

		SetActorLocation(NewLocation);
	}
}


//TODO:
//Adjust length after how far up the character looks(like reaper)
//Fix camera so player rotates with camera
//Fix ball so it does the same as camera when met with collision(move closer to the character)
//Add timer so player dont teleport instantly, and turn off player control