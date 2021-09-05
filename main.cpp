#include <iostream>
#include <stdlib.h>
#include <string>
#include "Ghost.h"
using namespace std;
//                       0         1              2               3              4                 5                     6
string Evidences[7] = {"EMF 5", "Spirit Box", "Fingerprints", "Ghost Orb", "Ghost Writing", "Freezing Temperatures", "D.O.T.S."};
bool PosEvidences[7] = {false, false, false, false, false, false, false};
bool Impossible[7] = {false, false, false, false, false, false, false};
bool PossibleGhosts[16];
int Confirmed[3] = {-1, -1, -1};
int PosGhosts = 0;
int EvidenceOccurances[7] = {0, 0, 0, 0, 0, 0, 0};

Ghost GhostTypes[16];

void DrawGUI();

void GetGhosts();
void GetEviAmount();
void GetPosEvi();

int main() {
    GhostTypes[0].Set("Banshee", 3, 2, 6);
    GhostTypes[1].Set("Demon", 5, 4, 2);
    GhostTypes[2].Set("Goryo", 0, 2, 6);
    GhostTypes[3].Set("Hantu", 2, 3, 5);
    GhostTypes[4].Set("Jinn", 0, 5, 2);
    GhostTypes[5].Set("Mare", 3, 4, 1);
    GhostTypes[6].Set("Myling", 0, 2, 4);
    GhostTypes[7].Set("Oni", 0, 5, 6);
    GhostTypes[8].Set("Phantom", 1, 2, 6);
    GhostTypes[9].Set("Poltergeist", 4, 1, 2);
    GhostTypes[10].Set("Revenant", 3, 4, 5);
    GhostTypes[11].Set("Shade", 0, 4, 5);
    GhostTypes[12].Set("Spirit", 0, 4, 1);
    GhostTypes[13].Set("Wraith", 0, 1, 6);
    GhostTypes[14].Set("Yokai", 3, 1, 6);
    GhostTypes[15].Set("Yurei", 3, 5, 6);

    bool isInvalid = false;
    while(true) {
        DrawGUI();
        
        string Input;
        if(!isInvalid) {
            cin >> Input;
        } else {
            cout << "That input is invalid!\n";
            cin >> Input;
        }

        int NewEvi = 0;
        //Validate input and act accordingly
        try {
            NewEvi = stoi(Input);
        } catch(invalid_argument const &e) {
        }

        if(NewEvi >= 1 && NewEvi <= 7) {
            bool deleted = false;
            for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) {
                if(Confirmed[c] == NewEvi-1) {
                    //If the input is double, deselect that evidence
                    Confirmed[c] = -1;
                    deleted = true;
                    break;
                }
            }
            for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) {
                if(!deleted && Confirmed[c] == -1) {
                    //The evidence is new, add it to confirmed and stop checking
                    Confirmed[c] = NewEvi - 1;
                    break;
                }
            }
        } else if(NewEvi <= -1 && NewEvi >= -7) {
            //If negative, add to impossible evidence
            isInvalid = false;
            if(Impossible[abs(NewEvi)-1]) {
                Impossible[abs(NewEvi)-1] = false;
            } else {
                Impossible[abs(NewEvi)-1] = true;
            }
        } else if(Input == "restart") {
            for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) {
                Confirmed[c] = -1;
            }
        } else {
            isInvalid = true;
        }
    }
    
    return 0;
}

