#pragma once

#include "ImNodeFlow.h"
#include "image/image.hpp"
#include "imgui.h"
#include "stb_image.h"

#include <SDL3/SDL_opengl.h>
#include <cstdint>

class ImageShowNode : public ImFlow::BaseNode {
public:
  ~ImageShowNode() {}

  ImageShowNode() {
    setTitle("show Image");
    setStyle(ImFlow::NodeStyle::green());
    auto imageInPin = ImFlow::BaseNode::addIN<Custom::Image>(
        "In", Custom::Image(nullptr, 0, 0, 0),
        ImFlow::ConnectionFilter::SameType());

    // auto imageoutpin = ImFlow::BaseNode::addOUT<Image>("Out", nullptr);
    // imageoutpin->behaviour([this]() { return img; });
  }

  void draw() override {
    ImGui::SetNextItemWidth(100.f);
    auto val = getInVal<Custom::Image>("In");

    ImGui::Text("in.size = %d x %d", val.x, val.y);
  }

private:
  int x = 0, y = 0;
};
