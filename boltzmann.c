// boltzmann.c
#include "boltzmann.h"
#define TRAIN_MODE 1

void show_images_test(image_t* images, int num_image){
	int i;
	for(i = 0; i < 10; i++){
		show_image(images[i]);
	}
}

int main(){
	long magic_num, num_image, num_label, i; 
	FILE* image_fp;
	FILE* label_fp;
	image_t* images; // 配列

	if(TRAIN_MODE){
		label_fp = fopen("dataset/train-labels-idx1-ubyte", "rb");
		image_fp = fopen("dataset/train-images-idx3-ubyte", "rb");

	}else{
		label_fp = fopen("dataset/t10k-labels-idx1-ubyte", "rb");
		image_fp = fopen("dataset/t10k-images-idx3-ubyte", "rb");
	}

	// check if dataset are successfully opened
	assert(label_fp != NULL);
	assert(image_fp != NULL);

	// check magic number
	fread(&magic_num, 2, 1, label_fp);
	assert(magic_num == 0x00000801);
	fread(&magic_num, 2, 1, image_fp);
	assert(magic_num == 0x00000803);

	printf("preparation finished\n");

	// check number of images
	fread(&num_label, 2, 1, label_fp);
	fread(&num_image, 2, 1, image_fp);
	assert(num_image == num_label);
	assert(num_image > 0);

	images = (image_t*) malloc(sizeof(image_t) * num_image);


	// read image data
	for(i = 0; i < num_image; i++){
		images[i] = mk_image_from_file(image_fp, label_fp);
	}

	// finished! show some images for test
	show_images_test(images, num_image);

	// set up neural net


	return 0;
}