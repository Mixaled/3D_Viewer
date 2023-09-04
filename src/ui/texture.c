#include "texture.h"
#include <stb_image.h>

#include "../util/prettify_c.h"

static Texture texture_load_common(const char* path, GLenum wrap);

Texture texture_load_clamp(const char* path) {
    return texture_load_common(path, GL_CLAMP_TO_EDGE);
}

Texture texture_load_repeat(const char* path) {
    return texture_load_common(path, GL_REPEAT);
}

void texture_free(Texture texture) {
    glDeleteTextures(1, &texture.texture_id);
}

static GLenum format_by_channels(int channels) {
    GLenum format = GL_RGBA;
    switch (channels) {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: panic("Image has %d channes which is unsupported!", channels);
    }
    return format;
}

static Texture texture_load_common(const char* path, GLenum wrap) {
    int x, y, channels;
    GLuint tex;
    unsigned char *data = stbi_load(path, &x, &y, &channels, 0);

    if (!data) panic("texture_load: Failed to load image: %s", path);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, format_by_channels(channels), GL_UNSIGNED_BYTE,
                data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return (Texture) { tex };
}

typedef struct StbImage {
    unsigned char* bytes;
} StbImage;

void stb_image_data_free(StbImage image) {
    stbi_image_free(image.bytes);
}

#define VECTOR_ITEM_TYPE StbImage
#define VECTOR_IMPLEMENTATION
#include "../util/vector.h"


TextureArray texture_array_load_clamp(const char* paths[], int count) {
    vec_StbImage images = vec_StbImage_with_capacity(count);
    int width, height, channels;

    for (int i = 0; i < count; i++) {
        int local_width, local_height, local_channels;
        unsigned char *data = stbi_load(paths[i], &local_width, &local_height, &local_channels, 0);

        if (i > 0 and (local_width != width or local_height != height or local_channels != channels))
            panic(
                "All images in TextureArray must have the same size and channels count. "
                "Bad image id: %d. Total WHC: %d %d %d. Image WHC: %d %d %d",
                i, width, height, channels, local_width, local_height, local_channels
            );

        width = local_width;
        height = local_height;
        channels = local_channels;

        vec_StbImage_push(&images, (StbImage) {data});
    }


    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);

    GLenum format = format_by_channels(channels);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, count, 0, format, GL_UNSIGNED_BYTE, null);

    for (int i = 0; i < count; i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, 
            format, GL_UNSIGNED_BYTE, images.data[i].bytes);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    vec_StbImage_free(images);
    return (TextureArray) {tex};
}

void texture_array_free(TextureArray array) {
    glDeleteTextures(1, &array.texture_id);
}