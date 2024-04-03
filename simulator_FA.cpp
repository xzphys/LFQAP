#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <simulator.hpp>

#define PI 3.14159265358979

/*
typedef struct complex {
    double real, imag;
}Complex;

typedef struct qr {
    unsigned short size;
    Complex *matrix;
}quReg;
*/

//quReg* newQuReg(size_t n);
void X(quReg *qr, int idx);
void Y(quReg *qr, int idx);
void Z(quReg *qr, int idx);
void H(quReg *qr, int idx);
void RX(double angle, quReg *qr, int idx);
void RY(double angle, quReg *qr, int idx);
void RZ(double angle, quReg *qr, int idx);
void CNOT(quReg *qr, int idx1, int idx2);
void CZ(quReg *qr, int idx1, int idx2);
void SWAP(quReg *qr, int idx1, int idx2);
//void Qprint(quReg *qr);

quReg* newQuReg(size_t n) {
    quReg *qr = (quReg*) malloc(sizeof(quReg));

    qr->size = n;


    qr->matrix = (Complex*) malloc(pow(2, n) * sizeof(Complex));
    qr->matrix[0].real = 1;  
    qr->matrix[0].imag = 0;

    for(int i = 1; i < pow(2, n); i++) {
        qr->matrix[i].real = qr->matrix[i].imag = 0;
    }   

    return qr; 
}

void X(quReg *qr, int idx) {
	int prev_state = -1, next_state = -1;
	int i, size = pow(2, qr->size);


	for(i = 0; i < size; i++) {
		if((i & (1 << idx)) == 0) {
			prev_state = i;
			next_state = prev_state ^ (1 << idx);

			Complex temp = qr->matrix[prev_state];
			qr->matrix[prev_state] = qr->matrix[next_state];
			qr->matrix[next_state] = temp;
		}
	}

//	return qr;
}

void Y(quReg *qr, int idx) {
	int prev_state = 0, next_state = 0;
	int i, size = pow(2, qr->size);

	for(i = 0; i < size; i++) {
		if((i & (1 << idx)) == 0) {
			prev_state = i;
			next_state = prev_state ^ (1 << idx);

			Complex temp_ps = {qr->matrix[next_state].imag, -1 * qr->matrix[next_state].real};
			Complex temp_ns = {-1 * qr->matrix[prev_state].imag, qr->matrix[prev_state].real};

			qr->matrix[prev_state] = temp_ps;
			qr->matrix[next_state] = temp_ns;
		}
	}

//	return qr;
}

void Z(quReg *qr, int idx) {
	int i, size = pow(2, qr->size);

	for(i = 0; i < size; i++) {
		if((i & (1 << idx)) > 0) {
			qr->matrix[i].real *= -1;
			qr->matrix[i].imag *= -1;
		}
	}

//	return qr;
}


void RX(double angle, quReg *qr, int idx) {

	int size = pow(2, qr->size);
    bool *visited = (bool*) malloc(size * sizeof(bool));

    angle = angle / 2;

    int first, second;

    for(int i = 0; i < size; i++)
        visited[i] = false;

	for(int i = 0; i < size; i++) {
		if(!visited[i]/* && (i & (1 << idx)) == 0*/) {

            first = i;
            second = i ^ (1 << idx);

            visited[first] = visited[second] = true;

			double a_real = qr->matrix[i].real;
			double a_imag = qr->matrix[i].imag;

            double b_real = qr->matrix[second].real;
            double b_imag = qr->matrix[second].imag;

			qr->matrix[i].real = a_real * cos(angle) + b_imag * sin(angle);
            qr->matrix[i].imag = a_imag * cos(angle) - b_real * sin(angle);

			qr->matrix[second].real = a_imag * sin(angle) + b_real * cos(angle);
            qr->matrix[second].imag = -a_real * sin(angle) + b_imag * cos(angle);
		}
	}
	free(visited);

//	return qr;
}


