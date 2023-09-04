#ifndef UI_TEXTURE_
#define UI_TEXTURE_

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct Texture {
    GLuint texture_id;
} Texture;

Texture texture_load_repeat(const char* path);
Texture texture_load_clamp(const char* path);
void texture_free(Texture texture);


typedef struct TextureArray {
    GLuint texture_id;
} TextureArray;

TextureArray texture_array_load_clamp(const char* paths[], int count);
void texture_array_free(TextureArray array);


#endif //UI_TEXTURE_
