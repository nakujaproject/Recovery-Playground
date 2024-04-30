#include <Arduino.h>
#include <vector>
using namespace std;

vector<int> x;

void save_in_vectors(){
    for (int i = 0; i < 10; i++)
        x.push_back(i*10);
}

void read_vector(){
    for (int i = 0; i < 10; i++)
        Serial.println(x[i]);
}