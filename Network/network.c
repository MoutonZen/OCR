#include "network.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


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
        n.weights[i] = -0.5 + ((float)rand()/(float)(RAND_MAX));
    }
    for (int i = 0; i < n.len_Biases; ++i)
    {
        n.biases[i] = ((float)rand()/(float)(RAND_MAX));
    }
    n.hiddens_neur = malloc(n.Hiddens * sizeof(*n.hiddens_neur));
    for (int i = 0; i < n.Hiddens; ++i)
    {
        n.hiddens_neur[i] = 0;
    }
    n.outputs_neur = malloc(n.Outputs* sizeof(*n.outputs_neur));
    for (int i = 0; i < n.Outputs; ++i)
    {
        n.outputs_neur[i] = 0;
    }
    return n;
}

void wait_for_keypresse()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}



float sigmoid(float z){
    return 1 / (1 + expf(-z));
}

float sigmoid_prime(float z){
    return z * (1 - z);
}

void feedforward(Network n, float* Inputs){
    for(int i = 0; i < n.Hiddens; i++) {
        float sum = 0;
        for(int j = 0; j < n.Inputs; j++){
            sum += Inputs[j] * n.weights[i * n.Inputs + j];
        }
        n.hiddens_neur[i] = sigmoid(sum);
    }
}

void feedforward_bis(Network n){
    float* weights = n.weights + n.Inputs * n.Hiddens;
    for(int i = 0; i < n.Outputs; i++) {
        float sum = 0.0f;
        for(int j = 0; j < n.Hiddens; j++)
            sum += n.hiddens_neur[j] * weights[i * n.Hiddens + j];;
        n.outputs_neur[i] = sigmoid(sum);
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

void network_xor_train(Network net){
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
    free(input);
    free(output);
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

char* unknow_string_length(char* msg){
    unsigned int len_max = 10;
    unsigned int current_size = 0;
    char *tmp = malloc(len_max);
    current_size = len_max;
    printf("%s\n", msg);
    if(tmp != NULL){
        int c = EOF;
        unsigned int i =0;
        while (( c = getchar() ) != '\n' && c != EOF){
            printf("%c\n", c);
            tmp[i++]=(char)c;
            if(i == current_size){
                current_size = i+len_max;
                tmp = realloc(tmp, current_size);
            }
        }
        tmp[i] = '\0';
    }
    scanf("%s", tmp);
    return tmp;
}

void network_save(Network n, char* file_name){
    char* filename = "\0";
    int replace = 0;
    while(access(file_name, F_OK ) != -1 && !replace)
    {
        char res[1];
        do{
            printf("Ce fichier éxiste déjà, voulez vous le remplacer?(Y/N)\n");
            scanf("%s", res);
        }while(res[0]!='Y' && res[0] !='y' && res[0]!='n' && res[0]!='N');
        if (res[0] == 'n' || res[0] == 'N')
        {
            filename = unknow_string_length("Dans quel fichier souhaitez vous l'enregistrer?");
            replace = 1;
        }
        else
            remove(file_name);
    }
    if (filename[0] == '\0')
        filename = file_name;
    FILE *file;
    file = fopen(filename,"w");
    fprintf(file, "%d\n", n.Inputs);
    fprintf(file, "%d\n", n.Hiddens);
    fprintf(file, "%d\n", n.Outputs);
    for (int i = 0; i < n.len_Weights; ++i)
    {
        fprintf(file, "%f", n.weights[i]);
        if (i!=n.len_Weights-1)
            fprintf(file, " ");
        else
            fprintf(file, "\n");
    }
    for (int i = 0; i < n.len_Biases; ++i)
    {
        fprintf(file, "%f", n.biases[i]);
        if (i!=n.len_Biases-1)
            fprintf(file, " ");
        else
            fprintf(file, "\n");
    }
    for (int i = 0; i < n.Hiddens; ++i)
    {
        fprintf(file, "%f", n.hiddens_neur[i]);
        if (i!=n.Hiddens-1)
            fprintf(file, " ");
        else
            fprintf(file, "\n");
    }
    for (int i = 0; i < n.Outputs; ++i)
    {
        fprintf(file, "%f", n.outputs_neur[i]);
        if (i!=n.Outputs-1)
            fprintf(file, " ");
        else
            fprintf(file, "\n");
    }
    fclose(file);  
}

Network *network_load(){
	Network n;
    char res[1];
    do{
        printf("Souhaitez vous charger un network?(Y/N)\n");
        scanf("%s", res);
    }while(res[0]!='Y' && res[0] !='y' && res[0]!='n' && res[0]!='N');
    if(res[0] == 'N' || res[0] == 'n')
        return NULL;
    char* filename = unknow_string_length("Ou se trouve le fichier à charger?");
    while(access(filename, F_OK ) == -1){
        printf("Ce fichier n'existe pas.\n");
        filename = unknow_string_length("Ou se trouve le fichier à charger?");
    }
    FILE *file;
    file = fopen(filename,"r");
    int inputs = 0;
    fscanf(file, "%d", &inputs);
    int hiddens = 0;
    fscanf(file, "%d", &hiddens);
    int outputs = 0;
    fscanf(file, "%d", &outputs);
    n = __initNetwork__(inputs, hiddens, outputs);
    float net = 0;
    for (int i = 0; i < n.len_Weights; ++i)
    {
    	fscanf(file, "%f", &net);
    	n.weights[i] = net;
    }
    for (int i = 0; i < n.len_Biases; ++i)
    {
    	fscanf(file, "%f", &net);
    	n.biases[i] = net;
    }
    for (int i = 0; i < n.Hiddens; ++i)
    {
    	fscanf(file, "%f", &net);
    	n.hiddens_neur[i] = net;
    }
    for (int i = 0; i < n.Outputs; ++i)
    {
    	fscanf(file, "%f", &net);
    	n.outputs_neur[i] = net;
    }
    Network *network = &n;
    return network;
}

void load_outputfile(char *output)
{
    char res[1];
    do{
        printf("Souhaitez vous charger un network?(Y/N)\n");
        scanf("%s", res);
    }while(res[0]!='Y' && res[0] !='y' && res[0]!='n' && res[0]!='N');
    if(res[0] == 'N' || res[0] == 'n')
        return;
    char* filename = unknow_string_length("Ou se trouve le fichier à charger?");
    while(access(filename, F_OK ) == -1){
        printf("Ce fichier n'existe pas.\n");
        filename = unknow_string_length("Ou se trouve le fichier à charger?");
    }
    FILE *file;
    file = fopen(filename,"r");
    char ch[1];
    int tmp = fscanf(file, "%c", ch);
    output[0] = ch[0];
    int i = 1;
    while(tmp==1)
    {
        tmp = fscanf(file, "%c", ch);
        if (tmp ==1)
            output[i] = ch[0];
        i++;
    }
}
