// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightBase.h"


ATrafficLightBase::ATrafficLightBase(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
}

//void ATrafficLightBase::OnConstruction(const FTransform &Transform)
//{
//    Super::OnConstruction(Transform);
//    SetTrafficLightState(State);
//}

void ATrafficLightBase::Tick(float DeltaSeconds)
{    // if(TimeIsFrozen){  return;  }

    ElapsedTime += DeltaSeconds;

    float ChangeTime;

    switch (State)
    {
        case ETrafficLightState::Red:
            ChangeTime = RedTime;
            break;
        case ETrafficLightState::Yellow:
            ChangeTime = YellowTime;
            break;
        case ETrafficLightState::Green:
            ChangeTime = GreenTime;
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            return;
    }

    if (ElapsedTime > ChangeTime)
    {
        SwitchTrafficLightState();
    }
}

void ATrafficLightBase::SwitchTrafficLightState()
{
    switch (State)
    {
        case ETrafficLightState::Red:
            SetTrafficLightState(ETrafficLightState::Green);
            break;
        case ETrafficLightState::Yellow:
            SetTrafficLightState(ETrafficLightState::Red);
            break;
        case ETrafficLightState::Green:
            SetTrafficLightState(ETrafficLightState::Yellow);
            break;
        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            break;
    }
}

void ATrafficLightBase::SetTrafficLightState(const ETrafficLightState InState) {
    FString NextState = UEnum::GetValueAsString(InState);
    FString CurrentState = UEnum::GetValueAsString(State);
    UE_LOG(LogTemp, Warning, TEXT("Time: %f; Current State: %s; Next State: %s"), ElapsedTime, *CurrentState, *NextState);

    ElapsedTime = 0.0f;
    State = InState;
    //TODO

    // SetTrafficSignState(ToTrafficSignState(State));
    // VehicleController
    OnTrafficLightStateChanged(State);
}

// Change TrafficSignBase
//static ETrafficSignState ToTrafficSignState(ETrafficLightState State)
//{
//    switch (State)
//    {
//        case ETrafficLightState::Green:
//            return ETrafficSignState::TrafficLightGreen;
//        case ETrafficLightState::Yellow:
//            return ETrafficSignState::TrafficLightYellow;
//        default:
//        case ETrafficLightState::Red:
//            return ETrafficSignState::TrafficLightRed;
//    }
//}

// Getter
float ATrafficLightBase::GetGreenTime() const
{
    return GreenTime;
}


float ATrafficLightBase::GetYellowTime() const
{
    return YellowTime;
}

float ATrafficLightBase::GetRedTime() const
{
    return RedTime;
}

float ATrafficLightBase::GetElapsedTime() const
{
    return ElapsedTime;
}

bool ATrafficLightBase::GetTimeIsFrozen() const
{
    return TimeIsFrozen;
}

// Setter
void ATrafficLightBase::SetGreenTime(float InGreenTime)
{
    GreenTime = InGreenTime;
}

void ATrafficLightBase::SetYellowTime(float InYellowTime)
{
    YellowTime = InYellowTime;
}

void ATrafficLightBase::SetRedTime(float InRedTime)
{
    RedTime = InRedTime;
}


void ATrafficLightBase::SetElapsedTime(float InElapsedTime)
{
    ElapsedTime = InElapsedTime;
}

void ATrafficLightBase::SetTimeIsFrozen(bool InTimeIsFrozen)
{
    TimeIsFrozen = InTimeIsFrozen;
    if (!TimeIsFrozen)
    {
        ElapsedTime = 0.0f;
    }
}