void RY(double angle, quReg *qr, int idx) {

	int size = pow(2, qr->size);
    bool *visited = (bool*) malloc(size * sizeof(bool));

    angle = angle / 2;

    int first, second;

    for(int i = 0; i < size; i++)
        visited[i] = false;

	for(int i = 0; i < size; i++) {
		if(!visited[i]/* && (i & (1 << idx)) == 0*/) {

            first = i;
            second = i ^ (1 << idx);

            visited[first] = visited[second] = true;

			double a_real = qr->matrix[i].real;
			double a_imag = qr->matrix[i].imag;

            double b_real = qr->matrix[second].real;
            double b_imag = qr->matrix[second].imag;

			qr->matrix[i].real = a_real * cos(angle) - b_real * sin(angle);
            qr->matrix[i].imag = a_imag * cos(angle) - b_imag * sin(angle);

			qr->matrix[second].real = a_real * sin(angle) + b_real * cos(angle);
            qr->matrix[second].imag = a_imag * sin(angle) + b_imag * cos(angle);
		}
	}
	free(visited);

//	return qr;
}

void RZ(double angle, quReg *qr, int idx) {

	int size = pow(2, qr->size);
    bool *visited = (bool*) malloc(size * sizeof(bool));

  //  angle = angle / 2;

    int first, second;

    for(int i = 0; i < size; i++)
        visited[i] = false;

	for(int i = 0; i < size; i++) {
		if(!visited[i]/* && (i & (1 << idx)) == 0*/) {

            first = i;
            second = i ^ (1 << idx);

            visited[first] = visited[second] = true;

			double a_real = qr->matrix[i].real;
			double a_imag = qr->matrix[i].imag;

            double b_real = qr->matrix[second].real;
            double b_imag = qr->matrix[second].imag;

			qr->matrix[i].real = a_real;
            qr->matrix[i].imag = a_imag;

			qr->matrix[second].real = b_real * cos(angle) - b_imag * sin(angle);
            qr->matrix[second].imag = b_imag * cos(angle) + b_real * sin(angle);
		}
	}
	free(visited);

//	return qr;
}

void H(quReg *qr, int idx) {

	const int mat_size = pow(2, qr->size);
	bool *visited = (bool*) malloc(mat_size * sizeof(bool));

	for(int i = 0; i < mat_size; i++)
		visited[i] = false;

	int first, second;

	for(int i = 0; i < mat_size; i++) {
		if(!visited[i]) {
            if((i & (1 << idx)) == 0) {
                first = i;
                second = i ^ (1 << idx);
            }
            else {
                first = i ^ (1 << idx);
                second = i;
                printf("enterd.\n");
            }

			visited[first] = visited[second] = true;
            double temp_ps_real, temp_ns_real, temp_ps_imag, temp_ns_imag;

            temp_ps_real = qr->matrix[first].real;
            temp_ps_imag = qr->matrix[first].imag;

            temp_ns_real = qr->matrix[second].real;
            temp_ns_imag = qr->matrix[second].imag;
               
            qr->matrix[first].real = (temp_ps_real + temp_ns_real) / sqrt(2);
            qr->matrix[second].real = (temp_ps_real - temp_ns_real) / sqrt(2);

            qr->matrix[first].imag = (temp_ps_imag + temp_ns_imag) / sqrt(2);
            qr->matrix[second].imag = (temp_ps_imag - temp_ns_imag) / sqrt(2);
		}
	}

	free(visited);
//	return qr;
}

