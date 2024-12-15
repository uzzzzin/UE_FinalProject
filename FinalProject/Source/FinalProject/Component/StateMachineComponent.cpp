
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

	if (true == StateMachines.Contains(curFSMKey) && nullptr != StateMachines[curFSMKey]) // StateMachines에 들어있고, nullptr이 아니라면
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
	//! 1. 파일 경로 수정 후, 수정된 경로를 통해 DataTable을 읽어온다.
	// LoadObject<>에 호환되는 경로로 바꿔줘요.
	FString origin = FilePath;
	FString cleanPath = FilePath;
	cleanPath.RemoveFromStart(TEXT("/Script/Engine.DataTable'")); // 클래스 정보 제거
	cleanPath.RemoveFromEnd(TEXT("'"));                           // 마지막 따옴표 제거

	FSMData = LoadObject<UDataTable>(nullptr, *cleanPath); // datatable 불러와요
	
	if (nullptr != FSMData) // DataTable이 있어요.
	{
		//! 2. 행 단위로 순회해 FSM 이름들을 StateMachines에 넣고, 중복을 제거한다.
		
		// 행의 키를 집어넣는다. (DataTable에 포함되지 않고 고유 식별을 위한 키)
		TArray<FName> rowNames = FSMData->GetRowNames(); // 모든 행을 가져와 키를 rowNames에 집어 넣어요.
		TSet<FName> FSMValues; // 중복 제거를 위해 Set을 잠시 사용했어요.

		for (const FName& RowName : rowNames) // 중복 제거를 위한 순회.
		{
			// 행에 있는 데이터들을, 만들어둔 StateMachineData 구조체 단위로 뽑아요.
			FStateMachineData* Row = FSMData->FindRow<FStateMachineData>(RowName, TEXT(""));
			if (Row)
			{
				FSMValues.Add(FName(Row->FSM)); // 1번째 열에 있는 FSM 값을 추가해요. (셋이라 중복 자동 제거!)
			}
		}

		FSMKeys = FSMValues.Array(); // TSet을 TArray로 변환!

		for (const FName& FSM : FSMKeys) // 디버깅 해본 겁니다
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("( StateMachine Key --> %s )"), *FSM.ToString()));
		}

		//! 3. FSMData 에 든 String을 클래스 이름으로 설정해 해당 클래스 객체 생성 -> 해당 FSM 생성 이라는 뜻이에요.
		if (false == FSMKeys.IsEmpty())
		{
			for (const FName& className : FSMKeys) // 순회하면서 받아온 이름을 클래스 이름으로 바꿔줄 거예요.
			{
				FString FullPath = FString::Printf(TEXT("/Script/FinalProject.%sFSM"), *className.ToString()); // 클래스 이름을 전체 경로로 변환시켜요.
				UClass* FoundClass = LoadClass<UObject>(nullptr, *FullPath); // 클래스 소환했습니닷.

				if (!FoundClass) // 클래스 못 찾았어여. 없는듯여.
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Class Not Found ---> %s )"), *className.ToString()));
					return;
				}

				UBaseFSM* FoundFSM = NewObject<UBaseFSM>(this, FoundClass); // 현재 컴포넌트가 FSM 객체의 수명을 관리할게요.

				if (nullptr != FoundFSM && false == StateMachines.Contains(className))
				{
					StateMachines.Add(className, FoundFSM); // StateMachin[FSM Key, FSM ptr]
					FoundFSM->SetOwnerStateMachine(this); // FSM의 오너를 this(== SM Component)로 설정해줘요.
				}
				else // FSM을 못 만들었네요? 리턴.
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Can't Make FSM ---> %s )"), *className.ToString()));
					return;
				}
			}
		}
		else // 만들 FSM이 없네요? 리턴.
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("( FAIL : LoadFSMData() FSM Keys are Nothing. )"));
			return;
		}

		//! 4. 해당 FSM의 State까지 --> 로드된 FSMData에서 가져와 ---> AddState 한다.
		for (const FName& RowName : rowNames) // 현재 State들을 가져오기 위해 FSMData에서 행들을 뽑아 순회해요.
		{
			// 행에 있는 데이터들을, 만들어둔 StateMachineData 구조체 단위로 뽑아요.
			FStateMachineData* Row = FSMData->FindRow<FStateMachineData>(RowName, TEXT(""));

			for (const FName& FSMKey : FSMKeys)
			{
				if (FSMKey == FName(Row->FSM)) // StateMachine에 들어있는 FSM Key랑 지금 돌고 있는 DataTable For문에 든 Key랑 같아여
				{
					// 그럼 State 객체 만들어서 넣어줘요.
					FString stateName = Row->State;
					//FString stateClassName = FString::Printf(TEXT("%s%sState"), *FSMKey.ToString(), *stateName); //! State 클래스명 규칙 : FSM Key + State Key + 'State'
					FString StateClassPath = FString::Printf(TEXT("/Script/FinalProject.%s%sState"), *FSMKey.ToString(), *stateName); // 클래스 이름을 전체 경로로 변환시켜요.
					UClass* FoundClass = LoadClass<UObject>(nullptr, *StateClassPath); // 클래스 소환했습니닷.

					if (!FoundClass) // State 클래스 못 찾았어여. 없는듯여. 리턴.
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Class Not Found ---> %s%sState )"), *FSMKey.ToString(), *stateName));
						return;
					}

					UObject* FoundState = NewObject<UObject>(this, FoundClass); // 현재 컴포넌트가 State 객체의 수명을 관리할게요.
					
					//StateMachines[FName(Row->FSM)]->GetStates(); // 이렇게 접근하세요.

					if (nullptr != FoundState && false == StateMachines[FName(Row->FSM)]->GetStates().Contains(FName(stateName)))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *stateName));
						StateMachines[FName(Row->FSM)]->GetStateKeys().Add(FName(stateName)); // State Key만 모아둬요.
						if (0 == StateMachines[FName(Row->FSM)]->GetStateKeys().Num())
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL :holyShirrttttttttt");

						}
						StateMachines[FName(Row->FSM)]->GetStates().Add(FName(stateName), Cast<IState>(FoundState)); // States[State Key, State ptr]
						Cast<IState>(FoundState)->SetOwnerFSM(StateMachines[FName(Row->FSM)]); // 현재 State의 Owner를 FSM으로 지정해줬어요.
					}
					else // FSM을 못 만들었네요? 리턴.
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() Can't Make State ---> %s%sState )"), **FSMKey.ToString(), *stateName));
					}
				}
			}
		}

	}
	else // DataTable 이 없어요. 리턴.
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : LoadFSMData() DataTable is Nothing ---> %s )"), *cleanPath));
		return;
	}
}

bool UStateMachineComponent::ChangeFSM(FName _wannaKey)
{
	//! 실질적으로는 Key만 바꿔주는 거예요.
	if (false == StateMachines.Contains(_wannaKey)) // 바꿀 FSM이 안 들어있는데요?
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : ChangeFSM() Key is Nothing ---> %s )"), *_wannaKey.ToString()));
		return false;
	}
	else
	{
		// 현재 FSM Key를 바꿀 FSM Key으로 바꿔줘요.
		if (nullptr != StateMachines[curFSMKey] && true == StateMachines.Contains(curFSMKey))  // StateMachines에 들어있고, nullptr이 아니라면
		{
			StateMachines[curFSMKey]->FSM_Exit();
			curFSMKey = _wannaKey;
			StateMachines[curFSMKey]->FSM_Begin();
		}
		else // 현재 StateMachine 이 없습니닷
		{
			// ex) StateMachine이 처음 들어올 때는 현재State가 없을 떄? 비정상적일
		}

		return true;
	}
}