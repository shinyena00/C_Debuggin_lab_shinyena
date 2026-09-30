/*
 * Challenge 20 — 동적 배열 성장 후 stale 원소 포인터 (심화: 히스토그램 hot 포인터)
 *
 * [시나리오]
 *   키별 빈도를 세는 히스토그램. 버킷들을 동적 배열(Histogram.data)에 담는다.
 *   자주 갱신되는 버킷 하나의 "주소"를 hot 포인터로 캐시해 두고 빠르게 증가시킨다
 *   (인덱스 재조회 없이 hot->count 만 바로 갱신하는 흔한 최적화).
 *
 * [기대 동작]
 *   스트림의 키들을 모두 반영하고, hot 버킷을 갱신한 값과 전체 합을 출력.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int  key;
    long count;
} Bucket;

typedef struct {
    Bucket *data;
    size_t  len, cap;
} Histogram;

static void hist_grow(Histogram *h) {
    h->cap = h->cap ? h->cap * 2 : 16;
    Bucket *p = realloc(h->data, h->cap * sizeof(Bucket));  
    if (!p) { perror("realloc"); free(h->data); exit(1); }
    h->data = p;
}

static Bucket *hist_add(Histogram *h, int key) {
    if (h->len == h->cap) hist_grow(h);
    Bucket *b = &h->data[h->len++];
    b->key = key;
    b->count = 0;
    return b;
}

static long hist_total(const Histogram *h) {
    long t = 0;
    for (size_t i = 0; i < h->len; i++) t += h->data[i].count;
    return t;
}

int main(void) {
    Histogram h = { .data = NULL, .len = 0, .cap = 0 };

    for (int k = 0; k < 200000; k++) hist_add(&h, k);

    Bucket *hot = &h.data[100000];
    hot->count = 1;

    for (int k = 200000; k < 600000; k++) hist_add(&h, k);

    hot->count += 1000;

    printf("hot=%ld total=%ld len=%zu\n", hot->count, hist_total(&h), h.len);
    free(h.data);
    return 0;
}
