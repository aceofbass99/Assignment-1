/*These functions implement the functions called in the fractalsgui program. These include
 * drawing a Sierpinski triangle and flood-filling an area based on its color.
 *
 * Name: Noah Bailyn
 * Section leader: Gianna Chien
*/

#include "fractals.h"
using namespace std;

int floodFillHelper(GWindow& window, int x, int y, int color, int &surrounding, int &numPixels);

/*This method draws a Sierpinski triangle. This is done by drawing three lines making a
 * triangle every time the function is called, and calling the function three times
 * for each order of the triangle given. A triangle of order n will have 3^(n+1) + 1
 * triangles, counting the big one drawn at the start.
*/
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if(order > 0) {
        drawSierpinskiTriangle(window, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 2, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 4, y + size / 2, size / 2, order - 1);
        window.drawLine(x, y, x + size, y);
        window.drawLine(x, y, x + size / 2, y + size);
        window.drawLine(x + size / 2, y + size, x + size, y);
    }
}

/*This method checks if the color of the pixel given is the same as the color surrounding the
 * initial pixel clicked on, and then calls floodFillHelper if it is. This is how that function
 * recurs.
*/
void checkIfColor(GWindow& window, int x, int y, int color, int &surrounding, int &numPixels) {
    if(window.getPixel(x, y) == surrounding) {
        floodFillHelper(window, x, y, color, surrounding, numPixels);
    }
}

/*This is the helper method for the floodFill method. It sets a pixel to the selected color,
 * and then checks if the surrounding pixels are the chosen color using the checkIfColor method.
 * Every time a pixel is filled in, the numPixels increments, and is returned after all the
 * drawing finishes.
*/
int floodFillHelper(GWindow& window, int x, int y, int color, int &surrounding, int &numPixels) {
    window.setPixel(x, y, color);
    numPixels++;
    if(x > 0) {
        checkIfColor(window, x - 1, y, color, surrounding, numPixels);
    }
    if(x < window.getCanvasWidth()) {
        checkIfColor(window, x + 1, y, color, surrounding, numPixels);
    }
    if(y > 0) {
        checkIfColor(window, x, y - 1, color, surrounding, numPixels);
    }
    if(y < window.getCanvasHeight()) {
        checkIfColor(window, x, y + 1, color, surrounding, numPixels);
    }
    return numPixels;
}

/*The floodFill function fills in an area by calling its helper function, which
 * recursively checks if the four surrounding pixels have been filled in. It
 * returns the total number of pixels filled.
*/
int floodFill(GWindow& window, int x, int y, int color) {
    if(window.inCanvasBounds(x, y)){
        int surrounding = window.getPixel(x, y);
        int numPixels = 0;
        int value = floodFillHelper(window, x, y, color, surrounding, numPixels);
        return value;
    } else {
        throw "x and/or y are out of bounds.";
        return 0;
    }
}
