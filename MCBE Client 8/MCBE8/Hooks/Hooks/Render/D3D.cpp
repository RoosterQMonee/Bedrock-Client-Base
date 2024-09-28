#include <MCBE8/MCBE8.h>
#include <MCBE8/Hooks/Hooks/Render/D3D.h>
#include <MCBE8/Events/Events/RenderEvents.h>

#include <Utils/Memory/Memory.h>
#include <Utils/Includes.h>

#include <wrl/client.h>



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


WNDPROC	oWndProc = nullptr;


void Init(HWND hWindow)
{
	oWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)oWndProc);
}

void Remove(HWND hWindow)
{
	SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)oWndProc);
}

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (MCBE8::Globals.RenderUI) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

// -----------------------------------------------------------------------------

typedef long(__fastcall* ResizeBuffersD3D12)(IDXGISwapChain3* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
extern ResizeBuffersD3D12 oResizeBuffersD3D12;

typedef ULONG(__fastcall* ReleaseD3D12)(IDXGISwapChain3* pSwapChain);
extern ReleaseD3D12 oReleaseD3D12;

typedef long(__fastcall* PresentD3D12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern PresentD3D12 oPresentD3D12;

typedef void(__fastcall* DrawInstancedD3D12)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
extern DrawInstancedD3D12 oDrawInstancedD3D12;

typedef void(__fastcall* DrawIndexedInstancedD3D12)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);
extern DrawIndexedInstancedD3D12 oDrawIndexedInstancedD3D12;

extern void(*oExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
extern HRESULT(*oSignalD3D12)(ID3D12CommandQueue*, ID3D12Fence*, UINT64);

// -----------------------------------------------------------------------------

static std::mutex lock;

ID3D12Device* d3d12Device = nullptr;
IDXGISwapChain3* _pSwapChain = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12Fence* d3d12Fence = nullptr;
UINT64 d3d12FenceValue = 0;
ID3D12CommandQueue* d3d12CommandQueue = nullptr;

Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11ImmediateContext = nullptr;
Microsoft::WRL::ComPtr<ID3D11On12Device> d3d11On12Device = nullptr;
Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
Microsoft::WRL::ComPtr<ID2D1Device> d2d1Device = nullptr;
Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d1DeviceContext = nullptr;
Microsoft::WRL::ComPtr<ID2D1Factory7> d2d1Factory = nullptr;
Microsoft::WRL::ComPtr<ID2D1Bitmap> d2dRenderTarget = nullptr;

ResizeBuffersD3D12 oResizeBuffersD3D12;
ReleaseD3D12 oReleaseD3D12;
PresentD3D12 oPresentD3D12;
DrawInstancedD3D12 oDrawInstancedD3D12;
DrawIndexedInstancedD3D12 oDrawIndexedInstancedD3D12;

void(*oExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
HRESULT(*oSignalD3D12)(ID3D12CommandQueue*, ID3D12Fence*, UINT64);

struct __declspec(uuid("189819f1-1db6-4b57-be54-1821339b85f7")) ID3D12Device;

// -----------------------------------------------------------------------------

struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;

	Microsoft::WRL::ComPtr<IDXGISurface1> surface = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Resource> resource11 = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dRenderTarget = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> backBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Effect> blur = nullptr;
};

// -----------------------------------------------------------------------------

unsigned int buffersCounts = -1;
std::unique_ptr<FrameContext[]> frameContext;

static bool init = false;


void RenderGUI() {
	ImGui::NewFrame();
	{
		if (MCBE8::Globals.RenderUI) {
			ImGuiStyle& style = ImGui::GetStyle();

			style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.98f);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.08f, 0.50f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.60f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.90f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.83f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.60f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.90f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.60f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.90f);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.78f);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.60f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.90f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 0.86f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.60f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 0.97f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

			ImGuiWindowFlags TargetFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

			ImGuiIO& io = ImGui::GetIO();

			ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

			if (ImGui::Begin(("BRC"), 0, TargetFlags)) {
				ImGui::Text("BRC");

//				if (ImGui::CollapsingHeader("Visuals", ImGuiTreeNodeFlags_DefaultOpen)) {
//					for (Module* module : BRC::module_manager.getModulesInCategory(ModuleCategory::VISUAL)) {
//						module->renderImGui();
//					}
//				}
//
				ImGui::End();
			}

			//for (Module* module : BRC::module_manager.getModulesInCategory(ModuleCategory::VISUAL)) {
			//	module->renderAutonomousImGui();
			//}
		}
	}

	ImGui::Render();
	ImGui::EndFrame(); // <-- Added
}

