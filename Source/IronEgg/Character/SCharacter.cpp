// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../SGameplayTags.h"
#include "../Input/SEnhancedInputComponent.h"
#include "../Input/SInputConfig.h"
#include "../SLogChannels.h"
#include "IronEgg/Player/SPlayerController.h"
#include "IronEgg/Player/SPlayerState.h"
#include "Kismet/GameplayStatics.h"


ASCharacter::ASCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	// We control the rotation of spring arm with pawn control rotation already, this disables a subtle side effect
	// where rotating our CapsuleComponent (eg. caused by bOrientRotationToMovement in Character Movement) will rotate our spring arm until it self corrects later in the update
	// This may cause unwanted effects when using CameraLocation during Tick as it may be slightly offset from our final camera position.
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;


	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WidgetComponent=CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetRelativeLocation(FVector(0,0,120));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2d(580,120));
	
}


void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}



void ASCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Value.Y);
	}
}

void ASCharacter::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(Value.X);

	AddControllerPitchInput(Value.Y * -1);

}

void ASCharacter::Input_LookStick(const FInputActionValue& InputActionValue)
{

}

void ASCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (bIsCrouched || GetCharacterMovement()->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (GetCharacterMovement()->IsMovingOnGround())
	{
		Crouch();
	}
}

void ASCharacter::Input_AutoRun(const FInputActionValue& InputActionValue)
{

}

void ASCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->ClearAllMappings();

	// Add mappings for our game, more complex games may have multiple Contexts that are added/removed at runtime
	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	USEnhancedInputComponent* EnhancedInputComponent = Cast<USEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);
	const SGameplayTags& GameplayTags = SGameplayTags::Get();
	EnhancedInputComponent->BindSAction(SInputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, true);
	EnhancedInputComponent->BindSAction(SInputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, true);
	EnhancedInputComponent->BindSAction(SInputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch,true);
}

void ASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	ASPlayerState * PS= Cast<ASPlayerState> (GetPlayerState());
	if(PS)
	{
		USAbilitySystemComponent * ASC=PS->GetSAbilitySystemComponent();
		if (ASC)
		{
			AbilitySystemComponent=ASC;
			ASC->InitAbilityActorInfo(PS,this);
		}

		USAttributeSet * Attributes=PS->GetSAttributeSet();
		if (Attributes)
		{
			AttributeSetBase=Attributes;
		}
		InitializeAttributes();
		OnPlayerStateClient_Implementation();
		
	}
}

//server only
void ASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASPlayerState * PS= Cast<ASPlayerState> (GetPlayerState());
	if(PS)
	{
		USAbilitySystemComponent * ASC=PS->GetSAbilitySystemComponent();
		if (ASC)
		{
			AbilitySystemComponent=ASC;
			ASC->InitAbilityActorInfo(PS,this);
		}
		USAttributeSet * Attributes=PS->GetSAttributeSet();
		if (Attributes)
		{
			AttributeSetBase=Attributes;
		}
		InitializeAttributes();
		OnPossesseServer_Implementation();
	}
	
}

USAbilitySystemComponent* ASCharacter::GetSAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASCharacter::AddCharacterAbilities()
{
}

void ASCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

 	 FGameplayEffectContextHandle EffectContextHandle= AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	 FGameplayEffectSpecHandle EffectSpecHandle= AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes,-1,EffectContextHandle);

	if (EffectContextHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),AbilitySystemComponent.Get());
	}
}

void ASCharacter::AddStartupEffects()
{
}

void ASCharacter::InitializeFloatingStatusBar()
{
	// Only create once
	if (HealthWidget || !AbilitySystemComponent.IsValid())
	{
		return;
	}
	
}



