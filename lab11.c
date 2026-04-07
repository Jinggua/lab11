/*
 * Lab 11: Graphics
 * EETG3024: Advanced Embedded Systems
 * 
 * Nuclear Power Plant Process Diagram using PLplot
 * 
 * This program draws an industrial process diagram of a nuclear power plant
 * including: Containment Structure, Reactor, Control Rods, Steam Generator,
 * Steam Line, Pumps, Turbine, Generator, Condenser, Cooling Towers,
 * Switchyard, Reservoir, and connecting pipes.
 *
 * Compile: gcc -o lab11_graphics lab11_graphics.c $(pkg-config --cflags --libs plplot) -lm
 * Run:     ./lab11_graphics
 */

#include <plplot/plplot.h>
#include <math.h>

/* ================================================================
 * HELPER FUNCTIONS
 * ================================================================ */

/* Draw a filled rectangle with optional outline */
void fill_rect(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int fill_col, int line_col)
{
    PLFLT x[4] = {x1, x2, x2, x1};
    PLFLT y[4] = {y1, y1, y2, y2};
    plcol0(fill_col);
    plfill(4, x, y);
    if (line_col >= 0) {
        plcol0(line_col);
        PLFLT bx[5] = {x1, x2, x2, x1, x1};
        PLFLT by[5] = {y1, y1, y2, y2, y1};
        plline(5, bx, by);
    }
}

/* Draw a filled circle */
void fill_circle(PLFLT cx, PLFLT cy, PLFLT r, int fill_col, int line_col)
{
    int n = 72;
    PLFLT x[73], y[73];
    for (int i = 0; i <= n; i++) {
        double a = 2.0 * M_PI * i / n;
        x[i] = cx + r * cos(a);
        y[i] = cy + r * sin(a);
    }
    plcol0(fill_col);
    plfill(n + 1, x, y);
    if (line_col >= 0) {
        plcol0(line_col);
        plline(n + 1, x, y);
    }
}

/* Draw a thick line (pipe) */
void pipe_line(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int col, PLFLT width)
{
    plcol0(col);
    plwidth(width);
    PLFLT x[2] = {x1, x2};
    PLFLT y[2] = {y1, y2};
    plline(2, x, y);
    plwidth(1.0);
}

/* Draw a down arrow */
void arrow_down(PLFLT cx, PLFLT cy, PLFLT sz, int col)
{
    PLFLT x[3] = {cx - sz, cx, cx + sz};
    PLFLT y[3] = {cy + sz, cy - sz, cy + sz};
    plcol0(col);
    plfill(3, x, y);
}

/* Draw an up arrow */
void arrow_up(PLFLT cx, PLFLT cy, PLFLT sz, int col)
{
    PLFLT x[3] = {cx - sz, cx, cx + sz};
    PLFLT y[3] = {cy - sz, cy + sz, cy - sz};
    plcol0(col);
    plfill(3, x, y);
}

/* Draw left arrow */
void arrow_left(PLFLT cx, PLFLT cy, PLFLT sz, int col)
{
    PLFLT x[3] = {cx + sz, cx - sz, cx + sz};
    PLFLT y[3] = {cy - sz, cy, cy + sz};
    plcol0(col);
    plfill(3, x, y);
}

/* Draw text centered at position */
void text_at(PLFLT x, PLFLT y, const char *str, PLFLT size, int col)
{
    plcol0(col);
    plschr(0, size);
    plptex(x, y, 1, 0, 0.5, str);
}

/* ================================================================
 * COMPONENT DRAWING FUNCTIONS
 * ================================================================ */

/* Containment Structure - large dome building on the left */
void draw_containment(void)
{
    /* Main rectangular body */
    fill_rect(2, 10, 28, 60, 7, 8);

    /* Dome on top - half circle */
    int n = 50;
    PLFLT dx[52], dy[52];
    dx[0] = 2;  dy[0] = 60;
    for (int i = 0; i <= n; i++) {
        double a = M_PI * i / n;
        dx[i + 1] = 15 - 13 * cos(a);
        dy[i + 1] = 60 + 12 * sin(a);
    }
    plcol0(7);
    plfill(n + 2, dx, dy);
    plcol0(8);
    plline(n + 2, dx, dy);

    /* Label */
    text_at(15, 68, "CONTAINMENT", 0.55, 15);
    text_at(15, 65, "STRUCTURE", 0.55, 15);
}

