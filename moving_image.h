#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include<deque>
#include<utility>
#include<stack>

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image {
private:
  unsigned char **red_layer; // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer; // Capa de tonalidades azules
  std::deque<std::pair<int,int>> ctrlz;
  std::stack<std::pair<int,int>> ctrlmz;

public:
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

  // Función que similar desplazar la imagen, de manera circular, d pixeles a la izquierda
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
    temp.first = 0;
    temp.second = d;
    ctrlz.push_back(temp);
    
    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }

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
            tmp_layer[i][j] = green_layer[i][j-d];  // Movemos los píxeles hacia la derecha
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0, k=W_IMG-d; j < d; j++, k++)
            tmp_layer[i][j] = green_layer[i][k];  // Copiamos los píxeles de la izquierda al final
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
        for(int j=d; j < W_IMG; j++)
            tmp_layer[i][j] = blue_layer[i][j-d];  // Movemos los píxeles hacia la derecha
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0, k=W_IMG-d; j < d; j++, k++)
            tmp_layer[i][j] = blue_layer[i][k];  // Copiamos los píxeles de la izquierda al final
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];

    std::pair<int,int> temp;
    temp.first = 1;
    temp.second = d;
    ctrlz.push_back(temp);

    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }

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

    std::pair<int,int> temp;
    temp.first = 2;
    temp.second = d;
    ctrlz.push_back(temp);

    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }

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
    
    std::pair<int,int> temp;
    temp.first = 3;
    temp.second = d;
    ctrlz.push_back(temp);

    std::stack<std::pair<int,int>> basura;
    basura.swap(ctrlmz);
  }

  void undo(){
    if (ctrlz.size() == 0) return;
    

    std::pair<int,int> temp =  ctrlz.back();
    ctrlz.pop_back();

    std::pair<int,int> temp_redo;
    std::stack<std::pair<int,int>> respaldo_redo;
    respaldo_redo.swap(ctrlmz);

    switch (temp.first){
    case 0:
      temp_redo.first = 0;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      move_right(temp.second);
      ctrlz.pop_back();

      break;
    case 1:
      temp_redo.first = 1;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      move_left(temp.second);
      ctrlz.pop_back();
      break;
    case 2:
      temp_redo.first = 2;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      move_down(temp.second);
      ctrlz.pop_back();
      break;
    case 3:
      temp_redo.first = 3;
      temp_redo.second = temp.second;
      respaldo_redo.push(temp_redo); 
      move_up(temp.second);
      ctrlz.pop_back();
    default:
      break;
    }
    respaldo_redo.swap(ctrlmz);
  }

  void redo(){
    if (ctrlmz.size() == 0) return;
    
    std::pair<int,int> temp = ctrlmz.top();
    std::stack<std::pair<int,int>> respaldo_redo;
    respaldo_redo.swap(ctrlmz);
    switch (temp.first){
    case 0:
      move_left(temp.second);
      break;
    case 1:
      move_right(temp.second);
      break;
    case 2:
      move_up(temp.second);
      break;
    case 3:
      move_down(temp.second);
      break;
    default:
      break;
    }
    
    respaldo_redo.swap(ctrlmz);
    ctrlmz.pop();
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
