// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoGameMode.h"
#include "Misc/App.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
// #include "SumoWheeledVehicle.h"
// #include "WheeledVehiclePawn.h"
#include "GameFramework/PlayerStart.h"
#include <algorithm>    // std::find
#include <vector>
// #include "SumoPlayerController.h"
#include "SumoGameInstance.h"


//ASumoGameMode::ASumoGameMode(const FObjectInitializer& ObjectInitializer)
//        : Super(ObjectInitializer)
//{
//
//    // set default player class to customed MyPlayerController
//    // PlayerControllerClass = ASumoPlayerController::StaticClass();
//
//}

void ASumoGameMode::BeginPlay() {
    Super::BeginPlay();

    // Setup socket for UE and connect to SUMO
    try {

        // Validate the port number
        if ((PortNumber <= 0) || (PortNumber > LargestPortNumber)) {
            PortNumber = DefaultPortNumber;
            UE_LOG(LogTemp, Warning, TEXT("Missing PortNum. Set to 24000....."))
        }
        UE_LOG(LogTemp, Warning, TEXT("Connect....."))

        client.connect("localhost", PortNumber);

//        if (!SetupEgoWheeledVehicle()) {
//            // UE_LOG(LogTemp, Error, TEXT("Fail to set up EGOVehcile"))
//            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EGO Wheeled Vehicle is not set.")));
//        }

        if (!SetupGameInstance()) {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection fail. Please set SumoGameInstance in the project setting before use.")));
        }
        // Validate FPS between Unreal and SUMO.
        MatchFrameRatePerSecond();

        /* APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0) //for singleplayer
        ASumoPlayerController* PlayerController = (ASumoPlayerController*)GetWorld()->GetFirstPlayerController();
        check(PlayerController);
        UE_LOG(LogTemp, Error, TEXT("%s.\n"), *PlayerController->GetName())
        PlayerController->Possess(EgoWheeledVehicle);
        AController* Player;
        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            APlayerController* PlayerController = *Iterator;
            PlayerController->Possess(EgoWheeledVehicle);
            if (Player != PlayerController && PlayerController->GetPawn())
            {
                // MinPlayerDistance = FMath::Min(MinPlayerDistance, (SpawnLocation - PlayerController->GetPawn()->GetActorLocation()).Size());
            }
        }*/

    }
    catch (tcpip::SocketException& e) {
        UE_LOG(LogTemp, Error, TEXT("#Error while connecting: %s.\n"), e.what())
    }

    UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay"))

}

void ASumoGameMode::MatchFrameRatePerSecond() {
    /* Seconds between 2 frame in SUMO */
    double SUMODeltaT = client.simulation.getDeltaT();

    /* Seconds between 2 frame in Unreal */
    double UEDeltaT = FApp::GetDeltaTime();

    /* Get FPS and set TickCounter */
    SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS = FMath::RoundHalfFromZero(1.0f / SUMODeltaT);
    SumoGameInstance->SUMOToUnrealFrameRate.UnrealFPS = FMath::RoundHalfFromZero(1.0f / UEDeltaT);

    UE_LOG(LogTemp, Warning, TEXT("SUMO DeltaT is %f. UE DeltaT is %f. Sumo fps is %d. Unreal FPS is %d"), SUMODeltaT, UEDeltaT, SumoGameInstance->SUMOToUnrealFrameRate.SUMOFPS, SumoGameInstance->SUMOToUnrealFrameRate.UnrealFPS)

    if (SUMODeltaT > 0) {

        if (SUMODeltaT < UEDeltaT) {
            SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower = true;
            UE_LOG(LogTemp, Error, TEXT("Unreal Engine frame rate is lower than SUMO. Cannot change it within code. Please change in setting."));
        }
        else {
            SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower = false;
            SumoGameInstance->SUMOToUnrealFrameRate.NextTimeToUpdate = SumoGameInstance->SUMOToUnrealFrameRate.UpdateDeltaT = SUMODeltaT;
            SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates = FMath::RoundHalfFromZero(1 / SUMODeltaT);
        }

    }

    // UE_LOG(LogTemp, Warning, TEXT("Update at %f per second. Next update at %f"), SUMOToUnrealFrameRate.UpdateDeltaT, SUMOToUnrealFrameRate.NextTimeToUpdate)
    UE_LOG(LogTemp, Warning, TEXT("Current tick is %d. Tick between SUMO updates is %d"), SumoGameInstance->SUMOToUnrealFrameRate.TickCount, SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates)
}