/* Reactor - purple vessel with red core */
void draw_reactor(void)
{
    /* Outer vessel - dark purple */
    fill_circle(12, 32, 8, 13, 8);

    /* Inner glow - lighter */
    fill_circle(12, 32, 5.5, 5, -1);

    /* Core - red at center bottom */
    fill_rect(10, 27, 14, 31, 1, -1);

    /* Fuel rod lines inside core */
    for (int i = 0; i < 3; i++) {
        pipe_line(10.8 + i * 1.4, 27, 10.8 + i * 1.4, 31, 8, 1.5);
    }

    /* Circulation arrows inside reactor (simple curved indicators) */
    arrow_up(8, 36, 0.6, 15);
    arrow_down(16, 28, 0.6, 15);

    /* Label */
    text_at(12, 33, "Reactor", 0.5, 15);
}

/* Control Rods above reactor */
void draw_control_rods(void)
{
    /* Three rods */
    pipe_line(9.5, 40, 9.5, 55, 8, 2.0);
    pipe_line(12, 40, 12, 55, 8, 2.0);
    pipe_line(14.5, 40, 14.5, 55, 8, 2.0);

    /* Blocks at top */
    fill_rect(8.5, 54, 10.5, 57, 8, 15);
    fill_rect(11, 54, 13, 57, 8, 15);
    fill_rect(13.5, 54, 15.5, 57, 8, 15);

    /* Label */
    text_at(5, 52, "Control Rods", 0.45, 15);
}

/* Steam Generator - tall blue vessel */
void draw_steam_generator(void)
{
    /* Main body */
    fill_rect(20, 28, 27, 56, 9, 8);

    /* Dome top */
    int n = 30;
    PLFLT sx[32], sy[32];
    sx[0] = 20; sy[0] = 56;
    for (int i = 0; i <= n; i++) {
        double a = M_PI * i / n;
        sx[i + 1] = 23.5 - 3.5 * cos(a);
        sy[i + 1] = 56 + 3 * sin(a);
    }
    plcol0(9);
    plfill(n + 2, sx, sy);
    plcol0(8);
    plline(n + 2, sx, sy);

    /* Internal U-tube details */
    for (int i = 0; i < 3; i++) {
        pipe_line(21.5, 33 + i * 7, 21.5, 36 + i * 7, 15, 1.0);
        pipe_line(21.5, 36 + i * 7, 25.5, 36 + i * 7, 15, 1.0);
        pipe_line(25.5, 33 + i * 7, 25.5, 36 + i * 7, 15, 1.0);
    }

    /* Label */
    text_at(23.5, 52, "Steam", 0.45, 15);
    text_at(23.5, 49, "Generator", 0.45, 15);
}

/* Steam Line */
void draw_steam_line(void)
{
    pipe_line(23.5, 59, 23.5, 62, 7, 3.0);
    pipe_line(23.5, 62, 50, 62, 7, 3.0);
    pipe_line(50, 52, 50, 62, 7, 3.0);

    text_at(37, 64, "Steam Line", 0.5, 15);
}

/* Pumps - yellow circles */
void draw_pumps(void)
{
    /* Pump 1 - below reactor */
    fill_circle(15, 16, 2.5, 2, 8);
    text_at(15, 12, "Pump", 0.45, 15);

    /* Pump 2 - between containment and turbine building */
    fill_circle(33, 40, 2.5, 2, 8);
    text_at(33, 44, "Pump", 0.45, 15);

    /* Small arrows on pump 2 showing flow direction */
    arrow_left(30, 40, 0.8, 15);
}

/* Turbine/Condenser building background */
void draw_building(void)
{
    fill_rect(38, 6, 72, 56, 7, 8);
}

