#include "voleMachine.h"

Machine::Machine() {
    PC = 0;
    show = 0;
    for (int i = 0; i < 256; i++) {
        memory_cells[i].set_Memory_cell("00");
    }
    for (int i = 0; i < 16; i++) {
        registers[i].set_Register("00");
    }
}

void Machine::menu() {
    cout << "1) To show status after each instruction enter"<<endl;
    cin >> show;
}

void Machine::load_program() {
    int line_counter = 0;
    string file_name;
    string operand, instruction;
    cout << "Enter file name:" << endl;
    cin >> file_name;
    input.open(file_name);
    if (input.fail()) {
        cerr << "The file couldn't be opened!" << endl;
        return;
    }
    while (input >> operand >> operand >> operand) {
        line_counter++;
    }
    if (line_counter > 128) {
        cerr << "There isn't enough memory to load the program!" << endl;
        return;
    }
    input.close();
    input.open(file_name);
    
    for (int i { 0 }, j { 0 }; i < line_counter; i++, j += 2) {
        input >> operand;
        instruction = operand.substr(operand.size() - 1);
        input >> operand;
        instruction += operand.substr(operand.size() - 1);
        memory_cells[j].set_Memory_cell(instruction);
        input >> operand;
        instruction = operand.substr(operand.size() - 2);
        memory_cells[j + 1].set_Memory_cell(instruction);
    }
}

void Machine::run_program() {
    menu();
    fetch_instruction();
}

void Machine::show_status() {
    cout << "Program counter : " << hex << uppercase << PC << endl;
    cout << "Instruction register : " << IR << endl;
    cout << "General purpose registers :" << endl;
    for (int i { 0 }; i < 8; i++) {
        cout << hex << uppercase << i << ' ' << registers[i].get_Register() << "\t\t";
        cout << hex << uppercase << i + 8 << ' ' << registers[i + 8].get_Register() << endl;
    }
    cout << "Memory cells :" << endl;
    for (int i { 0 }; i < 32; i++) {
        cout << hex << uppercase << i << ' ' << memory_cells[i].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 32 << ' ' << memory_cells[i + 32].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 64 << ' ' << memory_cells[i + 64].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 96 << ' ' << memory_cells[i + 96].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 128 << ' ' << memory_cells[i + 128].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 160 << ' ' << memory_cells[i + 160].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 192 << ' ' << memory_cells[i + 192].get_Memory_cell() << "  \t  ";
        cout << hex << uppercase << i + 224 << ' ' << memory_cells[i + 224].get_Memory_cell() << endl;
    }
}

bool Machine::validate1() {
    string charset = "12345BC";
    return charset.find(toupper(IR[0])) != string::npos;
}

bool Machine::validate2() {
    string charset = "0123456789ABCDEF";
    return charset.find(toupper(IR[1])) != string::npos;
}

bool Machine::validate3() {
    string str = IR.substr(2, 2);
    int int_value = stoi(str, nullptr, 16);
    return int_value >= 0 && int_value < 256;
}

void Machine::fetch_instruction() {
    IR = memory_cells[PC].get_Memory_cell() + memory_cells[PC + 1].get_Memory_cell();
    PC += 2;
    decode_execute_instruction();
}

void Machine::decode_execute_instruction() {
    call_instruction();
    if (IR == "C000")
        return;
    if (!(validate1() && validate2() && validate3())) {
        return;
    }
    if (show)
    {
        show_status();
        fetch_instruction();
    }

}

void Machine::call_instruction() {
    switch (IR[0]) {
        case '1':
            i_1();
            break;
        case '2':
            i_2();
            break;
        case '3':
            i_3();
            break;
        case '4':
            i_4();
            break;
        case '5':
            i_5();
            break;
        case '6':
            i_6();
            break;
        case 'B':
            i_B();
            break;
        case 'C':
            i_C();
            break;
    }
}

void Machine::i_1() {
    int cell = stoi(IR.substr(2, 2), nullptr, 16);
    int address = stoi(IR.substr(1, 1), nullptr, 16);
    registers[address].set_Register(memory_cells[cell].get_Memory_cell());
}

void Machine::i_2() {
    int address = stoi(IR.substr(1, 1), nullptr, 16);
    registers[address].set_Register(IR.substr(2, 2));
}

void Register::set_Register(string new_value) {
    value = new_value;
}

string Register::get_Register() {
    return value;
}

void Memory::set_Memory_cell(string new_value) {
    value = new_value;
}

string Memory::get_Memory_cell() {
    return value;
}

int convert_to_decimal(char ch) {
    if (48 <= ch && ch <= 57)
        return ch - 48;
    return ch - 55;
}

void Machine::i_3() {
    int r_idx = convert_to_decimal(IR[1]);
    int first_memory_cell_bit = convert_to_decimal(IR[2]);
    int second_memory_cell_bit = convert_to_decimal(IR[3]);
    int mem_idx = (first_memory_cell_bit * 16) + second_memory_cell_bit;
    memory_cells[mem_idx].set_Memory_cell(registers[r_idx].get_Register());
}

void Machine::i_4() {
    int R = convert_to_decimal(IR[2]);
    int S = convert_to_decimal(IR[3]);
    registers[S].set_Register(registers[R].get_Register());
}

string hex_add(string hex1, string hex2) {
    map<char, int> hex_to_dec{
        {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
        {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
        {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
    };

    map<int, char> dec_to_hex{
        {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'},
        {5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'},
        {10, 'A'}, {11, 'B'}, {12, 'C'}, {13, 'D'}, {14, 'E'}, {15, 'F'}
    };

    if (hex1.size() < hex2.size())
        swap(hex1, hex2);

    string ans = "";
    int carry = 0, i, j;

    for (i = hex1.size() - 1, j = hex2.size() - 1; j >= 0; i--, j--) {
        int sum = hex_to_dec[hex1[i]] + hex_to_dec[hex2[j]] + carry;
        char add_bit = dec_to_hex[sum % 16];
        ans.push_back(add_bit);
        carry = sum / 16;
    }

    while (i >= 0) {
        int sum = hex_to_dec[hex1[i]] + carry;
        char add_bit = dec_to_hex[sum % 16];
        ans.push_back(add_bit);
        carry = sum / 16;
        i--;
    }

    if (carry)
        ans.push_back(dec_to_hex[carry]);

    reverse(ans.begin(), ans.end());
    return ans;
}

void Machine::i_5() {
    int R_idx = convert_to_decimal(IR[1]);
    int S_idx = convert_to_decimal(IR[2]);
    int T_idx = convert_to_decimal(IR[3]);

    string R_hex = registers[R_idx].get_Register();
    string S_hex = registers[S_idx].get_Register();
    string T_hex = registers[T_idx].get_Register();

    registers[R_idx].set_Register(hex_add(S_hex, T_hex));
}

void Machine::i_6() {
    int S_idx = convert_to_decimal(IR[1]);
    int T_idx = convert_to_decimal(IR[2]);
    int R_idx = convert_to_decimal(IR[3]);

    float val_S = stof(registers[S_idx].get_Register());
    float val_T = stof(registers[T_idx].get_Register());
    float result = val_S + val_T;

    registers[R_idx].set_Register(to_string(result));
}

void Machine::i_B() {
    int R = convert_to_decimal(IR[1]);
    int address = stoi(IR.substr(2, 2), nullptr, 16);
    if (registers[R].get_Register() == registers[0].get_Register()) {
        PC = address;
    }
}

void Machine::i_C() {
    cout << "Execution halted." << endl;
}
