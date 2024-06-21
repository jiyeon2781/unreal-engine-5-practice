// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate);
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() { }
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;
};

UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABStageGimmick();

public:
	FORCEINLINE int32 GetStageNum() const { return CurrentStageNum; }
	FORCEINLINE void SetStageNum(int32 NewStageNum) { CurrentStageNum = NewStageNum; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

// Stage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;
	
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// Gate Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;
	// 게이트는 네개가 필요하므로 Map으로 구성
	// 각각의 고유한 키를 전달해 네 방향 중 하나를 키로 전달 -> 해당하는 문에 대한 StaticMesh를 얻을 수 있도록 설정

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGates();
	void CloseAllGates();

// State Section
protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	void SetState(EStageState InNewState); // 상태 변경

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

// Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABCharacterNonPlayer> OpponentClass;
	// NPC를 생성하기 위해 클래스 정보를 넣도록 변수 추가
	// TSubClassOf : 우리가 지정한 클래스로부터 상속받은 클래스 목록만 표시되도록 한정해서 지정할 수 있게 기능 제공
	// -> 해당 기능(클래스 정보를 한정시켜주는 기능)을 사용해 편리하게 스폰시킬 NPC 지정 가능

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;
	// 스폰 딜레이 시간

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor); // NPC 사망 시 보상 단계 진행

	FTimerHandle OpponentTimerHandle;
	void OnOpponentSpawn();

// Reward Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> RewardBoxClass; // 보상 박스 Spawn 클래스

	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> RewardBoxes;
	// TWeakObjectPtr : 약참조 -> RewardBoxes라고 하는 변수는 스폰된 상자를 관리하기 위해 선언
	// 스폰된 상자들은 작업하고 있는 스테이지 기믹 액터와는 무관하게 자기 스스로 동작
	// 외부의 영향, 내부의 로직으로 인해 스스로 소멸 가능 
	// -> 강참조(TObjectPtr)로 걸게되면 언리얼 엔진은 이 상자는 아직 사용되고 있다고 판단해 메모리에서 소멸시키지 않을 수 있음
	// 얘와 무관하지만 해당 변수를 관리해야 한다는 측면에서 약참조로 선언

	// 액터 선언에서 멤버변수로 지니고 있는 언리얼 오브젝트들은 모두 강참조로 선언했음 -> 액터가 소멸될 때 같이 소멸되는 것이 맞으므로 강참조가 알맞음
	// 액터와 무관하게 동작하는 액터들의 경우 -> 약참조를 걸어서 관리하는 것이 좋음

	TMap<FName, FVector> RewardBoxLocations;
	// 리워드 박스를 스폰 시킬 위치 -> Key로 관리

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnRewardBoxes();

// Stage Stat
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum; // 레벨 정보에 해당하는 값
};