/* Turbine - cone shape pointing left */
void draw_turbine(void)
{
    /* Cone/fan shape - narrow on left, wide on right */
    PLFLT tx[4] = {44, 60, 60, 44};
    PLFLT ty[4] = {41, 35, 53, 47};
    plcol0(8);
    plfill(4, tx, ty);
    plcol0(15);
    PLFLT bx[5] = {44, 60, 60, 44, 44};
    PLFLT by[5] = {41, 35, 53, 47, 41};
    plline(5, bx, by);

    /* Blade lines radiating from narrow end */
    for (int i = 0; i < 10; i++) {
        double frac = (double)i / 9.0;
        double y_end = 36 + frac * 16;
        pipe_line(45, 44, 59, y_end, 15, 0.5);
    }

    text_at(52, 44, "Turbine", 0.5, 15);
}

/* Generator */
void draw_generator(void)
{
    fill_rect(61, 40, 71, 52, 2, 8);
    fill_rect(63.5, 42, 68.5, 48, 6, 8);

    /* Shaft line connecting turbine to generator */
    pipe_line(60, 44, 61, 44, 15, 2.5);

    text_at(66, 50, "Generator", 0.5, 15);
}

/* Condenser - blue area below turbine */
void draw_condenser(void)
{
    fill_rect(42, 10, 64, 33, 9, 8);

    /* Down arrows showing water flow */
    for (int i = 0; i < 5; i++) {
        arrow_down(47 + i * 3.5, 35, 1.0, 15);
    }

    text_at(53, 26, "Condenser", 0.55, 15);
}

/* Cooling Tower - hyperbolic shape */
void draw_cooling_tower(void)
{
    int n = 50;
    PLFLT lx[51], ly[51], rx[51], ry[51];

    /* Build left and right profiles */
    for (int i = 0; i <= n; i++) {
        double t = (double)i / n;
        double yy = 15 + t * 55;
        /* Hyperbolic: wide bottom, narrow middle, flare at top */
        double r = 5.5 - 3.0 * sin(M_PI * t * 0.65);
        lx[i] = 82 - r;
        rx[i] = 82 + r;
        ly[i] = yy;
        ry[i] = yy;
    }

    /* Combine into single polygon */
    PLFLT px[102], py[102];
    for (int i = 0; i <= n; i++) {
        px[i] = rx[i];
        py[i] = ry[i];
    }
    for (int i = 0; i <= n; i++) {
        px[n + 1 + i] = lx[n - i];
        py[n + 1 + i] = ly[n - i];
    }

    plcol0(7);
    plfill(2 * n + 2, px, py);
    plcol0(8);
    plline(2 * n + 2, px, py);

    text_at(83, 73, "Cooling", 0.5, 15);
    text_at(83, 70, "Towers", 0.5, 15);
}

/* Reservoir - blue water body */
void draw_reservoir(void)
{
    PLFLT rx[] = {70, 75, 82, 90, 95, 96, 94, 88, 78, 72, 70};
    PLFLT ry[] = {20, 15, 13, 14, 18, 25, 32, 35, 34, 28, 20};

    plcol0(11); /* light blue */
    plfill(11, rx, ry);
    plcol0(9);
    plline(11, rx, ry);

    /* Wave lines */
    plcol0(9);
    for (int j = 0; j < 3; j++) {
        PLFLT wx[30], wy[30];
        for (int i = 0; i < 30; i++) {
            double t = (double)i / 29.0;
            wx[i] = 74 + 17 * t;
            wy[i] = 19 + j * 4 + 0.7 * sin(5 * M_PI * t);
        }
        plline(30, wx, wy);
    }

    text_at(84, 24, "Reservoir", 0.55, 15);
}

