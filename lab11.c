/*
 * Lab 11: Graphics
 * EETG3024: Advanced Embedded Systems
 * 
 * Nuclear Power Plant Process Diagram using PLplot
 * 
 * This program draws an industrial process diagram of a nuclear power plant.
 * Components: Containment Structure, Reactor, Control Rods, Steam Generator,
 * Steam Line, Pumps, Turbine, Generator, Condenser, Cooling Towers,
 * Switchyard, Reservoir, and connecting pipes with flow arrows.
 *
 * Compile: gcc -o lab11_graphics lab11_graphics.c $(pkg-config --cflags --libs plplot) -lm
 * Run:     ./lab11_graphics
 */

#include <plplot/plplot.h>
#include <math.h>

/* ================================================================
 * CUSTOM COLORS - set up in main() via plscol0
 * Index  Color
 * 0      Black (background)
 * 1      Red
 * 2      Yellow
 * 3      Dark Blue (pipes)
 * 4      Purple (reactor)
 * 5      Light Purple (reactor inner)
 * 6      Medium Blue (steam gen / condenser)
 * 7      Gray (buildings)
 * 8      Dark Gray (outlines)
 * 9      Cyan / Light Blue (cooling water / reservoir)
 * 10     Light Gray
 * 11     Pale Blue (turbine building lower)
 * 12     Dark Purple (primary pipes)
 * 13     Magenta/Pink
 * 14     Gold/Orange
 * 15     White
 * ================================================================ */

/* ================================================================
 * HELPER FUNCTIONS
 * ================================================================ */

/* Filled rectangle with optional outline */
void fill_rect(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int fc, int lc)
{
    PLFLT fx[4] = {x1, x2, x2, x1};
    PLFLT fy[4] = {y1, y1, y2, y2};
    plcol0(fc);
    plfill(4, fx, fy);
    if (lc >= 0) {
        plcol0(lc);
        PLFLT bx[5] = {x1, x2, x2, x1, x1};
        PLFLT by[5] = {y1, y1, y2, y2, y1};
        plline(5, bx, by);
    }
}

/* Filled circle */
void fill_circle(PLFLT cx, PLFLT cy, PLFLT r, int fc, int lc)
{
    int n = 72;
    PLFLT x[73], y[73];
    for (int i = 0; i <= n; i++) {
        double a = 2.0 * M_PI * i / n;
        x[i] = cx + r * cos(a);
        y[i] = cy + r * sin(a);
    }
    plcol0(fc);
    plfill(n + 1, x, y);
    if (lc >= 0) {
        plcol0(lc);
        plline(n + 1, x, y);
    }
}

/* Thick line */
void pline(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int c, PLFLT w)
{
    plcol0(c);
    plwidth(w);
    PLFLT x[2] = {x1, x2};
    PLFLT y[2] = {y1, y2};
    plline(2, x, y);
    plwidth(1.0);
}

/* Down arrow */
void arr_down(PLFLT cx, PLFLT cy, PLFLT s, int c)
{
    PLFLT x[3] = {cx - s, cx, cx + s};
    PLFLT y[3] = {cy + s, cy - s, cy + s};
    plcol0(c);
    plfill(3, x, y);
}

/* Up arrow */
void arr_up(PLFLT cx, PLFLT cy, PLFLT s, int c)
{
    PLFLT x[3] = {cx - s, cx, cx + s};
    PLFLT y[3] = {cy - s, cy + s, cy - s};
    plcol0(c);
    plfill(3, x, y);
}

/* Left arrow */
void arr_left(PLFLT cx, PLFLT cy, PLFLT s, int c)
{
    PLFLT x[3] = {cx + s, cx - s, cx + s};
    PLFLT y[3] = {cy + s, cy, cy - s};
    plcol0(c);
    plfill(3, x, y);
}

/* Right arrow */
void arr_right(PLFLT cx, PLFLT cy, PLFLT s, int c)
{
    PLFLT x[3] = {cx - s, cx + s, cx - s};
    PLFLT y[3] = {cy + s, cy, cy - s};
    plcol0(c);
    plfill(3, x, y);
}

/* Centered text */
void txt(PLFLT x, PLFLT y, const char *s, PLFLT sz, int c)
{
    plcol0(c);
    plschr(0, sz);
    plptex(x, y, 1, 0, 0.5, s);
}

/* Left-aligned text */
void txt_l(PLFLT x, PLFLT y, const char *s, PLFLT sz, int c)
{
    plcol0(c);
    plschr(0, sz);
    plptex(x, y, 1, 0, 0.0, s);
}

