/*
Compile: make sizeof
Run: ./sizeof
make sizeof && ./sizeof
*/
 
#include "base.h"

int main(void) {
#if 0
    printiln(1);
    test_equal_i(0, 0);
#endif
    
#if 0
    char c = 0x7f; // 7F16 = 011111112 = 12710
    printiln(c);
    c = c + 1;
    printiln(c);
    c = 127;
    printiln(c);
    c = 2 * c;
    printiln(c);
#endif

#if 0
    unsigned char c = 0xff; // FF16 = 111111112 = 25510
    printiln(c);
    c = c + 1;
    printiln (c);
    c = 200;
    printiln (c);
    c = c * 2;
    printiln (c);
#endif
    
    float f = 2e10f + 1;
    printdln(f - 2e10f);

    double d = 2e10 + 1;
    printdln(d - 2e10);


    printiln(sizeof(char));        
    printiln(sizeof(short int));        
    printiln(sizeof(int));        
    printiln(sizeof(long int));        

    printiln(sizeof(float));         
    printiln(sizeof(double));        
    printiln(sizeof(long double));    

    printiln(sizeof(bool));
    
    int fs = 0, ts = 0;
    for (int i = 0; i < 100000; i++) {
        bool b = b_rnd();
        if (b) ts++; else fs++;
    }
    printf("ts = %d, fs = %d\n", ts, fs);

    // 1.5
    // int e = 0; double m = 1.5;
    // 3.14
    int e = 1; double m = 3.1415624 / pow(2.0, e);
    int S = 0, E = (127 + e) & 0xff, M = (int)round((m - 1.0) * 0x800000) & 0x7fffff;
    int x = (S << 31) | (E << 23) | M;
    float *p = (float*)&x;
    printdln(*p);
    
    
    
    short s = 0x1234;    // 16 bits
    char c = 0;        // 8 bits
    printiln(s);        // prints 0x1234
    c = s;
    printiln(c);        // prints 0x34
    
    
    
    return 0;
}
