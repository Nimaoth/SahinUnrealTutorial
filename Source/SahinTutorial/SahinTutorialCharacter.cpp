// Copyright Epic Games, Inc. All Rights Reserved.

#include "SahinTutorialCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ASahinTutorialCharacter

ASahinTutorialCharacter::ASahinTutorialCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    HealCharges = 3;
}

void ASahinTutorialCharacter::BeginPlay() {
    Super::BeginPlay();
    CurrentHP = MaxHP;
}


void ASahinTutorialCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    CurrentHP = FMath::Max(CurrentHP - 10 * DeltaTime, 0.0f);

    if (CurrentHP <= 0.0f) {
        GetCharacterMovement()->MaxWalkSpeed = 150.0f;
        //GetCharacterMovement()->SetActive(false);
    }
    else {
        GetCharacterMovement()->MaxWalkSpeed = 600.0f;
        //GetCharacterMovement()->SetActive(true);
    }
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASahinTutorialCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASahinTutorialCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASahinTutorialCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &ASahinTutorialCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &ASahinTutorialCharacter::LookUpAtRate);

    // handle touch devices
    PlayerInputComponent->BindTouch(IE_Pressed, this, &ASahinTutorialCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &ASahinTutorialCharacter::TouchStopped);

    PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ASahinTutorialCharacter::Heal);
}


void ASahinTutorialCharacter::OnResetVR()
{
    // If SahinTutorial is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in SahinTutorial.Build.cs is not automatically propagated
    // and a linker error will result.
    // You will need to either:
    //		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
    // or:
    //		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASahinTutorialCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
        Jump();
}

void ASahinTutorialCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
        StopJumping();
}

void ASahinTutorialCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASahinTutorialCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASahinTutorialCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASahinTutorialCharacter::MoveRight(float Value)
{
    if ( (Controller != nullptr) && (Value != 0.0f) )
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
    
        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void ASahinTutorialCharacter::ChangeHP(int amount) {
    CurrentHP = FMath::Clamp(CurrentHP + amount, 0.0f, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("HP: %f"), CurrentHP);
}

void ASahinTutorialCharacter::Heal() {
    UE_LOG(LogTemp, Warning, TEXT("Heal"));
    if (HealCharges > 0) {
        HealCharges -= 1;
        ChangeHP(50);
    }
}