/* ================================================================
 * COMPONENT FUNCTIONS
 * ================================================================ */

/* ---- Containment Structure ---- */
void draw_containment(void)
{
    /* Main body - light gray */
    fill_rect(2, 8, 28, 60, 10, 8);

    /* Dome - half ellipse on top */
    int n = 50;
    PLFLT dx[52], dy[52];
    dx[0] = 2; dy[0] = 60;
    for (int i = 0; i <= n; i++) {
        double a = M_PI * i / n;
        dx[i + 1] = 15 - 13 * cos(a);
        dy[i + 1] = 60 + 13 * sin(a);
    }
    plcol0(10);
    plfill(n + 2, dx, dy);
    plcol0(8);
    plwidth(2.0);
    plline(n + 2, dx, dy);
    plwidth(1.0);

    /* Thick outline on sides */
    pline(2, 8, 2, 60, 8, 3.0);
    pline(28, 8, 28, 60, 8, 3.0);
    pline(2, 8, 28, 8, 8, 3.0);

    /* Label - white bold text */
    txt(15, 68, "CONTAINMENT", 0.65, 15);
    txt(15, 64, "STRUCTURE", 0.65, 15);
}

/* ---- Reactor ---- */
void draw_reactor(void)
{
    /* Outer vessel - purple */
    fill_circle(12, 32, 8.5, 4, 8);

    /* Inner region - lighter purple */
    fill_circle(12, 32, 6, 5, -1);

    /* Core - red rectangle */
    fill_rect(9.5, 26, 14.5, 30, 1, 8);

    /* Fuel rods inside core */
    for (int i = 0; i < 4; i++) {
        pline(10.2 + i * 1.2, 26, 10.2 + i * 1.2, 30, 8, 1.5);
    }

    /* White circulation arrows */
    arr_left(7, 34, 0.7, 15);
    arr_right(17, 34, 0.7, 15);
    arr_up(9, 37, 0.7, 15);
    arr_down(15, 27, 0.7, 15);

    /* Label */
    txt(12, 33, "Reactor", 0.55, 15);
}

/* ---- Control Rods ---- */
void draw_control_rods(void)
{
    /* Three vertical rods */
    pline(9.5, 41, 9.5, 56, 8, 2.5);
    pline(12, 41, 12, 56, 8, 2.5);
    pline(14.5, 41, 14.5, 56, 8, 2.5);

    /* Handle blocks at top */
    fill_rect(8.5, 54, 10.5, 57, 8, 15);
    fill_rect(11, 54, 13, 57, 8, 15);
    fill_rect(13.5, 54, 15.5, 57, 8, 15);

    /* Label */
    txt_l(3, 52, "Control Rods", 0.45, 15);
}

/* ---- Steam Generator ---- */
void draw_steam_generator(void)
{
    /* Main body - blue */
    fill_rect(20, 26, 27, 55, 6, 8);

    /* Dome top */
    int n = 30;
    PLFLT sx[32], sy[32];
    sx[0] = 20; sy[0] = 55;
    for (int i = 0; i <= n; i++) {
        double a = M_PI * i / n;
        sx[i + 1] = 23.5 - 3.5 * cos(a);
        sy[i + 1] = 55 + 4 * sin(a);
    }
    plcol0(6);
    plfill(n + 2, sx, sy);
    plcol0(8);
    plline(n + 2, sx, sy);

    /* Internal U-tubes */
    for (int i = 0; i < 4; i++) {
        pline(21.5, 30 + i * 6, 21.5, 33 + i * 6, 15, 0.8);
        pline(21.5, 33 + i * 6, 25.5, 33 + i * 6, 15, 0.8);
        pline(25.5, 30 + i * 6, 25.5, 33 + i * 6, 15, 0.8);
    }

    /* Label */
    txt(23.5, 51, "Steam", 0.5, 15);
    txt(23.5, 48, "Generator", 0.5, 15);
}

/* ---- Steam Line ---- */
void draw_steam_line(void)
{
    /* Dark blue pipe from steam gen top, going right to turbine building */
    pline(23.5, 59, 23.5, 63, 3, 4.0);
    pline(23.5, 63, 52, 63, 3, 4.0);
    pline(52, 55, 52, 63, 3, 4.0);

    /* Label */
    txt(38, 65, "Steam Line", 0.55, 15);
}

/* ---- Pumps ---- */
void draw_pumps(void)
{
    /* Pump 1 - below reactor */
    fill_circle(15, 14, 2.8, 2, 8);
    txt(15, 10, "Pump", 0.5, 15);

    /* Pump 2 - between containment and turbine */
    fill_circle(34, 40, 2.8, 2, 8);
    txt(34, 44, "Pump", 0.5, 15);

    /* Flow arrow near pump 2 */
    arr_left(30.5, 40, 0.8, 15);
}