void CNOT(quReg *qr, int idx1, int idx2) {
    const int mat_size = pow(2, qr->size);
    bool *visited = (bool*) malloc(mat_size * sizeof(bool));

    for(int i = 0; i < mat_size; i++) {
        visited[i] = false;
    }

    int first, second, third, fourth;

    for(int i = 0; i < mat_size; i++) {
        if(!visited[i]) {
            if((i & (1 << idx1)) != 0) {
                int tmp = i ^ (1 << idx2);
                double tmp1_real = qr->matrix[i].real;
                double tmp1_imag = qr->matrix[i].imag;

                qr->matrix[i].real = qr->matrix[tmp].real;
                qr->matrix[i].imag = qr->matrix[tmp].imag;

                qr->matrix[tmp].real = tmp1_real;
                qr->matrix[tmp].imag = tmp1_imag;

                second = i ^ (1 << idx1);
                fourth = i ^ (1 << idx1) ^ (1 << idx2);

                visited[second] = visited[fourth] = true;
            }
            first = i;
            third = i ^ (1 << idx2);
            visited[first] = visited[third] = true;
        }
    }
      free(visited);
//    return qr;
}

void CZ(quReg *qr, int idx1, int idx2) {
    const int mat_size = pow(2, qr->size);
    bool *visited = (bool*) malloc(mat_size * sizeof(bool));

    for(int i = 0; i < mat_size; i++) {
        visited[i] = false;
    }

    int first, second, third, fourth;

    for(int i = 0; i < mat_size; i++) {
        if(!visited[i]) {
            if((i & (1 << idx1)) != 0 && (i & (1 << idx2)) != 0) {
                qr->matrix[i].real = -qr->matrix[i].real;
                qr->matrix[i].imag = -qr->matrix[i].imag;

                first = i;
                second = i ^ (1 << idx1);
                third = i ^ (1 << idx2);
                fourth = i ^ (1 << idx1) ^ (1 << idx2);
                visited[first] = visited[second] = visited[third] = visited[fourth] = true;
            }
        }
    }
    free(visited);

   // return qr;
}

void SWAP(quReg *qr, int idx1, int idx2) {

	const int mat_size = pow(2, qr->size);
	bool *visited = (bool*) malloc(mat_size * sizeof(bool));
	int state1, state2;
	int first, second;

	for(int i = 0; i < mat_size; i++)
		visited[i] = false;

	for(int i = 0; i < mat_size; i++) {
		if(!visited[i]) {
			state1 = (i & (1 << idx1)) >> idx1;
			state2 = (i & (1 << idx2)) >> idx2;
			first = i;
			second = (i ^ (1 << idx1)) ^ (1 << idx2);
			visited[first] = visited[second] = true;

			if(state1 != state2) {
				Complex temp = qr->matrix[first];
				qr->matrix[first] = qr->matrix[second];
				qr->matrix[second] = temp;
			}
		}
	}
	free(visited);

//	return qr;
}

void Qprint(quReg *qr) {
    int size = pow(2, qr->size);

    double total_prob = 0;

    for(int i = 0; i < size; i++) {
        printf("|");
        int tmp[32];
        for(int j = 0; j < qr->size; j++) {
            tmp[j] = (i & (1 << j)) >> j;
        }
        for(int k = qr->size - 1; k >= 0; k--) {
            printf("%d", tmp[k]);
        }

        printf("> ");
        printf("%f + %fi : %f%\n", qr->matrix[i].real, qr->matrix[i].imag, 100 * (pow(qr->matrix[i].real, 2) + pow(qr->matrix[i].imag, 2)));
        total_prob += 100 * (pow(qr->matrix[i].real, 2) + pow(qr->matrix[i].imag, 2));
    }
    printf("total_prob:%f\n",total_prob);
}

