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
#include <stdint.h> // 수정할 때 SIZE_MAX 로 곱셈 오버플로를 검사하라고 미리 넣어 둔 헤더

typedef struct {
    int width;
    int height;
    int channels;
    int nbytes;              
    unsigned char *px;
} Image;

static Image *image_new(int width, int height, int channels) {
    Image *img = malloc(sizeof *img);
    if (!img) { perror("malloc"); exit(1); }
    img->width = width;
    img->height = height;
    img->channels = channels;

    img->nbytes = width * height * channels;
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
    printf("allocated nbytes(int)=%d for %dx%d x%d\n",
           img->nbytes, img->width, img->height, img->channels);

    image_fill(img, 0xFF);                       

    printf("px[0]=%u\n", img->px[0]);
    free(img->px);
    free(img);
    return 0;
}
