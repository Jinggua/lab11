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
 * Compile: gcc -o lab11_graphics lab11_graphics.c -lplplotd -lm
 * Run:     ./lab11_graphics
 */

#include <plplot/plplot.h>
#include <math.h>

/* ----------------------------------------------------------------
 * Helper: draw a filled rectangle
 * ---------------------------------------------------------------- */
void draw_filled_rect(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int color)
{
    PLFLT x[4] = {x1, x2, x2, x1};
    PLFLT y[4] = {y1, y1, y2, y2};
    plcol0(color);
    plfill(4, x, y);
    plcol0(15); /* white outline */
    plline(4, x, y);
    /* close the rectangle */
    PLFLT cx[2] = {x1, x1};
    PLFLT cy[2] = {y1, y2};
    plline(2, cx, cy);
}

/* ----------------------------------------------------------------
 * Helper: draw an outlined rectangle (no fill)
 * ---------------------------------------------------------------- */
void draw_rect(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int color)
{
    PLFLT x[5] = {x1, x2, x2, x1, x1};
    PLFLT y[5] = {y1, y1, y2, y2, y1};
    plcol0(color);
    plline(5, x, y);
}

/* ----------------------------------------------------------------
 * Helper: draw a filled circle
 * ---------------------------------------------------------------- */
void draw_filled_circle(PLFLT cx, PLFLT cy, PLFLT r, int color)
{
    int n = 60;
    PLFLT x[61], y[61];
    int i;
    for (i = 0; i <= n; i++) {
        double angle = 2.0 * M_PI * i / n;
        x[i] = cx + r * cos(angle);
        y[i] = cy + r * sin(angle);
    }
    plcol0(color);
    plfill(n + 1, x, y);
    plcol0(15);
    plline(n + 1, x, y);
}

/* ----------------------------------------------------------------
 * Helper: draw a circle outline
 * ---------------------------------------------------------------- */
void draw_circle(PLFLT cx, PLFLT cy, PLFLT r, int color)
{
    int n = 60;
    PLFLT x[61], y[61];
    int i;
    for (i = 0; i <= n; i++) {
        double angle = 2.0 * M_PI * i / n;
        x[i] = cx + r * cos(angle);
        y[i] = cy + r * sin(angle);
    }
    plcol0(color);
    plline(n + 1, x, y);
}

/* ----------------------------------------------------------------
 * Helper: draw a line between two points
 * ---------------------------------------------------------------- */
void draw_line(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int color)
{
    PLFLT x[2] = {x1, x2};
    PLFLT y[2] = {y1, y2};
    plcol0(color);
    plline(2, x, y);
}

/* ----------------------------------------------------------------
 * Helper: draw a pipe (thick line) between two points
 * ---------------------------------------------------------------- */
void draw_pipe(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2, int color)
{
    plcol0(color);
    plwidth(3.0);
    PLFLT x[2] = {x1, x2};
    PLFLT y[2] = {y1, y2};
    plline(2, x, y);
    plwidth(1.0);
}

/* ----------------------------------------------------------------
 * Helper: draw an arrow (for flow direction)
 * ---------------------------------------------------------------- */
void draw_arrow_down(PLFLT cx, PLFLT cy, PLFLT size, int color)
{
    PLFLT x[3] = {cx - size, cx, cx + size};
    PLFLT y[3] = {cy + size, cy - size, cy + size};
    plcol0(color);
    plfill(3, x, y);
}

void draw_arrow_up(PLFLT cx, PLFLT cy, PLFLT size, int color)
{
    PLFLT x[3] = {cx - size, cx, cx + size};
    PLFLT y[3] = {cy - size, cy + size, cy - size};
    plcol0(color);
    plfill(3, x, y);
}

/* ----------------------------------------------------------------
 * Draw the Containment Structure (dome shape)
 * ---------------------------------------------------------------- */
