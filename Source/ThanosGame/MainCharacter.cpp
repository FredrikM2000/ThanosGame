// Fill out your copyright notice in the Description page of Project Settings.
//https://www.youtube.com/watch?v=X4gMS3u1ACk&ab_channel=AwesomeTutsAwesomeTuts Setup camera, movement


#include "MainCharacter.h"
#include "Math/Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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

	BallBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("BallBoom"));
	BallBoom->SetupAttachment(FollowCamera);
	BallBoom->TargetArmLength = 2000.0f;

	CapsuleBall = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleBall"));
	CapsuleBall->SetupAttachment(BallBoom, USpringArmComponent::SocketName);

	BallBoom2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("BallBoom2"));
	BallBoom2->SetupAttachment(CapsuleBall);
	BallBoom2->TargetArmLength = 200.0f;

	TeleportBall = CreateDefaultSubobject<UStaticMeshComponent>("TeleportBall");
	TeleportBall->SetupAttachment(BallBoom2, USpringArmComponent::SocketName);
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

	int32 TeleportDistance = FVector::Dist(TeleportBall->GetComponentLocation(), GetMesh()->GetComponentLocation());

	if (bTeleportationAbility && TeleportDistance >= 1750.0f) {
		TeleportBall->SetVisibility(false);
	}
	if (bTeleportationAbility && TeleportDistance <= 1750.0f) {
		TeleportBall->SetVisibility(true);
	}
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

	PlayerInputComponent->BindAction("StartPickStone", IE_Pressed, this, &AMainCharacter::StartPickStone);
	PlayerInputComponent->BindAction("StopPickStone", IE_Released, this, &AMainCharacter::StopPickStone);
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
		Change(true);
	}
	else if (bTeleportationAbility) {
		Change(false);
	}
}

void AMainCharacter::Teleport()
{
	if (bTeleportationAbility && TeleportBall->IsVisible()) {
		Change(false);

		FVector NewLocation = TeleportBall->GetComponentLocation();

		SetActorLocation(NewLocation);
	}
}

void AMainCharacter::Change(bool bVisible)
{
	TeleportBall->SetVisibility(bVisible);
	bTeleportationAbility = bVisible;
}

void AMainCharacter::StartPickStone()
{
	//OpenWidget
	//Start slowmo
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.5);
}

void AMainCharacter::StopPickStone()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);

}







//TODO:
//Fix camera so player rotates with camera
//Add timer so player dont teleport instantly, and turn off player control