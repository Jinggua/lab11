/*
 * Lab 11: Graphics
 * EETG3024: Advanced Embedded Systems
 *
 * Simple industrial process diagram (Nuclear Power Plant)
 * 
 * Compile: gcc -o lab11_graphics lab11_graphics.c $(pkg-config --cflags --libs plplot) -lm
 * Run:     ./lab11_graphics
 */

#include <plplot/plplot.h>
#include <math.h>

/* helper: draw a filled rectangle */
void draw_rect(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int color) {
    PLFLT x[4] = {x1, x2, x2, x1};
    PLFLT y[4] = {y1, y1, y2, y2};
    plcol0(color);
    plfill(4, x, y);
    plcol0(0);          // black outline
    plline(5, x, y);
    x[4] = x1; y[4] = y1;
}

/* helper: draw a filled circle */
void draw_circle(PLFLT cx, PLFLT cy, PLFLT r, int color) {
    int n = 40;
    PLFLT x[41], y[41];
    for (int i = 0; i <= n; i++) {
        double a = 2 * M_PI * i / n;
        x[i] = cx + r * cos(a);
        y[i] = cy + r * sin(a);
    }
    plcol0(color);
    plfill(n+1, x, y);
    plcol0(0);
    plline(n+1, x, y);
}

/* helper: draw a line with thickness */
void thick_line(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int color, PLFLT w) {
    plcol0(color);
    plwidth(w);
    PLFLT x[2] = {x1, x2};
    PLFLT y[2] = {y1, y2};
    plline(2, x, y);
    plwidth(1.0);
}

/* helper: draw an upward arrow */
void arrow_up(PLFLT cx, PLFLT cy, PLFLT size, int color) {
    PLFLT x[3] = {cx - size, cx, cx + size};
    PLFLT y[3] = {cy - size, cy + size, cy - size};
    plcol0(color);
    plfill(3, x, y);
}

/* helper: draw a downward arrow */
void arrow_down(PLFLT cx, PLFLT cy, PLFLT size, int color) {
    PLFLT x[3] = {cx - size, cx, cx + size};
    PLFLT y[3] = {cy + size, cy - size, cy + size};
    plcol0(color);
    plfill(3, x, y);
}

/* helper: draw a right arrow */
void arrow_right(PLFLT cx, PLFLT cy, PLFLT size, int color) {
    PLFLT x[3] = {cx - size, cx + size, cx - size};
    PLFLT y[3] = {cy + size, cy, cy - size};
    plcol0(color);
    plfill(3, x, y);
}

/* helper: draw a left arrow */
void arrow_left(PLFLT cx, PLFLT cy, PLFLT size, int color) {
    PLFLT x[3] = {cx + size, cx - size, cx + size};
    PLFLT y[3] = {cy + size, cy, cy - size};
    plcol0(color);
    plfill(3, x, y);
}

/* helper: draw text centered */
void draw_text(PLFLT x, PLFLT y, const char *s, PLFLT size, int color) {
    plcol0(color);
    plschr(0, size);
    plptex(x, y, 1, 0, 0.5, s);
}

