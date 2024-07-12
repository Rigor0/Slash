// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Slash/DebugMacros.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "NiagaraComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	DeathEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dissolve Effect"));
	DeathEffect->SetupAttachment(GetRootComponent());
	DeathEffect->SetAutoActivate(false);

	Hat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turban"));
	Hat->SetupAttachment(GetMesh());
}

void AEnemy::OnDeathEffectFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
	FAttachmentTransformRules TransfromRules(EAttachmentRule::SnapToTarget, true);
	Hat->AttachToComponent(GetMesh(), TransfromRules, FName("Hat"));

	/*FString Message = FString::Printf(TEXT("The value of Index Num is: %d"), DeathMontages.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);*/
}

void AEnemy::Die()
{
	int randomMontageIndex = FMath::RandRange(0, DeathMontages.Num()- 1);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontages.Num() > 0)
	{
		AnimInstance->Montage_Play(DeathMontages[randomMontageIndex]);
		if (DeathEffect) 
		{
			DeathEffect->Activate(true);
		}

		FTimerHandle MemberTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::DestroyMeshes, 2.0f, false);
		DeathEffect->OnSystemFinished.AddDynamic(this, &AEnemy::OnDeathEffectFinished);
	}
}

void AEnemy::DestroyMeshes()
{
	if (GetMesh())
	{
		GetMesh()->DestroyComponent();
		Hat->DestroyComponent();
	}
}

void AEnemy::PlayHitReactMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Attributes && Attributes->IsAlive())
		PlayHitReactMontage();
	else
		Die();

	const FVector Forward = GetActorForwardVector();
	const FVector ToHit = (ImpactPoint - GetActorLocation());

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarComponent)
	{
		Attributes->RecieveDamage(DamageAmount);
		HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
	}
	return DamageAmount;
}

