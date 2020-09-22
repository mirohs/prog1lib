/*
Compile: make tax
Run: ./tax
make tax && ./tax
*/
 
#include "base.h"

/*
A fictitious country has decided to introduce a three-stage sales tax. Cheap
items below 1 k€ are not taxed. Luxury goods of more than 10 k€ are taxed at
10%. Items in between are taxed at a rate of 5%. Give a data definition and
define a function that computes the amount of tax for a given item price.
*/

enum TaxStage {
    NO_TAX,
    LOW_TAX,
    HIGH_TAX
};

typedef int Euro; // int represents Euro
/*
Note: This typedef may be omitted and int may be used directly.
As defined above, Euro is just a synonym (another name for) int.
The function signature then becomes:
// int -> int
int sales_tax(int price);
*/

const Euro LOW_TAX_BOUNDARY  =  1000; // interpret.: price in Euro
const Euro HIGH_TAX_BOUNDARY = 10000; // interpret.: price in Euro
const double LOW_TAX_RATE = 0.05;
const double HIGH_TAX_RATE = 0.10;

// Euro -> Euro
// Return the amount of tax for the given price.
Euro sales_tax(Euro price);

// helper function for rounding to whole Euros
int round_to_int(double d);

static void sales_tax_test() {
    // test_equal_i(sales_tax(-10), 0);
    test_equal_i(sales_tax(0), 0);
    test_equal_i(sales_tax(537), 0);
    test_equal_i(sales_tax(1000),
                        round_to_int(1000 * 0.05));
    test_equal_i(sales_tax(1282),
                        round_to_int(1282 * 0.05));
    test_equal_i(sales_tax(10000),
                        round_to_int(10000 * 0.10));
    test_equal_i(sales_tax(12017),
                        round_to_int(12017 * 0.10));
}

// Round the given double value to whole Euros.
int round_to_int(double d) {
    return round(d); // round from math.h
}

// Return the amount of tax for the given price.
Euro sales_tax(Euro price) {
    require("not negative", price >= 0);
    /*if (price < 0) {
        printsln("sales_tax, error: negative price");
        exit(1);
    } else*/ 
    if (price < LOW_TAX_BOUNDARY) { // NO_TAX
        return 0;
    } else if (price < HIGH_TAX_BOUNDARY) { // LOW_TAX
        return round_to_int(LOW_TAX_RATE * price);
    } else { // HIGH_TAX
        return round_to_int(HIGH_TAX_RATE * price);
    }
}

int main(void) {
    sales_tax_test();
    return 0;
}



/*
1. Problem statement (given)
A fictitious country has decided to introduce a three-stage sales tax. Cheap
items below 1 k€ are not taxed. Luxury goods of more than 10 k€ are taxed at
10%. Items in between are taxed at a rate of 5%. Give a data definition and
define a function that computes the amount of tax for a given item price.

2. Write a data definition
enum TaxStage {
    NO_TAX,
    LOW_TAX,
    HIGH_TAX
};

typedef int Euro; // int represents Euro

const Euro LOW_TAX_BOUNDARY  =  1000; // interpret.: price in Euro
const Euro HIGH_TAX_BOUNDARY = 10000; // interpret.: price in Euro
const double LOW_TAX_RATE = 0.05;
const double HIGH_TAX_RATE = 0.10;

3. Write a function header
// Euro -> Euro
Euro sales_tax(Euro price);

4. Write a purpose statement for the function
Return the amount of tax for the given price.

5. Give examples and expected results
For a price of 0 €  expect a sales tax of 0 €.
For a price of 537 € expect a sales tax of 0 €.
For a price of 1000 € expect a sales tax of 50 €.
For a price of 1282 € expect a sales tax of 64 €.
For a price of 10000 € expect a sales tax of 1000 €.
For a price of 12017 € expect a sales tax of 1202 €.


6. Implement the function body
Euro sales_tax(Euro price) {
    if (price < 0) {
        printsln("sales_tax, error: negative price");
        exit(1);
    } else if (price < LOW_TAX_BOUNDARY) { // NO_TAX
        return 0;
    } else if (price < HIGH_TAX_BOUNDARY) { // LOW_TAX
        return round_to_int(LOW_TAX_RATE * price);
    } else { // HIGH_TAX
        return round_to_int(HIGH_TAX_RATE * price);
    }
}

7. Test the function
static void sales_tax_test() {
    test_equal_i(sales_tax(0), 0);
    test_equal_i(sales_tax(537), 0);
    test_equal_i(sales_tax(1000),
                        round_to_int(1000 * 0.05));
    test_equal_i(sales_tax(1282),
                        round_to_int(1282 * 0.05));
    test_equal_i(sales_tax(10000),
                        round_to_int(10000 * 0.10));
    test_equal_i(sales_tax(12017),
                        round_to_int(12017 * 0.10));
}

// Round the given double value to whole Euros.
int round_to_int(double d) {
    return round(d); // round from math.h
}

int main(void) {
    sales_tax_test();
    return 0;
}

8. Review and revise the function
no revision required
*/
