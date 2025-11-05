
#pragma once

#include "imgui.h"
#include <SDL3/SDL_opengl.h>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <tuple>
namespace Custom {

class Image {
public:
  Image() = default;
  // Image &operator=(Image &other) = default; // copy assignment operator

  Image(const Image &a)
      : x(a.x), y(a.y), BytesPerPixel(a.BytesPerPixel), cleanup(true) {

    buffer = (uint8_t *)malloc((size_t)(x * y * BytesPerPixel));

    assert(buffer != nullptr);

    memcpy(buffer, a.buffer, a.size());
  }

  size_t size() const { return (size_t)(x * y * BytesPerPixel); }
  Image(uint8_t *data_buffer, int x, int y, int BytesPerPixel)
      : x(x), y(y), BytesPerPixel(BytesPerPixel), buffer(data_buffer) {}

  Image(int x, int y, int BytesPerPixel)
      : x(x), y(y), BytesPerPixel(BytesPerPixel) {

    buffer = (uint8_t *)malloc((size_t)(x * y * BytesPerPixel));
    cleanup = true;
  }
  ~Image() {
    if (cleanup) {
      free(buffer);
    }
  }

  std::tuple<GLuint, ImVec2> ToTexture() {
    LoadTextureFromMemory();
    return std::make_tuple(out_texture, ImVec2(x, y));
  }
  bool LoadTextureFromMemory() {

    if (out_texture != 0) {
      glDeleteTextures(6, &out_texture);
    }
    // Create a OpenGL texture identifier
    glGenTextures(6, &out_texture);
    glBindTexture(GL_TEXTURE_2D, out_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 buffer);

    return true;
  }

  GLuint out_texture = 0;

  int x;
  int y;
  int BytesPerPixel;
  uint8_t *buffer = nullptr;
  bool is_updated = false;
  bool cleanup = false;
};
} // namespace Custom