/* ---- Turbine/Condenser Building ---- */
void draw_building(void)
{
    /* Upper part - light gray */
    fill_rect(38, 35, 72, 58, 10, -1);

    /* Lower part - pale blue (condenser area) */
    fill_rect(38, 5, 72, 35, 11, -1);

    /* Outline */
    plcol0(8);
    plwidth(2.0);
    PLFLT bx[5] = {38, 72, 72, 38, 38};
    PLFLT by[5] = {5, 5, 58, 58, 5};
    plline(5, bx, by);
    plwidth(1.0);
}

/* ---- Turbine - multiple triangular blades fanning out ---- */
void draw_turbine(void)
{
    /* Draw multiple triangular fan blades from a point on the left */
    PLFLT tip_x = 44;   /* narrow tip (left) */
    PLFLT tip_y = 45;
    int num_blades = 12;

    /* Overall fan spread */
    double start_angle = -0.45;
    double end_angle = 0.45;

    for (int i = 0; i < num_blades; i++) {
        double a1 = start_angle + (end_angle - start_angle) * i / num_blades;
        double a2 = start_angle + (end_angle - start_angle) * (i + 1) / num_blades;

        PLFLT len = 16.0;
        PLFLT tx[3], ty[3];
        tx[0] = tip_x;
        ty[0] = tip_y;
        tx[1] = tip_x + len * cos(a1);
        ty[1] = tip_y + len * sin(a1);
        tx[2] = tip_x + len * cos(a2);
        ty[2] = tip_y + len * sin(a2);

        /* Alternate colors for blade effect */
        if (i % 2 == 0) {
            plcol0(8);
        } else {
            plcol0(10);
        }
        plfill(3, tx, ty);
        plcol0(8);
        plline(3, tx, ty);
    }

    /* Arrow shape at tip (left-pointing triangle) */
    PLFLT ax[3] = {tip_x - 4, tip_x, tip_x};
    PLFLT ay[3] = {tip_y, tip_y + 3, tip_y - 3};
    plcol0(8);
    plfill(3, ax, ay);
    plcol0(15);
    plline(3, ax, ay);

    /* Shaft line from steam */
    pline(40, 45, 44, 45, 8, 3.0);

    /* Label */
    txt(52, 45, "Turbine", 0.55, 15);
}

/* ---- Generator ---- */
void draw_generator(void)
{
    /* Main body - yellow */
    fill_rect(62, 40, 71, 52, 2, 8);

    /* Inner detail - gold/orange square */
    fill_rect(64.5, 42.5, 68.5, 49, 14, 8);

    /* Shaft connecting turbine to generator */
    pline(60, 45, 62, 45, 8, 3.0);

    /* Small circle on shaft */
    fill_circle(61, 45, 0.6, 10, 8);

    /* Label */
    txt(66.5, 50.5, "Generator", 0.5, 15);
}

/* ---- Condenser ---- */
void draw_condenser(void)
{
    /* Main body - blue */
    fill_rect(42, 8, 65, 33, 6, 8);

    /* Down arrows (steam condensing) - white */
    for (int i = 0; i < 5; i++) {
        arr_down(47 + i * 3.5, 36, 1.0, 15);
    }

    /* Label */
    txt(53.5, 25, "Condenser", 0.6, 15);
}

/* ---- Cooling Water label ---- */
void draw_cooling_water_label(void)
{
    txt(63, 7, "Cooling Water", 0.5, 15);
}