void draw_containment_structure(void)
{
    /* Main rectangular body */
    draw_filled_rect(2, 20, 22, 65, 7);

    /* Dome (half-circle on top) */
    int n = 30;
    PLFLT x[32], y[32];
    int i;
    x[0] = 2;
    y[0] = 65;
    for (i = 0; i <= n; i++) {
        double angle = M_PI * i / n;
        x[i + 1] = 12 - 10 * cos(angle);
        y[i + 1] = 65 + 8 * sin(angle);
    }
    plcol0(7);  /* gray fill */
    plfill(n + 2, x, y);
    plcol0(15); /* outline */
    plline(n + 2, x, y);

    /* Label */
    plcol0(15);
    plschr(0, 0.6);
    plptex(12, 70, 1, 0, 0.5, "CONTAINMENT");
    plptex(12, 67, 1, 0, 0.5, "STRUCTURE");
}

/* ----------------------------------------------------------------
 * Draw the Reactor (circle with internal details)
 * ---------------------------------------------------------------- */
void draw_reactor(void)
{
    /* Reactor vessel - large circle */
    draw_filled_circle(10, 38, 7, 4); /* purple/blue */

    /* Inner details - fuel rods */
    draw_line(8, 33, 8, 43, 14);
    draw_line(10, 33, 10, 43, 14);
    draw_line(12, 33, 12, 43, 14);

    /* Core glow - inner circle */
    draw_filled_circle(10, 38, 3, 5);

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(10, 38, 1, 0, 0.5, "Reactor");
}

/* ----------------------------------------------------------------
 * Draw Control Rods
 * ---------------------------------------------------------------- */
void draw_control_rods(void)
{
    /* Vertical lines above reactor */
    draw_line(8, 45, 8, 55, 15);
    draw_line(10, 45, 10, 55, 15);
    draw_line(12, 45, 12, 55, 15);

    /* Small rectangles at top of rods */
    draw_filled_rect(7, 54, 9, 56, 8);
    draw_filled_rect(9, 54, 11, 56, 8);
    draw_filled_rect(11, 54, 13, 56, 8);

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(4, 52, 1, 0, 0.5, "Control Rods");
}

/* ----------------------------------------------------------------
 * Draw the Steam Generator
 * ---------------------------------------------------------------- */
void draw_steam_generator(void)
{
    /* Main body - tall rectangle */
    draw_filled_rect(17, 42, 23, 60, 6); /* yellow/brown */

    /* Internal tube details */
    int i;
    for (i = 0; i < 4; i++) {
        draw_line(18, 44 + i * 4, 22, 44 + i * 4, 15);
    }

    /* Label */
    plcol0(15);
    plschr(0, 0.45);
    plptex(20, 52, 1, 0, 0.5, "Steam");
    plptex(20, 49, 1, 0, 0.5, "Generator");
}

/* ----------------------------------------------------------------
 * Draw Steam Line
 * ---------------------------------------------------------------- */
void draw_steam_line(void)
{
    /* Horizontal pipe from steam generator to turbine area */
    draw_pipe(23, 62, 48, 62, 7);

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(35, 64, 1, 0, 0.5, "Steam Line");
}

/* ----------------------------------------------------------------
 * Draw Pumps (yellow circles)
 * ---------------------------------------------------------------- */
void draw_pumps(void)
{
    /* Pump 1 - below reactor (bottom left) */
    draw_filled_circle(14, 22, 2.5, 2);  /* yellow */
    plcol0(15);
    plschr(0, 0.45);
    plptex(14, 18, 1, 0, 0.5, "Pump");

    /* Pump 2 - near steam generator (middle) */
    draw_filled_circle(30, 45, 2.5, 2);  /* yellow */
    plcol0(15);
    plschr(0, 0.45);
    plptex(30, 48, 1, 0, 0.5, "Pump");
}

/* ----------------------------------------------------------------
 * Draw the Turbine
 * ---------------------------------------------------------------- */
