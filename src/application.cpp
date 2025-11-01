#include "application.hpp"

#include "ImNodeFlow.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "imgui_internal.h"
#include "imgui_sugar.hpp"
#include "src/example.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>
#include <asm-generic/errno.h>
#include <iostream>
#include <ostream>

application::application() {}

application::~application() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int application::init() {
  // Setup SDL
  // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts
  // would likely be your SDL_AppInit() function]
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return 1;
  }

  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN |
                 SDL_WINDOW_HIGH_PIXEL_DENSITY;
  window = SDL_CreateWindow("Dear ImGui SDL3+OpenGL3 example",
                            (int)(1280 * main_scale), (int)(800 * main_scale),
                            window_flags);
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  std::cerr << "window:(w,h) (" << w << "," << h << ")";
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return 1;
  }
  gl_context = SDL_GL_CreateContext(window);
  if (gl_context == nullptr) {
    printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(window);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  imContext = ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO(imContext);

  std::cerr << "DisplaySize:(x,y) (" << io.DisplaySize.x << ","
            << io.DisplaySize.y << ")";
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup scaling
  ImGuiStyle &style = ImGui::GetStyle();

  style.ScaleAllSizes(main_scale);
  style.FontScaleDpi = main_scale;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Our state
  clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  neditor = new (NodeEditor)(500, 500);
  io.AddKeyEvent(ImGuiKey_A, true);
  update();
  return 0;
}

bool application::SelectionRect(ImVec2 *start_pos, ImVec2 *end_pos,
                                ImGuiMouseButton mouse_button) {
  IM_ASSERT(start_pos != NULL);
  IM_ASSERT(end_pos != NULL);
  if (ImGui::IsMouseClicked(mouse_button)) {
    *start_pos = ImGui::GetMousePos();
  }

  if (ImGui::IsMouseDown(mouse_button)) {
    *end_pos = ImGui::GetMousePos();

    ImDrawList *draw_list =
        ImGui::GetForegroundDrawList(); // ImGui::GetWindowDrawList();

    draw_list->AddRect(
        *start_pos, *end_pos,
        ImGui::GetColorU32(IM_COL32(0, 130, 216, 255))); // Border

    draw_list->AddRectFilled(
        *start_pos, *end_pos,
        ImGui::GetColorU32(IM_COL32(0, 130, 216, 50))); // Background
  }
  return ImGui::IsMouseReleased(mouse_button);
}

bool application::update() {
  ImGuiIO &io = ImGui::GetIO(imContext);
  // ImGuiStyle &style = ImGui::GetStyle();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
      return true;
    }
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
        event.window.windowID == SDL_GetWindowID(window)) {
      return true;
    }
  }

  if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
    SDL_Delay(10);
  }

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  const auto window_size = io.DisplaySize - ImVec2(1, 1);

  const auto window_pos = ImVec2(1, 1);
  const auto node_editor_size = window_size - ImVec2(16, 16);
  std::shared_ptr<ImFlow::BaseNode> Node;
  ImVec2 Pos_Node;
  std::string name;
  bool nodevalid = false;

  ImGui::SetNextWindowSize(window_size);
  ImGui::SetNextWindowPos(window_pos);
  with_Window("Node Editor", nullptr,
              ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoBringToFrontOnFocus) {
    neditor->set_size(node_editor_size);

    for (const auto &[key, callback] : keymap) {
      if (ImGui::IsKeyPressed(key, false)) {
        callback(io, neditor);
      }
    }
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0)) {
      if (!SelectionRect(&dragStart, &draggedArea, ImGuiMouseButton_Right)) {
        // neditor->mINF.screen2grid(dragStart);
        neditor->handleDragArea(neditor->mINF.screen2grid(dragStart),
                                neditor->mINF.screen2grid(draggedArea));
      }
    }

    neditor->mINF.droppedLinkPopUpContent([this](ImFlow::Pin *dragged) {
      ImGuiIO &io = ImGui::GetIO(imContext);
      // if (io.MouseReleased[0]) {}

      auto pos = io.MousePos;
      auto offset = neditor->mINF.screen2grid(pos);

      ImGui::OpenPopup("PopupFromDrop");
      if (ImGui::BeginPopup("PopupFromDrop")) {

        if (ImGui::Button("Simple")) {
          auto node = neditor->addNode<SimpleSum>(offset);
          auto nodeins = node->getIns();
          if (nodeins.size() > 0) {
            nodeins.at(0)->createLink(dragged);
          }

          ImGui::ClosePopupsExceptModals();
        } else if (ImGui::Button("Result")) {
          auto node = neditor->addNode<ResultNode>(offset);
          auto nodeins = node->getIns();
          if (nodeins.size() > 0) {
            nodeins.at(0)->createLink(dragged);
          }

          ImGui::ClosePopupsExceptModals();
        } else {
        }
        ImGui::EndPopup();
      }
    });

    auto MaybeNode = neditor->getHoveredNode();
    if (MaybeNode) {
      Node = MaybeNode.value();
      Pos_Node = Node->getPos();
      name = Node->getName();
      nodevalid = true;
    }
    neditor->draw();
  }

  with_Window("debug info", nullptr) {
    auto scroll = neditor->mINF.getScroll();
    auto pos = neditor->mINF.getPos();
    auto s2c1 = neditor->mINF.screen2grid(ImVec2(1, 1));
    auto s2c0 = neditor->mINF.screen2grid(ImVec2(0, 0));
    ImGui::Text("(scrool offset: %f, %f)", scroll.x, scroll.y);
    ImGui::Text("(pos offset: %f, %f)", pos.x, pos.y);
    ImGui::Text("(screen2grid0: %f, %f)", s2c0.x, s2c0.y);
    ImGui::Text("(screen2grid1: %f, %f)", s2c1.x, s2c1.y);
    ImGui::Text("(screen2griddif: %f, %f)", s2c1.x - s2c0.x, s2c1.y - s2c0.x);
    ImGui::Text("______");
    auto mouse = io.MousePos;
    auto grid = neditor->mINF.screen2grid(io.MousePos);
    ImGui::Text("(screen mouse: %f, %f)", mouse.x, mouse.y);
    ImGui::Text("(grid mouse: %f, %f)", grid.x, grid.y);

    if (nodevalid) {
      ImGui::Text("(name: %s)", name.c_str());
      ImGui::Text("(uid: %lu)", Node->getUID());
      ImGui::Text("(%f,%f)", Pos_Node.x, Pos_Node.y);
    }

    auto nodes = neditor->getSelectedNodes();
    if (nodes.size() > 0) {
      ImGui::Text("___ Selected ___");
    }
    for (auto node : nodes) {
      auto name = node->getName();
      ImGui::Text("(name: %s)", name.c_str());
      ImGui::Text("(uid: %lu)", node->getUID());
      ImGui::Text("(%f,%f)", node->getPos().x, node->getPos().y);
      ImGui::Text("values");
      if (name == "Result node") {
        ImGui::Text("\t(in A: %lu)", 2);
        ImGui::Text("\t(in B: %lu)", 2);
      } else if (name == "Simple sum") {
        ImGui::Text("\t(in val: %lu)", 1);
        ImGui::Text("\t(out val: %lu)", 1);
      }
      ImGui::Text("______");
    }
  }

  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
  return false;
}
