#pragma once

#include "ImNodeFlow.h"
#include "imgui.h"
#include <algorithm>
#include <memory>
#include <optional>

class ResultNode : public ImFlow::BaseNode {
public:
  ResultNode() {
    setTitle("Result node");
    setStyle(ImFlow::NodeStyle::brown());
    ImFlow::BaseNode::addIN<int>("A", 0, ImFlow::ConnectionFilter::SameType());
    ImFlow::BaseNode::addIN<int>("B", 0, ImFlow::ConnectionFilter::SameType());
  }

  void draw() override {
    ImGui::Text("Result: %d", getInVal<int>("A") + getInVal<int>("B"));
  }
};
