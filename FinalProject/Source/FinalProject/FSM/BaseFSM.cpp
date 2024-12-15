// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFSM.h"
#include "../Component/StateMachineComponent.h"

UBaseFSM::UBaseFSM()
{
}

void UBaseFSM::FSM_Begin()
{
    // States 맵 순회하고 State들의 Owner를 this로 세팅해주세요.
    States[curStateName]->Enter_Implementation();

    // 상속한 자식에서 무조건 초기화해줘요
    //animInst = GetOwnerStateMachine()->GetOwner()->GetMesh()->GetAnimInstance());
}

void UBaseFSM::FSM_Tick(float DeltaTime)
{
	if (nullptr == States[curStateName])
	{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FSM::FSM_Tick ---> %s )"), *curStateName.ToString()));
        return;
	}
    else
    {
		States[curStateName]->Update_Implementation(DeltaTime); // 현재 State의 틱도 돌려줘야지요.
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("(FSM::FSM_Tick ---> %s )"), *curStateName.ToString()));
    }
}

void UBaseFSM::FSM_Exit()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UBaseFSM::FSM_Exit()");
}

void UBaseFSM::Initialize()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UBaseFSM::Initialize()");
    // 자식들은 여기서 State들을 추가하거나 초기 세팅을 할 거예요.
}

FName UBaseFSM::GetStateNameByStateValue(IState* _value)
{
    for (const auto& pair : States)
    {
        if (pair.Value == _value)
        {
            return pair.Key; // 일치하면 키인 State Name 반환해요.
        }
    }

    // 키를 찾지 못했어요.
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : UBaseFSM::GetStateNameByStateValue()"));
    return FName(); // IsNone()으로 체크 가능!!
}

void UBaseFSM::ChangeState(FName _key)
{
	FName prevStateName = curStateName;

	// 1. 현재 State -> Exit
	// 2. 전환될 State ->Exter
	// 3. 현재 StateName -> 전환될 StateName
    if (nullptr != States[_key])
    {
	    States[curStateName]->Exit_Implementation();
	    curStateName = _key;
	    States[curStateName]->Enter_Implementation();
	    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("( ChangeState() %s ---> %s  )"), *prevStateName.ToString(), *curStateName.ToString()));

        SetAnimInstState(prevStateName, _key);
    }
    else
    {
	    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : ChangeState() %s ---> %s  )"), *prevStateName.ToString(), *curStateName.ToString()));
    }
}

void UBaseFSM::SetAnimInstState(FName _prev, FName _key)
{
    // 자식에서 각각 꼭 처리해주세요 우진씨 까먹지 말고 우진아 까먹지 마라 제발 지짜
    // ChangeState 할 떄 실행되는 함수예요.
}

//void UBaseFSM::ChangeState(FName _key)
//{
//	//IState* tmpState = FindState(_key);
//
//	//if (nullptr == FindState<T>(_key))
//	if (false == States.Contains(_key))
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FindState() Not Found key. ---> %s)"), *_key.ToString()));
//		ChangeState(curStateName); // 바꿀 State가 없는 State라면 현재 State를 다시 실행해요
//		return;
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( SUCCESS : FindState() ---> %s )"), *_key.ToString()));
//
//		// 1. 현재 State -> Exit
//		// 2. 전환될 State ->Exter
//		// 3. 현재 StateName -> 전환될 StateName
//		States[curStateName]->Exit_Implementation();
//		curStateName = _key;
//		States[curStateName]->Enter_Implementation();
//	}
//}
