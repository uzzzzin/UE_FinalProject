// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFSM.h"

UBaseFSM::UBaseFSM()
{
}

void UBaseFSM::FSM_Begin()
{
    // States �� ��ȸ�ϰ� State���� Owner�� this�� �������ּ���.
    States[curStateName]->Enter_Implementation();
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
		States[curStateName]->Update_Implementation(DeltaTime); // ���� State�� ƽ�� �����������.
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
    // �ڽĵ��� ���⼭ State���� �߰��ϰų� �ʱ� ������ �� �ſ���.
}

FName UBaseFSM::GetStateNameByStateValue(IState* _value)
{
    for (const auto& pair : States)
    {
        if (pair.Value == _value)
        {
            return pair.Key; // ��ġ�ϸ� Ű�� State Name ��ȯ�ؿ�.
        }
    }

    // Ű�� ã�� ���߾��.
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : UBaseFSM::GetStateNameByStateValue()"));
    return FName(); // IsNone()���� üũ ����!!
}

//void UBaseFSM::ChangeState(FName _key)
//{
//	//IState* tmpState = FindState(_key);
//
//	//if (nullptr == FindState<T>(_key))
//	if (false == States.Contains(_key))
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FindState() Not Found key. ---> %s)"), *_key.ToString()));
//		ChangeState(curStateName); // �ٲ� State�� ���� State��� ���� State�� �ٽ� �����ؿ�
//		return;
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( SUCCESS : FindState() ---> %s )"), *_key.ToString()));
//
//		// 1. ���� State -> Exit
//		// 2. ��ȯ�� State ->Exter
//		// 3. ���� StateName -> ��ȯ�� StateName
//		States[curStateName]->Exit_Implementation();
//		curStateName = _key;
//		States[curStateName]->Enter_Implementation();
//	}
//}
