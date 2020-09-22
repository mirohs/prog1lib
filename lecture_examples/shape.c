/*
Compile: make shape
Run: ./shape
make shape && ./shape
*/

#include "base.h"

typedef struct {
    enum { CIRCLE, RECTANGLE } tag;
	union {	
        struct { int x, y, radius; } circle;
		struct { int x, y, width, height; } rectangle;
	};
} Shape;

Shape make_circle(int x, int y, int r) {
    require("non-negative radius", r >= 0);
    Shape s;
    s.tag = CIRCLE;
    s.circle.x = x;
    s.circle.y = y;
    s.circle.radius = r;
    return s;
}

Shape make_rectangle(int x, int y, int w, int h) {
    require("non-negative width", w >= 0);
    require("non-negative height", h >= 0);
    Shape s;
    s.tag = RECTANGLE;
    s.rectangle.x = x;
    s.rectangle.y = y;
    s.rectangle.width = w;
    s.rectangle.height = h;
    return s;
}

void print_shape(Shape s) {
    switch (s.tag) {
    case CIRCLE:
    	printf("circle: %d %d %d\n", 
    		s.circle.x , s.circle.y , 
    		s.circle.radius);
    	break;
    case RECTANGLE:
    	printf("rectangle: %d %d %d %d\n", 
    		s.rectangle.x, s.rectangle.y, 
    		s.rectangle.width, 
    		s.rectangle.height);
    	break;
    }
}

int main(void) {
    Shape c = make_circle(100, 50, 20);
    print_shape(c);

    Shape r = make_rectangle(10, 20, 30, 40);
    print_shape(r);
    
    printf("%lu, %lu\n", sizeof(c), sizeof(Shape));

    return 0;
}
