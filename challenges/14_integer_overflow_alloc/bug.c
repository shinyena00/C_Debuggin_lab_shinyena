/*
 * Challenge 14 — Integer Overflow → 과소할당 → 오버플로 (심화: 이미지 버퍼)
 *
 * [시나리오]
 *   RGBA 이미지 버퍼를 만든다. 픽셀 바이트 수 = width * height * channels 로 계산해
 *   할당하고, 전 픽셀을 초기값으로 채운다.
 *
 * [기대 동작]
 *   이미지 버퍼를 할당·초기화하고 몇몇 픽셀을 읽어 확인한 뒤 정상 종료.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int width;
    int height;
    int channels;
    size_t nbytes;  // 이것도 바꿈             
    unsigned char *px;
} Image;

static Image *image_new(int width, int height, int channels) {
    Image *img = malloc(sizeof *img);
    if (!img) { perror("malloc"); exit(1); }
    img->width = width;
    img->height = height;
    img->channels = channels;

    if ((size_t) width > SIZE_MAX / height || (size_t)width * height > SIZE_MAX / channels){ // 오버플로우가 발생하면 바로 return 하게 만듬 
        fprintf(stderr, "integer overflow 발생");
        //abort(); 바로 abrot내면 좋은데 일단 컴파일 되게 만듬
        return NULL;
    }
    img->nbytes = (size_t)width * height * channels;// 밑이랑 비트 수 맞춤 
    img->px = malloc((size_t)img->nbytes);     
    if (!img->px) { perror("malloc px"); exit(1); }
    return img;
}

static void image_fill(Image *img, unsigned char value) {

    size_t total = (size_t)img->width * (size_t)img->height * (size_t)img->channels;

    for (size_t i = 0; i < total; i++) {
        img->px[i] = value;                     
    }
}

int main(void) {

    Image *img = image_new(65536, 65536, 4);
    if(!img){
        return 1; // 문제 생겻을 때는 1로 반환 
    }
    printf("allocated nbytes(int)=%zu for %dx%d x%d\n", // 이것도 zu로 변경함 
           img->nbytes, img->width, img->height, img->channels);

    image_fill(img, 0xFF);                       

    printf("px[0]=%u\n", img->px[0]);
    free(img->px);
    free(img);
    return 0;
}
