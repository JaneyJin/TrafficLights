// Fill out your copyright notice in the Description page of Project Settings.


#include "SumoGameInstance.h"
#include "Client.h"

USumoGameInstance::USumoGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    client = nullptr;
    EgoWheeledVehicleId = "";
}

void USumoGameInstance::SetSumoGameInstance(Client* c, FString v_id ) {
    this->client = c;
    this->EgoWheeledVehicleId = v_id;
    this->ArrivedNumber = -1;

    UE_LOG(LogTemp, Warning, TEXT("# of tick between SUMOUpdate: %d"), SUMOToUnrealFrameRate.UETickBetweenSUMOUpdates)
}


void USumoGameInstance::CloseSocket() {
    SUMOToUnrealFrameRate.UnrealTickSlower = false;
    client = nullptr;
}