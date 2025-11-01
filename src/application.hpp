#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "imgui.h"
#include "imgui_internal.h"
#include "src/example.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

using keyCallback = std::function<void(ImGuiIO &io, NodeEditor *neditor)>;

class application {
public:
  application();
  ~application();
  int init();
  bool update();
  bool SelectionRect(ImVec2 *start_pos, ImVec2 *end_pos,
                     ImGuiMouseButton mouse_button = ImGuiMouseButton_Left);

private:
  SDL_Window *window;
  SDL_GLContext gl_context;
  SDL_WindowFlags window_flags;

  ImVec4 clear_color;
  ImGuiContext *imContext = nullptr;
  ImGuiIO initval = ImGuiIO();
  float main_scale;

  std::map<ImGuiKey, keyCallback> keymap = {

      {ImGuiKey_Q,
       [](ImGuiIO &io, NodeEditor *neditor) {
         auto pos = io.MousePos;
         neditor->addNode<SimpleSum>(pos);
       }},
      {ImGuiKey_W,
       [](ImGuiIO &io, NodeEditor *neditor) {
         auto pos = io.MousePos;
         neditor->addNode<ResultNode>(pos);
       }},

  };
  ImVec2 dragStart;
  ImVec2 draggedArea;
  NodeEditor *neditor = nullptr;
};

#endif // APPLICATION_HPP_
