#include <MCBE8/MCBE8.h>
#include <MCBE8/Hooks/Hooks/Render/D3D.h>
#include <MCBE8/Events/Events/RenderEvents.h>

#include <Utils/Memory/Memory.h>
#include <Utils/Includes.h>


// not exactly my favorite hook

ID3D11Device* d3d11Device = nullptr;
ID3D12Device5* d3d12Device = nullptr;
static ImFont* DefaultFont = nullptr;

void* oPresent;
bool pToggle = false;
bool initContext = false;


void FirstInit() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;                                       // i dont like this.
    DefaultFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", MCBE8::Globals.ScreenData.ScreenHeight / 45, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGuiStyle& style = ImGui::GetStyle();
    {
        // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
        style.Alpha = 1.0f;
        style.FrameRounding = 3.0f;
        style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    }

    float _alpha = 0.5f;

    for (int i = 0; i <= ImGuiCol_COUNT; i++)
    {
        ImVec4& col = style.Colors[i];
        float H, S, V;

        ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);
        if (S < 0.1f)
            V = 1.0f - V;

        ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
        if (col.w < 1.00f)
            col.w *= _alpha;
    }
}


HRESULT D3D12_PresentDetour(IDXGISwapChain3* swapchain, UINT syncInterval, UINT flags) {
    if (!MCBE8::Window)
        return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);

    if (!SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
        if (SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
            swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
            static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
        }

        return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
    }

    ID3D11DeviceContext* ppContext = nullptr;
    d3d11Device->GetImmediateContext(&ppContext);
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    ID3D11RenderTargetView* mainRenderTargetView;
    d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
    pBackBuffer->Release();

    if (!initContext) {
        ImGui::CreateContext();
        FirstInit();
        ImGui_ImplWin32_Init(MCBE8::Window);
        ImGui_ImplDX11_Init(d3d11Device, ppContext);
        initContext = true;
    };

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGui::TeleportMousePos(MCBE8::Globals.MousePosition);

    if (MCBE8::Globals.Keymap[VK_INSERT] && MCBE8::Globals.Keymap[VK_INSERT] != pToggle)
        MCBE8::Globals.RenderUI = !MCBE8::Globals.RenderUI;

    pToggle = MCBE8::Globals.Keymap[VK_INSERT];

    {
        auto event = nes::make_holder<ImGuiPreRenderEvent>(false);
        MCBE8::EventDispatcher.trigger(event);

        // do imgui stuff.
        ImGui::Begin("Debug window");
        ImGui::Text("yay");
        ImGui::End();

        //auto event2 = nes::make_holder<ImGuiPostRenderEvent>(false);
        //MCBE8::EventDispatcher.trigger(event2);
    }

    ImGui::EndFrame();
    ImGui::Render();

    ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    mainRenderTargetView->Release();
    d3d11Device->Release();

    //if (MCBE8::NoVsync)
    //    return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, DXGI_PRESENT_DO_NOT_WAIT);

    //else
    return Memory::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
}


void D3D::Init() {
    const int index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
    kiero::bind(index, (void**)&oPresent, D3D12_PresentDetour);
}


void D3D::UnHook() {
}