void DrawGUI() {
    system("cls");
    MakeLine();

    //Write confirmed evidence
    cout << "Confirmed evidence:  ";
    for(int e = 0; e < sizeof(Confirmed)/sizeof(Confirmed[0]); e++) {
        if(Confirmed[e] != -1) {
            cout << Evidences[Confirmed[e]] + ", ";
        }
    }
    cout << "\b\b \n";
    //Write impossible evidence
    cout << "Impossible evidence:  ";
    for(int e = 0; e < sizeof(Impossible)/sizeof(Impossible[0]); e++) {
        if(Impossible[e]) {
            cout << Evidences[Impossible[e]] + ", ";
        }
    }
    cout << "\b\b \n";
    
    GetGhosts();
    GetEviAmount();

    MakeLine();
    cout << "Possible ghosts:\n";
    //Loop through every possible ghost. Current ghost is GhostTypes[i]
    for(int i = 0; i < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); i++) {
        //If the ghost is possible, print it along with non-confirmed evidence
        if(PossibleGhosts[i]) {
            //Print ghost.name + ": "
            cout << GhostTypes[i].Name + ": ";
            
            //Loop through every evidence of current ghost and print non confirmed
            for(int e = 0; e < sizeof(GhostTypes[i].Values)/sizeof(GhostTypes[i].Values[0]); e++) {
                if(!InArr(Confirmed, GhostTypes[i].Values[e])) {
                    cout << Evidences[GhostTypes[i].Values[e]] + ", ";
                }
            }
            cout << "\b\b \n";
        }
    }

    GetPosEvi();

    //
    //  Print every possible evidence
    //
    cout << "Possible remaining evidence:\n";
    //Find highest and lowest chance
    float HChance = 0;
    float LChance = 1;
    for(int i = 0; i < sizeof(PosEvidences)/sizeof(PosEvidences[0]); i++) {
        if(PosEvidences[i]) {
            float Chance = (float)EvidenceOccurances[i] / (float)PosGhosts;
            if(Chance > HChance) {
                HChance = Chance;
            } else if(Chance < LChance) {
                LChance = Chance;
            }
        }
    }
    for(int i = 0; i < sizeof(PosEvidences)/sizeof(PosEvidences[0]); i++) {
        if(PosEvidences[i]) {
            float EviOcc = (float)EvidenceOccurances[i] / (float)PosGhosts;

            if(EviOcc <= HChance) {
                cout << "\033[1;32m";
            }
            if(EviOcc <= lerp(LChance, HChance, 0.67)) {
                cout << "\033[01;33m";
            }
            if(EviOcc < lerp(LChance, HChance, 0.33)) {
                cout << "\033[1;31m";
            }

            cout << to_string(i+1) + ". " + Evidences[i] + "\n";
        }
    }
    cout << "\033[0m";

    //The end
}

void GetGhosts() {
    for (int b = 0; b < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); b++) {
        PossibleGhosts[b] = true;
    }
    //Loop gennem alle spøgelsestyper
    for(int i = 0; i < sizeof(GhostTypes)/sizeof(GhostTypes[0]); i++) {
        //----NOTE---- for loop kører ikke når der ikke er inputtet noget confirmed evidence
        for(int e = 0; Confirmed[e] != -1 && e < sizeof(Confirmed)/sizeof(Confirmed[0]); e++) {
            //Find out if the current ghost type has the current confirmed evidence
            bool hasEvi = false;

            //Loop gennem current ghost's evidences/values
            for(int h = 0; h < sizeof(GhostTypes[i].Values)/sizeof(GhostTypes[i].Values[0]); h++) {
                //Hvis vi finder current confirmed evidence er spøgelset possible
                if(GhostTypes[i].Values[h] == Confirmed[e]) {
                    hasEvi = true;
                    PosGhosts += 1;
                }
            }

            //If it doesn't have the evidence, the ghost is not possible, and the next ghost will be checked
            if(!hasEvi) {
                PossibleGhosts[i] = false;
                break;
            }
        }

        //I tilfældet af, at der ikke er confirmed evidence:
        if(Confirmed[0] == -1) {
            PosGhosts = sizeof(GhostTypes)/sizeof(GhostTypes[0]);
        }
    }
}

void GetEviAmount() {
    for(int i = 0; i < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); i++) {
        //If ghost is possible, then log evidence
        if(PossibleGhosts[i]) {
            for(int j = 0; j < sizeof(GhostTypes[i].Values)/sizeof(GhostTypes[i].Values[0]); j++) {
                EvidenceOccurances[GhostTypes[i].Values[j]]++;
            }
        }
    }
}

void GetPosEvi() {
    MakeLine();
    //Check every possible ghost for it's evidences
    for(int i = 0; i < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); i++) {
        if(PossibleGhosts[i]) {
            PosEvidences[GhostTypes[i].Values[0]] = true;
            PosEvidences[GhostTypes[i].Values[1]] = true;
            PosEvidences[GhostTypes[i].Values[2]] = true;
        }
    }
    //Remove confirmed evidence from PosEvidences
    for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) {
        if(Confirmed[c] != -1) {
            PosEvidences[Confirmed[c]] = false;
        }
    }
}