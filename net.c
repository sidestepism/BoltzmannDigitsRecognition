// net.c
#include "boltzmann.h"
#include "net.h"
#include "limits.h"


net_t mk_net(int units, int next_units, net_t next)
{
    net_t net = (net_t) malloc(sizeof(struct net));

    net->units = units;

    net->value = (int*) malloc(sizeof(int) * units);
    memset(net->value, '\0', sizeof(int) * units);

    net->value_sg = (int*) malloc(sizeof(int) * units);
    memset(net->value_sg, '\0', sizeof(int) * units);

    net->weight = (int**) malloc(sizeof(int*) * units);
    for(int i = 0; i < units; i++){
        net->weight[i] = (int*) malloc(sizeof(int) * next_units);
        memset(net->weight[i], '\0', sizeof(int) * next_units);

        // random weight
        for (int j = 0; j < next_units; ++j){
            net->weight[i][j] = (rand() % (HIGH - LOW)) + LOW;
        }
    }
    net->next = next;

    return net;
}

void propagates_one_net(net_t net)
{
    assert(net->next != NULL);

    for(int i = 0; i < net->next->units; i++){
        long sum = 0;
        for(int j = 0; j < net->units; j++){
            sum += net->value_sg[j] * net->weight[j][i];
//            printf("%d * %d\n", net->value[j], net->weight[j][i]);
        }
        // わる
//        printf("sum: %ld\n", sum);
        sum = sum / (HIGH * net->units) * 4;
//        printf("sum: %ld\n", sum);
        // 次の値を更新
        net->next->value_sg[i] = sigmoid(sum);
        net->next->value[i] = (int)sum;
//        printf("sig(%d) = %d", sum, sigmoid(sum));
    }
}

void back_propagates_one_net(net_t net)
{
    assert(net->prev != NULL);

    for(int i = 0; i < net->prev->units; i++){
        long sum = 0;
        for(int j = 0; j < net->units; j++){
            sum += net->value[j] * net->prev->weight[j][i];
            //            printf("%d * %d\n", net->value[j], net->weight[j][i]);
        }
        // わる
        //        printf("sum: %ld\n", sum);
        sum = sum / (HIGH * net->units) * 4;
        //        printf("sum: %ld\n", sum);
        // 次の値を更新
        net->prev->value[i] = sigmoid(sum);
    }
}

int sigmoid(long input)
{
    // @TODO ちゃんとした sigmoid にする
    return (int)(tanh((double)input / HIGH) * HIGH);
}

int sigmoid_dash(long input)
{
    int s = sigmoid(input);
    return 1 - s * s;
}

void setup_net(net_t* input_layer, net_t* mid_layer, net_t* output_layer)
{
    // 2層のニューラルネットを用意する
    *output_layer = mk_net(OUTPUT_UNITS, OUTPUT_UNITS, NULL);
    *mid_layer = mk_net(MID_UNITS, OUTPUT_UNITS, *output_layer);
    *input_layer = mk_net(PIXELS, MID_UNITS, *mid_layer);
    printf("input_layer_units: %d\n", (*input_layer)->units);
}

void back_propagation(net_t input_layer, net_t mid_layer, net_t output_layer, int label)
{
    int i, j;
    // 誤差逆伝播
    for(i = 0; i < output_layer->units; i++){
        output_layer->value[i] = output_layer->value_sg[i] - (label == i ? HIGH : LOW);
    }

    for(i = 0; i < mid_layer->units; i++){
        int sum = 0;
        for(j = 0; j < output_layer->units; j++){
            sum += output_layer->weight[j][i] * output_layer->value[j];
        }
        mid_layer->value[i] = sum * (1 - mid_layer->value_sg[i] * mid_layer->value_sg[i]);
    }
}

void update_weight(net_t input_layer, net_t mid_layer, net_t output_layer)
{
    int i, j;
    for(i = 0; i < output_layer->units; i++){
        for(j = 0; j < mid_layer->units; j++){
//            printf("weight add: %d * %d = %d\n", output_layer->value[i], mid_layer->value_sg[j], output_layer->value[i] * mid_layer->value_sg[j] / HIGH);
            mid_layer->weight[j][i] -= output_layer->value[i] * mid_layer->value_sg[j] * ETA / HIGH;
        }
    }

    for(i = 0; i < mid_layer->units; i++){
        for(j = 0; j < input_layer->units; j++){
            input_layer->weight[j][i] -= mid_layer->value[i] * input_layer->value_sg[j] * ETA / HIGH;
        }
    }



}

void set_input(net_t input_layer, image_t image)
{
    // 入力設定
    for(int i = 0; i < PIXELS; i++){
        input_layer->value[i] = image->pixels[i] * HIGH / 256;
        input_layer->value_sg[i] = image->pixels[i] * HIGH / 256;
    }
}

int get_result(net_t output_layer)
{
    int max_value = INT_MIN;
    int max_answer = 0;

    for(int i = 0; i < OUTPUT_UNITS; i++){
        if(output_layer->value[i] > max_value){
            max_value = output_layer->value[i];
            max_answer = i;
        }
    }
//    printf("[get_result] answer: %d\n", max_answer);
    return max_answer;
}

void show_net(net_t layer){
    printf("[propagation] ");
    for(int i = 0; i < layer->units; i++){
        printf("%d ", layer->value[i]);
    }
    printf("\n");
}

void simulate_net(net_t input_layer)
{
    while(input_layer->next != NULL){
//        show_net(input_layer);
        propagates_one_net(input_layer);
        input_layer = input_layer->next;
    }
///    show_net(input_layer);
}
