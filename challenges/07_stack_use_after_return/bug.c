/*
 * Challenge 07 — Stack Use After Return (심화: 지역 배열 주소가 탈출)
 *
 * [시나리오]
 *   문자열을 줄 단위로 쪼개, 각 줄의 시작 주소들을 담은 "뷰(LineView)"를 만든다.
 *   split_lines() 가 만든 뷰를 호출자가 받아서 출력한다.

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 8
typedef struct {
    char *lines[MAX_LINES];  
    int    count;
} LineView;

static void view_set(LineView *out, int n) {
    out->count = n;
}

static void split_lines(LineView *out, char *text) {          
    int n = 0;

    for (char *ln = strtok(text, "\n"); ln && n < MAX_LINES; ln = strtok(NULL, "\n"))
        out->lines[n++] = ln;

    view_set(out, n);      

}


static void warm_stack(void) {
    char *scratch[MAX_LINES];
    for (int i = 0; i < MAX_LINES; i++)
        scratch[i] = (char *)0x4141414141414141ULL; 
    __asm__ volatile("" :: "r"(scratch) : "memory"); 
}

int main(void) {
    char text[] = "alpha\nbeta\ngamma";

    LineView v;
    split_lines(&v, text);               
    warm_stack();                        

    long checksum = 0;
    for (int i = 0; i < v.count; i++)
        checksum += (unsigned char)v.lines[i][0];

    printf("lines = %d, checksum = %ld\n", v.count, checksum);
    return 0;
}
