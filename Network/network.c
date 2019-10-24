#include "network.h"


Network __initNetwork__(int Inputs, int Hiddens, int Outputs){
    Network n;
    n.Inputs = Inputs;
    n.Outputs = Outputs;
    n.Hiddens = Hiddens;
    n.len_Weights = Hiddens * (Inputs + Outputs);
    n.len_Biases = 2;
    n.weights = malloc(n.len_Weights * sizeof(*n.weights));
    n.biases = malloc(n.len_Biases   * sizeof(*n.biases));
    for (int i = 0; i < n.len_Weights; ++i)
    {
        n.weights[i] = ((float)rand()/(float)(RAND_MAX));
    }
    for (int i = 0; i < n.len_Biases; ++i)
    {
        n.biases[i] = ((float)rand()/(float)(RAND_MAX));
    }
    n.hiddens_neur = malloc((n.Hiddens + n.Outputs)
        * sizeof(*n.hiddens_neur));
    n.outputs_neur    = malloc((n.Hiddens + n.Outputs)
        * sizeof(*n.outputs_neur)) + Hiddens;
    return n;
}


float sigmoid(float z){
    return 1.0f / (1.0f + expf(-z));
}

float sigmoid_prime(float z){
    return z * (1 - z);
}

void feedforward(Network n, float* Inputs){
    for(int i = 0; i < n.Hiddens; i++) {
        float sum = 0;
        for(int j = 0; j < n.Inputs; j++)
            sum += Inputs[j] * n.weights[i * n.Inputs + j];
        n.hiddens_neur[i] = sigmoid(sum + n.biases[0]);
    }
}

void feedforward_bis(Network n){
    float* tmp = n.weights + n.Inputs * n.Hiddens;
    for(int i = 0; i < n.Outputs; i++) {
        float sum = 0.0f;
        for(int j = 0; j < n.Hiddens; j++)
            sum += n.hiddens_neur[j] * tmp[i * n.Hiddens + j];
        n.outputs_neur[i] = sigmoid(sum + n.biases[1]);
    }
}

void backPropagation(Network n, float* Inputs, float* Outputs){
    for(int i = 0; i < n.Hiddens; i++) {
        float sum = 0.0f;
        for (int j = 0; j < n.Outputs; j++) {
            float sig_prime = sigmoid_prime(n.outputs_neur[j]);
            int   pos  = n.Hiddens * (n.Inputs + j) + i;
            sum +=  sig_prime * (n.outputs_neur[j] - Outputs[j])
            * n.weights[pos];

            n.weights[pos] -= sig_prime * (n.outputs_neur[j] - Outputs[j])
            * n.hiddens_neur[i];
        }

        for(int j = 0; j < n.Inputs; j++) {
            float sig_prime = sigmoid_prime(n.hiddens_neur[i]);
            n.weights[i * n.Inputs + j] -= sum * sig_prime * Inputs[j];
        }
    }
}


void network_train(Network n, float* Inputs, float* Outputs){
    feedforward(n, Inputs);
    feedforward_bis(n);
    backPropagation(n, Inputs, Outputs);
}

float* network_evaluate(Network n, float* Inputs) {
    feedforward(n, Inputs);
    feedforward_bis(n);
    return n.outputs_neur;
}

void free_network(Network n) {
    free(n.hiddens_neur);
    free(n.weights);
    free(n.biases);
}