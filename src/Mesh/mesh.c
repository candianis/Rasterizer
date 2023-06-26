#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "../DArray/array.h"

mesh_t mesh = {
  .vertices = NULL,
  .tex_coords = NULL,
  .normals = NULL,
  .faces = NULL,
  .rotation = { 0, 0, 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
  { .x = -1, .y = -1, .z = -1 }, //1
  { .x = -1, .y =  1, .z = -1 }, //2
  { .x =  1, .y =  1, .z = -1 }, //3
  { .x =  1, .y = -1, .z = -1 }, //4
  { .x =  1, .y =  1, .z =  1 }, //5
  { .x =  1, .y = -1, .z =  1 }, //6
  { .x = -1, .y =  1, .z =  1 }, //7
  { .x = -1, .y = -1, .z =  1 }  //8
};

face_t cube_faces[N_CUBE_FACES] = {
    //front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    //right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    //back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    //left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    //top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    //bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

void load_cube_mesh_data(void) {
  for(size_t i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }
  for(size_t i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cube_faces[i];
    array_push(mesh.faces, cube_face);
  }
}

void load_obj_file(char* filename) {
  FILE* file_id;
  //int size = 1024, pos;
  char line[256];

  if((file_id = fopen(filename, "r")) == NULL) {
    printf("There was an error trying to open the file \n");
    return;
  }

  while(fgets(line, sizeof(line), file_id)) {
    switch(line[0]){
      case 'v':
        if(line[1] == ' ') {
          //Letter is used to correctly separate the information inside each line with a v(space) 
          char letter;
          vec3_t vertex;
          sscanf(line, "%c %f %f %f", &letter, &vertex.x, &vertex.y, & vertex.z);
          array_push(mesh.vertices, vertex);
        }
        
        else if(line[1] == 't') {
          char letter;
          vec2_t tex_coord;
          sscanf(line, "%c %c %f %f", &letter, &letter, &tex_coord.x, &tex_coord.y);
          array_push(mesh.tex_coords, tex_coord);
        }

        else if(line[1] == 'n') {
          char letter;
          vec3_t normal;
          sscanf(line, "%c %c %f %f %f", &letter, &letter, &normal.x, &normal.y, &normal.z);
          array_push(mesh.normals, normal);
        }

        break;

      case 'f':
        char* saveptr = NULL;
        char* token;
        face_t face;
        //This first assignment of token returns f so the second assignment gives us the index information like 1/1/1 and the second would return 2/2/1 inside cube.obj
        token = strtok_r(line, " ", &saveptr);
        token = strtok_r(NULL, " ", &saveptr);
        /* 
        //The next five lines allow us to get each vvalue inside for example 1/1/1
        int face_indexes[3];
        face_indexes[0] = token[0] - '0';
        face_indexes[1] = token[2] - '0';
        face_indexes[2] = token[4] - '0';
        printf("Test values: %d %d %d\n", face_indexes[0], face_indexes[1], face_indexes[2]);
        */

        sscanf(token, "%d", &face.a);

        token = strtok_r(NULL, " ", &saveptr);
        sscanf(token, "%d", &face.b);

        token = strtok_r(NULL, " ", &saveptr);
        sscanf(token, "%d", &face.c);

        array_push(mesh.faces, face);
               
        break;

      default:

      break;
    }
  }

  fclose(file_id);
}