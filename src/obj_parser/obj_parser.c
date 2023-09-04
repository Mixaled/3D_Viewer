#include "obj_parser.h"

#include <stdio.h>
#include <string.h>

#include "../util/better_io.h"
#include "../util/prettify_c.h"

#define VECTOR_C FaceIndex
#include "../util/vector.h"

#define VECTOR_C Face
#define VECTOR_ITEM_DESTRUCTOR face_free
#include "../util/vector.h"

#define VECTOR_C Vertex
#include "../util/vector.h"

#define VECTOR_C Normal
#include "../util/vector.h"

static int scan_type(const char* line);
static Vertex parse_vertex(const char* line);

static vec_FaceIndex parse_indices(const char* line, int cur_vertices_count);

void face_print(const Face* face, OutStream os) {
  for (size_t i = 1; i < (size_t)face->indices.length; i++) {
    FaceIndex index = face->indices.data[i];
    x_sprintf(os, "%d/%d/%d ", index.point, index.texture_pos, index.normal);
  }
}

// Free all the owned resources
void face_free(Face face) { vec_FaceIndex_free(face.indices); }

#define TYPE1 1
#define TYPE2 2
#define TYPE3 3
#define TYPE4 4

static int scan_type(const char* line) {
  int vCount = 0;
  for (int i = 0; line[i] != ' '; i++) {
    if (line[i] == '/') vCount++;
    if (line[i] == '/' and line[i + 1] == '/') return TYPE4;  // type 4
  }
  if (vCount == 0)
    return TYPE1;  // Type 1
  else if (vCount == 1)
    return TYPE2;  // Type 2
  else if (vCount == 2)
    return TYPE3;  // Type 3
  else
    panic("Unsupported type");  // Unsupported type
}

static char* my_strdup(const char* text) {
  size_t len = strlen(text);
  char* buffer = (char*)malloc((len + 1) * sizeof(char));
  strcpy(buffer, text);
  return buffer;
}

static vec_FaceIndex parse_indices(const char* line, int cur_vertices_count) {
    int type = 0;

    type = scan_type(line);

    vec_FaceIndex indices = vec_FaceIndex_create();
    char* line_tmp = my_strdup(line); // дубликат строки тк strtok может уродовать строку и насиловать
    char* token = strtok(line_tmp, " "); // обрезка f 
    
    while (token != NULL){
        FaceIndex index;

        if (type is TYPE1){
            sscanf(token, "%d", &index.point); // it is v
            index.texture_pos = 0;
            index.normal = 0;
        } else if (type is TYPE2) {
            sscanf(token, "%d/%d", &index.point, &index.texture_pos); // it is v/vt
            index.normal = 0;
        } else if (type is TYPE3) {
            sscanf(token, "%d/%d/%d", &index.point, &index.texture_pos, &index.normal); // it is v/vt/vn
        } else if (type is TYPE4) {
            sscanf(token, "%d//%d",  &index.point, &index.normal);// it is v//vn
            index.texture_pos = 0;
        } else {
            panic("Unsupported index type");
        }

        if (index.point < 0)
            index.point = ((index.point % cur_vertices_count) + cur_vertices_count) % cur_vertices_count;

        vec_FaceIndex_push(&indices, index);
        token = strtok(NULL, " "); //последовательное извлечение каждого токена
    } free(line_tmp);
    return indices; 
}

static Vertex parse_vertex(const char* line) {
  Vertex vertex;
  if (sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z) == 3) {
    return vertex;
  } else {
    panic("Unsupported vertex format!");
  }
}

static Normal parse_normal(const char* line) {
  Normal n;
  if (sscanf(line, "vn %f %f %f", &n.x, &n.y, &n.z) is 3) {
    return n;
  } else {
    panic("Unsupported vertex format!");
  }
}

ObjModel obj_parse_model(const char* filepath) {
    vec_Vertex vertices = vec_Vertex_create();
    vec_Face faces = vec_Face_create();
    vec_Normal normals = vec_Normal_create();

    FILE *file = fopen(filepath, "r");
    assert_m(file and "Failed to open file"); 

    char line[10240];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vertex v = parse_vertex(line);
            vec_Vertex_push(&vertices, v);
        }
        if (strncmp(line, "vn ", 3) is 0) {
            Normal n = parse_normal(line);
            vec_Normal_push(&normals, n);
        }
        if (line[0] == 'f' && line[1] == ' ') {
            vec_FaceIndex indices = parse_indices(line + 2, vertices.length); // +2 to skip 'f '
            Face face = {.indices = indices};
            vec_Face_push(&faces, face); 
        } 
    } 
    fclose(file);

    return (ObjModel) {
        .faces = faces,
        .vertices = vertices,
        .normals = normals,
    };
}

void obj_model_free(ObjModel mdl) {
  vec_Face_free(mdl.faces);
  vec_Vertex_free(mdl.vertices);
  vec_Normal_free(mdl.normals);
}
