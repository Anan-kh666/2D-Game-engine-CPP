#include "Application.h"
#include "LightingExtension.h"

int main() {
  Forge2D::LightingExtension::Init();
  Application app;
  app.Run();
  return 0;
}