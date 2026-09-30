/*
 * Challenge 19 — realloc 로 줄인 뒤 옛 길이로 접근 (심화: 신호 버퍼 트림)
 *
 * [시나리오]
 *   센서 신호를 담는 Signal 버퍼. 앞부분의 유효 구간만 남기고 나머지를 잘라내
 *   메모리를 절약하는 signal_trim() 을 호출한 뒤, 에너지(제곱합)를 계산한다.
 *
 * [예시 상황]
 *   마이크 / 음성
 *   음성 구간 검출(VAD)이 이 패턴을 사용한다.
 *   44,100Hz로 녹음하면 1초에 samples가 44,100개다.
 *   무음인 앞부분만 남기고 뒤를 자르는 게 signal_trim이고, 소리가 얼마나 큰지(에너지)를 보려면 제곱합을 쓴다.
 *

 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double *samples;
    // size_t  len; len을 쓰는 이유가 없음       
    size_t  cap;      
} Signal;

static void signal_init(Signal *s, size_t n) {
    s->samples = malloc(n * sizeof(double));
    if (!s->samples) { perror("malloc"); exit(1); }
    s->cap = n;
    for (size_t i = 0; i < n; i++) s->samples[i] = (double)(i % 7) - 3.0;
}

static void signal_trim(Signal *s, size_t keep) {

    if (keep > s->cap) return;
    double *temp = s->samples;
    double *p = realloc(s->samples, keep * sizeof(double));
    if (p) s->samples = p;
    else if(keep == 0){ // keep이 0이어서 samples가 댕글링 포인터가 되는 경우 대비 
        s->samples = NULL;
    }
    else s->samples = temp; // realloc이 null을 반환하는 경우 대비
    s->cap = keep;                 
}

static double signal_energy(const Signal *s) {
    double e = 0.0;
    for (size_t i = 0; i < s->cap; i++) {   
        e += s->samples[i] * s->samples[i];
    }
    return e;
}


int main(void) {
    Signal s;
    signal_init(&s, 2000000);       

    signal_trim(&s, 8);             

    double e = signal_energy(&s);   
    
    // printf("energy = %.1f (len=%zu cap=%zu)\n", e, s.len, s.cap);
    printf("energy = %.1f (cap=%zu)\n", e, s.cap); // len 이 의미가 없어서 지웟음 
    free(s.samples);
    return 0;
}
