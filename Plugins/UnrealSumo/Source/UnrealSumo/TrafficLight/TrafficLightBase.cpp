// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightBase.h"
#include "UnrealSumo/World/SumoGameMode.h"
#include "Engine/World.h"
#include "UnrealSumo/World/SumoGameInstance.h"
#include "UnrealSumo/src/TraCIDefs.h"


ATrafficLightBase::ATrafficLightBase(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATrafficLightBase::BeginPlay() {
    Super::BeginPlay();

//    // Require SumoGameMode to setup SumoGameInstance and then get SumoGameInstance in TrafficLightBase BeginPlay
//    ASumoGameMode* GameMode = Cast<ASumoGameMode>(GetWorld()->GetAuthGameMode());
//    if(!GameMode->HasActorBegunPlay()){
//        GameMode->DispatchBeginPlay();
//    }
//    UE_LOG(LogTemp, Error, TEXT("TrafficLightBase BeginPlay"))
//
//    SumoGameInstance = Cast<USumoGameInstance>(GetGameInstance());
//    if(SumoGameInstance->client){
//
//        std::vector<libsumo::TraCILogic> SumoTL_Logic = SumoGameInstance->client->trafficlights.getAllProgramLogics("gneJ8");
//
//        std::vector<libsumo::TraCIPhase*>  TL_Phases = SumoTL_Logic[0].phases;
//        for(int i = 0; i < TL_Phases.size(); i++){
//            FirstTrafficLightLogic.push_back({ ExtractLightState(TL_Phases[i]->state, 0), TL_Phases[i]->duration });
//        }
//
//        char InitialStateCharacter = FirstTrafficLightLogic.at(0).first;
//        if(InitialStateCharacter == 'r' ){
//            SetTrafficLightState(ETrafficLightState::Red);
//
//        }else if(InitialStateCharacter == 'y'){
//            SetTrafficLightState(ETrafficLightState::Yellow);
//        }else if(InitialStateCharacter == 'g'){
//            SetTrafficLightState(ETrafficLightState::Green);
//        }
//
//        for(int i = 0; i < FirstTrafficLightLogic.size(); i++){
//
//            if(FirstTrafficLightLogic.at(i).first == 'r' ){
//                RedTick += FirstTrafficLightLogic.at(i).second * SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS;
//            }else if(FirstTrafficLightLogic.at(i).first == 'y'){
//                YellowTick += FirstTrafficLightLogic.at(i).second * SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS;
//            }else if(FirstTrafficLightLogic.at(i).first == 'g'){
//                GreenTick += FirstTrafficLightLogic.at(i).second * SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS;
//            }
//
//        }
//        UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"),RedTick, YellowTick, GreenTick)
//
//    }

}
char ATrafficLightBase::ExtractLightState(std::string TL_State, int TL_Group){
    int group = 0;
    for(int i = 0; i < TL_State.length(); i++){
        int j = i + 1;
        char left = tolower(TL_State.at(i));
        char right = tolower(TL_State.at(j));
        if(left != right ){
            if(group == TL_Group){
                return left;
            }
            group++;
        }

    }

    return '\0';
}
void ATrafficLightBase::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
    SetTrafficLightState(State);
}

void ATrafficLightBase::Tick(float DeltaSeconds)
{
    // TickByMachineTime(DeltaSeconds);

//    if (SumoGameInstance && !SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower) {
//        if (SumoGameInstance->SUMOToUnrealFrameRate.TickCount == SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
//
//            UE_LOG(LogTemp, Warning, TEXT("ATrafficLightBase -> Tick() %d. Update from SUMo."), SumoGameInstance->SUMOToUnrealFrameRate.TickCount)
//            TickByCount();
//        }
//    }
}


void ATrafficLightBase::TickByCount(){
    ElapsedTick = ElapsedTick + 1;

    float ChangeTick;

    switch (State)
    {
        case ETrafficLightState::Red:
            ChangeTick = RedTick;
            break;
        case ETrafficLightState::Yellow:
            ChangeTick = YellowTick;
            break;
        case ETrafficLightState::Green:
            ChangeTick = GreenTick;
            break;
        default:
        UE_LOG(LogTemp, Error, TEXT("Invalid traffic light state!"));
            SetTrafficLightState(ETrafficLightState::Red);
            return;
    }

    if(ElapsedTick > ChangeTick){
        SwitchTrafficLightState();
        ElapsedTick = 0;
    }
}

void ATrafficLightBase::TickByMachineTime(float DeltaSeconds) {
    // if(TimeIsFrozen){  return;  }

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