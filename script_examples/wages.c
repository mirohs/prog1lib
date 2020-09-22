/*
Compile: make wages
Run: ./wages
make wages && ./wages
*/
 
#include "base.h"

/*
Design a function that computes weekly wages with overtime from hours worked.
The hourly rate is €10/hour. Regular working time is 40 hours/week. Overtime
is paid 150% of the normal rate of pay.
*/

typedef int Hours; // represents hours worked
typedef int Cents; // represents wage in cents
/*
Note: This typedef may be omitted and int may be used directly.
As defined above, Hours and Cents are just synonyms (other names for) int.
The function signature then becomes:
// int -> int
int hours_to_wages(int hours);
*/

// Hours -> Cents
// Compute the wage in cents given the number of hours worked.
Cents hours_to_wages(Hours hours);

static void hours_to_wages_test() {
    // test_equal_i(hours_to_wages(-1), 0);
    // test_equal_i(hours_to_wages(7 * 24 + 1), 0);
    test_equal_i(hours_to_wages(0), 0);
    test_equal_i(hours_to_wages(20), 20 * 1000);
    test_equal_i(hours_to_wages(39), 39 * 1000);
    test_equal_i(hours_to_wages(40), 40 * 1000);
    test_equal_i(hours_to_wages(41), 40 * 1000 + 1 * 1500);
    test_equal_i(hours_to_wages(45), 40 * 1000 + 5 * 1500);
}

Cents hours_to_wages(Hours hours) {
    require("not negative", hours >= 0);
    require("not more than one week", hours <= 24 * 7);
    if (hours <= 40) {
        return hours * 1000;
    } else {
        return 40 * 1000 + (hours - 40) * 1500;
    }
}

int main(void) {
    hours_to_wages_test();
    return 0;
}



/*
1. Problem statement (given)
Design a function that computes weekly wages with overtime from hours worked. 
The hourly rate is €10/hour. Regular working time is 40 hours/week. Overtime 
is paid 150% of the normal rate of pay.

2. Write a data definition
typedef int Hours; // represents hours worked
typedef int Cents; // represents wage in cents

3. Write a function header
// Hours -> Cents
Cents hours_to_wages(Hours hours);

4. Write a purpose statement for the function
Compute the wage in cents given the number of hours worked.

5. Give examples and expected results
For 0 hours worked, expect 0 cents.
For 20 hours worked, expect 20 * 1000 cents.
For 39 hours worked, expect 39 * 1000 cents.
For 40 hours worked, expect 40 * 1000 cents.
For 41 hours worked, expect 40 * 1000 + 1 * 1500 cents.
For 45 hours worked, expect 40 * 1000 + 5 * 1500 cents.

6. Implement the function body
Cents hours_to_wages(Hours hours) {
    if (hours <= 40) {
        return hours * 1000;
    } else {
        return 40 * 1000 + (hours - 40) * 1500;
    }
}

7. Test the function
void hours_to_wages_test() {
    test_equal_i(hours_to_wages(0), 0);
    test_equal_i(hours_to_wages(20), 20 * 1000);
    test_equal_i(hours_to_wages(39), 39 * 1000);
    test_equal_i(hours_to_wages(40), 40 * 1000);
    test_equal_i(hours_to_wages(41), 40 * 1000 + 1 * 1500);
    test_equal_i(hours_to_wages(45), 40 * 1000 + 5 * 1500);
}

int main(void) {
    hours_to_wages_test();
    return 0;
}

8. Review and revise the function
no revision required
*/
