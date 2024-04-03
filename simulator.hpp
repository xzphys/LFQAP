#ifndef SIMULATOR_DMTN_H
#define SIMULATOR_DMTN_H

#include <vector>
#include <algorithm>

using namespace std;

typedef struct complextmp {
    double real, imag;
    double assist;
    double array[5];
}Complex;

typedef struct qr {
    unsigned short size;
    Complex *matrix;
}quReg;

float simulator_DM(char* read_path/*int argc, char *argv[]*/);

void Qprint(quReg *qr);
quReg* simulator(vector <vector<char>> fp_name);
quReg* newQuReg(size_t n);
//quReg* simulator(FILE *fp);

#endif
