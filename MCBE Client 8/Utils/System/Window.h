#pragma once

#include <string>

#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>


class SystemUtils {
public:
	static void SetWindowTitle(const winrt::param::hstring& title);
};