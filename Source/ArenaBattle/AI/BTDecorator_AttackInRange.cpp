// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "ABAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// 판단하는 함수 추가

	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory); // 상위 클래스 값을 받아옴

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // Pawn 가져옴
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn); // Pawn Interface를 가져옴
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET)); // Target 값을 가져옴
	if (nullptr == Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target); // 거리
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange(); // AI의 공격 범위
	bResult = (DistanceToTarget <= AttackRangeWithRadius); // 비교 해 반환

	return bResult;
}
