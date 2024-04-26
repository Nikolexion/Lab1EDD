/* Compilación: g++ -g -o img test.cpp
 * Ejecución: ./img
 */

#include "moving_image.h"
#include <unistd.h>

int main() {
  moving_image im;
  im.move_right(100);
  im.draw("imagen.png");
  sleep(1);
  im.undo();
  im.draw("imagen.png");
  sleep(1);
  im.redo();
  im.draw("imagen.png");
  sleep(1);
  im.rotate();
  im.draw("imagen.png");
  sleep(1);
  im.repeat();
  im.draw("imagen.png");
  sleep(1);
  im.move_up(100);
  im.draw("imagen.png");
  sleep(1);
  im.move_down(100);
  im.draw("imagen.png");
  sleep(1);
  im.move_left(100);
  im.draw("imagen.png");
  sleep(1);
  im.rotate();
  im.draw("imagen.png");
  sleep(1);
  im.undo();
  im.draw("imagen.png");
  sleep(1);
  
  im.repeat_all();
  
  return 0;
}
