/*
make test_all && ./test_all
*/

#include "base.h"

void s_test_all(void);

void a_test_all(void);
void ia_test_all(void);
void da_test_all(void);
void sa_test_all(void);
void pa_test_all(void);
void ba_test_all(void);

void l_test_all(void);
void il_test_all(void);
void dl_test_all(void);
void sl_test_all(void);
void pl_test_all(void);

int main(void) {
    report_memory_leaks(true);

    s_test_all();

#if 1
    a_test_all();
    ia_test_all();
    da_test_all();
    sa_test_all();
    pa_test_all();
    ba_test_all();
#endif

#if 1
    l_test_all();
    il_test_all();
    dl_test_all();
    sl_test_all();
    pl_test_all();
#endif

#if 0
    List il = il_create();
    il_append(il, 123);
    int i = il_get(il, -1);
    printiln(i);
    l_free(il);
#endif

    return 0;
}