void RenderImGui() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	RenderGUI();
}

void CopyBitmap(ID2D1DeviceContext* ctx, Microsoft::WRL::ComPtr<ID2D1Bitmap1>& from, Microsoft::WRL::ComPtr<ID2D1Bitmap>& to) {
	if (to.Get() == nullptr) {
		D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(from->GetPixelFormat());
		ctx->CreateBitmap(from->GetPixelSize(), props, &to);
	}
	else if (from->GetPixelSize().width != to->GetPixelSize().width &&
		from->GetPixelSize().height != to->GetPixelSize().height) {
		to = nullptr;
		D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(from->GetPixelFormat());
		ctx->CreateBitmap(from->GetPixelSize(), props, &to);
	}

	auto destPoint = D2D1::Point2U(0, 0);
	auto size = from->GetPixelSize();
	auto rect = D2D1::RectU(0, 0, size.width, size.height);

	to->CopyFromBitmap(&destPoint, from.Get(), &rect);
}

void CreateD11o12DeviceContext() {
	if (d3d11Device == nullptr && d3d12CommandQueue != nullptr) {
		D3D11On12CreateDevice(
			(IUnknown*)d3d12Device,
			D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_SINGLETHREADED
			| D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr,
			0,
			(IUnknown**)&d3d12CommandQueue,
			1,
			0,
			&d3d11Device,
			&d3d11ImmediateContext,
			nullptr
		);

		if (d3d11Device.Get() != nullptr) {
			d3d11Device.As(&d3d11On12Device);

			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory7), {}, &d2d1Factory);

			Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
			d3d11On12Device.As(&dxgiDevice);

			d2d1Factory->CreateDevice(dxgiDevice.Get(), &d2d1Device);
			d2d1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2d1DeviceContext);
		}
	}
}

void ClearFrameContext(size_t index) {
	FrameContext& context = frameContext[index];
	context.commandAllocator->Release();
	context.commandAllocator = {};
	context.main_render_target_resource->Release();
	context.main_render_target_resource = {};
	context.main_render_target_descriptor = {};

	context.surface.Reset();
	context.resource11.Reset();
	context.d2dRenderTarget.Reset();
	context.backBuffer.Reset();
	context.blur.Reset();
}

void ReleaseRender() {
	d3d12Device->Release();
	d3d12Device = {};
	_pSwapChain = {};
	d3d12DescriptorHeapBackBuffers->Release();
	d3d12DescriptorHeapBackBuffers = {};
	d3d12DescriptorHeapImGuiRender->Release();
	d3d12DescriptorHeapImGuiRender = {};
	d3d12CommandList->Release();
	d3d12CommandList = {};
	d3d12Fence = {};
	d3d12CommandQueue->Release();
	d3d12CommandQueue = {};

	d3d11Device.Reset();
	d3d11ImmediateContext.Reset();
	d3d11On12Device.Reset();
	dxgiDevice.Reset();
	d2d1Device.Reset();
	d2d1DeviceContext.Reset();
	d2d1Factory.Reset();
	d2dRenderTarget.Reset();

	for (size_t i = 0; i < buffersCounts; i++) {
		ClearFrameContext(i);
	}

	frameContext = nullptr;
	init = false;
}

