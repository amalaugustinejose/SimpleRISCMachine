#include <iostream>
#include "memory.h"

using namespace std;

void memory_class_test() {
    Memory mem;

    for (int i = 0; i < MAX_CODE_SIZE + 10; i++)
        mem.codeWrite(i, 100-i);

    for (int i = 0; i < MAX_CODE_SIZE + 10; i++)
        cout << (int) mem.codeRead(i) << ' ';

    for (int i = 0; i < MAX_DATA_SIZE + 10; i++)
        mem.dataWrite(i, i);

    for (int i = 0; i < MAX_DATA_SIZE; i++)
        cout << (int) mem.dataRead(i) << ' ';

    cout << endl;

    for (int i = 0; i < MAX_CODE_SIZE; i++)
        cout << (int) mem.codeRead(i) << ' ';
}

// int main(int argc, char** argv) {
//     memory_class_test();

//     return 0;
// }