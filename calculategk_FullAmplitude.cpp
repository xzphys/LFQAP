#include <calculategk.hpp>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include <iostream>
#include <iomanip>
//#include <cevd.h>

#include <simulator.hpp>
//class instruction;
using namespace std;
//using namespace splab;

typedef double  Type;

float Mz(quReg* qr, int qubit) {

    int size = pow(2, qr->size);
    float prob = 0;

    for(int i = 0; i < size; i++) {
        int tmp[32];
        for(int j = 0; j < qr->size; j++) {
            tmp[j] = (i & (1 << j)) >> j;
        }

        if(tmp[qubit] == 0) {
            float tmp_prob = pow(qr->matrix[i].real, 2) + pow(qr->matrix[i].imag, 2);
            prob = prob + tmp_prob;
        }
    }
    return prob;
}

float Mz2(quReg* qr, int qubit1, int qubit2, int l1, int l2) {

    int size = pow(2, qr->size);
    float prob = 0;

    for(int i = 0; i < size; i++) {
        int tmp[32];
        for(int j = 0; j < qr->size; j++) {
            tmp[j] = (i & (1 << j)) >> j;
        }

	if(tmp[qubit1] == l1 && tmp[qubit2] == l2) {
	    float tmp_prob = pow(qr->matrix[i].real, 2) + pow(qr->matrix[i].imag, 2);
	    prob = prob + tmp_prob;
	}
    }
    return prob;
}

float calculate_prob(instruction inst_list[], float feature_array[], float *traning_parameter_array, int param, int sample, int epoch, float shift, int l1, int l2) {
    char tmp_name[20] = {0};
  //  printf("debug 1\n");
    //sprintf(tmp_name,"training/epoch_%d",epoch);
    //mkdir(tmp_name,S_IRWXU);

    vector <vector <char>> network_N;
    //char network_N[500][20];;

    const float pi = 3.1415927;
    //int Meas_qubit = 0;

    int count = 0;
    int feature_count = 0;
    int traning_parameter_count = 0;

    int qubit_num = 0;
    int qubit_count = 0;

    vector<int> dump_density_matrix_p;//[10] = {0};

    while(1) {

        if(inst_list[qubit_count].flag == '@') break;
        if(inst_list[qubit_count].qubit0 > qubit_num) qubit_num = inst_list[qubit_count].qubit0;
        if(inst_list[qubit_count].qubit1 > qubit_num) qubit_num = inst_list[qubit_count].qubit1;
        qubit_count++;
    }

    int Meas_qubit0 = -1;
    int Meas_qubit1 = -1;
    int Meas_qubit2 = -1;

    char tmp_inst[30];
    sprintf(tmp_inst,"QREG qr1 %d\n",qubit_num + 1);
    vector<char> tmp_inst_v(20);
    for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
    network_N.push_back(tmp_inst_v);

    while(1) {
        if(inst_list[count].flag == '@') break;

    /*    if(inst_list[count].flag == '$') {
	    dump_density_matrix_p.push_back(network_N.size());
            break;
	}*/

        if(strcmp(inst_list[count].inst_name, "X") == 0 || strcmp(inst_list[count].inst_name, "Y") == 0 || strcmp(inst_list[count].inst_name, "Z") == 0 || strcmp(inst_list[count].inst_name, "H") == 0) {
            //fprintf(fp_net_N,"%s qr1 %d\n",inst_list[count].inst_name,inst_list[count].qubit0);
	    sprintf(tmp_inst,"%s qr1 %d\n",inst_list[count].inst_name,inst_list[count].qubit0);
	    vector<char> tmp_inst_v(20);
            for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
	    network_N.push_back(tmp_inst_v);
        }
        else if( strcmp(inst_list[count].inst_name, "MZ") == 0) {
            Meas_qubit0 = inst_list[count].qubit0;
	    Meas_qubit1 = inst_list[count].qubit1;
        }
        else if(strcmp(inst_list[count].inst_name, "CNOT") == 0 || strcmp(inst_list[count].inst_name, "CZ") == 0 || strcmp(inst_list[count].inst_name, "SWAP") == 0) {
            //fprintf(fp_net_N,"%s qr1 %d %d\n",inst_list[count].inst_name,inst_list[count].qubit0,inst_list[count].qubit1);
	    sprintf(tmp_inst,"%s qr1 %d %d\n",inst_list[count].inst_name,inst_list[count].qubit0,inst_list[count].qubit1);
	    vector<char> tmp_inst_v(20);
            for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
	    network_N.push_back(tmp_inst_v);
        }
        else if(strcmp(inst_list[count].inst_name, "RX") == 0 || strcmp(inst_list[count].inst_name, "RY") == 0 || strcmp(inst_list[count].inst_name, "RZ") == 0) {
            if(inst_list[count].flag == '*') {
                //fprintf(fp_net_N,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,feature_array[feature_count]);
		sprintf(tmp_inst,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,feature_array[inst_list[count].feature_index/*feature_count*/]);
		vector<char> tmp_inst_v(20);
                for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
		network_N.push_back(tmp_inst_v);
                feature_count++;
            }
            else if(inst_list[count].flag == '#') {

		if(traning_parameter_count == param) {
		    sprintf(tmp_inst,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,traning_parameter_array[param] + shift);
                    vector<char> tmp_inst_v(20);
                    for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
                    network_N.push_back(tmp_inst_v);
                }
                else {
                    sprintf(tmp_inst,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,traning_parameter_array[traning_parameter_count]);
                    vector<char> tmp_inst_v(20);
                    for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
                    network_N.push_back(tmp_inst_v);
                }
                traning_parameter_count++;

            }
            else if(inst_list[count].flag == '&') {
                //fprintf(fp_net_N,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,inst_list[count].angle);
		sprintf(tmp_inst,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,inst_list[count].angle);
		vector<char> tmp_inst_v(20);
                for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
		network_N.push_back(tmp_inst_v);
            }
            else {
                printf("ERROR! NO THIS FLAG");
            }
        }
	else if( strcmp(inst_list[count].inst_name, "dumpStat") == 0) {
	    dump_density_matrix_p.push_back(network_N.size());
	}
        else {
            printf("ERROR! NO THIS GATE");
        }

        count++;
    }

   // fclose(fp_net_N);
    quReg* state_N = simulator(network_N);
    

    //float N_prob = Mz(state_N, Meas_qubit);
    float N_prob = Mz2(state_N, Meas_qubit1, Meas_qubit0, l1, l2);
    
    free(state_N->matrix);
    free(state_N);
    state_N->matrix = NULL;
    state_N = NULL;

    return N_prob;
}


