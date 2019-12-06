# ifndef NETWORK_H
# define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <unistd.h>

typedef struct Network Network;
struct Network{
    float *hiddens_neur;
    float *outputs_neur;

    int Inputs;
    int Hiddens;
    int Outputs;

    float *weights;
    float *biases;

    int len_Weights;
    int len_Biases;
};

Network __initNetwork__(int Inputs, int Hiddens, int Outputs);
float sigmoid(float z);
float sigmoid_prime(float z);

void feedforward(Network n, float* Inputs);
void feedforward_bis(Network n);

void backPropagation(Network n, float* Inputs, float* Outputs);

void network_xor_train(Network net);

void network_train(Network n, float* Inputs, float* Outputs);
float* network_evaluate(Network n, float* Inputs);

void network_save(Network n, char* filename);
Network *network_load();
void load_outputfile(char *output);

void free_network(Network n);


# endif