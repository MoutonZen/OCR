#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>


int main(int argc, char const *argv[]) {
    if (argc != 3)
        errx(1, "2 arguments are required to make a XOR");
    int param1 = strtoul(argv[1], NULL, 10);
    int param2 = strtoul(argv[2], NULL, 10);
    if ((param1 != 1 && param1 != 0) || (param2 != 1 && param2 != 0))
        errx(2, "the arguments must be equal to 1 or 0");

    Network *network = network_load();
    Network net;
    if (network == NULL)
    {
        net = __initNetwork__(2, 3, 1);
        network_xor_train(net);
    }
    else
        net = *network;
    float *input = malloc(2);
    input[0] = param1;
    input[1] = param2;
    float* out = network_evaluate(net, input);
    printf("%d XOR %d --> %f\n", param1, param2, out[0]);
    free_network(net);
    free(input);
    if (network == NULL)
        free(network);
    else
        free_network(*network);
    return 0;
}

