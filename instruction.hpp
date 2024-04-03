#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class instruction {
public:
    char flag = '@';
    char inst_name[10];
    int qubit0 = -1;
    int qubit1 = -1;
    float angle = 100;
    int feature_index = -1;
};

#endif
