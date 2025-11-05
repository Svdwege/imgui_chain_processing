#pragma once

#include "ImNodeFlow.h"
#include "imgui.h"
#include <algorithm>
#include <memory>
#include <optional>

// nodes
#include "imgui_sugar.hpp"
#include "nodes/result.hpp"
#include "nodes/simple.hpp"

/* Node editor that sets up the grid to place nodes */
struct NodeEditor : ImFlow::BaseNode {
  ImFlow::ImNodeFlow mINF;

  NodeEditor(float d, std::size_t r) : BaseNode() {
    mINF.setSize({d, d});
    if (r > 0) {
      auto n1 = mINF.addNode<SimpleSum>({40, 40});
      auto n2 = mINF.addNode<SimpleSum>({40, 150});
      auto result = mINF.addNode<ResultNode>({250, 80});

      // Add links between nodes
      n1->outPin("Out")->createLink(result->inPin("A"));
      n2->outPin("Out")->createLink(result->inPin("B"));
    }
  }

  template <typename T, typename... Params>
  std::shared_ptr<T> addNode(const ImVec2 &pos, Params &&...args) {
    return mINF.addNode<T>(mINF.screen2grid(pos),
                           std::forward<Params>(args)...);
  }

  void handleDragArea(ImVec2 start, ImVec2 end) {
    with_Window("debug info", nullptr) {
      auto min = ImVec2(std::min(start.x, end.x), std::min(start.y, end.y));
      auto max = ImVec2(std::max(start.x, end.x), std::max(start.y, end.y));

      auto nodes = mINF.getNodes();
      int32_t nodecount = 0;

      for (auto [_, Node] : nodes) {
        auto pos = Node->getPos();
        if (pos.x < max.x && pos.x > min.x && pos.y < max.y && pos.y > min.y) {
          nodecount++;
          Node->selected(true);
        } else {
          Node->selected(false);
        }
        Node->updatePublicStatus();
      }
      ImGui::Text("nodes in selection: %d", nodecount);
    }
  }

  std::optional<std::shared_ptr<ImFlow::BaseNode>> getHoveredNode() {
    auto nodes = mINF.getNodes();
    for (auto [_, Node] : nodes) {
      if (Node->isHovered()) {
        return Node;
      }
    }
    return {};
  }
  std::vector<std::shared_ptr<ImFlow::BaseNode>> getSelectedNodes() {
    std::vector<std::shared_ptr<ImFlow::BaseNode>> selectedNodes;
    auto nodes = mINF.getNodes();
    for (auto [_, node] : nodes) {
      if (node->isSelected()) {
        selectedNodes.push_back(node);
      }
    }
    return selectedNodes;
  }

  void set_size(ImVec2 d) { mINF.setSize(d); }

  void draw() override { mINF.update(); }
};