/* ------------------------------------------------------------
   Main: draw a simplified nuclear power plant diagram
------------------------------------------------------------ */
int main(int argc, char *argv[]) {
    plparseopts(&argc, argv, PL_PARSE_FULL);
    plsdev("xcairo");
    plspage(0, 0, 1000, 700, 0, 0);
    plinit();

    // Set coordinate system: x from 0 to 100, y from 0 to 80
    plenv(0, 100, 0, 80, 0, -1);
    plcol0(0);   // black for lines

    // ----- TITLE -----
    draw_text(50, 78, "Simple Nuclear Power Plant Process", 0.8, 1);

    // ----- CONTAINMENT BUILDING (gray rectangle with dome) -----
    draw_rect(5, 10, 30, 65, 7);                     // main box

    // dome: half circle using plfill (no plmove needed)
    int n = 30;
    PLFLT dome_x[33], dome_y[33];
    dome_x[0] = 5;   dome_y[0] = 65;
    for (int i = 0; i <= n; i++) {
        double a = M_PI * i / n;
        dome_x[i+1] = 5 + 25 * (1 - cos(a)) / 2.0;
        dome_y[i+1] = 65 + 12 * sin(a);
    }
    dome_x[n+2] = 30; dome_y[n+2] = 65;
    plcol0(7);
    plfill(n+3, dome_x, dome_y);
    plcol0(0);
    plline(n+3, dome_x, dome_y);

    draw_text(17.5, 70, "Containment", 0.55, 1);

    // ----- REACTOR (simple purple capsule) -----
    draw_rect(9, 25, 21, 48, 4);                    // rectangle
    draw_circle(15, 48, 6, 4);                     // top half
    draw_circle(15, 25, 6, 4);                     // bottom half
    draw_text(15, 40, "Reactor", 0.55, 1);

    // ----- CONTROL RODS (three lines above reactor) -----
    thick_line(11, 48, 11, 60, 0, 2.5);
    thick_line(15, 48, 15, 60, 0, 2.5);
    thick_line(19, 48, 19, 60, 0, 2.5);
    draw_text(10, 62, "Control Rods", 0.5, 1);

    // ----- STEAM GENERATOR (tall blue capsule) -----
    draw_rect(35, 30, 43, 58, 6);
    draw_circle(39, 58, 4, 6);
    draw_circle(39, 30, 4, 6);
    draw_text(39, 48, "Steam", 0.55, 1);
    draw_text(39, 44, "Gen.", 0.55, 1);

    // ----- STEAM LINE (thick pipe) -----
    thick_line(39, 62, 39, 68, 3, 4.0);
    thick_line(39, 68, 65, 68, 3, 4.0);
    thick_line(65, 62, 65, 68, 3, 4.0);
    draw_text(52, 70, "Steam Line", 0.55, 1);

    // ----- TURBINE BUILDING (big rectangle) -----
    draw_rect(45, 8, 75, 58, 10);                  // light gray building
    draw_text(60, 60, "Turbine / Generator Building", 0.6, 1);

    // ----- TURBINE (simple fan-like triangles) -----
    for (int i = 0; i < 5; i++) {
        PLFLT xb = 48 + i * 3.5;
        PLFLT yc = 45;
        PLFLT h = 2.5 + i * 0.8;
        PLFLT xt[3] = {xb, xb, xb + 2.5};
        PLFLT yt[3] = {yc + h, yc - h, yc};
        plcol0(1);
        plfill(3, xt, yt);
        plcol0(0);
        plline(3, xt, yt);
        xt[2] = xt[0]; yt[2] = yt[0];
        plline(4, xt, yt);
    }
    draw_text(58, 38, "Turbine", 0.55, 1);

    // ----- GENERATOR (yellow box) -----
    draw_rect(65, 40, 74, 52, 2);
    draw_text(69.5, 53, "Generator", 0.55, 1);

    // ----- CONDENSER (blue area) -----
    draw_rect(50, 12, 70, 30, 6);
    draw_text(60, 25, "Condenser", 0.6, 1);
    // arrows showing condensation
    for (int i = 0; i < 4; i++) {
        arrow_down(54 + i * 5, 35, 1.0, 1);
    }

    // ----- PUMPS (yellow circles) -----
    draw_circle(25, 15, 3, 2);
    draw_text(25, 11, "Pump", 0.5, 1);
    draw_circle(42, 40, 3, 2);
    draw_text(42, 36, "Pump", 0.5, 1);

    // ----- COOLING TOWER (hyperbolic shape) -----
    PLFLT tx[100], ty[100];
    int m = 40;
    for (int i = 0; i <= m; i++) {
        double t = (double)i / m;
        double yy = 12 + t * 55;
        double r = 5.5 - 3.5 * t + 2.0 * t * t;
        tx[i] = 80 + r;
        ty[i] = yy;
    }
    for (int i = 0; i <= m; i++) {
        double t = (double)i / m;
        double yy = 67 - t * 55;
        double r = 5.5 - 3.5 * (1-t) + 2.0 * (1-t)*(1-t);
        tx[m+1+i] = 80 - r;
        ty[m+1+i] = yy;
    }
    plcol0(7);
    plfill(2*m+2, tx, ty);
    plcol0(0);
    plline(2*m+2, tx, ty);
    draw_text(80, 73, "Cooling", 0.55, 1);
    draw_text(80, 69, "Tower", 0.55, 1);

    // ----- RESERVOIR (blue wavy shape) -----
    PLFLT rx[] = {78, 82, 88, 92, 94, 92, 88, 82, 78};
    PLFLT ry[] = {14, 10, 9, 12, 18, 24, 26, 23, 14};
    plcol0(9);
    plfill(9, rx, ry);
    plcol0(0);
    plline(9, rx, ry);
    draw_text(85, 20, "Reservoir", 0.55, 1);

    // ----- COOLING WATER PIPES (cyan) -----
    thick_line(70, 18, 78, 18, 9, 3.0);
    arrow_right(73, 18, 1.0, 1);
    thick_line(70, 28, 78, 28, 9, 3.0);
    arrow_right(73, 28, 1.0, 1);
    thick_line(50, 18, 42, 18, 9, 3.0);
    arrow_left(46, 18, 1.0, 1);
    thick_line(42, 18, 42, 37, 9, 3.0);
    arrow_up(42, 27, 1.0, 1);
    thick_line(90, 28, 90, 50, 9, 3.0);
    arrow_up(90, 40, 1.0, 1);
    thick_line(80, 50, 90, 50, 9, 3.0);
    arrow_right(84, 50, 1.0, 1);

    // ----- PRIMARY PIPES (dark purple) -----
    thick_line(9, 32, 9, 20, 4, 3.5);
    thick_line(9, 20, 22, 20, 4, 3.5);
    thick_line(22, 20, 22, 30, 4, 3.5);
    thick_line(35, 30, 35, 20, 4, 3.5);
    thick_line(35, 20, 28, 20, 4, 3.5);
    arrow_down(9, 26, 0.8, 1);
    arrow_up(22, 24, 0.8, 1);
    arrow_up(35, 26, 0.8, 1);

    // ----- SWITCHYARD (simple towers and lines) -----
    draw_text(62, 76, "Switchyard", 0.55, 1);
    // two towers
    for (int x = 58; x <= 66; x += 8) {
        thick_line(x, 60, x, 74, 1, 1.5);
        thick_line(x+2, 60, x+2, 74, 1, 1.5);
        thick_line(x, 74, x+1, 77, 1, 1.5);
        thick_line(x+2, 74, x+1, 77, 1, 1.5);
    }
    // power lines (sagging)
    for (int l = 0; l < 3; l++) {
        PLFLT px[20], py[20];
        for (int i = 0; i < 20; i++) {
            double t = i / 19.0;
            px[i] = 59 + 8 * t;
            py[i] = 73 - l*2.5 - 2.5 * sin(3.14159 * t);
        }
        thick_line(59, 73-l*2.5, 67, 73-l*2.5, 2, 1.5);
    }
    thick_line(74, 46, 62, 60, 2, 2.0);

    // ----- WATER INTAKE (arrow from bottom) -----
    thick_line(55, 0, 55, 8, 9, 4.0);
    arrow_up(55, 8, 1.5, 9);
    draw_text(55, -3, "Water Intake", 0.55, 1);

    plend();
    return 0;
}