long __fastcall HookResizeBuffersD3D12(IDXGISwapChain3* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
	auto _lock = std::lock_guard(lock);
	if (_pSwapChain == pSwapChain) {
		ReleaseRender();
	}
	return oResizeBuffersD3D12(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

ULONG __fastcall HookReleaseD3D12(IDXGISwapChain3* pSwapChain) {
	if (_pSwapChain == pSwapChain) {
		ReleaseRender();
	}
	return oReleaseD3D12(pSwapChain);
}

void RenderBlur(float strength) {
	if (_pSwapChain == nullptr) return;
	if (!MCBE8::Globals.RenderUI) return;

	//auto _lock = std::lock_guard(lock);

	FrameContext& currentFrameContext = frameContext[_pSwapChain->GetCurrentBackBufferIndex()];

	if (d2d1DeviceContext.Get() != nullptr) {
		auto d3d11Resources = currentFrameContext.resource11.Get();

		if (d3d11Resources != nullptr && d3d11On12Device.Get() != nullptr && d3d11ImmediateContext.Get() != nullptr) {
			d3d11On12Device->AcquireWrappedResources(&d3d11Resources, 1);

			d2d1DeviceContext->SetTarget(currentFrameContext.d2dRenderTarget.Get());
			d2d1DeviceContext->BeginDraw();
			CopyBitmap(d2d1DeviceContext.Get(), currentFrameContext.d2dRenderTarget, currentFrameContext.backBuffer);

			if (currentFrameContext.blur.Get() == nullptr) {
				d2d1DeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &currentFrameContext.blur);

				currentFrameContext.blur->SetInput(0, currentFrameContext.backBuffer.Get());
				currentFrameContext.blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, strength);
				currentFrameContext.blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
				currentFrameContext.blur->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);

				/*d2d1DeviceContext->CreateEffect(CLSID_D2D1DirectionalBlur, &currentFrameContext.blur);
				currentFrameContext.blur->SetInput(0, currentFrameContext.backBuffer.Get());
				currentFrameContext.blur->SetValue(D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION, 10.0f);
				currentFrameContext.blur->SetValue(D2D1_DIRECTIONALBLUR_PROP_ANGLE, 90.0f);

				currentFrameContext.blur->SetValue(D2D1_DIRECTIONALBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
				currentFrameContext.blur->SetValue(D2D1_DIRECTIONALBLUR_PROP_OPTIMIZATION, D2D1_DIRECTIONALBLUR_OPTIMIZATION_QUALITY);*/
			}

			d2d1DeviceContext->DrawImage(currentFrameContext.blur.Get());
			d2d1DeviceContext->EndDraw();

			d3d11On12Device->ReleaseWrappedResources(&d3d11Resources, 1);
			d3d11ImmediateContext->Flush();
		}
	}
}

