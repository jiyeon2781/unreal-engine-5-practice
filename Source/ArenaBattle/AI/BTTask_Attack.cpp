// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn()); // AI Controller가 조종하고 있는 Pawn을 가져옴
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn); // Interface를 통해 공격 명령
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			// 끝날 때 필요한 함수 호출
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			// FinishLatentTask -> 델리게이트가 마무리되었다고 알려주는 함수
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished); // 델리게이트 설정
	AIPawn->AttackByAI(); // 공격을 시작하고 나서 바로 끝나는 것이 아닌 몽타주가 끝난후 Succeded를 반환해줘야 함 -> 공격이 끝나는 타이밍을 캐치해야 함
	return EBTNodeResult::InProgress;
}
