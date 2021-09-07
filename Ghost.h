#include <string>
using namespace std;

class Ghost {
    public:
        string Name;
        int Values[3];
        
        void Set(string name, int val1, int val2, int val3) {
            Name = name;
            Values[0] = val1;
            Values[1] = val2;
            Values[2] = val3;
        }
};

float lerp(float a, float b, float t) {
    return (a + t*(b-a));
}

bool InArr(int arr[], int object) {
    for(int e = 0; e < sizeof(arr)/sizeof(arr[0]) && arr[e] != -1; e++) {
        if(arr[e] == object) {
            return true;
        }
    }
    return false;
}

void MakeLine() {
    for(int i = 0; i < 120; i++) {
        cout << (char)205;
    }
    cout << "\n";
}