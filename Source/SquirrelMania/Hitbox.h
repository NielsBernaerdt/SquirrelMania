// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SquirrelCharacter.h"
#include "Hitbox.generated.h"

class UBoxComponent;

UENUM(Blueprintable)
enum class EHitboxEnum : uint8
{
	ACTORBOX		UMETA(DisplayName = ACTOR),
	PROXYBOX		UMETA(DisplayName = PROXY),
	STRIKEBOX		UMETA(DisplayName = STRIKE),
	GROUNDSTRIKEBOX UMETA(DisplayName = GROUNDSTRIKE)
};

UCLASS()
class SQUIRRELMANIA_API AHitbox : public AActor
{
	GENERATED_BODY()
public:	
	AHitbox();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = "HitboxParameters")
		EHitboxEnum m_HitboxType;

	UPROPERTY(BlueprintReadWrite, Category = "HitboxParameters")
		FVector m_HitboxLocation;

	UPROPERTY(BlueprintReadWrite, Category = "HitboxParameters")
		FRotator m_HitBoxRotation;

	UPROPERTY(BlueprintReadWrite, Category = "HitboxParameters")
		float m_KnockbackRange = 2000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Construction)
		UBoxComponent* m_pCollisionBox;

	UFUNCTION()
		void CheckCollision();
	UFUNCTION(BlueprintCallable)
		void SpawnHitbox(AActor* owningPlayer);

private:
	AActor* m_pOwningPlayer = nullptr;;
};