//int main(int argc, char const *argv[]) {
quReg* simulator(vector <vector<char>> fp_name) {
    quReg *qr1, *qr2, *qr3, *qr4;
    int Ireg1, Ireg2, Ireg3, Ireg4;
    float Freg1, Freg2, Freg3, Freg4;

   // FILE *fp=fopen(argv[1],"r");
   // FILE *fp=fopen(fp_name,"r");
    char buf[20];
  //  if(fp == NULL) {
  //      printf("can't read instruction file.\n");
  //      exit(0);
  //  }


    int inst_count = 0;
   // char buf[20];
    while(/*fgets(buf, 20, fp) != NULL*/1) {
	//if(fp_name[inst_count][0] != 'Q' && fp_name[inst_count][0] != 'H' && fp_name[inst_count][0] != 'X' && fp_name[inst_count][0] != 'Y' && fp_name[inst_count][0] != 'Z' && fp_name[inst_count][0] != 'R' && fp_name[inst_count][0] != 'C' && fp_name[inst_count][0] != 'S') break;
	if(inst_count >= fp_name.size()) break;
	for(int i = 0; i < 20; i++) {
	    buf[i] = fp_name[inst_count][i];
	}
	inst_count++;
        int count = 0;
        for(int i = 0; i < 20; i++) {
           if(buf[i] == ' ') {
               char tmp_char[6];
               for(int t = 0; t < 6; t++) tmp_char[t] = 0;
               int char_long = i - count;
               for(int j = 0; j < char_long; j++) {
                   tmp_char[j] = buf[count + j];
               }
               count = i + 1;

               if(strcmp(tmp_char, "QREG") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == '\n') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int NReg = atoi(tmp_char3);
                                       qr1 = newQuReg(NReg);
                                       break;
                                   }
                               }
                           }
                        /*   else if(strcmp(tmp_char2, "qr2") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {
                           }*/
                           break;
                       }
                   }
               }
               else if(strcmp(tmp_char, "X") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == '\n') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       X(qr1, N1);
                                       break;
                                   }
                               }
                           }
                        /*   else if(strcmp(tmp_char2, "qr2") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {
                           }*/
                           break;
                       }
                   }
               }
               else if (strcmp(tmp_char, "Y") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == '\n') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       Y(qr1, N1);
                                       break;
                                   }
                               }
                           }
                        /*   else if(strcmp(tmp_char2, "qr2") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {
                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "Z") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == '\n') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       Z(qr1, N1);
                                       break;
                                   }
                               }
                           }
                        /*   else if(strcmp(tmp_char2, "qr2") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {
                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "RX") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == ' ') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       count = m + 1;

                                       for(int o = count; o < 20; o++) {
                                           if(buf[o] == '\n') {
                                               char tmp_char4[8];
                                               for(int t = 0; t < 8; t++) tmp_char4[t] = 0;
                                               int char_long4 = o - count;
                                               for(int p = 0; p < char_long4; p++) {
                                                   tmp_char4[p] = buf[count + p];
                                               }
                                               float N2 = atof(tmp_char4);
                                               RX(N2, qr1, N1);
                                               break;
                                           }
                                       }
                                       break;
                                   }
                               }
                           }
                          /* else if(strcmp(tmp_char2, "qr2") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {

                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "RY") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == ' ') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       count = m + 1;

                                       for(int o = count; o < 20; o++) {
                                           if(buf[o] == '\n') {
                                               char tmp_char4[8];
                                               for(int t = 0; t < 8; t++) tmp_char4[t] = 0;
                                               int char_long4 = o - count;
                                               for(int p = 0; p < char_long4; p++) {
                                                   tmp_char4[p] = buf[count + p];
                                               }
                                               float N2 = atof(tmp_char4);
                                               RY(N2, qr1, N1);
                                               break;
                                           }
                                       }
                                       break;
                                   }
                               }
                           }
                          /* else if(strcmp(tmp_char2, "qr2") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {

                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "RZ") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == ' ') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       count = m + 1;

                                       for(int o = count; o < 20; o++) {
                                           if(buf[o] == '\n') {
                                               char tmp_char4[8];
                                               for(int t = 0; t < 8; t++) tmp_char4[t] = 0;
                                               int char_long4 = o - count;
                                               for(int p = 0; p < char_long4; p++) {
                                                   tmp_char4[p] = buf[count + p];
                                               }
                                               float N2 = atof(tmp_char4);
                                               RZ(N2, qr1, N1);
                                               break;
                                           }
                                       }
                                       break;
                                   }
                               }
                           }
                          /* else if(strcmp(tmp_char2, "qr2") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {

                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "H") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == '\n') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       H(qr1, N1);
                                       break;
                                   }
                               }
                           }
                        /*   else if(strcmp(tmp_char2, "qr2") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {
                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "CNOT") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == ' ') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       count = m + 1;

                                       for(int o = count; o < 20; o++) {
                                           if(buf[o] == '\n') {
                                               char tmp_char4[6];
                                               for(int t = 0; t < 6; t++) tmp_char4[t] = 0;
                                               int char_long4 = o - count;
                                               for(int p = 0; p < char_long4; p++) {
                                                   tmp_char4[p] = buf[count + p];
                                               }
                                               int N2 = atoi(tmp_char4);
                                               CNOT(qr1, N1, N2);
                                               break;
                                           }
                                       }
                                       break;
                                   }
                               }
                           }
                          /* else if(strcmp(tmp_char2, "qr2") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {

                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "SWAP") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == ' ') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       count = m + 1;

                                       for(int o = count; o < 20; o++) {
                                           if(buf[o] == '\n') {
                                               char tmp_char4[6];
                                               for(int t = 0; t < 6; t++) tmp_char4[t] = 0;
                                               int char_long4 = o - count;
                                               for(int p = 0; p < char_long4; p++) {
                                                   tmp_char4[p] = buf[count + p];
                                               }
                                               int N2 = atoi(tmp_char4);
                                               SWAP(qr1, N1, N2);
                                               break;
                                           }
                                       }
                                       break;
                                   }
                               }
                           }
                          /* else if(strcmp(tmp_char2, "qr2") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {

                           }*/
                           break;
                       }
                   }

               }
               else if (strcmp(tmp_char, "CZ") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == ' ') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               for(int m = count; m < 20; m++) {
                                   if(buf[m] == ' ') {
                                       char tmp_char3[6];
                                       for(int t = 0; t < 6; t++) tmp_char3[t] = 0;
                                       int char_long3 = m - count;
                                       for(int n = 0; n < char_long3; n++) {
                                           tmp_char3[n] = buf[count + n];
                                       }
                                       int N1 = atoi(tmp_char3);
                                       count = m + 1;

                                       for(int o = count; o < 20; o++) {
                                           if(buf[o] == '\n') {
                                               char tmp_char4[6];
                                               for(int t = 0; t < 6; t++) tmp_char4[t] = 0;
                                               int char_long4 = o - count;
                                               for(int p = 0; p < char_long4; p++) {
                                                   tmp_char4[p] = buf[count + p];
                                               }
                                               int N2 = atoi(tmp_char4);
                                               CZ(qr1, N1, N2);
                                               break;
                                           }
                                       }
                                       break;
                                   }
                               }
                           }
                          /* else if(strcmp(tmp_char2, "qr2") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {

                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {

                           }*/
                           break;
                       }
                   }

               }
	       else if(strcmp(tmp_char, "QPRINT") == 0) {
                   for(int k = count; k < 20; k++) {
                       if(buf[k] == '\n') {
                           char tmp_char2[6];
                           for(int t = 0; t < 6; t++) tmp_char2[t] = 0;
                           int char_long2 = k - count;
                           for(int l = 0; l < char_long2; l++) {
                               tmp_char2[l] = buf[count + l];
                           }
                           count = k + 1;

                           if(strcmp(tmp_char2, "qr1") == 0) {
                               Qprint(qr1); 
                           }
                        /*   else if(strcmp(tmp_char2, "qr2") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr3") == 0) {
                           }
                           else if(strcmp(tmp_char2, "qr4") == 0) {
                           }*/
                           break;
                       }
                   }
               }

               break;
           }
        }
    }
    
    //fclose(fp);

//    printf("\n\n");
//    Qprint(qr1);

    return qr1;
}

