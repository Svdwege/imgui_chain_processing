#pragma once

#include "ImNodeFlow.h"
#include "imgui.h"
#include <algorithm>
#include <memory>
#include <optional>

class SimpleSum : public ImFlow::BaseNode {
public:
  SimpleSum() {
    setTitle("Simple sum");
    setStyle(ImFlow::NodeStyle::green());
    ImFlow::BaseNode::addIN<int>("In", 0, ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]() {
      return getInVal<int>("In") + m_valB;
    });
  }

  void draw() override {
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputInt("##ValB", &m_valB);
  }

private:
  int m_valB = 0;
};
