// Fill out your copyright notice in the Description page of Project Settings.


#include "US_MainMenuPawn.h"
#include "Camera/CameraComponent.h" 
#include "Components/ArrowComponent.h"
#include "US_CharacterSkins.h" 
#include "US_GameInstance.h"

// Sets default values
AUS_MainMenuPawn::AUS_MainMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(450.f, 90.f, 160.f));
	Camera->SetRelativeRotation(FRotator(-10.f, 180.f, 0.f));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation(FVector(0.f, -30.f, 90.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue"));
	if (SkeletalMeshAsset.Succeeded())
	{
		Mesh->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AUS_MainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled())
	{
		RandomizeCharacterSkin();
	}
}

void AUS_MainMenuPawn::RandomizeCharacterSkin()
{
	if (CharacterSkinDataTable)
	{
		TArray<FUS_CharacterSkins*> CharacterSkinsRows;
		CharacterSkinDataTable->GetAllRows<FUS_CharacterSkins>(TEXT("US_Character"), CharacterSkinsRows);
		if (CharacterSkinsRows.Num() > 0)
		{
			const auto NewIndex = FMath::RandRange(0, CharacterSkinsRows.Num() - 1);
			CharacterSkin = CharacterSkinsRows[NewIndex];
			Mesh->SetMaterial(0, CharacterSkinsRows[NewIndex]->Material0);
			Mesh->SetMaterial(1, CharacterSkinsRows[NewIndex]->Material1);
			Mesh->SetMaterial(2, CharacterSkinsRows[NewIndex]->Material2);
			Mesh->SetMaterial(4, CharacterSkinsRows[NewIndex]->Material4);
			if (const auto GameInstance = Cast<UUS_GameInstance>(GetGameInstance()))
			{
				GameInstance->SkinIndex = NewIndex;
			}
		}
	}
}

// Called every frame
void AUS_MainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUS_MainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

