// boltzmann.c
#include "boltzmann.h"
#include "net.h"
#define TRAIN_MODE 0

void show_images_test(image_t* images, int num_image){
	int i;
	for(i = 100; i < 200; i++){
		show_image(images[i]);
	}
}

int main(){
	int magic_num, num_image, num_label, num_train_image, num_train_label, i, res;
	FILE *image_fp, *label_fp, *image_train_fp, *label_train_fp;
	image_t *images, *train_images; // 配列

		label_train_fp = fopen("dataset/train-labels-idx1-ubyte", "rb");
		image_train_fp = fopen("dataset/train-images-idx3-ubyte", "rb");
		label_fp = fopen("dataset/t10k-labels-idx1-ubyte", "rb");
		image_fp = fopen("dataset/t10k-images-idx3-ubyte", "rb");

	// check if dataset are successfully opened
	assert(label_train_fp != NULL);
	assert(image_train_fp != NULL);
	assert(label_fp != NULL);
	assert(image_fp != NULL);

	magic_num = 0;

	// check magic number
	res = fread(&magic_num, 4, 1, label_train_fp);
	magic_num = ntohl(magic_num);
	assert(res == 1);
	printf("label magic_num: 0x%08x (expects 0x00000801)\n", magic_num);
	assert(magic_num == 0x00000801);

	res = fread(&magic_num, 4, 1, image_train_fp);
	magic_num = ntohl(magic_num);
	assert(res == 1);
	printf("image magic_num: 0x%08x (expects 0x00000803)\n", magic_num);
	assert(magic_num == 0x00000803);

    res = fread(&magic_num, 4, 1, label_fp);
	magic_num = ntohl(magic_num);
	assert(res == 1);
	printf("label magic_num: 0x%08x (expects 0x00000801)\n", magic_num);
	assert(magic_num == 0x00000801);

	res = fread(&magic_num, 4, 1, image_fp);
	magic_num = ntohl(magic_num);
	assert(res == 1);
	printf("image magic_num: 0x%08x (expects 0x00000803)\n", magic_num);
	assert(magic_num == 0x00000803);

	printf("preparation finished\n");

	// check number of images
	fread(&num_label, 4, 1, label_fp);
	fread(&num_image, 4, 1, image_fp);
	num_label = ntohl(num_label);
	num_image = ntohl(num_image);

	fread(&num_train_label, 4, 1, label_train_fp);
	fread(&num_train_image, 4, 1, image_train_fp);
	num_train_label = ntohl(num_train_label);
	num_train_image = ntohl(num_train_image);

	assert(num_image == num_label);
	assert(num_image > 0);
	assert(num_train_image == num_train_label);
	assert(num_train_label > 0);

	// check width, height
	int width, height;
	fread(&width, 4, 1, image_fp);
	fread(&height, 4, 1, image_fp);

	fread(&width, 4, 1, image_train_fp);
	fread(&height, 4, 1, image_train_fp);

    assert(ntohl(width) == WIDTH);
	assert(ntohl(height) == HEIGHT);

	train_images = (image_t*) malloc(sizeof(image_t) * num_train_image);
	images = (image_t*) malloc(sizeof(image_t) * num_image);

	printf("processing training images:         ");
	// read image data
	for(i = 0; i < num_train_image; i++){
        //		printf("\b\b\b\b\b\b\b\b%8d", i+1);
		train_images[i] = mk_image_from_file(image_train_fp, label_train_fp);
	}
	printf("\n");

	printf("processing test images:         ");
	// read image data
	for(i = 0; i < num_image; i++){
        //		printf("\b\b\b\b\b\b\b\b%8d", i+1);
		images[i] = mk_image_from_file(image_fp, label_fp);
	}
	printf("\n");

	// finished! show some images for test
	show_images_test(images, num_image);
	show_images_test(train_images, num_train_image);

	// set up neural net
	net_t input_layer = NULL;
    net_t mid_layer = NULL;
    net_t output_layer = NULL;
	setup_net(&input_layer, &mid_layer, &output_layer);

    printf("MID_LAYER: %d\n", MID_UNITS);
    printf("ALPHA: %f\n", ETA);

    for (int k = 0; k < 10; k++) {
        int incorrect_num = 0;
//        printf("start\n");
        for(int i = 0; i < num_train_image; i++){
            // 画像ごとに training
            set_input(input_layer, train_images[i]);
            simulate_net(input_layer);

            int result = get_result(output_layer);
            if(result != train_images[i]->label){
                //			printf("[incorrect] result: %d, expected: %d\n", result, images[i]->label);
            }
            back_propagation(input_layer, mid_layer, output_layer, train_images[i]->label);
            update_weight(input_layer, mid_layer, output_layer);
        }

        for(int i = 0; i < num_image; i++){
            // 画像ごとに試験
            set_input(input_layer, images[i]);
            simulate_net(input_layer);

            int result = get_result(output_layer);
            if(result != images[i]->label){
                incorrect_num ++;
            }
        }

        printf("iteration: %d, incorrect rate: %f\n", k, (double)incorrect_num/num_image);

    }

	return 0;
}