void ASumoGameMode::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    // auto DateTime = FDateTime::Now().GetMillisecond();
    // UE_LOG(LogTemp, Warning, TEXT("Machine Time: %s"), *DateTime.ToString())

    // Unreal Engine frame rate must be higher than SUMO frame rate.
    if (SumoGameInstance && SumoGameInstance->client && !SumoGameInstance->SUMOToUnrealFrameRate.UnrealTickSlower) {
        UpdateFromSUMOByTickCount();
        // UpdateSUMOByMachineTime();

    }
//    else {
//        UE_LOG(LogTemp, Warning, TEXT("Tick. Socket Close."))
//    }
}

bool ASumoGameMode::SetupGameInstance() {
    SumoGameInstance = NewObject<USumoGameInstance>(this, TEXT("MyGameInstance"));
    GetWorld()->SetGameInstance(SumoGameInstance);

    UE_LOG(LogTemp, Warning, TEXT("GameInstance class name: %s; GameInstance name: %s"), *GetGameInstance()->GetClass()->GetName(), *GetGameInstance()->GetName())
    if (GetGameInstance()->GetClass()->GetName() == "SumoGameInstance") {

        // FString EnumAsString = UEnum::GetValueAsString(TrafficLightSyn.GetValue());
        // UE_LOG(LogTemp, Error, TEXT("%s: %d"), *EnumAsString, TrafficLightSyn.GetValue())
        SumoGameInstance->SetSumoGameInstance(&client, DefaultPawnClass->GetName());
        return true;
    }

    return false;
}

bool ASumoGameMode::SynBySUMOTrafficLight(){
    if(TrafficLightSyn == ETrafficLightController::SumoTrafficLight) {
        return true;
    }
    return false;
}
void ASumoGameMode::UpdateFromSUMOByTickCount() {

    /* Fix Tick() rate to update vehicle from SUMO */
    if (SumoGameInstance->SUMOToUnrealFrameRate.TickCount < SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
        // UE_LOG(LogTemp, Display, TEXT("SUMOGameMode -> #of ticks between SUMO updates is %d. GameMode Tick() %d. Update from SUMo."), SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates, SumoGameInstance->SUMOToUnrealFrameRate.TickCount)

        SumoGameInstance->SUMOToUnrealFrameRate.TickCount++;

    }
    else if (SumoGameInstance->SUMOToUnrealFrameRate.TickCount == SumoGameInstance->SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates) {
        // UE_LOG(LogTemp, Warning, TEXT("SUMOGameMode -> Tick %d : Update from SUMO."), SumoGameInstance->SUMOToUnrealFrameRate.TickCount)
        UpdateSimulationFromSUMO();

        SumoGameInstance->SUMOToUnrealFrameRate.TickCount = 1;

    }


}


void ASumoGameMode::UpdateSimulationFromSUMO() {
    if (client.simulation.getMinExpectedNumber() > 0) {
        client.simulationStep();

    }
    else {
        // Close socket
        client.close();
        SumoGameInstance->CloseSocket();

        // Exit game in UE
        //FGenericPlatformMisc::RequestExit(false);
        //GetWorld()->Exec(GetWorld(), TEXT("Exit"));
    }
}




#if WITH_EDITOR
bool ASumoGameMode::CanEditChange(const UProperty* InProperty) const
{
    const bool ParentVal = Super::CanEditChange(InProperty);

    if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ASumoGameMode, RouteId))
    {
        auto CurrentDefaultPawnSuperClassName = DefaultPawnClass->GetSuperClass()->GetName();
        return  CurrentDefaultPawnSuperClassName == "WheeledVehiclePawn";
    }

    return ParentVal;
}
#endif


