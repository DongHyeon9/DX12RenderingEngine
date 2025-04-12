#include "Client.h"
#include "Engine\Engine.h"
#include "App\ExampleApp.h"

int APIENTRY MAIN(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    EngineDesc desc{};
    desc.hInstance = hInstance;
    desc.app = std::make_shared<ExampleApp>();

    if (!GEngine->Init(desc))
    {
        LOG("Initialize Fail");
        std::wcout << L"Please Press Any Key";
        while (true)
            if (_kbhit()) break;

        return -1;
    }
    return static_cast<int>(GEngine->Run());
}