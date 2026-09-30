/*
 * Challenge 13 — Linked List Use-After-Free (심화: 잡 큐 필터링)
 *
 * [시나리오]
 *   우선순위가 있는 잡(Job)들을 단일 연결 리스트 큐로 관리한다. 스케줄러가
 *   "임계값 미만 우선순위"의 잡을 큐에서 제거(취소)하며, 제거된 잡의 id 를 감사
 *   로그(동적 배열)에 기록한다.
 *
 * [기대 동작]
 *   저우선순위 잡을 모두 제거하고, 취소된 개수와 남은 개수를 출력한 뒤 정상 종료.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Job {
    int id;
    int priority;
    struct Job *next;
} Job;

typedef struct {
    int   *ids;
    size_t len, cap;
} Audit;

static void audit_add(Audit *a, int id) {
    if (a->len == a->cap) {
        a->cap = a->cap ? a->cap * 2 : 16;
        int *p = realloc(a->ids, a->cap * sizeof(int));   
        if (!p) { perror("realloc"); exit(1); }
        a->ids = p;
    }
    a->ids[a->len++] = id;
}

static Job *push_job(Job *head, int id, int priority) {
    Job *n = malloc(sizeof *n);
    if (!n) { perror("malloc"); exit(1); }
    n->id = id;
    n->priority = priority;
    n->next = head;
    return n;
}


static void job_release(Job *j) {
    free(j);
}

static Job *filter_jobs(Job *head, int threshold, Audit *audit) {
    Job *keep = NULL, *keep_tail = NULL;
    Job *cur = head;

    while (cur != NULL) {
        if (cur->priority < threshold) {
            audit_add(audit, cur->id);   
            // job_release(cur);            
            // cur = cur->next;
            Job *temp = cur; //temp에 free해야할 cur를 저장 
            cur = cur->next; // cur는 옮겨줌
            job_release(temp); // free는 temp 로 바꿈 
        } else {
            Job *nx = cur->next;
            cur->next = NULL;
            if (keep_tail) keep_tail->next = cur; else keep = cur;
            keep_tail = cur;
            cur = nx;
        }
    }
    return keep;
}

int main(void) {
    Job *head = NULL;
    for (int i = 1; i <= 4000; i++)
        head = push_job(head, i, (i * 7) % 10);   

    Audit audit = {0};
    head = filter_jobs(head, 5, &audit);           

    int remaining = 0;
    for (Job *c = head; c; c = c->next) remaining++;
    printf("cancelled=%zu remaining=%d\n", audit.len, remaining);

    free(audit.ids);
    for (Job *c = head; c; ) { Job *nx = c->next; free(c); c = nx; }
    return 0;
}
