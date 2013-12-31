// net.h
#pragma once
#define HIGH 10000
#define LOW -10000

#define MID_UNITS 100
#define OUTPUT_UNITS 10

// ニューラルネットの1段

struct net {
    int units; // この段のユニットの数
    int* value; // この段の value
    int* value_sg; // この段の value (sigmoid関数通した後)
    int** weight; // この段の weight
    struct net* next; // 次の段
    struct net* prev; // 前の段
};

typedef struct net* net_t;


net_t mk_net(int units, int next_units, net_t next);
void propagates_one_net(net_t net);
int sigmoid(long input);
void setup_net(net_t* input_layer, net_t* mid_layer, net_t* output_layer);
void set_input(net_t input_layer, image_t image);
int get_result(net_t output_layer);
void simulate_net(net_t input_layer);

void back_propagation(net_t input_layer, net_t mid_layer, net_t output_layer, int label);
void update_weight(net_t input_layer, net_t mid_layer, net_t output_layer);

