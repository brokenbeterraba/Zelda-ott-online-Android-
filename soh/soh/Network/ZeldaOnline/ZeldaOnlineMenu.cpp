#include "ZeldaOnlineClient.hpp"
#include "ZeldaOnlineRoomWindow.hpp"
#include "soh/SohGui/SohGui.hpp"
#include "soh/SohGui/SohMenu.h"
#include "soh/util.h"

namespace SohGui {
extern std::shared_ptr<SohMenu> mSohMenu;
extern std::shared_ptr<ZeldaOnline::ZeldaOnlineRoomWindow> mZeldaOnlineRoomWindow;
} // namespace SohGui

static void ShowZeldaOnlineWindow() {
    if (SohGui::mZeldaOnlineRoomWindow == nullptr)
        return;

    // Enable() registra o handler de ações da janela e carrega as configurações; só precisa rodar uma vez,
    // depois que o ImGui já está de pé (por isso não é chamado durante a inicialização).
    static bool sClientEnabled = false;
    if (!sClientEnabled && ZeldaOnline::ZeldaOnlineClient::Instance != nullptr) {
        sClientEnabled = true;
        ZeldaOnline::ZeldaOnlineClient::Instance->Enable();
    }

    SohGui::mZeldaOnlineRoomWindow->Show();
    SohGui::mZeldaOnlineRoomWindow->CenterAndExpand();
}

void ZeldaOnlineMainMenu(WidgetInfo& info) {
    bool isEnabled = CVarGetInteger("gZeldaOnline.Enabled", 1) != 0;

    ImGui::SeparatorText("Zelda Online");

    const char* buttonLabel = isEnabled ? "Disable" : "Enable";
    UIWidgets::PushStyleButton(isEnabled ? UIWidgets::ColorValues.at(UIWidgets::Colors::Red)
                                         : UIWidgets::ColorValues.at(UIWidgets::Colors::Green));

    if (ImGui::Button(buttonLabel, ImVec2(-1.0f, 0.0f))) {
        if (isEnabled) {
            CVarSetInteger("gZeldaOnline.Enabled", 0);

            if (SohGui::mZeldaOnlineRoomWindow != nullptr)
                SohGui::mZeldaOnlineRoomWindow->Hide();

            if (ZeldaOnline::ZeldaOnlineClient::Instance != nullptr)
                ZeldaOnline::ZeldaOnlineClient::Instance->Disable();
        } else {
            CVarSetInteger("gZeldaOnline.Enabled", 1);
            ShowZeldaOnlineWindow();
        }

        Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
    }

    UIWidgets::PopStyleButton();

    if (!isEnabled)
        return;

    ImGui::Spacing();

    UIWidgets::PushStyleButton(UIWidgets::ColorValues.at(UIWidgets::Colors::Blue));

    if (ImGui::Button("Open Window", ImVec2(-1.0f, 0.0f)))
        ShowZeldaOnlineWindow();

    UIWidgets::PopStyleButton();
}

void RegisterZeldaOnlineMenu() {
    WidgetPath path = { "Network", "Zelda Online", SECTION_COLUMN_1 };
    SohGui::mSohMenu->AddWidget(path, "ZeldaOnlineMainMenu", WIDGET_CUSTOM)
        .CustomFunction(ZeldaOnlineMainMenu)
        .HideInSearch(true);
}

static RegisterMenuInitFunc menuInitFunc(RegisterZeldaOnlineMenu);