/*
 * Challenge 11 — Global Buffer Overflow (심화: 전역 아레나 bump 할당기)
 *
 * [시나리오]
 *   고정 크기 전역 버퍼(arena, .bss)를 "bump 포인터" 방식으로 나눠 쓰는 초간단
 *   할당기. 문자열 인터너(intern)가 들어온 문자열을 아레나에 복사해 보관한다.
 *
 * [기대 동작]
 *   문자열을 차례로 아레나에 인터닝하고, 마지막 문자열과 전체 길이 합을 출력한 뒤 정상 종료.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_SIZE 4096
static unsigned char arena[ARENA_SIZE];
static size_t arena_off = 0;

static void *arena_alloc(size_t n) {
    void *p = &arena[arena_off];
    arena_off += n;
    if(arena_off > ARENA_SIZE){
        return NULL; // 처음에 만들었던범위를 넘으면 NULL을 반환 
    }
    return p;
}

static char *intern(const char *s) {
    size_t n = strlen(s) + 1;
    char *dst = arena_alloc(n);
    if(!dst){
        return NULL;
    }
    memcpy(dst, s, n); 
    return dst;
}

int main(void) {
    
    const char *words[] = {
        "insert", "delete", "search", "traverse", "balance",
        "rotate", "rehash", "compact", "serialize", "checkpoint",
    };
    int nwords = (int)(sizeof(words) / sizeof(words[0]));

    char *last = NULL;
    long total = 0;
    for (int i = 0; i < 100000; i++) {
        char buf[32];
        snprintf(buf, sizeof buf, "%s-%d", words[i % nwords], i);
        last = intern(buf);
        if(!last){
            fprintf(stderr, "global overflow 발생"); // 에러 발생 알림 
            abort();
        }                 
        total += (long)strlen(last);
    }

    printf("interned, last=%s total_len=%ld\n", last, total);
    return 0;
}
