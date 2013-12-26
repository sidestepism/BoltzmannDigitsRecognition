// image.c 

typedef struct image {
	unsigned short pixels[PIXELS];
	unsigned short label;
} * image_t;

image_t mk_image(){
	image_t image = (image_t) malloc(sizeof(struct image));
	return image;
}

image_t mk_image_from_file(FILE* image_fp, FILE* label_fp){
	int i;
	image_t image = mk_image();
	fread(image->pixels, 1, PIXELS, image_fp);
	fread(&image->label, 1, 1, label_fp);
	return image;
}

void show_image(image_t image){
	// 1ピクセルを文字で表現する
	char* pixel_chars = " .-+=*$#";
	int i, j;
	for(i = 0; i < HEIGHT; ++i){
		for (j = 0; j < WIDTH; ++j)
		{
			// pixel_chars = 8 bytes
			// max of pixel data = 256
			int p = image->pixels[i*WIDTH+j] / (256 / 8);
			putchar(pixel_chars[p]);
		}
		putchar('\n');
	}
	printf("----------------------------\n");
}