void draw_turbine(void)
{
    /* Turbine body - trapezoid shape */
    PLFLT x[4] = {42, 54, 56, 40};
    PLFLT y[4] = {40, 40, 50, 50};
    plcol0(7); /* gray */
    plfill(4, x, y);
    plcol0(15);
    plline(4, x, y);
    /* close shape */
    PLFLT cx2[2] = {40, 42};
    PLFLT cy2[2] = {50, 40};
    plline(2, cx2, cy2);

    /* Internal lines for blades */
    int i;
    for (i = 0; i < 5; i++) {
        draw_line(43 + i * 2.5, 41, 43 + i * 2.5, 49, 15);
    }

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(48, 45, 1, 0, 0.5, "Turbine");
}

/* ----------------------------------------------------------------
 * Draw the Generator
 * ---------------------------------------------------------------- */
void draw_generator(void)
{
    /* Generator body - rectangle */
    draw_filled_rect(54, 48, 66, 58, 2); /* yellow */

    /* Label */
    plcol0(15);
    plschr(0, 0.55);
    plptex(60, 53, 1, 0, 0.5, "Generator");
}

/* ----------------------------------------------------------------
 * Draw the Condenser
 * ---------------------------------------------------------------- */
void draw_condenser(void)
{
    /* Main body */
    draw_filled_rect(38, 22, 56, 36, 7);

    /* Down arrows representing cooling */
    int i;
    for (i = 0; i < 5; i++) {
        draw_arrow_down(42 + i * 3, 38, 0.8, 9); /* blue arrows */
    }

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(47, 30, 1, 0, 0.5, "Condenser");
}

/* ----------------------------------------------------------------
 * Draw Cooling Towers
 * ---------------------------------------------------------------- */
void draw_cooling_towers(void)
{
    /* Tower 1 - hourglass/hyperbola shape */
    PLFLT x1[8] = {76, 80, 79, 78, 78, 79, 80, 76};
    PLFLT y1[8] = {20, 20, 30, 40, 50, 60, 70, 70};
    plcol0(7);
    plfill(8, x1, y1);
    plcol0(15);
    plline(8, x1, y1);

    /* Tower 2 */
    PLFLT x2[8] = {82, 86, 85, 84, 84, 85, 86, 82};
    PLFLT y2[8] = {20, 20, 30, 40, 50, 60, 70, 70};
    plcol0(7);
    plfill(8, x2, y2);
    plcol0(15);
    plline(8, x2, y2);

    /* Steam coming out of towers */
    plcol0(15);
    plschr(0, 0.7);
    plptex(78, 73, 1, 0, 0.5, "~");
    plptex(84, 73, 1, 0, 0.5, "~");

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(83, 75, 1, 0, 0.5, "Cooling");
    plptex(83, 72, 1, 0, 0.5, "Towers");
}

/* ----------------------------------------------------------------
 * Draw the Switchyard (electrical towers)
 * ---------------------------------------------------------------- */
void draw_switchyard(void)
{
    /* Tower structure - simple representation */
    draw_line(52, 65, 52, 78, 15);
    draw_line(56, 65, 56, 78, 15);
    draw_line(52, 78, 54, 82, 15);
    draw_line(56, 78, 54, 82, 15);

    /* Cross beams */
    draw_line(52, 70, 56, 70, 15);
    draw_line(52, 75, 56, 75, 15);

    /* Power lines */
    draw_line(48, 76, 52, 76, 2);
    draw_line(56, 76, 62, 76, 2);
    draw_line(48, 73, 52, 73, 2);
    draw_line(56, 73, 62, 73, 2);

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(54, 84, 1, 0, 0.5, "Switchyard");
}

/* ----------------------------------------------------------------
 * Draw the Reservoir
 * ---------------------------------------------------------------- */
