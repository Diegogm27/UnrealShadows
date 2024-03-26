#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "US_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPChanged, int32, NewXp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelUp, int32, NewLevelXp);

UCLASS()
class UNREALSHADOWS_API AUS_PlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_XP", Category = "Experience")
	int XP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_CharacterLevelUp", Category = "Experience")
	int CharacterLevel = 1;

	UFUNCTION()
	void OnRep_XP(int32 OldValue) const;

	UFUNCTION()
	void OnRep_CharacterLevelUp(int32 OldValue) const;

public:

	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddXP(int32 Value);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnXPChanged OnXPChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterLevelUp OnCharacterLevelUp;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
