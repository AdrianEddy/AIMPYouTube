#include "YouTubeDL.h"

#include "AIMPYouTube.h"
#include "AIMPString.h"
#include "Tools.h"
#include <windows.h>
#include <string>
#include <array>

std::wstring YouTubeDL::Params = L"-f best[ext=mp4]/best";
int YouTubeDL::Timeout = 30;
bool YouTubeDL::Force = false;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
std::wstring YouTubeDL::GetPath() {
    static std::wstring youtube_dl;
    if (youtube_dl.empty()) {
        WCHAR path[MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR], file[_MAX_FNAME], ext[_MAX_EXT];

        if (GetModuleFileName(reinterpret_cast<HINSTANCE>(&__ImageBase), path, MAX_PATH)) {
            _wsplitpath_s(path, drive, dir, file, ext);
            _wmakepath_s (path, drive, dir, L"youtube-dl", L"exe");
            youtube_dl = path;
        } else {
            Tools::ShowLastError(L"YouTubeDL::GetPath(): GetModuleFileName");
        }
    }
    return youtube_dl;
}
std::wstring YouTubeDL::GetStreamUrl(const std::wstring &id) {
    std::wstring cmd = L"\"" + GetPath() + L"\" -g " + Params + L" -- " + id;

    HANDLE pipeReadOut = nullptr, pipeWriteOut = nullptr;
    HANDLE pipeReadErr = nullptr, pipeWriteErr = nullptr;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;

    if (!CreatePipe(&pipeReadOut, &pipeWriteOut, &sa, 0)) { Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): CreatePipe"); return {}; }
    if (!CreatePipe(&pipeReadErr, &pipeWriteErr, &sa, 0)) { Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): CreatePipe"); return {}; }
    if (!SetHandleInformation(pipeReadOut, HANDLE_FLAG_INHERIT, 0)) { Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): SetHandleInformation"); return {}; }
    if (!SetHandleInformation(pipeReadErr, HANDLE_FLAG_INHERIT, 0)) { Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): SetHandleInformation"); return {}; }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof si);
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = pipeWriteOut;
    si.hStdError = pipeWriteErr;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof pi);

    if (!CreateProcess(nullptr, const_cast<LPWSTR>(cmd.c_str()), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): CreateProcess");
        return {};
    }

    const auto waitResult = WaitForSingleObject(pi.hProcess, Timeout * 1000);
    if (waitResult == WAIT_TIMEOUT) return {};
    if (waitResult != WAIT_OBJECT_0) {
        Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): WaitForSingleObject");
        return {};
    }

    DWORD exitCode;
    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): GetExitCodeProcess");
        return {};
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(pipeWriteOut);
    CloseHandle(pipeWriteErr);

    std::array<char, 512> buffer;
    auto readPipe = [&](HANDLE pipe) -> std::wstring {
        DWORD dwRead;
        std::string result;
        do {
            ReadFile(pipe, buffer.data(), buffer.size(), &dwRead, nullptr);
            result.append(buffer.data(), dwRead);
        } while (dwRead > 0);
        CloseHandle(pipe);
        return Tools::ToWString(result);
    };
    const auto result = readPipe(pipeReadOut);
    const auto error  = readPipe(pipeReadErr);

    if (exitCode || result.empty()) {
        Tools::ShowLastError(L"YouTubeDL::GetStreamUrl(): " + error);
        static bool updated = false;
        if (!updated) {
            updated = true;
            YouTubeDL::Update();
        }
        return {};
    }

    return result;
}

bool YouTubeDL::Update() {
    auto cmd = L"/c \"" + GetPath() + L"\" -U & pause";
    auto result = (int)ShellExecuteW(nullptr, L"runas", L"cmd", cmd.c_str(), nullptr, SW_SHOWNORMAL);
    if (result <= 32) {
        Tools::ShowLastError(L"YouTubeDL::Update(): ShellExecuteW - " + std::to_wstring(result));
        return false;
    }

    return true;
}
