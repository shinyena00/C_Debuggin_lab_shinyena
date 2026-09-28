/*
 * Challenge 12 — free() 대상이 힙이 아님 (심화: CSV 필드 내부 포인터)
 *
 * [시나리오]
 *   한 줄짜리 CSV 를 파싱한다. 원본 줄을 힙에 복사(strdup)한 뒤 strtok 으로 쉼표를
 *   '\0' 로 바꿔가며 각 필드의 시작 주소를 Row.fields[] 에 담는다.
 *
 * [기대 동작]
 *   필드들을 출력하고, 할당한 버퍼를 누수 없이 해제.
 *
*/
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELDS 8
typedef struct {
    char *base; 
    char *fields[MAX_FIELDS]; 
    int   n;
} Row;

static void parse_row(Row *r, const char *csv) {
    r->base = strdup(csv);       
    if (!r->base) { perror("strdup"); exit(1); }
    r->n = 0;

    for (char *tok = strtok(r->base, ","); tok && r->n < MAX_FIELDS;
         tok = strtok(NULL, ",")) {
        r->fields[r->n++] = tok;
    }
}

static void row_print(const Row *r) {
    printf("%d fields:", r->n);
    for (int i = 0; i < r->n; i++) printf(" [%s]", r->fields[i]);
    printf("\n");
}

static void row_free(Row *r) {
    for (int i = 0; i < r->n; i++) {
        free(r->fields[i]);       
    }
    r->n = 0;
}

int main(void) {
    Row r;
    parse_row(&r, "id,name,dept,salary");
    row_print(&r);

    row_free(&r);                 
    printf("done\n");
    return 0;
}
