// g++ koch.cpp -lGL -lGLU -lglut -o koch
// ./koch 5
// hit <esc> to exit

#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/glut.h>

#define SQRT3 1.7320508075688772

typedef unsigned int uint;

using namespace std;

struct point {
    float x, y;
    point() { x = 0; y = 0; }
    point(float x_, float y_) { x = x_; y = y_; }
    void operator=(const point &p) { x = p.x; y = p.y; }
    bool operator==(const point &p) { return x == p.x && y == p.y; }
    bool operator!=(const point &p) { return x != p.x || y != p.y; }
};

// globals
vector<point> koch;
float render_scale;
int window;

void
display() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.f, 0.f, 0.f);

    glBegin(GL_LINE_STRIP);
        for (uint i = 0; i < koch.size(); i++)
            glVertex2f(koch[i].x / render_scale - 0.5,
                       koch[i].y / render_scale + 0.25);
    glEnd();

    glBegin(GL_LINE_STRIP);
        for (uint i = 0; i < koch.size(); i++) {
            float x = koch[i].x, y = koch[i].y;
            float x_new = render_scale + (-x + SQRT3*y) / 2;
            float y_new = (-SQRT3*x - y) / 2;
            glVertex2f(x_new / render_scale - 0.5,
                       y_new / render_scale + 0.25);
        }
    glEnd();

    glBegin(GL_LINE_STRIP);
        for (uint i = 0; i < koch.size(); i++) {
            float x = koch[i].x, y = koch[i].y;
            float x_new = (x - SQRT3*y) / 2;
            float y_new = (-SQRT3*x - y) / 2;
            glVertex2f(x_new / render_scale - 0.5,
                       y_new / render_scale + 0.25);
        }
    glEnd();

    glFlush();
}

void
keyboard(unsigned char key, int x, int y) {
    if (key == 27)
        glutDestroyWindow(window);
}

void
next_seg(point &p, point &q, point &out1, point &out2, point &out3) {
    // out1 divides pq internally in the ratio 1:2
    out1.x = (2*p.x + q.x) / 3; out1.y = (2*p.y + q.y) / 3;

    // out3 divides pq internally in the ratio 2:1
    out3.x = (p.x + 2*q.x) / 3; out3.y = (p.y + 2*q.y) / 3;

    // to get out2, first obtain the direction from p to q...
    float dx = q.x - p.x, dy = q.y - p.y;
    // ...then rotate it by 60 deg CCW, scale down by 3 and offset by out1
    out2.x = out1.x + (dx - SQRT3*dy) / 6;
    out2.y = out1.y + (SQRT3*dx + dy) / 6;
}

vector<point>
koch_direct(uint n) {
    vector<point> result;

    uint size = 2;
    for (uint i = 0; i < n; i++)
        size = 4*size - 3;

    result.resize(size);

    result[0].x = 0; result[0].y = 0;
    result[size-1].x = 81; result[size-1].y = 0;

    uint step_size = size - 1;

    while (step_size > 1) {
        for (uint i = 0; i + step_size < size ; i += step_size) {
            uint left = i, right = left + step_size;

            uint mid = (left + right) / 2;
            uint leftmid = (left + mid) / 2;
            uint rightmid = (mid + right) / 2;

            next_seg(result[left], result[right],
                     result[leftmid], result[mid], result[rightmid]);
        }
        step_size /= 4;
    }

    return result;
}

int
main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "usage: ./a.out <iter>" << endl;
        return 1;
    }

    int max_iter = atoi(argv[1]);

    koch = koch_direct(max_iter);

    // determine scale for rendering
    for (uint i = 0; i < koch.size(); i++) {
        float x = koch[i].x, y = koch[i].y;
        if (x > render_scale) render_scale = x;
        if (y > render_scale) render_scale = y;
    }

    cout << "rendering " << koch.size() << " points" << endl;

    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(400, 0);
    window = glutCreateWindow("Koch");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}