/* Switchyard - electrical towers with sagging power lines */
void draw_switchyard(void)
{
    text_at(58, 80, "Switchyard", 0.5, 15);

    /* Tower 1 */
    pipe_line(53, 62, 53, 76, 8, 1.5);
    pipe_line(57, 62, 57, 76, 8, 1.5);
    pipe_line(53, 76, 55, 79, 8, 1.5);
    pipe_line(57, 76, 55, 79, 8, 1.5);
    pipe_line(53, 67, 57, 67, 8, 1.0);
    pipe_line(53, 71, 57, 71, 8, 1.0);

    /* Tower 2 */
    pipe_line(61, 62, 61, 76, 8, 1.5);
    pipe_line(65, 62, 65, 76, 8, 1.5);
    pipe_line(61, 76, 63, 79, 8, 1.5);
    pipe_line(65, 76, 63, 79, 8, 1.5);
    pipe_line(61, 67, 65, 67, 8, 1.0);
    pipe_line(61, 71, 65, 71, 8, 1.0);

    /* Sagging yellow power lines */
    plcol0(2);
    plwidth(1.5);
    for (int line = 0; line < 3; line++) {
        PLFLT px[25], py[25];
        double base_y = 74 - line * 3;
        for (int i = 0; i < 25; i++) {
            double t = (double)i / 24.0;
            px[i] = 55 + 8 * t;
            py[i] = base_y - 2.5 * sin(M_PI * t);
        }
        plline(25, px, py);
    }
    plwidth(1.0);

    /* Wire from generator to switchyard */
    pipe_line(66, 52, 60, 62, 2, 2.0);
}

/* Water intake */
void draw_water_intake(void)
{
    pipe_line(53, 1, 53, 10, 0, 3.0);
    arrow_up(53, 10, 1.5, 0);
    text_at(53, -1, "Water", 0.55, 15);
}

/* Primary loop pipes (purple - inside containment) */
void draw_primary_pipes(void)
{
    int c = 13;
    PLFLT w = 3.0;

    /* Reactor bottom to pump 1 */
    pipe_line(12, 24, 12, 19, c, w);
    pipe_line(12, 19, 12.5, 16, c, w);

    /* Pump 1 to right, up to steam gen bottom */
    pipe_line(17.5, 16, 23.5, 16, c, w);
    pipe_line(23.5, 16, 23.5, 28, c, w);

    /* Steam gen side to reactor top area */
    pipe_line(20, 42, 16, 42, c, w);

    /* Left side of reactor, down */
    pipe_line(4, 32, 4, 19, c, w);
    pipe_line(4, 32, 8, 32, c, w);
    pipe_line(4, 19, 12, 19, c, w);
}

/* Secondary loop pipes (dark blue - outside containment) */
void draw_secondary_pipes(void)
{
    int c = 9;
    PLFLT w = 3.0;

    /* Pump 2 to steam generator */
    pipe_line(30.5, 40, 27, 40, c, w);

    /* Condenser right side to reservoir */
    pipe_line(64, 20, 70, 20, c, w);

    /* Reservoir top to cooling tower area */
    pipe_line(90, 28, 90, 50, c, w);
    pipe_line(82, 50, 90, 50, c, w);

    /* Cooling tower bottom back to condenser */
    pipe_line(70, 30, 70, 33, c, w);
    pipe_line(64, 33, 70, 33, c, w);

    /* Condenser left side back to pump 2 */
    pipe_line(42, 20, 34, 20, c, w);
    pipe_line(34, 20, 34, 37.5, c, w);

    /* Cooling Water label */
    text_at(67, 17, "Cooling Water", 0.45, 15);
}

/* ================================================================
 * MAIN FUNCTION
 * ================================================================ */
int main(int argc, char *argv[])
{
    /* Initialize PLplot */
    plparseopts(&argc, argv, PL_PARSE_FULL);
    plsdev("xcairo");
    plspage(0, 0, 1100, 700, 0, 0);
    plinit();

    /* Coordinate system */
    plenv(0, 100, -3, 85, 0, -1);

    /* Title */
    text_at(50, 83, "Nuclear Power Plant Process Diagram", 0.8, 15);

    /* Draw back-to-front for proper layering */
    draw_containment();
    draw_building();
    draw_condenser();
    draw_reactor();
    draw_control_rods();
    draw_steam_generator();
    draw_steam_line();
    draw_pumps();
    draw_turbine();
    draw_generator();
    draw_cooling_tower();
    draw_reservoir();
    draw_switchyard();
    draw_water_intake();
    draw_primary_pipes();
    draw_secondary_pipes();

    plend();
    return 0;
}