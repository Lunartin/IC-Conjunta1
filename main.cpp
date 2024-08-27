#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "data_storage.h" // Inclui a biblioteca de armazenamento de dados
#include <stdio.h>
#include "window.h" // biblioteca da interface

int main()
{
    IniciarInterface(); // inicia largura, altura, fps, imgui e data_storage
    loopPrograma();     // rodar a interface

    // Limpeza
    rlImGuiShutdown();
    CloseWindow();
    FreeData(); // Libera a memória dos dados antes de fechar o programa

    return 0;
}
