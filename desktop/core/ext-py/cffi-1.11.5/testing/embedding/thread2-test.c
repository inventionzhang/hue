#include <stdio.h>
#include <assert.h>
#include "thread-test.h"

extern int add1(int, int);
extern int add2(int, int, int);

static sem_t done;


static void *start_routine_1(void *arg)
{
    int x, status;
    x = add1(40, 2);
    assert(x == 42);

    status = sem_post(&done);
    assert(status == 0);

    return arg;
}

static void *start_routine_2(void *arg)
{
    int x, status;
#ifdef T2TEST_AGAIN_ADD1
    add1(-1, -1);
#endif
    x = add2(1000, 200, 30);
    assert(x == 1230);

    status = sem_post(&done);
    assert(status == 0);

    return arg;
}

int main(void)
{
    pthread_t th;
    int i, status = sem_init(&done, 0, 0);
    assert(status == 0);

    printf("starting\n");
    fflush(stdout);
    status = pthread_create(&th, NULL, start_routine_1, NULL);
    assert(status == 0);
    status = pthread_create(&th, NULL, start_routine_2, NULL);
    assert(status == 0);

    for (i = 0; i < 2; i++) {
        status = sem_wait(&done);
        assert(status == 0);
    }
    printf("done\n");
    return 0;
}
