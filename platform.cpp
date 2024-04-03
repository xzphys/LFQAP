#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <assert.h>

#include <calculategk.hpp>
#include <instruction.hpp>



using namespace std;

int main(int argc, char const *argv[]) {

    int epoch = 100;

    instruction inst_list[1000];
    int inst_num = 0;
    int traning_parameter_num = 0;

    srand((unsigned)time(NULL));

    //reading network file
    FILE *fp=fopen(argv[1],"r");
//    std::cout << fp << std::endl;
    const int buf_size = 30;
    char buf[buf_size];
    if(fp == NULL) {
        printf("can't read network file.\n");
        exit(0);
    }
    int count = 0;

    while(fgets(buf, buf_size, fp) != NULL) {
	
	instruction tmp_inst;
	int last_blank_postion = 0;
	int count_blank = 0;

	if(buf[0] == '#') traning_parameter_num++;

        for(int i = 0; i < buf_size; i++) {
	    if(buf[0] != '*' && buf[0] != '#' && buf[0] != '&' && buf[0] != '$') break;
            if(buf[i] == ' ' || buf[i] == '\n') {
                char tmp_char[10];
	        for(int t = 0; t < 10; t++) tmp_char[t] = 0;
	        int tmp_char_long = i - last_blank_postion;
                for(int j = 0; j < tmp_char_long; j++) {
                    tmp_char[j] = buf[last_blank_postion + j];
                }
		if(count_blank == 0) {
		    inst_list[count].flag = tmp_char[0];
		}
		else if(count_blank == 1) {
		    for(int j = 0; j < 10; j++) inst_list[count].inst_name[j] = tmp_char[j];
		}
		else if(count_blank == 2) {
		    inst_list[count].qubit0 = atoi(tmp_char);//tmp_char;//atoi(tmp_char);
		}
		else if(count_blank == 3) {
		    if(strcmp(inst_list[count].inst_name, "CNOT") == 0 || strcmp(inst_list[count].inst_name, "CZ") == 0 || strcmp(inst_list[count].inst_name, "SWAP") == 0) {
		        inst_list[count].qubit1 = atoi(tmp_char);//tmp_char;//atoi(tmp_char);
		    }
		    else if(strcmp(inst_list[count].inst_name, "RX") == 0 || strcmp(inst_list[count].inst_name, "RY") == 0 || strcmp(inst_list[count].inst_name, "RZ") == 0) {
			if(inst_list[count].flag == '*') {
			    inst_list[count].feature_index = atoi(tmp_char);
			}
			else {
			    inst_list[count].angle = atof(tmp_char);
			}
		    }
		    else if(strcmp(inst_list[count].inst_name, "MZ") == 0) {
                        inst_list[count].qubit1 = atoi(tmp_char);
		    }
		    else {
	                inst_list[count].angle = atof(tmp_char);//tmp_char;//atof(tmp_char);
		    }
		}

	        last_blank_postion = i + 1;
		count_blank++;

		if(buf[i] == '\n') {
		    count++;
		    break;
		}

	    }
	}
    }
    fclose(fp);
    //end reading network file

    for(int i = 0; i < count; i++){
	std::cout << inst_list[i].flag << ' ' << inst_list[i].inst_name << ' ' << inst_list[i].qubit0  << ' ' << inst_list[i].qubit1 << ' ' << inst_list[i].angle << std::endl;
    }
    std::cout << std::endl;

    //read feature
    FILE *fp_data = fopen(argv[2],"r");
    //pair<vector<vector<double>>,int> data_set;
    vector<pair<vector<double>,int>> data_set;
    vector<pair<vector<double>,int>> training_data_set;
    vector<pair<vector<double>,int>> test_data_set;


    const int data_buf_size = 10 * 100;//(feature_num + 1);
    char data_buf[data_buf_size];
    if(fp == NULL) {
        printf("can't read dataset file.\n");
        exit(0);
    }
    int sample_count = 0;


    while(fgets(data_buf, data_buf_size, fp_data) != NULL) {
	int last_blank_postion = 0;
	int blank_count = 0;
	vector<double> feature;
	int lable = 0;
	for(int i = 0; i < data_buf_size; i++) {
	    if(data_buf[i] == ' ') {
	        char tmp_char[10];
		for(int t = 0; t < 10; t++) tmp_char[t] = 0;
		int tmp_char_long = i - last_blank_postion;
		for(int t = 0; t < tmp_char_long; t++) {
		    tmp_char[t] = data_buf[last_blank_postion + t];
		}
		float tmp_feature = atof(tmp_char);
		feature.push_back(tmp_feature);
		//feature_array[sample_count][blank_count] = feature;
		//blank_count++;
		last_blank_postion = i;
	    }
            else if(data_buf[i] == '\r' || data_buf[i] == '\n') {
	        char tmp_char[10];
		for(int t = 0; t < 10; t++) tmp_char[t] = 0;
		tmp_char[0] = data_buf[i - 1];
		lable = atoi(tmp_char);
                //int lable = atoi(tmp_char);
                //lable_array[sample_count] = lable;
		break;
	    }
	    //push_back(make_pair(feature,lable));
	}
	data_set.push_back(make_pair(feature,lable));
	sample_count++;
    }

    fclose(fp_data);
    for(int i = 0; i < data_set.size(); i++) {
	if(i < 0.6 * data_set.size()) {//划分训练集、测试集
	    training_data_set.push_back(data_set[i]);
	}
	else {
	    test_data_set.push_back(data_set[i]);
	}
    }
    //end read feature



    float *traning_parameter_array = new float[traning_parameter_num]();
    for(int i = 0; i < traning_parameter_num; i++) {
	traning_parameter_array[i] = rand()/(RAND_MAX + 1.0) * 180.0 - 90.0;//初始化参数
	std::cout << traning_parameter_array[i] << ' ';
    }
    std::cout << "traning_parameter_num: " << traning_parameter_num << std::endl << std::endl;

    //traning
    //rmdir("training");
    //mkdir("training",S_IRWXU);

    //double learning_rate = 0.9;
    double decay1 = 0.9;
    double decay2 = 0.999;
    double stepsize = 0.01;
    double eps = 1.0e-8;

    double m(0), v(0), mk(0), vk(0);

    double* last_m = new double[traning_parameter_num]();
    double* last_v = new double[traning_parameter_num]();

    //FILE *result = fopen("result_loss.data","w+");


    for(int i = 0; i < epoch; i++) {
	float* g_array00 = new float[training_data_set.size()]();
	float* g_array01 = new float[training_data_set.size()]();
	float* g_array10 = new float[training_data_set.size()]();
	float LCE = 0;
	int training_correct_count = 0;
	int test_correct_count = 0;

	//char dump_name[30] = {0};
        //sprintf(dump_name,"training/epoch%d_dump.data",i);
        //FILE *dump = fopen(dump_name,"w+");

	//double rhoC = 0.0;

	for(int sample = 0; sample < training_data_set.size(); sample++) {
	    float *feature_array = new float[training_data_set[sample].first.size()]();//[feature_num];
	    for(int j = 0; j < training_data_set[sample].first.size(); j++) {
		feature_array[j] = training_data_set[sample].first[j];
	    }
	    //int lable = lable_array[sample];
	    int lable = training_data_set[sample].second;
	    float N_prob00 = calculate_prob(inst_list,feature_array,traning_parameter_array,-2,sample,i,0,0,0);
	    float N_prob01 = calculate_prob(inst_list,feature_array,traning_parameter_array,-2,sample,i,0,1,0);
	    float N_prob10 = calculate_prob(inst_list,feature_array,traning_parameter_array,-2,sample,i,0,0,1);

	    g_array00[sample] = N_prob00;
	    g_array01[sample] = N_prob01;
	    g_array10[sample] = N_prob10;

	    //fprintf(dump,"epoch%d no_shift sample%d: %f\n",i,sample,N_prob);

	    delete feature_array;

            if(training_data_set[sample].second/*lable_array[sample]*/ == 0) {
                LCE -= log(N_prob00);
	    }
	    else if(training_data_set[sample].second/*lable_array[sample]*/ == 1) {
		LCE -= log(N_prob01);
	    }
	    else {
		LCE -= log(N_prob10);
	    }

            int pred_lable = -1;
            if(N_prob00 >= N_prob01 && N_prob00 >= N_prob10) {
                pred_lable = 0;
            }
            else if(N_prob01 >= N_prob00 && N_prob01 >= N_prob10){
                pred_lable = 1;
            }
            else if(N_prob10 >= N_prob00 && N_prob10 >= N_prob01) {
                pred_lable = 2;
            }

	    if(pred_lable == -1) assert(0);

	    if(pred_lable == training_data_set[sample].second/*lable_array[sample]*/) training_correct_count++;
	}

	float training_accuracy = (float)training_correct_count / training_data_set.size();



        for(int sample = 0; sample < test_data_set.size(); sample++) {
            float *feature_array = new float[test_data_set[sample].first.size()]();//[feature_num];
            for(int j = 0; j < test_data_set[sample].first.size(); j++) {
                feature_array[j] = test_data_set[sample].first[j];
            }
            //int lable = lable_array[sample];
            int lable = test_data_set[sample].second;
            float N_prob00 = calculate_prob(inst_list,feature_array,traning_parameter_array,-1,sample,i,0,0,0);
            float N_prob01 = calculate_prob(inst_list,feature_array,traning_parameter_array,-1,sample,i,0,1,0);
	    float N_prob10 = calculate_prob(inst_list,feature_array,traning_parameter_array,-1,sample,i,0,0,1);

            delete feature_array;

            int pred_lable = -1;
            if(N_prob00 >= N_prob01 && N_prob00 >= N_prob10) {
                pred_lable = 0;
            }
            else if(N_prob01 >= N_prob00 && N_prob01 >= N_prob10){
                pred_lable = 1;
            }
	    else {
		pred_lable = 2;
	    }


            if(pred_lable == test_data_set[sample].second/*lable_array[sample]*/) test_correct_count++;
        }

	//float training_accuracy = (float)training_correct_count / training_data_set.size();
	float test_accuracy = (float)test_correct_count / test_data_set.size();
	printf("epoch:%d loss:%f training accuracy:%f test accuracy:%f\n",i,LCE,training_accuracy,test_accuracy);


	//calculate_LEC();
	double parameter_dev[32] = {0.0};
	for(int param = 0; param < traning_parameter_num; param++) {
	    float rand0 = rand()/(RAND_MAX + 1.0);
         //   if(rand0 > 0.25) continue;
	    //int param = (int) (rand()/(RAND_MAX + 1.0) * 24.0);

	    float pcost = 0;
	   /* float *feature_array = new float[training_data_set[sample].first.size()]();//[feature_num];
            for(int j = 0; j < training_data_set[sample].first.size(); j++) {
                feature_array[j] = training_data_set[sample].first[j];
            }*/

	    for(int sample = 0; sample < training_data_set.size(); sample++) {
		float rand1 = rand()/(RAND_MAX + 1.0);
	//	if(rand1 > 0.25) continue;

		float tmp_LEC = 0;
		
		float *feature_array = new float[training_data_set[sample].first.size()]();//[feature_num];
                for(int j = 0; j < training_data_set[sample].first.size(); j++) {
                    feature_array[j] = training_data_set[sample].first[j];
                }

		float gkp00 = calculate_prob(inst_list,feature_array,traning_parameter_array, param,sample,i,3.141492/2,0,0);
                float gkp01 = calculate_prob(inst_list,feature_array,traning_parameter_array, param,sample,i,3.141492/2,1,0);
		float gkp10 = calculate_prob(inst_list,feature_array,traning_parameter_array, param,sample,i,3.141492/2,0,1);
		//gkp = gkp_resu.real;
		//fprintf(dump,"epoch%d P_shift param%d sample%d: %f\n",i,param,sample,gkp);
		float gkm00 = calculate_prob(inst_list,feature_array,traning_parameter_array, param,sample,i,-3.141492/2,0,0);
                float gkm01 = calculate_prob(inst_list,feature_array,traning_parameter_array, param,sample,i,-3.141492/2,1,0);
		float gkm10 = calculate_prob(inst_list,feature_array,traning_parameter_array, param,sample,i,-3.141492/2,0,1);

		float gk00 = (gkp00 - gkm00)/2;
		float gk01 = (gkp01 - gkm01)/2;
		float gk10 = (gkp10 - gkm10)/2;

		
		if(training_data_set[sample].second == 0) {
		    pcost = pcost - (1 / g_array00[sample]) * gk00;
                }
                else if(training_data_set[sample].second == 1) {
		    pcost = pcost - (1 / g_array01[sample]) * gk01;
                }
		else {
		    pcost = pcost - (1 / g_array10[sample]) * gk10;
		}
	//	pcost = pcost - (1 / g_array[sample]) * gk - (1 / (1 - g_array[sample])) * ( -gk);
		delete feature_array;
	    }
	    //delete feature_array;
	    pcost = 0.25 * pcost / sample_count;
	    
	    double grad = pcost;

	    m = decay1 * last_m[param] + (1.0 - decay1)*grad;
            v = decay2 * last_v[param] + (1.0 - decay2)*grad*grad;
	    int t = epoch;
            mk = m / (1.0 - pow(decay1, t));
            vk = v / (1.0 - pow(decay2, t));

	    //adam
	    traning_parameter_array[param] -= stepsize * (mk / (sqrt(vk) + eps));
	    //sgd
            //traning_parameter_array[param] -= 0.02 * pcost;

            double tmp_dump = stepsize * (mk / (sqrt(vk) + eps));
            parameter_dev[param] = tmp_dump;


	    last_m[param] = m;
	    last_v[param] = v;

	}

        delete g_array00, g_array01, g_array10;
	//fclose(dump);

  }

    delete last_m;
    delete last_v;

    delete traning_parameter_array;

    return 0;
}
