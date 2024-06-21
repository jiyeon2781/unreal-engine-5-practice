// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true); // 로딩이 완료되기 전에는 Mesh는 Hide

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0); // 해당 값이 들어왔는지 체크
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1); // 랜덤으로 지정
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
	// 비동기 방식으로 로드 진행 (RequestAsyncLoad)
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AABAIController* ABAIController = Cast<AABAIController>(GetController());
	if (ABAIController)
	{
		ABAIController->StopAI(); // Stop Behavior Tree
	}
	

	// 월드 타이머 서비스 활용
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid()) // 핸들이 유효하면
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset()); // GetLoadedAsset 함수를 사용해 Mesh를 가져옴
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false); // 로딩 후에는 Mesh를 보여줌
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

float AABCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AABCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AABCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AABCharacterNonPlayer::AttackByAI()
{
	// ProcessComboCommand 함수를 입력이 들어올 때마다 호출(ABCharacterPlayer.cpp에서 확인 가능)
	// -> 동일하게 NPC도 해당 함수를 호출해주면 됨
	ProcessComboCommand();
}

void AABCharacterNonPlayer::NotifyComboActionEnd()
{ 
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound(); // 공격이 종료된 후 OnAttackFinished 델리게이트 실행
}
