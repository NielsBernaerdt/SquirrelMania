// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SquirrelCharacter.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SQUIRRELMANIA_API ASquirrelCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ASquirrelCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void SetForegroundTransparency();


#pragma region DoubleJump
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Double Jump")
		bool m_FallingThrough = false;


	virtual void Jump() override;
private:
	bool m_bRecentJumpReset = false;
	bool m_bHasHitGround = true;
	void SetJumpReset();
#pragma endregion DoubleJump
#pragma region Jetpack
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
		bool m_bHasJetpack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jetpack")
		UStaticMeshComponent* m_pJetpackMesh;

	FTimerHandle m_DestroyJetpackTimer;
private:
	bool m_bFlying = false;
	FTimerHandle m_LaucnhTimer;

	void DestroyJetpack();
	void DestroyJetpackDelay();
	void FlyingJetpack();
#pragma endregion Jetpack
#pragma region Combat
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool m_bAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool m_bCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool m_bHeavyAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool m_bGroundPound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_KnockbackRangeHeavy = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float m_KnockbackRangeHeavyDefault = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool m_bStunned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool m_bHeavyReset = false;

	void StunCharacter();
	void UnStunCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int m_Lives = 4;

	UFUNCTION()
		void MakeArmor();

	UFUNCTION()
		void DestroyArmor();
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
		bool m_bArmor = false;

private:
	float m_CurrentStunTime = 0.f;
	float m_StunResetTime = 0.5f;
#pragma endregion Combat
#pragma region Player Customization
public:
	UPROPERTY(EditAnywhere, Category = "Player Customization")
		TArray<USkeletalMesh*> m_pPlayerMeshes;
	UPROPERTY(EditAnywhere, Category = "Player Customization")
		TArray<UMaterialInterface*> m_pMaterials;

	void SetCorrectMesh(int playerIndex);
#pragma endregion Player Customization
#pragma region Particles
public:
	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraSystem* m_pLandingParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraComponent* m_pArmorParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraComponent* m_pIndicatorParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraSystem* m_pJetpackBeamParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraComponent* m_pJetpackBeamComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraSystem* m_pBonkParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraComponent* m_pBonkParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraSystem* m_pJumpParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraSystem* m_pChargeParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraSystem* m_pChargeLoopParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraComponent* m_pChargeLoopParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UNiagaraComponent* m_pChargeParticleComponent;

	void ChargeParticles();
	void DestroyChargeParticles();

	void SetVisiblityIndication(bool b);
private:
	UNiagaraComponent* m_pCurrentLandingParticle;
	bool m_bIsChargeParticle;

	bool m_bIsChargeLooping;
#pragma endregion Particles
#pragma region PlayerFeedback|Other
public:
	UPROPERTY(EditAnywhere, Category = "PlayerFeedback|Other")
		TSubclassOf<UCameraShakeBase> m_CamShake;

	UPROPERTY(EditAnywhere, Category = "PlayerFeedback|Other")
		UForceFeedbackEffect* m_pHitVibration;

	UPROPERTY(EditAnywhere, Category = "PlayerFeedback|Other")
		USoundBase* m_pHitSound;

	UFUNCTION()
		void Bonk();
private:
	FTimerHandle m_BonkTimer;
	bool m_bIsBonked = false;

	void ResetBonk();

	void PlayLandingParticles();
#pragma endregion PlayerFeedback|Other

	
};