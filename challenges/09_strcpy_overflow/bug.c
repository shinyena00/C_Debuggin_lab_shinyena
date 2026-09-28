/*
 * Challenge 09 — strcpy 힙 오버플로 (심화: join 크기계산 off-by-one)
 *
 * [시나리오]
 *   여러 조각(parts)을 구분자 없이 이어 붙여 하나의 문자열을 만드는 join().
 *   필요한 크기를 먼저 계산(joined_size)해 malloc 한 뒤, 각 조각을 순서대로 복사한다.
 *
 * [기대 동작]
 *   모든 조각을 이어 붙인 결과 길이를 출력하고 정상 종료.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t joined_size(const char *const *parts, int n) {
    size_t total = 1;           
    for (int i = 0; i < n; i++) {        
        total += strlen(parts[i]);
    }
    return total;
}

static char *join(const char *const *parts, int n) {
    size_t need = joined_size(parts, n);
    char *out = malloc(need);
    if (!out) { perror("malloc"); exit(1); }

    size_t off = 0;
    for (int i = 0; i < n; i++) { 
        int w = snprintf(out + off, need-off, "%s", parts[i]);
        if(w<=0 || w>=need-off){
            fprintf(stderr, "buffer overflow 발생");
            abort();
        }
        off += strlen(parts[i]);
    }
    out[off] = '\0';
    return out;
}

int main(void) {
    
    static char body[200000];
    memset(body, 'x', sizeof body - 1);
    body[sizeof body - 1] = '\0';

    const char *parts[] = { "GET ", "/index.html", " HTTP/1.1\r\n\r\n", body };
    int n = (int)(sizeof(parts) / sizeof(parts[0]));

    char *msg = join(parts, n);

    printf("joined length = %zu\n", strlen(msg));
    free(msg);
    return 0;
}
