#ifndef OBJ_PARSER_H_
#define OBJ_PARSER_H_


// SINGLE INDEX
typedef struct FaceIndex {
    int point, texture_pos, normal;
} FaceIndex;

#define VECTOR_H FaceIndex
#include "../util/vector.h" // vec_FaceIndex

// THE WHOLE 'f'ace line
typedef struct Face {
    vec_FaceIndex indices;
} Face;

void face_free(Face face);

#define VECTOR_H Face
#include "../util/vector.h" // vec_Face


// VERTEX
typedef struct {
    float x,y,z;
} Vertex;

#define VECTOR_H Vertex
#include "../util/vector.h" // vec_Vertex 



// Normal
typedef struct {
    float x,y,z;
} Normal;

#define VECTOR_H Normal
#include "../util/vector.h" // vec_Normal 


void face_print(const Face* face, OutStream os);

typedef struct ObjModel {
    vec_Vertex vertices;
    vec_Face faces;
    vec_Normal normals;
} ObjModel;

ObjModel obj_parse_model(const char* filepath);
void obj_model_free(ObjModel mdl);

#endif // OBJ_PARSER_H_