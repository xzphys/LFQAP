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
//#include <Python.h>

#include <simulator.hpp>
//class instruction;
using namespace std;

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

float calculate_prob(instruction inst_list[], float feature_array[], float *traning_parameter_array, int param, int sample, int epoch, float shift) {
    char tmp_name[20] = {0};
  //  printf("debug 1\n");
    sprintf(tmp_name,"training/epoch_%d",epoch);
    mkdir(tmp_name,S_IRWXU);

    vector <vector <char>> network_N;
    //char network_N[500][20];;

    char exefile_name_N[30] = {0};

    if(param < 0) {
        sprintf(exefile_name_N,"training/epoch_%d/%d_N.py",epoch,sample);
    }
    else if (param >= 0 && shift > 0) {
	sprintf(exefile_name_N,"training/epoch_%d/%d_%d_P.py",epoch,param,sample);
    }
    else if (param >= 0 && shift < 0) {
        sprintf(exefile_name_N,"training/epoch_%d/%d_%d_M.py",epoch,param,sample);
    }
    

    FILE *fp_net_N = fopen(exefile_name_N,"w+");

    setbuf(fp_net_N,NULL);
    fprintf(fp_net_N,"\n");
    fprintf(fp_net_N,"from ezQgd import * \n\n");
    fprintf(fp_net_N,"account = Account(login_key='xxxxxxxxx', machine_name='gd_qc1')\n");
    fprintf(fp_net_N,"create_lib = xxxxxxxxx\n\n");
    fprintf(fp_net_N,"qcis_test = '''\n");

    const float pi = 3.1415927;
    int Meas_qubit = 0;

    int count = 0;
    int feature_count = 0;
    int traning_parameter_count = 0;

    int qubit_num = 0;
    int qubit_count = 0;
    while(1) {

        if(inst_list[qubit_count].flag == '@') break;
        if(inst_list[qubit_count].qubit0 > qubit_num) qubit_num = inst_list[qubit_count].qubit0;
        if(inst_list[qubit_count].qubit1 > qubit_num) qubit_num = inst_list[qubit_count].qubit1;
        qubit_count++;
    }



//the blow for zucongzhi platform
int mapping[4] = {19,25,32,38};

    while(1) {
        if(inst_list[count].flag == '@') break;

        if(strcmp(inst_list[count].inst_name, "X") == 0 || strcmp(inst_list[count].inst_name, "Y") == 0 || strcmp(inst_list[count].inst_name, "Z") == 0 || strcmp(inst_list[count].inst_name, "H") == 0) {
            //fprintf(fp_net_N,"%s qr1 %d\n",inst_list[count].inst_name,inst_list[count].qubit0);
	    
	    fprintf(fp_net_N,"%s Q%d\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0]);
	    /*sprintf(tmp_inst,"%s Q%d\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0]);
	    vector<char> tmp_inst_v(20);
            for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
	    network_N.push_back(tmp_inst_v);*/
        }
        else if( strcmp(inst_list[count].inst_name, "MZ") == 0) {
            //Meas_qubit = inst_list[count].qubit0;
	    fprintf(fp_net_N,"M Q%d\n",mapping[inst_list[count].qubit0]);
	    /*vector<char> tmp_inst_v(20);
            for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
            network_N.push_back(tmp_inst_v);*/
        }
        else if(strcmp(inst_list[count].inst_name, "CNOT") == 0 || strcmp(inst_list[count].inst_name, "CZ") == 0 || strcmp(inst_list[count].inst_name, "SWAP") == 0) {
            //fprintf(fp_net_N,"%s qr1 %d %d\n",inst_list[count].inst_name,inst_list[count].qubit0,inst_list[count].qubit1);
	    fprintf(fp_net_N,"%s Q%d Q%d\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0],mapping[inst_list[count].qubit1]);
	    //vector<char> tmp_inst_v(20);
            //for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
	    //network_N.push_back(tmp_inst_v);
        }
        else if(strcmp(inst_list[count].inst_name, "RX") == 0 || strcmp(inst_list[count].inst_name, "RY") == 0 || strcmp(inst_list[count].inst_name, "RZ") == 0) {
            if(inst_list[count].flag == '*') {
                //fprintf(fp_net_N,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,feature_array[feature_count]);
		fprintf(fp_net_N,"%s Q%d %f\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0],feature_array[inst_list[count].feature_index/*feature_count*/]);
		//vector<char> tmp_inst_v(20);
                //for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
		//network_N.push_back(tmp_inst_v);
                //feature_count++;
            }
            else if(inst_list[count].flag == '#') {

		if(traning_parameter_count == param) {
		    fprintf(fp_net_N,"%s Q%d %f\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0],traning_parameter_array[param] + shift);
                    //vector<char> tmp_inst_v(20);
                    //for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
                    //network_N.push_back(tmp_inst_v);
                }
                else {
                    fprintf(fp_net_N,"%s Q%d %f\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0],traning_parameter_array[traning_parameter_count]);
                    //vector<char> tmp_inst_v(20);
                    //for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
                    //network_N.push_back(tmp_inst_v);
                }
                traning_parameter_count++;

            }
            else if(inst_list[count].flag == '&') {
                //fprintf(fp_net_N,"%s qr1 %d %f\n",inst_list[count].inst_name,inst_list[count].qubit0,inst_list[count].angle);
		fprintf(fp_net_N,"%s Q%d %f\n",inst_list[count].inst_name,mapping[inst_list[count].qubit0],inst_list[count].angle);
		//vector<char> tmp_inst_v(20);
                //for(int l = 0; l < 20; l++) tmp_inst_v[l] = tmp_inst[l];
		//network_N.push_back(tmp_inst_v);
            }
            else {
                printf("ERROR! NO THIS FLAG");
            }
        }
        else {
            printf("ERROR! NO THIS GATE");
        }

        count++;
    }

    fprintf(fp_net_N,"'''\n\n");
    fprintf(fp_net_N,"query_id = account.submit_job(circuit=qcis_test, num_shots=100, lab_id=create_lib,version=\"%s\")\n\n",exefile_name_N);
  //  fprintf(fp_net_N,"abc = 123\n");
    fprintf(fp_net_N,"if query_id:\n");
    fprintf(fp_net_N,"    result=account.query_experiment(query_id, max_wait_time=600)\n");
    fprintf(fp_net_N,"    probability = result[0].get('probability').get('0')\n");
    fprintf(fp_net_N,"else :\n");
    fprintf(fp_net_N,"    print(\"ERROR, cannot get result\")\n");
    fprintf(fp_net_N,"    probability = 0.5\n\n");

    fprintf(fp_net_N,"f = open(r'%s_result.data','w', encoding='utf-8')\n",exefile_name_N);
    fprintf(fp_net_N,"f.write(str(probability))\n");
    //fprintf(fp_net_N,"f.write('\%s')\n","n");
    fprintf(fp_net_N,"f.close()\n");

    fflush(fp_net_N);

    char running[50] = {0};
  //  printf("debug 1\n");
    sprintf(running,"python %s",exefile_name_N);

    system(running);

    float N_prob = -1;

    char buf[8];
    while(1) {
	char filename[50] = {0};
	sprintf(filename,"%s_result.data",exefile_name_N);
	FILE *fp=fopen(filename,"r");
        if(fp) {
	    fgets(buf, 8, fp);
	    N_prob = atof(buf);
	    fclose(fp);

	    break;
	}
	//fclose(fp);
    }



    fclose(fp_net_N);
/*    quReg* state_N = simulator(network_N);
    float N_prob = Mz(state_N, Meas_qubit);
    free(state_N->matrix);
    free(state_N);
    state_N->matrix = NULL;
    state_N = NULL;*/
//}




    return N_prob;
}


