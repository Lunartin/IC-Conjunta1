#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "data_storage.h" // Inclui a biblioteca de armazenamento de dados
#include <stdio.h>
#include "window.h"

ImU32 ColorToImU32(Color color)
{
    return IM_COL32(color.r, color.g, color.b, color.a);
}

void IniciarInterface()
{
    const int screenWidth = 1280;
    const int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Software de cálculo de esforços em concreto armado"); // setar altura e largura e titulo do programa
    SetTargetFPS(60);                                                                            // setar fps da interface
    rlImGuiSetup(true);                                                                          // ativar o rlImGui
    InitData();                                                                                  // iniciar os dados, quais dados? - dados referentes a data.storage.h
}

void loopPrograma()
{
    static int tempNumPoints = GetNumPoints();
    while (!WindowShouldClose())
    {
        // Inicia o desenho do Raylib
        BeginDrawing();
        ClearBackground(BLACK); // Fundo preto

        // Inicia o frame do ImGui
        rlImGuiBegin();

        // Cria a interface ImGui
        ImGui::Begin("Software de cálculo de esforços em concreto armado");

        // Cria abas
        if (ImGui::BeginTabBar("Abas"))
        {
            // Aba de Dados
            if (ImGui::BeginTabItem("Dados"))
            {
                // Título da tabela
                ImGui::Text("DADOS");

                // Campo para escolher o número de pontos
                ImGui::InputInt("Número de Pontos", &tempNumPoints);

                // Atualiza o número de pontos
                if (tempNumPoints != GetNumPoints())
                {
                    // Ajusta o número de pontos no armazenamento de dados
                    SetNumPoints(tempNumPoints);
                }

                // Cria a tabela
                if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    // Cabeçalhos da tabela
                    ImGui::TableSetupColumn("x", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("y", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();

                    // Preenchimento da tabela pelo usuário
                    for (int row = 0; row < GetNumPoints(); row++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);

                        // Cria rótulos para x e y
                        char labelX[10];
                        char labelY[10];

                        snprintf(labelX, sizeof(labelX), "x%d", row);
                        snprintf(labelY, sizeof(labelY), "y%d", row);

                        float x, y;
                        GetTableData(row, &x, &y);

                        ImGui::InputFloat(labelX, &x);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::InputFloat(labelY, &y);

                        // Atualiza os dados na biblioteca
                        SetTableData(row, x, y);
                    }

                    ImGui::EndTable();
                }

                // Botão para mostrar os valores armazenados
                if (ImGui::Button("Mostrar Valores"))
                {
                    // Mostrar os valores armazenados no console
                    TraceLog(LOG_INFO, "Valores armazenados:");
                    for (int row = 0; row < GetNumPoints(); row++)
                    {
                        float x, y;
                        GetTableData(row, &x, &y);
                        TraceLog(LOG_INFO, "Linha %d: x = %.2f, y = %.2f", row + 1, x, y);
                    }
                }

                ImGui::EndTabItem();
            }

            // Aba do Gráfico
            if (ImGui::BeginTabItem("Gráfico"))
            {
                // Configurações do gráfico
                const int graphWidth = 600;
                const int graphHeight = 400;

                // Define a posição e tamanho do gráfico
                ImVec2 graphPos = ImGui::GetCursorScreenPos();
                ImGui::Dummy(ImVec2(graphWidth, graphHeight)); // Espaço para o gráfico

                ImDrawList *draw_list = ImGui::GetWindowDrawList();

                // Desenha o fundo do gráfico
                draw_list->AddRectFilled(graphPos, ImVec2(graphPos.x + graphWidth, graphPos.y + graphHeight), ColorToImU32(DARKGRAY));
                draw_list->AddLine(ImVec2(graphPos.x, graphPos.y + graphHeight / 2), ImVec2(graphPos.x + graphWidth, graphPos.y + graphHeight / 2), ColorToImU32(WHITE));
                draw_list->AddLine(ImVec2(graphPos.x + graphWidth / 2, graphPos.y), ImVec2(graphPos.x + graphWidth / 2, graphPos.y + graphHeight), ColorToImU32(WHITE));

                // Desenha os pontos e linhas do gráfico
                for (int row = 0; row < GetNumPoints(); row++)
                {
                    float x, y;
                    GetTableData(row, &x, &y);

                    // Normaliza as coordenadas para o gráfico
                    float xPos = graphPos.x + (x + 5) * (graphWidth / 10);
                    float yPos = graphPos.y + (5 - y) * (graphHeight / 10);

                    // Verifica se os pontos estão dentro dos limites do gráfico
                    if (xPos >= graphPos.x && xPos <= graphPos.x + graphWidth &&
                        yPos >= graphPos.y && yPos <= graphPos.y + graphHeight)
                    {
                        // Desenha o ponto
                        draw_list->AddCircle(ImVec2(xPos, yPos), 3.0f, ColorToImU32(RED));
                    }

                    // Conectar os pontos com linhas
                    if (row > 0)
                    {
                        float prevX, prevY;
                        GetTableData(row - 1, &prevX, &prevY);
                        float prevXPos = graphPos.x + (prevX + 5) * (graphWidth / 10);
                        float prevYPos = graphPos.y + (5 - prevY) * (graphHeight / 10);
                        draw_list->AddLine(ImVec2(prevXPos, prevYPos), ImVec2(xPos, yPos), ColorToImU32(GREEN));
                    }
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End(); // Finaliza a janela principal

        rlImGuiEnd();
        EndDrawing();
    }
}