long __fastcall hookPresentD3D12(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (MCBE8::Globals.Running) {
		auto _lock = std::lock_guard(lock);

		_pSwapChain = pSwapChain;
		CreateD11o12DeviceContext();

		if (!init) {
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&d3d12Device))) {
				ImGui::CreateContext();

				unsigned char* pixels;
				int width, height;
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui::StyleColorsDark();
				io.Fonts->AddFontDefault();
				io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
				io.IniFilename = NULL;

				CreateEvent(nullptr, false, false, nullptr);

				DXGI_SWAP_CHAIN_DESC sdesc;
				pSwapChain->GetDesc(&sdesc);
				sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				sdesc.OutputWindow = MCBE8::Window;
				sdesc.Windowed = ((GetWindowLongPtr(MCBE8::Window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

				buffersCounts = sdesc.BufferCount;
				frameContext = std::unique_ptr<FrameContext[]>(new FrameContext[buffersCounts]);

				D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
				descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				descriptorImGuiRender.NumDescriptors = buffersCounts;
				descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				if (d3d12Device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&d3d12DescriptorHeapImGuiRender)) != S_OK)
					return false;

				ID3D12CommandAllocator* allocator;
				if (d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)) != S_OK)
					return false;

				for (size_t i = 0; i < buffersCounts; i++) {
					frameContext[i].commandAllocator = allocator;
				}

				if (d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, NULL, IID_PPV_ARGS(&d3d12CommandList)) != S_OK ||
					d3d12CommandList->Close() != S_OK)
					return false;

				D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
				descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				descriptorBackBuffers.NumDescriptors = buffersCounts;
				descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				descriptorBackBuffers.NodeMask = 1;

				if (d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)) != S_OK)
					return false;
			}

		}

		const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

		for (size_t i = 0; i < buffersCounts; i++) {
			Microsoft::WRL::ComPtr<ID3D12Resource> pBackBuffer;

			frameContext[i].main_render_target_descriptor = rtvHandle;
			pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			d3d12Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, rtvHandle);

			frameContext[i].main_render_target_resource = pBackBuffer.Get();
			rtvHandle.ptr += rtvDescriptorSize;

			if (d3d11On12Device.Get() != nullptr && frameContext[i].resource11.Get() == nullptr) {
				D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
				d3d11On12Device->CreateWrappedResource(
					pBackBuffer.Get(),
					&d3d11Flags,
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_PRESENT,
					IID_PPV_ARGS(&frameContext[i].resource11));

				frameContext[i].resource11.As(&frameContext[i].surface);

				float dpi = (float)GetDpiForSystem();
				D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
					dpi,
					dpi);

				d2d1DeviceContext->CreateBitmapFromDxgiSurface(
					frameContext[i].surface.Get(),
					&bitmapProperties,
					&frameContext[i].d2dRenderTarget
				);
			}
		}


		if (!init) {
			ImGui_ImplWin32_Init(MCBE8::Window);
			ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
				DXGI_FORMAT_R8G8B8A8_UNORM, d3d12DescriptorHeapImGuiRender,
				d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
				d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

			ImGui_ImplDX12_CreateDeviceObjects();

			Init(MCBE8::Window);
			init = true;
		}

		if (d3d12CommandQueue == nullptr)
			return oPresentD3D12(pSwapChain, SyncInterval, Flags);

		RenderImGui();

		FrameContext& currentFrameContext = frameContext[pSwapChain->GetCurrentBackBufferIndex()];
		currentFrameContext.commandAllocator->Reset();

		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = currentFrameContext.main_render_target_resource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		d3d12CommandList->Reset(currentFrameContext.commandAllocator, nullptr);
		d3d12CommandList->ResourceBarrier(1, &barrier);
		d3d12CommandList->OMSetRenderTargets(1, &currentFrameContext.main_render_target_descriptor, FALSE, nullptr);
		d3d12CommandList->SetDescriptorHeaps(1, &d3d12DescriptorHeapImGuiRender);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);

		RenderBlur(5.0f);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		d3d12CommandList->ResourceBarrier(1, &barrier);
		d3d12CommandList->Close();

		d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));
	}

	return oPresentD3D12(pSwapChain, SyncInterval, Flags);
}

void __fastcall hookkDrawInstancedD3D12(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) {
	return oDrawInstancedD3D12(dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void __fastcall hookDrawIndexedInstancedD3D12(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex) {
	return oDrawIndexedInstancedD3D12(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
}

void hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
	if (!d3d12CommandQueue)
		d3d12CommandQueue = queue;

	oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
}

HRESULT hookSignalD3D12(ID3D12CommandQueue* queue, ID3D12Fence* fence, UINT64 value) {
	if (d3d12CommandQueue != nullptr && queue == d3d12CommandQueue) {
		d3d12Fence = fence;
		d3d12FenceValue = value;
	}

	return oSignalD3D12(queue, fence, value);
}

void D3D::Init() {
    ///const int index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
    ///kiero::bind(index, (void**)&oPresentD3D12, hookPresentD3D12);
	kiero::bind(54, (void**)&oExecuteCommandListsD3D12, hookExecuteCommandListsD3D12);
	kiero::bind(58, (void**)&oSignalD3D12, hookSignalD3D12);
	kiero::bind(140, (void**)&oPresentD3D12, hookPresentD3D12);
	kiero::bind(145, (void**)&oResizeBuffersD3D12, HookResizeBuffersD3D12);
	kiero::bind(84, (void**)&oDrawInstancedD3D12, hookkDrawInstancedD3D12);
	kiero::bind(85, (void**)&oDrawIndexedInstancedD3D12, hookDrawIndexedInstancedD3D12);
}


void D3D::UnHook() {
	kiero::unbind(54);
	kiero::unbind(58);
	kiero::unbind(140);
	kiero::unbind(145);
	kiero::unbind(84);
	kiero::unbind(85);

	ReleaseRender();
	Remove(MCBE8::Window);
}