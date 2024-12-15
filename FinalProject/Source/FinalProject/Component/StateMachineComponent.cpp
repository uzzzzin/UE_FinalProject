
#include "StateMachineComponent.h"
#include "GameFramework/Character.h"
#include "../FSM/BaseFSM.h"

UStateMachineComponent::UStateMachineComponent()
	: FSMDataPath(TEXT(""))
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCurFSM(GetFSMKeys()[0]);
	//GetCurFSM()->SetCurStateName(GetCurFSM()->GetStateKeys()[0]);
	//StateMachines[curFSMKey]->
	StateMachines[curFSMKey]->Initialize();
	StateMachines[curFSMKey]->FSM_Begin();
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (true == StateMachines.Contains(curFSMKey) && nullptr != StateMachines[curFSMKey]) // StateMachines�� ����ְ�, nullptr�� �ƴ϶��
	{
		StateMachines[curFSMKey]->FSM_Tick(DeltaTime);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("(UStateMachineComponent::Tick() ---> %s )"), *curFSMKey.ToString()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : UStateMachineComponent::Tick() ---> %s )"), *curFSMKey.ToString()));
	}
}

void UStateMachineComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

void UStateMachineComponent::LoadFSMData(const FString& FilePath)
{
	//! 1. ���� ��� ���� ��, ������ ��θ� ���� DataTable�� �о�´�.
	// LoadObject<>�� ȣȯ�Ǵ� ��η� �ٲ����.
	FString origin = FilePath;
	FString cleanPath = FilePath;
	cleanPath.RemoveFromStart(TEXT("/Script/Engine.DataTable'")); // Ŭ���� ���� ����
	cleanPath.RemoveFromEnd(TEXT("'"));                           // ������ ����ǥ ����

	FSMData = LoadObject<UDataTable>(nullptr, *cleanPath); // datatable �ҷ��Ϳ�
	
	if (nullptr != FSMData) // DataTable�� �־��.
	{
		//! 2. �� ������ ��ȸ�� FSM �̸����� StateMachines�� �ְ�, �ߺ��� �����Ѵ�.
		
		// ���� Ű�� ����ִ´�. (DataTable�� ���Ե��� �ʰ� ���� �ĺ��� ���� Ű)
		TArray<FName> rowNames = FSMData->GetRowNames(); // ��� ���� ������ Ű�� rowNames�� ���� �־��.
		TSet<FName> FSMValues; // �ߺ� ���Ÿ� ���� Set�� ��� ����߾��.

		for (const FName& RowName : rowNames) // �ߺ� ���Ÿ� ���� ��ȸ.
		{
			// �࿡ �ִ� �����͵���, ������ StateMachineData ����ü ������ �̾ƿ�.
			FStateMachineData* Row = FSMData->FindRow<FStateMachineData>(RowName, TEXT(""));
			if (Row)
			{
				FSMValues.Add(FName(Row->FSM)); // 1��° ���� �ִ� FSM ���� �߰��ؿ�. (���̶� �ߺ� �ڵ� ����!)
			}
		}

		FSMKeys = FSMValues.Array(); // TSet�� TArray�� ��ȯ!

		for (const FName& FSM : FSMKeys) // ����� �غ� �̴ϴ�
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("( StateMachine Key --> %s )"), *FSM.ToString()));
		}

		//! 3. FSMData �� �� String�� Ŭ���� �̸����� ������ �ش� Ŭ���� ��ü ���� -> �ش� FSM ���� �̶�� ���̿���.
		if (false == FSMKeys.IsEmpty())
		{
			for (const FName& className : FSMKeys) // ��ȸ�ϸ鼭 �޾ƿ� �̸��� Ŭ���� �̸����� �ٲ��� �ſ���.
			{
				FString FullPath = FString::Printf(TEXT("/Script/FinalProject.%sFSM"), *className.ToString()); // Ŭ���� �̸��� ��ü ��η� ��ȯ���ѿ�.
				UClass* FoundClass = LoadClass<UObject>(nullptr, *FullPath); // Ŭ���� ��ȯ�߽��ϴ�.

				if (!FoundClass) // Ŭ���� �� ã�Ҿ. ���µ�.
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Class Not Found ---> %s )"), *className.ToString()));
					return;
				}

				UBaseFSM* FoundFSM = NewObject<UBaseFSM>(this, FoundClass); // ���� ������Ʈ�� FSM ��ü�� ������ �����ҰԿ�.

				if (nullptr != FoundFSM && false == StateMachines.Contains(className))
				{
					StateMachines.Add(className, FoundFSM); // StateMachin[FSM Key, FSM ptr]
					FoundFSM->SetOwnerStateMachine(this); // FSM�� ���ʸ� this(== SM Component)�� ���������.
				}
				else // FSM�� �� ������׿�? ����.
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Can't Make FSM ---> %s )"), *className.ToString()));
					return;
				}
			}
		}
		else // ���� FSM�� ���׿�? ����.
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("( FAIL : LoadFSMData() FSM Keys are Nothing. )"));
			return;
		}

		//! 4. �ش� FSM�� State���� --> �ε�� FSMData���� ������ ---> AddState �Ѵ�.
		for (const FName& RowName : rowNames) // ���� State���� �������� ���� FSMData���� ����� �̾� ��ȸ�ؿ�.
		{
			// �࿡ �ִ� �����͵���, ������ StateMachineData ����ü ������ �̾ƿ�.
			FStateMachineData* Row = FSMData->FindRow<FStateMachineData>(RowName, TEXT(""));

			for (const FName& FSMKey : FSMKeys)
			{
				if (FSMKey == FName(Row->FSM)) // StateMachine�� ����ִ� FSM Key�� ���� ���� �ִ� DataTable For���� �� Key�� ���ƿ�
				{
					// �׷� State ��ü ���� �־����.
					FString stateName = Row->State;
					//FString stateClassName = FString::Printf(TEXT("%s%sState"), *FSMKey.ToString(), *stateName); //! State Ŭ������ ��Ģ : FSM Key + State Key + 'State'
					FString StateClassPath = FString::Printf(TEXT("/Script/FinalProject.%s%sState"), *FSMKey.ToString(), *stateName); // Ŭ���� �̸��� ��ü ��η� ��ȯ���ѿ�.
					UClass* FoundClass = LoadClass<UObject>(nullptr, *StateClassPath); // Ŭ���� ��ȯ�߽��ϴ�.

					if (!FoundClass) // State Ŭ���� �� ã�Ҿ. ���µ�. ����.
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Class Not Found ---> %s%sState )"), *FSMKey.ToString(), *stateName));
						return;
					}

					UObject* FoundState = NewObject<UObject>(this, FoundClass); // ���� ������Ʈ�� State ��ü�� ������ �����ҰԿ�.
					
					//StateMachines[FName(Row->FSM)]->GetStates(); // �̷��� �����ϼ���.

					if (nullptr != FoundState && false == StateMachines[FName(Row->FSM)]->GetStates().Contains(FName(stateName)))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *stateName));
						StateMachines[FName(Row->FSM)]->GetStateKeys().Add(FName(stateName)); // State Key�� ��Ƶֿ�.
						if (0 == StateMachines[FName(Row->FSM)]->GetStateKeys().Num())
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL :holyShirrttttttttt");

						}
						StateMachines[FName(Row->FSM)]->GetStates().Add(FName(stateName), Cast<IState>(FoundState)); // States[State Key, State ptr]
						Cast<IState>(FoundState)->SetOwnerFSM(StateMachines[FName(Row->FSM)]); // ���� State�� Owner�� FSM���� ����������.
					}
					else // FSM�� �� ������׿�? ����.
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Can't Make State ---> %s%sState )"), **FSMKey.ToString(), *stateName));
					}
				}
			}
		}

	}
	else // DataTable �� �����. ����.
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() DataTable is Nothing ---> %s )"), *cleanPath));
		return;
	}
}

bool UStateMachineComponent::ChangeFSM(FName _wannaKey)
{
	//! ���������δ� Key�� �ٲ��ִ� �ſ���.
	if (false == StateMachines.Contains(_wannaKey)) // �ٲ� FSM�� �� ����ִµ���?
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : ChangeFSM() Key is Nothing ---> %s )"), *_wannaKey.ToString()));
		return false;
	}
	else
	{
		// ���� FSM Key�� �ٲ� FSM Key���� �ٲ����.
		if (nullptr != StateMachines[curFSMKey] && true == StateMachines.Contains(curFSMKey))  // StateMachines�� ����ְ�, nullptr�� �ƴ϶��
		{
			StateMachines[curFSMKey]->FSM_Exit();
			curFSMKey = _wannaKey;
			StateMachines[curFSMKey]->FSM_Begin();
		}
		else // ���� StateMachine �� �����ϴ�
		{
			// ex) StateMachine�� ó�� ���� ���� ����State�� ���� ��? ����������
		}

		return true;
	}
}