void draw_reservoir(void)
{
    /* Water body - irregular shape */
    PLFLT x[8] = {68, 88, 90, 88, 68, 66, 64, 66};
    PLFLT y[8] = {18, 18, 25, 32, 32, 28, 25, 22};
    plcol0(9); /* blue */
    plfill(8, x, y);
    plcol0(15);
    plline(8, x, y);
    /* close */
    PLFLT cx2[2] = {66, 68};
    PLFLT cy2[2] = {22, 18};
    plline(2, cx2, cy2);

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(78, 25, 1, 0, 0.5, "Reservoir");
}

/* ----------------------------------------------------------------
 * Draw Cooling Water label and pipe
 * ---------------------------------------------------------------- */
void draw_cooling_water(void)
{
    /* Pipe from reservoir to condenser */
    draw_pipe(66, 25, 56, 25, 9);
    draw_pipe(66, 29, 56, 29, 9);

    /* Label */
    plcol0(15);
    plschr(0, 0.5);
    plptex(62, 20, 1, 0, 0.5, "Cooling Water");
}

/* ----------------------------------------------------------------
 * Draw Water intake at bottom
 * ---------------------------------------------------------------- */
void draw_water_intake(void)
{
    /* Arrow coming up from bottom */
    draw_pipe(47, 8, 47, 22, 9);
    draw_arrow_up(47, 22, 1.2, 9);

    /* Label */
    plcol0(15);
    plschr(0, 0.55);
    plptex(47, 5, 1, 0, 0.5, "Water");
}

/* ----------------------------------------------------------------
 * Draw connecting pipes between components
 * ---------------------------------------------------------------- */
void draw_pipes(void)
{
    /* Reactor to Steam Generator (left side internal pipes) */
    draw_pipe(17, 45, 17, 55, 1);  /* red - hot water up */
    draw_pipe(10, 31, 10, 22, 9);  /* blue - water down to pump */
    draw_pipe(10, 22, 14, 22, 9);  /* to pump */
    draw_pipe(14, 22, 22, 22, 9);  /* pump to right */
    draw_pipe(22, 22, 22, 42, 9);  /* up to steam gen */

    /* Steam Generator to Steam Line */
    draw_pipe(20, 60, 20, 62, 7);

    /* Turbine to Generator connection */
    draw_pipe(54, 45, 54, 48, 15);

    /* Turbine down to Condenser */
    draw_pipe(48, 40, 48, 36, 9);

    /* Condenser output - back toward containment */
    draw_pipe(38, 29, 30, 29, 9);
    draw_pipe(30, 29, 30, 45, 9);  /* up to pump */
    draw_pipe(30, 45, 23, 45, 9);  /* to steam gen */

    /* Generator to Switchyard */
    draw_pipe(60, 58, 54, 65, 2);

    /* Cooling towers to reservoir */
    draw_pipe(78, 20, 78, 18, 9);
    draw_pipe(84, 20, 84, 18, 9);
}

/* ----------------------------------------------------------------
 * Main function
 * ---------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    /* Initialize PLplot */
    plparseopts(&argc, argv, PL_PARSE_FULL);

    /* Set output device - use xcairo for screen display */
    plsdev("xcairo");

    /* Set page size */
    plspage(0, 0, 900, 600, 0, 0);

    /* Initialize */
    plinit();

    /* Set up coordinate system: 0-100 x, 0-90 y */
    plenv(0, 100, 0, 90, 0, -1);

    /* Set background color */
    plcol0(0);

    /* Title */
    plcol0(15);
    plschr(0, 0.8);
    plptex(50, 87, 1, 0, 0.5, "Nuclear Power Plant Process Diagram");

    /* Draw all components */
    draw_containment_structure();
    draw_reactor();
    draw_control_rods();
    draw_steam_generator();
    draw_steam_line();
    draw_pumps();
    draw_turbine();
    draw_generator();
    draw_condenser();
    draw_cooling_towers();
    draw_switchyard();
    draw_reservoir();
    draw_cooling_water();
    draw_water_intake();
    draw_pipes();

    /* Close PLplot */
    plend();

    return 0;
}