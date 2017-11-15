/*
Compile: make celsius_to_fahrenheit
Run: ./celsius_to_fahrenheit
make celsius_to_fahrenheit && ./celsius_to_fahrenheit
*/
 
#include "base.h"

/*
Design a function that converts degrees Celsius to degrees Fahrenheit.
*/

// int -> int
// Takes a temperature value in degrees Celsius and
// returns the corresponding value in degrees Fahrenheit.
int celsius_to_fahrenheit(int celsius);

int celsius_to_fahrenheit(int celsius) {
    return celsius * 9 / 5 + 32;
}

int main(void) {
    test_equal_i(celsius_to_fahrenheit(0), 32); // given 0, expect 32
    test_equal_i(celsius_to_fahrenheit(10), 50); // given 10, expect 50
    test_equal_i(celsius_to_fahrenheit(-5), 23); // given -5, expect 23
    test_equal_i(celsius_to_fahrenheit(100), 212); // given 100, expect 212
    return 0;
}



#if 0
#include "base.h"

/*
1. Problem statement (given)
"Design a function that converts degrees Celsius to degrees Fahrenheit."

2. Write a data definition
- temperature is a number (Celsius or Fahrenheit)
- int represents degrees Celsius (input)
- int represents degrees Fahrenheit (output)

3. Write a function header
int -> int
*/
int celsius_to_fahrenheit(int celsius);

/*
4. Write a purpose statement for the function
Takes a temperature value in degrees Celsius and returns the corresponding value in degrees Fahrenheit.

5. Give examples and expected results
Formula: °F = °C * 9 / 5 + 32
Given: 0, expect: 32
Given: 10, expect: 50
Given: -5, expect: 23
Given: 100, expect: 212

6. Implement the function body
*/
int celsius_to_fahrenheit(int celsius) {
    return celsius * 9 / 5 + 32;
}

/*
7. Test the function
*/
void celsius_to_fahrenheit_test(void) {
    test_equal_i(celsius_to_fahrenheit(0), 32);
    test_equal_i(celsius_to_fahrenheit(10), 50);
    test_equal_i(celsius_to_fahrenheit(-5), 23);
    test_equal_i(celsius_to_fahrenheit(100), 212);
}

/*
8. Review and revise the function
no revision required
*/

int main(void) {
//    celsius_to_fahrenheit_test();
    test_equal_i(celsius_to_fahrenheit(0), 32);
    test_equal_i(celsius_to_fahrenheit(10), 50);
    test_equal_i(celsius_to_fahrenheit(-5), 23);
    test_equal_i(celsius_to_fahrenheit(100), 212);
    return 0;
}
#endif
