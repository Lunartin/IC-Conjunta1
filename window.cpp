#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "implot.h"
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
    InitWindow(screenWidth, screenHeight, "Software de cálculo de esforços em concreto armado");
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImPlot::CreateContext();
    InitData();
}

void loopPrograma()
{
    static int tempNumPoints = GetNumPoints();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();

        ImGui::Begin("Software de cálculo de esforços em concreto armado");

        if (ImGui::BeginTabBar("Abas"))
        {
            // Aba de Dados
            if (ImGui::BeginTabItem("Dados"))
            {
                ImGui::Text("DADOS");
                ImGui::InputInt("Número de Pontos", &tempNumPoints);

                if (tempNumPoints != GetNumPoints())
                {
                    SetNumPoints(tempNumPoints);
                }

                if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    ImGui::TableSetupColumn("x", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("y", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();

                    for (int row = 0; row < GetNumPoints(); row++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);

                        char labelX[10], labelY[10];
                        snprintf(labelX, sizeof(labelX), "x%d", row);
                        snprintf(labelY, sizeof(labelY), "y%d", row);

                        float x, y;
                        GetTableData(row, &x, &y);

                        ImGui::InputFloat(labelX, &x);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::InputFloat(labelY, &y);

                        SetTableData(row, x, y);
                    }

                    ImGui::EndTable();
                }

                if (ImGui::Button("Mostrar Valores"))
                {
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

            // Aba de Gráfico
            if (ImGui::BeginTabItem("Gráfico"))
            {
                static float x_data[100];
                static float y_data[100];
                int numPoints = GetNumPoints();

                for (int i = 0; i < numPoints; i++)
                {
                    GetTableData(i, &x_data[i], &y_data[i]);
                }

                // Para fechar o polígono, adicione o primeiro ponto ao final
                if (numPoints > 0)
                {
                    x_data[numPoints] = x_data[0]; // Adiciona o primeiro x
                    y_data[numPoints] = y_data[0]; // Adiciona o primeiro y
                    numPoints++;                   // Incrementa o número de pontos para o plot
                }

                if (ImPlot::BeginPlot("Gráfico da Seção Transversal"))
                {
                    ImPlot::PlotLine("Seção Transversal", x_data, y_data, numPoints);
                    ImPlot::EndPlot();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar(); // Adicionei o fechamento da tab bar
        }

        ImGui::End(); // Finaliza a janela principal
        rlImGuiEnd();
        EndDrawing();
    }

    // Finaliza o contexto
    ImPlot::DestroyContext();
    rlImGuiShutdown();
}
