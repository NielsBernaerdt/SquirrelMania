// DAE - Group 15 - SquirrelMania
#include "SquirrelCharacter.h"
#include "Boundary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "SquirrelController.h"

ASquirrelCharacter::ASquirrelCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	m_pJetpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Jetpack"));
	m_pJetpackMesh->SetupAttachment(GetRootComponent());

	m_pArmorParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Armor"));
	m_pArmorParticle->SetupAttachment(GetRootComponent());

	m_pIndicatorParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Indicator"));
	m_pIndicatorParticle->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}
void ASquirrelCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASquirrelCharacter::OnCapsuleBeginOverlap);
}
void ASquirrelCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_bRecentJumpReset == true
		|| m_bStunned == true)
		return;

	if (Cast<ABoundary>(OtherActor))
	{
		JumpCurrentCount = -1;
		m_bRecentJumpReset = true;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASquirrelCharacter::SetJumpReset, 1, false);
	}
}

void ASquirrelCharacter::SetJumpReset()
{
	m_bRecentJumpReset = false;
}

void ASquirrelCharacter::SetCorrectMesh(int playerIndex)
//Player index starts from 1
{
	GetMesh()->SetSkeletalMesh(m_pPlayerMeshes[playerIndex - 1]);
	GetMesh()->SetMaterial(0, m_pMaterials[(2 * playerIndex) - 2]);
	GetMesh()->SetMaterial(1, m_pMaterials[(2 * playerIndex) - 1]);
}

void ASquirrelCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_CurrentStunTime += DeltaSeconds;
	UnStunCharacter();
	if(m_bCharging)
	{
		
		ChargeParticles();
	}
	PlayLandingParticles();

	if (m_bHasJetpack)
	{
		m_pJetpackMesh->SetHiddenInGame(false);
		if (m_bFlying == false)
		{
			if(m_pJetpackBeamParticle)
			{
				m_pJetpackBeamComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(m_pJetpackBeamParticle, GetRootComponent(), FName(TEXT("JetpackBeam")), FVector::ZeroVector, FRotator::ZeroRotator,EAttachLocation::SnapToTarget,false);
			}
			FlyingJetpack();
			DestroyJetpackDelay();
			m_bFlying = true;
		}
	}
}

void ASquirrelCharacter::MakeArmor()
{
	m_pArmorParticle->SetHiddenInGame(false);
	m_bArmor = true;
}

void ASquirrelCharacter::DestroyArmor()
{
	m_pArmorParticle->SetHiddenInGame(true);
	m_bArmor = false;
}

void ASquirrelCharacter::DestroyJetpack()
{
	m_pJetpackBeamComponent->DeactivateImmediate();
	m_pJetpackMesh->SetHiddenInGame(true);
	m_bHasJetpack = false;
	m_bFlying = false;
}

void ASquirrelCharacter::DestroyJetpackDelay()
{
	GetWorld()->GetTimerManager().SetTimer(m_DestroyJetpackTimer, this, &ASquirrelCharacter::DestroyJetpack, 3.f, false);
}

void ASquirrelCharacter::FlyingJetpack()
{
	if (m_bHasJetpack)
	{
		GetWorld()->GetTimerManager().SetTimer(m_LaucnhTimer, this, &ASquirrelCharacter::FlyingJetpack, 0.01f, true);
		LaunchCharacter(FVector(0, 0, 600.f), false, true);
	}
}

void ASquirrelCharacter::SetForegroundTransparency()
{
	//To create visible linetrace
	//const FName TraceTag("MyTraceTag");
	//GetWorld()->DebugDrawTraceTag = TraceTag;
	//FCollisionQueryParams CollisionParams;
	//CollisionParams.TraceTag = TraceTag;
	//

	FHitResult hitResult;
	FVector traceStart = GetActorLocation();
	FVector traceEnd = GetActorLocation();
	traceEnd.X += 500.f;
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_Visibility);

	if (hitResult.IsValidBlockingHit()
		&& GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Output: %s"), *FString{ hitResult.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass())->GetName() }));
	}
}

void ASquirrelCharacter::ResetBonk()
{
	m_bIsBonked = false;
	m_pBonkParticleComponent->DeactivateImmediate();
}

