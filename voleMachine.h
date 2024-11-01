#ifndef VOLEMACHINE_H
#define VOLEMACHINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>

using namespace std;

class Memory {
private:
    string value;
public:
    Memory() : value("00") {} 
    void set_Memory_cell(string new_value);
    string get_Memory_cell();
};

class Register {
private:
    string value;
public:
    Register() : value("00") {} 
    void set_Register(string new_value);
    string get_Register();
};

class Machine {
private:
    Memory memory_cells[256]; 
    Register registers[16]; 
    string IR; 
    int PC; 
    int show; 
    ifstream input; 

public:
    Machine();
    void menu();
    void load_program();
    void run_program();
    void show_status();
    bool validate1();
    bool validate2();
    bool validate3();
    void fetch_instruction();
    void decode_execute_instruction();
    void call_instruction();
    void i_1();
    void i_2();
    void i_3();
    void i_4();
    void i_5();
    void i_6(); 
    void i_B();
    void i_C();
};

int convert_to_decimal(char ch);
string hex_add(string hex1, string hex2);

#endif 