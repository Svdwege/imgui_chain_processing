#pragma once

#include "ImNodeFlow.h"
#include "image/image.hpp"
#include "imgui.h"
#include "stb_image.h"

#include <emmintrin.h>

#include <SDL3/SDL_opengl.h>
#include <cstdint>
#include <xmmintrin.h>

class Image2Gray : public ImFlow::BaseNode {
public:
  ~Image2Gray() = default;

  Image2Gray() {
    setTitle("convert Image to gray scale");
    setStyle(ImFlow::NodeStyle::brown());
    auto imageInPin = ImFlow::BaseNode::addIN<Custom::Image>(
        "In", Custom::Image(nullptr, 0, 0, 0),
        ImFlow::ConnectionFilter::SameType());

    auto imageoutpin = ImFlow::BaseNode::addOUT<Custom::Image>("Out", nullptr);
    imageoutpin->behaviour([this]() { return Gray; });
  }

  void draw() override { ImGui::SetNextItemWidth(100.f); }

private:
  void convert2Gray() {
    auto imagein = getInVal<Custom::Image>("In");
    for (size_t i = 0; i < imagein.size(); i += 3) {
      auto A1 = _mm_set_ps(imagein.buffer[i], imagein.buffer[i + 1],
                           imagein.buffer[i + 2], 0);

      A1 = _mm_mul_ps(A1, B1);
      alignas(16) float result[4];
      _mm_store_ps(result, A1);
      Gray.buffer[i] = uint8_t(result[0] + result[1] + result[2] + result[3]);

      // auto gR = (float)imagein.buffer[i] * 0.2989f;
      // auto gG = (float)imagein.buffer[i + 1] * 0.5870f;
      // auto gB = (float)imagein.buffer[i + 2] * 0.1140f;
      // Gray.buffer[i] = (uint8_t)(gR + gG + gB);
    }
  }

  const __m128 B1 = _mm_set_ps(0.2989f, 0.5870f, 0.1140f, 0.0f);
  Custom::Image Gray;
  // size_t x = 0, y = 0, channels = 0;
  // uint8_t *data = nullptr;
  //  const float RGBACoeffs[] = {0.2989f, 0.5870, 0.1140, 0};
};
// RGB weights 0.2989, 0.5870, and 0.1140
