#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>


int main(int argc, char const *argv[]) {
    if (argc != 3){
        errx(1, "2 arguments are required to make a XOR");
    }
    int param1 = strtoul(argv[1], NULL, 10);
    int param2 = strtoul(argv[2], NULL, 10);
    if ((param1 != 1 && param1 != 0) || (param2 != 1 && param2 != 0))
        errx(2, "the arguments must be equal to 1 or 0");
    Network net = __initNetwork__(2, 3, 1);
    float *input = malloc(2);
    float *output = malloc(1);
    for (int i = 0; i < 10000; ++i)
    {
        input[0] = 1;
        input[1] = 0;
        output[0] = 1;
        network_train(net, input, output);

        input[0] = 0;
        input[1] = 0;
        output[0] = 0;
        network_train(net, input, output);

        input[0] = 1;
        input[1] = 1;
        output[0] = 0;
        network_train(net, input, output);

        input[0] = 0;
        input[1] = 1;
        output[0] = 1;
        network_train(net, input, output);
    }
    input[0] = param1;
    input[1] = param2;
    float* out = network_evaluate(net, input);
    printf("%d XOR %d --> %f\n", (int)input[0], (int)input[1], out[0]);
    free_network(net);
    free(input);
    free(output);
    return 0;
}

