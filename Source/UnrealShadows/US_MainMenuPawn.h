// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "US_MainMenuPawn.generated.h"

UCLASS()
class UNREALSHADOWS_API AUS_MainMenuPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CharacterSkinDataTable;
	
	struct FUS_CharacterSkins* CharacterSkin;

public:
	// Sets default values for this pawn's properties
	AUS_MainMenuPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FORCEINLINE UArrowComponent* GetArrow() const { return Arrow; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }

	void RandomizeCharacterSkin();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE FUS_CharacterSkins* GetCharacterSkin() const { return CharacterSkin; }

};
