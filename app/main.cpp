#include <iostream>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

int main(int argc, char** argv)
{
    std::cout << "Test the app for " << argc << " arguments that will be ignored" << '\n';
    for (int i = 0; i < argc; ++i) {
        //NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        std::cout << "Argument " << i << ": " << argv[i] << '\n';
    }
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("App", "main");
    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }
    return app.exec(); //NOLINT (readability-static-accessed-through-instance)
}