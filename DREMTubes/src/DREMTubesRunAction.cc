//**************************************************
// \file DREMTubesRunAction.cc 
// \brief: Implementation of DREMTubesRunAction class 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 7 July 2021
//**************************************************

//Includers from project files
//
#include "DREMTubesRunAction.hh"
#include "DREMTubesEventAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//Includers from C++
//
#include <string>

//Define constructor
//
DREMTubesRunAction::DREMTubesRunAction( DREMTubesEventAction* eventAction )
    : G4UserRunAction(),
      fEventAction( eventAction ){ 
  
    //print event number per each event (default, can be overwritten with macro)
    //
    G4RunManager::GetRunManager()->SetPrintProgress(1);     

    //Instantiate analysis manager
    //
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel( 1 );
    analysisManager->SetNtupleMerging( 1 );

    //Using ROOT as analysisManager type, print it
    //
    G4cout << "DREMTubes-> Using " << analysisManager->GetType() << G4endl;

    //Define ntuple structure
    //
    analysisManager->CreateNtuple("DREMTubesout", "DREMTubesoutput");
    analysisManager->CreateNtupleDColumn("Energyem");
    analysisManager->CreateNtupleDColumn("EnergyScin");
    analysisManager->CreateNtupleDColumn("EnergyCher");
    analysisManager->CreateNtupleDColumn("NofCherenkovDetected"); 
    analysisManager->CreateNtupleDColumn("EnergyTot");
    analysisManager->CreateNtupleDColumn("PrimaryParticleEnergy");
    analysisManager->CreateNtupleIColumn("PrimaryPDGID");
    analysisManager->CreateNtupleDColumn("EscapedEnergy");
    analysisManager->CreateNtupleDColumn
        ("VectorSignals", fEventAction->GetVectorSignals());
    analysisManager->CreateNtupleDColumn
        ("VectorSignalsCher", fEventAction->GetVectorSignalsCher());
    analysisManager->FinishNtuple();
  
}

//Define de-constructor
//
DREMTubesRunAction::~DREMTubesRunAction(){
   
    //Delete only instance of G4AnalysisManager
    //
    delete G4AnalysisManager::Instance();  

}

//Define BeginOfRunAction() and EndOfRunAction() methods
//
void DREMTubesRunAction::BeginOfRunAction( const G4Run* Run )  { 
    
    //Save random seeds (optional)
    //
    //G4RunManager::GetRunManager()->SetRandomNumberStore( true );
    
    //Open output file, one per Run
    //
    auto analysisManager = G4AnalysisManager::Instance();
    std::string runnumber = std::to_string( Run->GetRunID() );
    G4String outputfile = "DREMTubesout_Run"+runnumber;
    analysisManager->OpenFile( outputfile );

}

void DREMTubesRunAction::EndOfRunAction( const G4Run* ) {
  
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();

}

//**************************************************