/* ---- Switchyard ---- */
void draw_switchyard(void)
{
    txt(59, 78, "Switchyard", 0.55, 15);

    /* Tower 1 - lattice style */
    /* Vertical legs */
    pline(54, 60, 54, 75, 1, 1.5);
    pline(58, 60, 58, 75, 1, 1.5);
    /* Top triangle */
    pline(54, 75, 56, 78, 1, 1.5);
    pline(58, 75, 56, 78, 1, 1.5);
    /* Cross members */
    pline(54, 63, 58, 66, 1, 0.8);
    pline(58, 63, 54, 66, 1, 0.8);
    pline(54, 66, 58, 69, 1, 0.8);
    pline(58, 66, 54, 69, 1, 0.8);
    pline(54, 69, 58, 72, 1, 0.8);
    pline(58, 69, 54, 72, 1, 0.8);
    /* Horizontal beams */
    pline(54, 63, 58, 63, 1, 1.0);
    pline(54, 66, 58, 66, 1, 1.0);
    pline(54, 69, 58, 69, 1, 1.0);
    pline(54, 72, 58, 72, 1, 1.0);

    /* Tower 2 */
    pline(62, 60, 62, 75, 1, 1.5);
    pline(66, 60, 66, 75, 1, 1.5);
    pline(62, 75, 64, 78, 1, 1.5);
    pline(66, 75, 64, 78, 1, 1.5);
    pline(62, 63, 66, 66, 1, 0.8);
    pline(66, 63, 62, 66, 1, 0.8);
    pline(62, 66, 66, 69, 1, 0.8);
    pline(66, 66, 62, 69, 1, 0.8);
    pline(62, 69, 66, 72, 1, 0.8);
    pline(66, 69, 62, 72, 1, 0.8);
    pline(62, 63, 66, 63, 1, 1.0);
    pline(62, 66, 66, 66, 1, 1.0);
    pline(62, 69, 66, 69, 1, 1.0);
    pline(62, 72, 66, 72, 1, 1.0);

    /* Sagging yellow power lines between towers */
    plwidth(2.0);
    for (int line = 0; line < 3; line++) {
        PLFLT px[30], py[30];
        double base = 74 - line * 3;
        for (int i = 0; i < 30; i++) {
            double t = (double)i / 29.0;
            px[i] = 56 + 8 * t;
            py[i] = base - 3.0 * sin(M_PI * t);
        }
        plcol0(2);
        plline(30, px, py);
    }
    plwidth(1.0);

    /* Yellow wire from generator up to switchyard */
    pline(67, 52, 62, 60, 2, 2.0);
}

/* ---- Cooling Tower ---- */
void draw_cooling_tower(void)
{
    /* Hyperbolic tower profile */
    int n = 60;
    PLFLT px[122], py[122];

    /* Right side (bottom to top) */
    for (int i = 0; i <= n; i++) {
        double t = (double)i / n;
        double yy = 12 + t * 60;
        /* Wider at bottom, narrow middle, flare at top */
        double r = 5.0 + 2.0 * (1.0 - t) * (1.0 - t) - 1.5 * t * (1.0 - t) * 4;
        if (r < 2.5) r = 2.5;
        px[i] = 83 + r;
        py[i] = yy;
    }
    /* Left side (top to bottom) */
    for (int i = 0; i <= n; i++) {
        double t = (double)i / n;
        double yy = 72 - t * 60;
        double t2 = 1.0 - t;
        double r = 5.0 + 2.0 * t2 * t2 - 1.5 * (1.0 - t2) * t2 * 4;
        if (r < 2.5) r = 2.5;
        px[n + 1 + i] = 83 - r;
        py[n + 1 + i] = yy;
    }

    plcol0(7);
    plfill(2 * n + 2, px, py);
    plcol0(8);
    plwidth(2.0);
    plline(2 * n + 2, px, py);
    plwidth(1.0);

    /* Label */
    txt(85, 76, "Cooling", 0.5, 15);
    txt(85, 73, "Towers", 0.5, 15);
}

/* ---- Reservoir ---- */
void draw_reservoir(void)
{
    /* Irregular light blue water body */
    PLFLT rx[] = {72, 76, 83, 91, 96, 97, 95, 89, 80, 74, 72};
    PLFLT ry[] = {18, 13, 11, 12, 16, 24, 31, 34, 33, 26, 18};

    plcol0(9);
    plfill(11, rx, ry);

    /* Wave details */
    plcol0(6);
    plwidth(0.7);
    for (int j = 0; j < 4; j++) {
        PLFLT wx[30], wy[30];
        for (int i = 0; i < 30; i++) {
            double t = (double)i / 29.0;
            wx[i] = 76 + 16 * t;
            wy[i] = 16 + j * 4 + 0.8 * sin(5 * M_PI * t);
        }
        plline(30, wx, wy);
    }
    plwidth(1.0);

    /* Outline */
    plcol0(3);
    plwidth(1.5);
    plline(11, rx, ry);
    plwidth(1.0);

    /* Label */
    txt(85, 23, "Reservoir", 0.55, 15);
}

/* ---- Water Intake ---- */
void draw_water_intake(void)
{
    pline(53.5, 0, 53.5, 8, 0, 4.0);
    arr_up(53.5, 8, 1.5, 0);
    txt(53.5, -2, "Water", 0.6, 15);
}

