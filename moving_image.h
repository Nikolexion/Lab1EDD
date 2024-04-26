#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include<queue>
#include<utility>
#include<stack>
#include<unistd.h>
#include <sstream>

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image {
private:
  unsigned char **red_layer; // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer; // Capa de tonalidades azules

  std::stack<std::pair<int,int>> ctrlz; // Ocupamos un stack para guardar los movimientos
                                        // hechos por el usuario para poder insertar y sacar
                                        // datos de la forma mas optima
  std::stack<std::pair<int,int>> ctrlmz;  // Ocupamos un stack para guardar los movimientos
                                          // hechos con el undo
  std::queue<std::pair<int,int>> rep; // Ocupamos un deque para guardar todos los movimientos
  int contador = 2;
  bool se_repitio = false;  // Ocupado para que si es una función como undo, 
                            // redo o repeat no agregue elementos a la cola de repeat_all

public:
  // Enum de acciones posibles
  enum Action {
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        ROTATE,
        UNDO,
        REDO,
        REPEAT
    };

  // Constructor de la imagen. Se crea una imagen por defecto
  moving_image() {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char*[H_IMG];
    green_layer = new unsigned char*[H_IMG];
    blue_layer = new unsigned char*[H_IMG];
    
    for(int i=0; i < H_IMG; i++) {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    // Llenamos la imagen con su color de fondo
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
        red_layer[i][j] = DEFAULT_R;
        green_layer[i][j] = DEFAULT_G;
        blue_layer[i][j] = DEFAULT_B;
      }

    // Dibujamos el objeto en su posición inicial
    for(int i=0; i < 322; i++)
      for(int j=0; j < 256; j++) {
        if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
          red_layer[INIT_Y+i][INIT_X+j] = DEFAULT_R;
          green_layer[INIT_Y+i][INIT_X+j] = DEFAULT_G;
          blue_layer[INIT_Y+i][INIT_X+j] = DEFAULT_B;
        } else {
          red_layer[INIT_Y+i][INIT_X+j] = s_R[i][j];
          green_layer[INIT_Y+i][INIT_X+j] = s_G[i][j];
          blue_layer[INIT_Y+i][INIT_X+j] = s_B[i][j];
        }
      }
  }

  // Destructor de la clase
  ~moving_image() {
    for(int i=0; i < H_IMG; i++) {
      delete red_layer[i];
      delete green_layer[i];
      delete blue_layer[i];
    }

    delete red_layer;
    delete green_layer;
    delete blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char* nb) {
    _draw(nb);
  }

  // Función que simila desplazar la imagen, de manera circular, d pixeles a la izquierda
  void move_left(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	      tmp_layer[i][j] = red_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][j] = red_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	  tmp_layer[i][j] = green_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][j] = green_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	  tmp_layer[i][j] = blue_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][j] = blue_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  blue_layer[i][j] = tmp_layer[i][j];

    std::pair<int,int> temp;
    temp.first = MOVE_LEFT;
    temp.second = d;
    ctrlz.push(temp);
    if(!se_repitio){
      rep.push(temp);
    } else {
      se_repitio = false;
    }
    
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }
  // Función que simila desplazar la imagen, de manera circular, d pixeles a la derecha
  void move_right(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
        for(int j=d; j < W_IMG; j++)
            tmp_layer[i][j] = red_layer[i][j-d];
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0, k=W_IMG-d; j < d; j++, k++)
            tmp_layer[i][j] = red_layer[i][k];
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
        for(int j=d; j < W_IMG; j++)
            tmp_layer[i][j] = green_layer[i][j-d];  
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0, k=W_IMG-d; j < d; j++, k++)
            tmp_layer[i][j] = green_layer[i][k];  
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
        for(int j=d; j < W_IMG; j++)
            tmp_layer[i][j] = blue_layer[i][j-d];
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0, k=W_IMG-d; j < d; j++, k++)
            tmp_layer[i][j] = blue_layer[i][k];
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];

    // Agregamos la accion a la pila de undo
    std::pair<int,int> temp;
    temp.first = MOVE_RIGHT;
    temp.second = d;
    ctrlz.push(temp);
    // Antes de agregarla a la cola de repeat_all verificamos que no venga de undo, redo o repeat
    // Si viene de alguno de estos métodos, no se agrega a la cola
    if(!se_repitio) rep.push(temp);
    else se_repitio = false;
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }
  // Función que simila desplazar la imagen, de manera circular, d pixeles hacia arriba
  void move_up(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = red_layer[i+d][j];      
    
    for(int i=H_IMG - d, k=0; i < H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = red_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = green_layer[i+d][j];      

    for(int i=H_IMG - d, k=0; i < H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = green_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = blue_layer[i+d][j];     

    for(int i=H_IMG - d, k=0; i < H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = blue_layer[k][j];   

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  blue_layer[i][j] = tmp_layer[i][j];
    
    // Agregamos la accion a la pila de undo
    std::pair<int,int> temp;
    temp.first = MOVE_UP;
    temp.second = d;
    ctrlz.push(temp);
    // Antes de agregarla a la cola de repeat_all verificamos que no venga de undo, redo o repeat
    // Si viene de alguno de estos métodos, no se agrega a la cola
    if(!se_repitio) rep.push(temp);
    else se_repitio = false;
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }
  // Función que simila desplazar la imagen, de manera circular, d pixeles hacia abajo
  void move_down(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=d; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	    tmp_layer[i][j] = red_layer[i-d][j];      
    
    for(int i=0, k=H_IMG - d; i < d; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = red_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	    red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=d; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = green_layer[i-d][j];      
    
    for(int i=0, k=H_IMG - d; i < d; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = green_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=d; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = blue_layer[i-d][j];      
    
    for(int i=0, k=H_IMG - d; i < d; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = blue_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	blue_layer[i][j] = tmp_layer[i][j];

    // Agregamos la accion a la pila de undo
    std::pair<int,int> temp;
    temp.first = MOVE_DOWN;
    temp.second = d;
    ctrlz.push(temp);
    // Antes de agregarla a la cola de repeat_all verificamos que no venga de undo, redo o repeat
    // Si viene de alguno de estos métodos, no se agrega a la cola
    if(!se_repitio) rep.push(temp);
    else se_repitio = false;
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }
  // Función que simila desplazar la imagen rotando antihorario en 90°
  void rotate(){
    // Agregamos la accion a la pila de undo
    std::pair<int,int> temp;
    temp.first = ROTATE;
    temp.second = 0;
    ctrlz.push(temp);
    // Antes de agregarla a la cola de repeat_all verificamos que no venga de undo, redo o repeat
    // Si viene de alguno de estos métodos, no se agrega a la cola
    if(!se_repitio) rep.push(temp);
    else se_repitio = false;

    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];

    // Mover la capa roja
    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[H_IMG - 1 - j][i] = red_layer[i][j];

    for(int i = 0; i < H_IMG; i++) 
        for(int j = 0; j < W_IMG; j++) 
            red_layer[i][j] = tmp_layer[i][j];
        
    // Mover la capa verde
    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[H_IMG - 1 - j][i] = green_layer[i][j];
      
    for(int i = 0; i < H_IMG; i++) 
        for(int j = 0; j < W_IMG; j++) 
            green_layer[i][j] = tmp_layer[i][j];
        
    // Mover la capa azul
    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[H_IMG - 1 - j][i] = blue_layer[i][j];
    
    for(int i = 0; i < H_IMG; i++) 
        for(int j = 0; j < W_IMG; j++) 
            blue_layer[i][j] = tmp_layer[i][j];
        
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }
  // Función que simila desplazar la imagen rotando horario en 90°, usada para undo
    void rotate_inv(){
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];

    // Mover la capa roja
    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[j][H_IMG - 1 - i] = red_layer[i][j];
      
    for(int i = 0; i < H_IMG; i++) 
        for(int j = 0; j < W_IMG; j++) 
            red_layer[i][j] = tmp_layer[i][j];
        
    // Mover la capa verde
    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[j][H_IMG - 1 - i] = green_layer[i][j];    
    
    for(int i = 0; i < H_IMG; i++) 
        for(int j = 0; j < W_IMG; j++) 
            green_layer[i][j] = tmp_layer[i][j];
        
    // Mover la capa azul
    for(int i = 0; i < H_IMG; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[j][H_IMG - 1 - i] = blue_layer[i][j];
      
    for(int i = 0; i < H_IMG; i++) 
        for(int j = 0; j < W_IMG; j++) 
            blue_layer[i][j] = tmp_layer[i][j];
  }

  void undo(){
    // No hacemos nada si el stack está vacío
    if (ctrlz.size() == 0) return;
    
    std::pair<int,int> temp =  ctrlz.top();
    ctrlz.pop();

    // Guardamos el par de datos correspondientes a undo en la queue relacionada a repeat_all
    std::pair<int,int> temp2;
    temp2.first = UNDO;
    temp2.second =0;
    rep.push(temp2);

    // Hacemos un respaldo del stack donde se guardan los datos para redo y asi no perder los datos de él
    std::pair<int,int> temp_redo;
    std::stack<std::pair<int,int>> respaldo_redo;
    respaldo_redo.swap(ctrlmz);

    // Accedemos a un switch para elegir la operación contraria a la última ejecutada
    switch (temp.first){
    case 0:
      temp_redo.first = MOVE_LEFT;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      se_repitio = true;
      move_right(temp.second);
      ctrlz.pop();
      break;
      
    case 1:
      temp_redo.first = MOVE_RIGHT;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      se_repitio = true;
      move_left(temp.second);
      ctrlz.pop();
      break;

    case 2:
      temp_redo.first = MOVE_UP;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      se_repitio = true;
      move_down(temp.second);
      ctrlz.pop();
      break;

    case 3:
      temp_redo.first = MOVE_DOWN;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      se_repitio = true;
      move_up(temp.second);
      ctrlz.pop();
      break;

    case 4:
      temp_redo.first = ROTATE;
      temp_redo.second = 0;
      respaldo_redo.push(temp_redo);
      se_repitio = true;
      rotate_inv();
      ctrlz.pop();
      break;

    default:
      break;
    }
    respaldo_redo.swap(ctrlmz);
  }

  void redo(){
    // No hacemos nada si no hay nada en el stack relacionado a redo
    if (ctrlmz.size() == 0) return;
    
    std::pair<int,int> temp = ctrlmz.top();

    // Guardamos el par de datos correspondientes a redo en la queue relacionada a repeat_all
    std::pair<int,int> temp2;
    temp2.first = REDO;
    temp2.second = 0;
    rep.push(temp2);
    
    // Hacemos un respaldo del stack relacionado con redo para poder hacer múltiples
    // redo sin perder la información
    std::stack<std::pair<int,int>> respaldo_redo;
    respaldo_redo.swap(ctrlmz);

    // Entramos al switch para rehacer la acción
    switch (temp.first){
    case MOVE_LEFT:
      se_repitio = true;
      move_left(temp.second);
      break;

    case MOVE_RIGHT:
      se_repitio = true;
      move_right(temp.second);
      break;

    case MOVE_UP:
      se_repitio = true;
      move_up(temp.second);
      break;

    case MOVE_DOWN:
      se_repitio = true;
      move_down(temp.second);
      break;

    case ROTATE:
      se_repitio = true;
      rotate();

    default:
      break;
    }
    
    // Recuperamos los datos del stack relacionado con redo
    respaldo_redo.swap(ctrlmz);
    ctrlmz.pop();
  }

  void repeat(){
    // Guardamos el par de datos correspondientes a repeat en la queue relacionada a repeat_all
    std::pair<int,int> temp2;
    temp2.first = REPEAT;
    temp2.second = 0;
    rep.push(temp2);
    std::pair<int,int> temp =  ctrlz.top();

    switch (temp.first){
    case MOVE_LEFT:
      se_repitio = true;
      move_left(temp.second);
      break;
    
    case MOVE_RIGHT:
      se_repitio = true;
      move_right(temp.second);
      break;

    case MOVE_UP:
      se_repitio = true;
      move_up(temp.second);
      break;

    case MOVE_DOWN:
      se_repitio = true;
      move_down(temp.second);
      break;
    
    case ROTATE:
      se_repitio = true;
      rotate();
      break;
    
    default:
      break;
    }
  }

  void repeat_all(){
    // Verificamos si la queue para el repeat_all esta vacia
    if (rep.empty())
      return;

    // Borramos los datos anteriormente guardados en el stack ctrlz
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlz);
    
    // Hacemos reset para que la imagen vuelva a su estado inicial
    reset_image();
    draw("imagen1.png");

    // Empezamos a iterar por toda la queue de repeat_all ejecutando todas las acciones de nuevo
    // y guardando cada una en un archivo distinto
    size_t count = rep.size();
    for(int i = 0; i < count; i++){
      std::pair<int,int> temp = rep.front();
      rep.pop();
      switch (temp.first){
        case MOVE_LEFT:
          move_left(temp.second);
          break;

        case MOVE_RIGHT:
          move_right(temp.second);
          break;

        case MOVE_UP:
          move_up(temp.second);
          break;

        case MOVE_DOWN:
          move_down(temp.second);
          break;

        case ROTATE:
          rotate();
          break;

        case UNDO:
          undo();
          break;

        case REDO:
          redo();
          break;

        case REPEAT:
          repeat();
          break;

        default:
          break;
        }
      //Le cambiamos el nombre al nuevo archivo cada vez que hacemos una iteracion
      std::stringstream ss;
      ss << "imagen" << contador << ".png";
      std::string nombre_imagen = ss.str();
      const char* nombre_imagen_cstr = nombre_imagen.c_str();
      draw(nombre_imagen_cstr);
      contador++;
      sleep(1);
    }
  }

  //Reutilizamos el codigo que hay en el constructor para devolver la imagen al inicio
  void reset_image(){ 
    // Llenamos la imagen con su color de fondo
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
        red_layer[i][j] = DEFAULT_R;
        green_layer[i][j] = DEFAULT_G;
        blue_layer[i][j] = DEFAULT_B;
      }

    // Dibujamos el objeto en su posición inicial
    for(int i=0; i < 322; i++)
      for(int j=0; j < 256; j++) {
        if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
          red_layer[INIT_Y+i][INIT_X+j] = DEFAULT_R;
          green_layer[INIT_Y+i][INIT_X+j] = DEFAULT_G;
          blue_layer[INIT_Y+i][INIT_X+j] = DEFAULT_B;
        } else {
          red_layer[INIT_Y+i][INIT_X+j] = s_R[i][j];
          green_layer[INIT_Y+i][INIT_X+j] = s_G[i][j];
          blue_layer[INIT_Y+i][INIT_X+j] = s_B[i][j];
        }
      }
  }


private:
  // Función privada que guarda la imagen en formato .png
  void _draw(const char* nb) {
    //    unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
    unsigned char *rgb = new unsigned char[H_IMG * W_IMG * 3];
    unsigned char *p = rgb;
    unsigned x, y;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");

    // Transformamos las 3 matrices en una arreglo unidimensional
    for (y = 0; y < H_IMG; y++)
        for (x = 0; x < W_IMG; x++) {
            *p++ = red_layer[y][x];    /* R */
            *p++ = green_layer[y][x];    /* G */
            *p++ = blue_layer[y][x];    /* B */
        }
    // La función svpng() transforma las 3 matrices RGB en una imagen PNG 
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
}
};

#endif
