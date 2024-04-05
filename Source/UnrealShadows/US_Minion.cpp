// Fill out your copyright notice in the Description page of Project Settings.


#include "US_Minion.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "US_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"

// Sets default values
AUS_Minion::AUS_Minion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SensingInterval = .8f;
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 1500.f;
	PawnSense->HearingThreshold = 400.f;
	PawnSense->LOSHearingThreshold = 800.f;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(100);
	Collision->SetupAttachment(RootComponent);
	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("/Game/KayKit/Skeletons/skeleton_minion"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
}

// Called when the game starts or when spawned
void AUS_Minion::BeginPlay()
{
	Super::BeginPlay();

	SetNextPatrolLocation();	
}

// Called every frame
void AUS_Minion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return;
	if (GetMovementComponent()->GetMaxSpeed() == ChaseSpeed) return;
	if ((GetActorLocation() - PatrolLocation).Size() < 500.f)
	{
		SetNextPatrolLocation();
	}
}

// Called to bind functionality to input
void AUS_Minion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUS_Minion::SetNextPatrolLocation()
{
	if (GetLocalRole() != ROLE_Authority) return;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	const auto LocationFound = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, GetActorLocation(), PatrolLocation, PatrolRadius);
	if (LocationFound)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
	}
}
void AUS_Minion::Chase(APawn* Pawn)
{
	if (GetLocalRole() != ROLE_Authority) return;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Pawn);
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 25.f, 12, FColor::Red, true, 10.f, 0, 2.f);
}

void AUS_Minion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetLocalRole() != ROLE_Authority) return;
	OnActorBeginOverlap.AddDynamic(this, &AUS_Minion::OnBeginOverlap);
	GetPawnSense()->OnSeePawn.AddDynamic(this, &AUS_Minion::OnPawnDetected);
}

void AUS_Minion::OnPawnDetected(APawn* Pawn)
{
	if (!Pawn->IsA<AUS_Character>()) return;
	if (GetCharacterMovement()->MaxWalkSpeed != ChaseSpeed)
	{
		Chase(Pawn);
	}
}

void AUS_Minion::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA<AUS_Character>()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Character captured!"));
}

