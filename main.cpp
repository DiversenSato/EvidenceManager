#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
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
        } else if(NewEvi <= -1 && NewEvi >= -7) { //If negative, add to impossible evidence
            isInvalid = false;
            NewEvi = abs(NewEvi)-1; //Make NewEvi range from 0-6; the evidence displayed
            if(Impossible[NewEvi]) {
                Impossible[NewEvi] = false;
            } else {
                Impossible[NewEvi] = true;
            }
        } else if(Input == "restart") {
            for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) {
                Confirmed[c] = -1;
            }
            for(int c = 0; c < sizeof(Impossible)/sizeof(Impossible[0]); c++) {
                Impossible[c] = false;
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
            cout << Evidences[e] + ", ";
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
            cout << GhostTypes[i].Name << ":";
            int nameLength = GhostTypes[i].Name.length() + 1;
            int curIndent = ceil(nameLength/8);
            int tarIndent = 3;
            for(int d = 0; d < tarIndent-curIndent; d++) {
                cout << "\t";
            }
            
            //Loop through every evidence of current ghost and print non confirmed
            for(int e = 0; e < sizeof(GhostTypes[i].Values)/sizeof(GhostTypes[i].Values[0]); e++) {
                if(!InArr(Confirmed, GhostTypes[i].Values[e])) {
                    cout << Evidences[GhostTypes[i].Values[e]] + ",\t";
                }
            }
            cout << "\b\b \n";
        }
    }

    //
    //  Print every possible evidence
    //
    MakeLine();
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
            float Chance = (float)EvidenceOccurances[i] / (float)PosGhosts;

            if(Chance <= HChance) {
                cout << "\033[1;32m";
            }
            if(Chance <= lerp(LChance, HChance, 0.67)) {
                cout << "\033[01;33m";
            }
            if(Chance < lerp(LChance, HChance, 0.33)) {
                cout << "\033[1;31m";
            }

            cout << to_string(i+1) + ". " + Evidences[i] << " " << EvidenceOccurances[i] << "/" << PosGhosts << "\n";
        }
    }
    cout << "\033[0m";

    //The end
}

void GetGhosts() {
    for (int b = 0; b < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); b++) { //Sæt alle spøgelser til mulige
        PossibleGhosts[b] = true;
    }
    for(int i = 0; i < sizeof(GhostTypes)/sizeof(GhostTypes[0]); i++) {                         //Loop gennem alle spøgelsestyper
        for(int b = 0; b < sizeof(GhostTypes[i].Values)/sizeof(GhostTypes[i].Values[0]); b++) { //Loop gennem current spøgelses evidences for at tjekke ban
            if(Impossible[GhostTypes[i].Values[b]]) {
                PossibleGhosts[i] = false;
            }
        }
    }
    for(int e = 0; e < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); e++) { //Loop gennem mulige spøgelser
        if(PossibleGhosts[e]) { //Led efter confirmed evidence her
            for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) { //Loop gennem confirmed evidence
                if(Confirmed[c] != -1) {
                    //Note, spøgelse skal have alle confirmed evidence før det tæller
                    bool hasEvi = false; //Has current evidence
                    for(int g = 0; g < sizeof(GhostTypes[e].Values)/sizeof(GhostTypes[e].Values[0]); g++) {
                        if(GhostTypes[e].Values[g] == Confirmed[c]) {
                            hasEvi = true;
                            break;
                        }
                    }

                    if(!hasEvi) {
                        PossibleGhosts[e] = false;
                        break;
                    }
                }
            }
        }
    }

    PosGhosts = 0;
    //Loop gennem PossibleGhosts[] for at finde PosGhosts
    for(int i = 0; i < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); i++) {
        if(PossibleGhosts[i]) {
            PosGhosts++;
        }
    }

}

void GetEviAmount() {
    //Set EvidenceOccurances til 0
    for(int i = 0; i < sizeof(EvidenceOccurances)/sizeof(EvidenceOccurances[0]); i++) {
        EvidenceOccurances[i] = 0;
    }
    for(int i = 0; i < sizeof(PosEvidences)/sizeof(PosEvidences[0]); i++) {
        PosEvidences[i] = false;
    }

    for(int i = 0; i < sizeof(PossibleGhosts)/sizeof(PossibleGhosts[0]); i++) {
        //If ghost is possible, then log evidence
        if(PossibleGhosts[i]) {
            for(int j = 0; j < sizeof(GhostTypes[i].Values)/sizeof(GhostTypes[i].Values[0]); j++) {
                EvidenceOccurances[GhostTypes[i].Values[j]]++;
                PosEvidences[GhostTypes[i].Values[j]] = true;
            }
        }
    }

    //Remove confirmed evidence from PosEvidences
    for(int c = 0; c < sizeof(Confirmed)/sizeof(Confirmed[0]); c++) {
        if(Confirmed[c] != -1) {
            PosEvidences[Confirmed[c]] = false;
        }
    }
}