void ASquirrelCharacter::ChargeParticles()
{
	ASquirrelController* pController = Cast<ASquirrelController>(GetController());
	if(pController->m_ChargeAmount < pController->m_bChargeMinTime)
	{
		if(!m_bIsChargeParticle)
		{
			m_bIsChargeParticle = true;
			if(m_pChargeParticle)
			{

				if (pController->m_PunchDirection.Y < 0)
				{
					m_pChargeParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(m_pChargeParticle, GetRootComponent(), FName(TEXT("Loop")), FVector(-100, 0, 0), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);
				}
				else
				{
					m_pChargeParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(m_pChargeParticle, GetRootComponent(), FName(TEXT("Loop")), FVector(100, 0, 0), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);

				}
			}
			
		}
	} else
	{

		if(!m_bIsChargeLooping)
		{
			m_bIsChargeLooping = true;
			if (m_pChargeLoopParticle)
			{

				if(pController->m_PunchDirection.Y < 0)
				{
					m_pChargeLoopParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(m_pChargeLoopParticle, GetRootComponent(), FName(TEXT("Loop")), FVector(-100, 0, 0), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);
				} else
				{
					m_pChargeLoopParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(m_pChargeLoopParticle, GetRootComponent(), FName(TEXT("Loop")), FVector(100, 0, 0), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);

				}
			}
		}

	}
}

void ASquirrelCharacter::DestroyChargeParticles()
{
	m_bIsChargeParticle = false;
	m_bIsChargeLooping = false;
	if(m_pChargeLoopParticle)
	{
		if(m_pChargeLoopParticleComponent)
			m_pChargeLoopParticleComponent->DeactivateImmediate();

	}
	if(m_pChargeParticle)
	{
		if(m_pChargeParticleComponent)
			m_pChargeParticleComponent->DeactivateImmediate();
	}
}

void ASquirrelCharacter::Bonk()
{
	if(!m_bIsBonked)
	{
		m_bIsBonked = true;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(m_CamShake,0.05f);
		if (m_pBonkParticle)
		{
			m_pBonkParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(m_pBonkParticle, GetRootComponent(), FName(TEXT("Bonk")), FVector(0,0,70), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
			GetWorld()->GetTimerManager().SetTimer(m_BonkTimer, this, &ASquirrelCharacter::ResetBonk, 0.75f, false);
		}
	}
}

void ASquirrelCharacter::PlayLandingParticles()
{
	if (GetCharacterMovement()->IsFalling() == false
		&& m_bHasHitGround == false)
	{
		m_bHasHitGround = true;

		if (m_pLandingParticle->IsValid())
		{
			FVector pos = GetActorLocation();
			pos.Z -= 50.f;
			m_pCurrentLandingParticle = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pLandingParticle, pos, FRotator::ZeroRotator);
		}
		else if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			                                 FString::Printf(TEXT("Output: %s"), *FString{"SQUIRRELCHARACTER PLAYLANDINGPARTICLES - NO VALID PARTICLE TEMPLATE"}));
		}
	}
	else if (GetCharacterMovement()->IsFalling() == true
		&& m_bHasHitGround == true)
	{
		m_bHasHitGround = false;
	}
}

void ASquirrelCharacter::Jump()
{
	Super::Jump();

	if (m_pJumpParticle->IsValid()
		&& (JumpMaxCount - JumpCurrentCount) >= 1)
	{
		FVector pos = GetActorLocation();
		pos.Z += 50.f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pJumpParticle, pos, FRotator::ZeroRotator);
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			FString::Printf(TEXT("Output: %s"), *FString{ "SQUIRRELCHARACTER PLAYJUMPPARTICLE - NO VALID PARTICLE TEMPLATE" }));
	}

}

void ASquirrelCharacter::StunCharacter()
{
	//ATTEMPT AT VIBRATION IN CONTROLLER
	//if(m_pHitVibration)
	//Cast<ASquirrelController>(GetController())->ClientPlayForceFeedback(m_pHitVibration);
	//else
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple,
	//		                                 FString::Printf(TEXT("Output: %s"), *FString{"nononono"}));
	//	}
	//}
	UGameplayStatics::PlaySound2D(GetWorld(), m_pHitSound);

	m_bStunned = true;
	m_CurrentStunTime = 0.f;
}

void ASquirrelCharacter::UnStunCharacter()
{
	if(m_bStunned == true
		&& m_CurrentStunTime >= m_StunResetTime)
	{
		m_bStunned = false;
	}
}

void ASquirrelCharacter::SetVisiblityIndication(bool b)
{
	if(m_pIndicatorParticle)
		m_pIndicatorParticle->SetHiddenInGame(!b);


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Output: %i"), int(b)));
	}
}