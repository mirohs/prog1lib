/*
Compile: make move_train
Run: ./move_train
make move_train && ./move_train
*/
 
#include "base.h"

/*
A critical section of a railway track has a length of 10 km. Trains pass
through the cricital sections in both directions. At most one train is allowed
on this critical section at any one time. A control system is to be
implemented that provides the position of a train in that section (if there is
one) or an indication that there is no train in that section. Define a data
definition for this information. Define a function takes this data definition
and advances the train's position by a given amount (in km). This may result
in the train leaving the critical section.
*/

enum TrainTag {
    T_ABSENT,
    T_PRESENT
};
 
struct Train {
    enum TrainTag tag;
    // the distance from the start of the critical section (in km)
    double position;
};
 
struct Train make_train_none() { // constructor
    struct Train t = { T_ABSENT, 0.0 };
    return t;
}
 
struct Train make_train_at(double position) { // constructor
    struct Train t = { T_PRESENT, position };
    return t;
}

const double START_POSITION = 0.0;
const double END_POSITION = 10.0;

// struct Train, double -> struct Train
// Advance the train by the given amount. Consider the case
// that the train enters or leaves the critical section.
struct Train move_train(struct Train train, double amount);

// helper function for comparing trains
bool test_equal_train(int line, struct Train t, struct Train s) {
    bool tags_equal = base_test_equal_i(__FILE__, line, 
                                           t.tag, s.tag);
    bool positions_within = base_test_within_d(__FILE__, line,
                              t.position, s.position, EPSILON);
    return tags_equal && positions_within;
}

static void move_train_test() {
    // absent trains, moving an absent train has no effect
    struct Train actual = make_train_none(); // absent train
    actual = move_train(actual, 3.0); // move absent train by 3.0 km
    struct Train expected = make_train_none(); // absent train
    test_equal_train(__LINE__, actual, expected);
    // same as above, but as nested calls
    test_equal_train(__LINE__, move_train(make_train_none(), 3.0),
                                 make_train_none());
 
    // borders
    test_equal_train(__LINE__, move_train(make_train_at(0.0), 0.0),
                                 make_train_at(0.0));
    test_equal_train(__LINE__, move_train(make_train_at(10.0), 0.0),
                                 make_train_at(10.0));
    test_equal_train(__LINE__, move_train(make_train_at(1.0), -1.0),
                                 make_train_at(0.0));
    test_equal_train(__LINE__, move_train(make_train_at(9.0), 1.0),
                                 make_train_at(10.0));
 
    // interior (both before and after advance)
    test_equal_train(__LINE__, move_train(make_train_at(1.0), 0.0),
                                 make_train_at(1.0));
    test_equal_train(__LINE__, move_train(make_train_at(5.5), 1.5),
                                 make_train_at(7.0));
    test_equal_train(__LINE__, move_train(make_train_at(4.5), -1.0),
                                 make_train_at(3.5));
 
    // leaving the section
    test_equal_train(__LINE__, move_train(make_train_at(9.0), 1.1),
                                 make_train_none());
    test_equal_train(__LINE__, move_train(make_train_at(1.0), -1.1),
                                 make_train_none());
 
    // entering the section
    test_equal_train(__LINE__, move_train(make_train_at(-0.1), 0.1),
                                 make_train_at(0.0));
    test_equal_train(__LINE__, move_train(make_train_at(10.1), -0.1),
                                 make_train_at(10.0));
}

// Advance the train by the given amount. Consider the case
// that the train enters or leaves the critical section.
struct Train move_train0(struct Train train, double amount) {
    if (train.tag == T_ABSENT) {
        return make_train_none();
    } else if (train.tag == T_PRESENT) {
        double new_position = train.position + amount;
        if (new_position < 0.0 || new_position > 10.0) {
            return make_train_none();
        } else {
            return make_train_at(new_position);
        }
    }
    return train;
}

// Advance the train by the given amount. Consider the case
// that the train enters or leaves the critical section.
struct Train move_train(struct Train train, double amount) {
    double new_position = train.position + amount;
    switch (train.tag) {
        case T_ABSENT:
            return make_train_none();
        case T_PRESENT:
            if (new_position < START_POSITION || 
                new_position > END_POSITION) 
            {
                return make_train_none();
            } else {
                return make_train_at(new_position);
            }
    }
    return train;
}

int main(void) {
    move_train_test();
    return 0;
}



/*
1. Problem statement (given)
A critical section of a railway track has a length of 10 km. Trains pass
through the cricital sections in both directions. At most one train is allowed
on this critical section at any one time. A control system is to be
implemented that provides the position of a train in that section (if there is
one) or an indication that there is no train in that section. Define a data
definition for this information. Define a function takes this data definition
and advances the train's position by a given amount (in km). This may result
in the train leaving the critical section.

2. Write a data definition
enum TrainTag {
    T_ABSENT,
    T_PRESENT
};
 
struct Train {
    enum TrainTag tag;
    // the distance from the start of the critical section (in km)
    double position;
};
 
struct Train make_train_none() { // constructor
    struct Train t = { T_ABSENT, 0.0 };
    return t;
}
 
struct Train make_train_at(double position) { // constructor
    struct Train t = { T_PRESENT, position };
    return t;
}

const double START_POSITION = 0.0;
const double END_POSITION = 10.0;

3. Write a function header
// struct Train, double -> struct Train
struct Train move_train(struct Train train, double amount);

4. Write a purpose statement for the function
Advance the train by the given amount. Consider the case
that the train enters or leaves the critical section.

5. Give examples and expected results
Advancing a train that is not in the critical section (make_train_none()) by
3.0 km results in a train that is still not in the critical section
(make_train_none()).
 
Advancing a train that is at the start of the critical section
(make_train_at(0.0)) by 0.0 km results in a train that is still at the start
of the critical section (make_train_at(0.0)).
 
Advancing a train that is at position 1.0 km of the critical section
(make_train_at(1.0)) by -1.0 km results in a train that is at the start of the
critical section (make_train_at(0.0)).
 
etc.

6. Implement the function body
struct Train move_train(struct Train train, double amount) {
    switch (train.tag) {
        case T_ABSENT:
            return make_train_none();
        case T_PRESENT:
            train.position += amount;
            if (train.position < START_POSITION ||
                train.position > END_POSITION) {
                return make_train_none();
            } else {
                return train;
            }
    }
    return train;
}

7. Test the function
static void move_train_test() {
    see above ...
}

int main(void) {
    move_train_test();
    return 0;
}

8. Review and revise the function
no revision required
*/
