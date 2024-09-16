#include <Utils/System/Exceptions.h>
#include <Utils/System/Misc.h>
#include <Utils/Logger/Logger.h>
#include <Utils/Data/String.h>

#include <string>
#include <windows.h>
#include <ImageHlp.h>
#include <Psapi.h>


// prax.


LONG WINAPI TopLevelExceptionHandler(const PEXCEPTION_POINTERS pExceptionInfo)
{
    const std::string exceptionCode = "Except. Code: " + String::ToHex(pExceptionInfo->ExceptionRecord->ExceptionCode) + "\n";
    const std::string exceptionAddress = "Except. Address: " + String::ToHex(reinterpret_cast<uintptr_t>(pExceptionInfo->ExceptionRecord->ExceptionAddress)) + "\n";
    const std::string exceptionFlags = "Except. Flags: " + String::ToHex(pExceptionInfo->ExceptionRecord->ExceptionFlags) + "\n";
    const std::string exceptionParams = "Except. Parameters: " + String::ToHex(pExceptionInfo->ExceptionRecord->NumberParameters) + "\n";
    const std::string exceptionContextRecord = "Except. Context: " + String::ToHex(reinterpret_cast<uintptr_t>(pExceptionInfo->ContextRecord)) + "\n";
    
    const std::string fullException = "! EXECEPTION !\n" + exceptionCode + exceptionAddress + exceptionFlags + exceptionParams + exceptionContextRecord;

    Logger::Write<LogLevel::ERR>("Exceptions", fullException);

    auto res = MessageBoxA(
        nullptr,
        (
            exceptionCode +
            exceptionAddress +
            exceptionFlags +
            exceptionParams +
            exceptionContextRecord +
            "\n\n" + "Base offset: " +
            MiscUtils::GetModuleOfAddress(reinterpret_cast<uintptr_t>(pExceptionInfo->ExceptionRecord->ExceptionAddress))
        ).c_str(),
        "kaboom", MB_RETRYCANCEL | MB_ICONERROR
    );

    if (res == IDRETRY)
        return EXCEPTION_CONTINUE_EXECUTION;

    MessageBoxA(nullptr, "MCBE8 Crashed.", "kaboom", MB_OK | MB_ICONERROR);

    return EXCEPTION_EXECUTE_HANDLER;
}

void Exceptions::Init() {
    Logger::Write<LogLevel::INFO>("Init", "Initialized exceptions filter");
    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
}
