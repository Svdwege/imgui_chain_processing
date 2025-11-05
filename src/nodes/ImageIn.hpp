#pragma once

#include "ImNodeFlow.h"
#include "image/image.hpp"
#include "imgui.h"
#include "stb_image.h"
#include <SDL3/SDL_opengl.h>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>

class ImageInNode : public ImFlow::BaseNode {
public:
  ~ImageInNode() { stbi_image_free(data); }

  ImageInNode() {
    setTitle("Load Image");
    setStyle(ImFlow::NodeStyle::red());
    auto imageoutpin =
        ImFlow::BaseNode::addOUT<Custom::Image>("imgOut", nullptr);
    imageoutpin->behaviour([this]() { return img; });

    data = stbi_load("steamTestPic640x480.png", &x, &y, &bytesPerPixel, 0);
    if (data == nullptr) {
      std::cout << "load of image unsuccessfull" << std::endl;
    } else {
      std::cout << "load of image successfull" << std::endl;
    }
    img = Custom::Image(data, x, y, bytesPerPixel);
  }

  void draw() override {
    ImGui::SetNextItemWidth(100.f);
    ImGui::Text("x: %d, y: %d \r\n number of bytes per pixel: %d \r\n Total N "
                "bytes %d ",
                x, y, bytesPerPixel, x * y * bytesPerPixel);
    LoadTextureFromMemory();
    ImGui::Text("pointer = %x", out_texture);
    ImGui::Text("size = %d x %d", x, y);
    ImGui::Image((ImTextureID)(intptr_t)out_texture, ImVec2(x, y));

    ImGui::Text("--------------------------------------------------");
    ImGui::Text("made from on img class fn");
    ImGui::Text("pointer = %x", out_texture);
    ImGui::Text("size = %d x %d", x, y);
    auto [tex, loc] = img.ToTexture();
    ImGui::Image((ImTextureID)(intptr_t)tex, loc);
  }

private:
  bool LoadTextureFromMemory() {
    // auto in = getInVal<Custom::Image>("In");
    // auto data = in.buffer;

    if (out_texture != 0) {
      glDeleteTextures(1, &out_texture);
    }
    // Create a OpenGL texture identifier
    glGenTextures(1, &out_texture);
    glBindTexture(GL_TEXTURE_2D, out_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);

    return true;
  }

  GLuint out_texture = 0;
  Custom::Image img;
  int x = 0, y = 0, bytesPerPixel = 0;
  uint8_t *data = nullptr;
};
