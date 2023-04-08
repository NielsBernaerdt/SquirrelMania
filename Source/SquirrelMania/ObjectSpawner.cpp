// DAE - Group 15 - SquirrelMania
#include "ObjectSpawner.h"
#include "Kismet/GameplayStatics.h"

AObjectSpawner::AObjectSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AFallingPlatform> FallingPlatformClass(TEXT("/Game/GamePlayObjects/Blueprints/BP_FallingPlatform"));
	if (FallingPlatformClass.Class != nullptr)
	{
		m_FallingPlatformRef = FallingPlatformClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AJetpack> JetpackClass(TEXT("/Game/GamePlayObjects/Blueprints/BP_Jetpack"));
	if (JetpackClass.Class != nullptr)
	{
		m_JetpackRef = JetpackClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AArmorPickup> ArmorClass(TEXT("/Game/GamePlayObjects/Blueprints/BP_ArmorPickup"));
	if (ArmorClass.Class != nullptr)
	{
		m_ArmorRef = ArmorClass.Class;
	}
}

void AObjectSpawner::BeginPlay()
{
	Super::BeginPlay();

	m_pController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	GetWorld()->GetTimerManager().SetTimer(m_FallingPlatformTimer, this, &AObjectSpawner::SpawnPlatform, 5, true);
	GetWorld()->GetTimerManager().SetTimer(m_JetpackTimer, this, &AObjectSpawner::SpawnJetpack, 30, true);
	GetWorld()->GetTimerManager().SetTimer(m_ArmorTimer, this, &AObjectSpawner::SpawnArmor, 45, true);
	m_ViewportSize = FVector(GEngine->GameViewport->Viewport->GetSizeXY());
}

void AObjectSpawner::SpawnPlatform()
{
	//AFallingPlatform* pFallingPlatform = GetWorld()->SpawnActor<AFallingPlatform>(m_FallingPlatformRef, FTransform(FVector(0, FMath::RandRange(m_OffsetYMin, m_OffsetYMax),m_WorldLoc.Z + m_ViewportSize.Y + m_OffsetZ)));
	//pFallingPlatform->LetPlatformFall();

	m_pFallingPlatform = GetWorld()->SpawnActor<AFallingPlatform>(m_FallingPlatformRef, FTransform(FVector(0, FMath::RandRange(m_OffsetYMin, m_OffsetYMax),m_WorldLoc.Z + m_ViewportSize.Y + m_OffsetZ)));
	GetWorld()->GetTimerManager().SetTimer(m_FallingPlatformWaitTimer, this, &AObjectSpawner::WaitPlatform, 3, false);
	m_bIsWaiting = true;
	InvisPlatform();
}

void AObjectSpawner::SpawnJetpack()
{
	AJetpack* pJetpack = GetWorld()->SpawnActor<AJetpack>(m_JetpackRef, FTransform(FVector(0, 0, 0)));
	FVector newLoc;
	bool canTp = false;
	while(canTp == false)
	{
		newLoc = FVector(0, FMath::RandRange(m_OffsetYMin, m_OffsetYMax), m_WorldLoc.Z - FMath::RandRange(m_OffsetZJetpackMin, m_OffsetZJetpackMax));
		canTp = GetWorld()->FindTeleportSpot(pJetpack, newLoc, FRotator(0, 0, 0));
		
	}
	if(canTp)
	{
		if(pJetpack)
		{
			pJetpack->SetActorLocation(newLoc);
			
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TP JETPACK"));
		}
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("JETPACK NOT PLACABLE"));
	}	
}

void AObjectSpawner::SpawnArmor()
{
	AArmorPickup* pArmor = GetWorld()->SpawnActor<AArmorPickup>(m_ArmorRef, FTransform(FVector(0, 0, 0)));
	FVector newLoc;
	newLoc = FVector(0, FMath::RandRange(m_OffsetYMin, m_OffsetYMax), m_WorldLoc.Z);
	bool canTp = false;
	while (canTp == false)
	{
		newLoc = FVector(0, FMath::RandRange(m_OffsetYMin, m_OffsetYMax), m_WorldLoc.Z - FMath::RandRange(m_OffsetZJetpackMin, m_OffsetZJetpackMax));
		canTp = GetWorld()->FindTeleportSpot(pArmor, newLoc, FRotator(0, 0, 0));
	}
	if (canTp)
	{
		if(pArmor)
		{
			pArmor->SetActorLocation(newLoc);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ARMOR NOT PLACABLE"));
	}
}
void AObjectSpawner::WaitPlatform()
{
	m_pFallingPlatform->SetHidden(false);
	m_bIsWaiting = false;
	m_pFallingPlatform->LetPlatformFall();
}
void AObjectSpawner::InvisPlatform()
{
	if(m_bIsWaiting)
	{
		if(m_pFallingPlatform)
		{
			if(m_pFallingPlatform->IsHidden())
			{
				m_pFallingPlatform->SetActorHiddenInGame(false);
			} else
			{
				m_pFallingPlatform->SetActorHiddenInGame(true);
			}
		}
	GetWorld()->GetTimerManager().SetTimer(m_FallingPlatformInvisTimer, this, &AObjectSpawner::InvisPlatform, 0.5f, false);
	}
}
void AObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_pController->DeprojectScreenPositionToWorld(0, 0, m_WorldLoc, m_WorldDir);
	if(m_bIsWaiting)
	{
		if(m_pFallingPlatform)
		{
			FVector curLoc = m_pFallingPlatform->GetActorLocation();
			m_pFallingPlatform->SetActorLocation(FVector(curLoc.X,curLoc.Y, m_WorldLoc.Z + m_ViewportSize.Y + m_OffsetZ));
			
		}
	}
}