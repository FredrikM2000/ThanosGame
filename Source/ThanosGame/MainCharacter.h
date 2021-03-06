// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Blueprint/UserWidget.h"


#include "MainCharacter.generated.h"

UCLASS()
class THANOSGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		USpringArmComponent* BallBoom;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		USpringArmComponent* BallBoom2;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	UCapsuleComponent* CapsuleBall;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	bool bDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	bool bSpaceStone = true;

	void Primary();


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TeleportBall;//Blue ball with smoke

	void StartTeleportAbility();

	void Teleport();

	bool bTeleportationAbility = false;

	void Change(bool bVisible);

	void StartPickStone();
	void StopPickStone();

};