/* ---- Primary Loop Pipes (dark blue/purple inside containment) ---- */
void draw_primary_pipes(void)
{
    int c = 12;  /* dark purple */
    PLFLT w = 3.5;

    /* Reactor left side -> down to pump 1 */
    pline(5, 32, 5, 16, c, w);
    pline(5, 32, 8, 32, c, w);
    pline(5, 16, 12.2, 16, c, w);

    /* Pump 1 -> right and up to steam gen bottom */
    pline(17.8, 14, 23.5, 14, c, w);
    pline(23.5, 14, 23.5, 26, c, w);

    /* Steam gen -> left back to reactor */
    pline(20, 40, 17, 40, c, w);
    pline(17, 32, 17, 40, c, w);

    /* Top loop from steam gen to steam line */
    pline(23.5, 55, 23.5, 59, c, w);

    /* Flow arrows */
    arr_down(5, 24, 0.7, 15);
    arr_up(23.5, 20, 0.7, 15);
}

/* ---- Secondary Loop Pipes (cyan/light blue cooling water) ---- */
void draw_secondary_pipes(void)
{
    int c = 9;   /* cyan */
    PLFLT w = 3.5;

    /* Pump 2 to steam gen */
    pline(31.2, 40, 28, 40, c, w);

    /* Condenser right side going out to reservoir */
    pline(65, 18, 72, 18, c, w);

    /* Top pipe from reservoir to cooling tower area */
    pline(91, 28, 91, 52, c, w);
    pline(83, 52, 91, 52, c, w);

    /* From cooling tower down to condenser right */
    pline(72, 28, 72, 33, c, w);
    pline(65, 33, 72, 33, c, w);

    /* Condenser left side back to pump 2 area */
    pline(42, 18, 35, 18, c, w);
    pline(35, 18, 35, 37, c, w);

    /* Flow arrows on cooling water pipes */
    arr_right(68, 18, 0.7, 15);
    arr_left(38, 18, 0.7, 15);
    arr_right(86, 52, 0.7, 15);
    arr_left(68, 33, 0.7, 15);
    arr_up(35, 28, 0.7, 15);
}

/* ================================================================
 * MAIN
 * ================================================================ */
int main(int argc, char *argv[])
{
    /* Initialize PLplot */
    plparseopts(&argc, argv, PL_PARSE_FULL);
    plsdev("xcairo");
    plspage(0, 0, 1200, 750, 0, 0);

    /* Set custom colors before plinit */
    plscol0(0,  0,   0,   0);    /* Black background */
    plscol0(1,  200, 30,  30);   /* Red */
    plscol0(2,  240, 200, 0);    /* Yellow/Gold */
    plscol0(3,  20,  20,  120);  /* Dark Blue */
    plscol0(4,  100, 30,  130);  /* Purple (reactor) */
    plscol0(5,  160, 100, 180);  /* Light Purple */
    plscol0(6,  60,  80,  180);  /* Medium Blue */
    plscol0(7,  140, 140, 140);  /* Gray */
    plscol0(8,  50,  50,  50);   /* Dark Gray (outlines) */
    plscol0(9,  80,  200, 240);  /* Cyan (cooling water) */
    plscol0(10, 190, 190, 190);  /* Light Gray */
    plscol0(11, 130, 160, 210);  /* Pale Blue */
    plscol0(12, 80,  20,  120);  /* Dark Purple (primary pipes) */
    plscol0(13, 220, 80,  160);  /* Magenta */
    plscol0(14, 200, 160, 40);   /* Gold/Orange */
    plscol0(15, 255, 255, 255);  /* White */

    plinit();
    plenv(0, 100, -4, 82, 0, -1);

    /* Title */
    txt(50, 80, "Nuclear Power Plant Process Diagram", 0.85, 15);

    /* Draw components back to front */
    draw_containment();          /* Gray dome building */
    draw_building();             /* Turbine/condenser building */
    draw_condenser();            /* Blue condenser area */
    draw_reactor();              /* Purple reactor vessel */
    draw_control_rods();         /* Rods above reactor */
    draw_steam_generator();      /* Blue tall vessel */
    draw_pumps();                /* Yellow circles */
    draw_turbine();              /* Fan blade triangles */
    draw_generator();            /* Yellow box */
    draw_cooling_tower();        /* Hyperbolic tower */
    draw_reservoir();            /* Light blue water */
    draw_switchyard();           /* Electrical towers + lines */
    draw_steam_line();           /* Dark blue pipe on top */
    draw_water_intake();         /* Arrow from bottom */
    draw_primary_pipes();        /* Purple internal pipes */
    draw_secondary_pipes();      /* Cyan cooling pipes */
    draw_cooling_water_label();  /* Label */

    plend();
    return 0;
}