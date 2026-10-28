#include <iostream>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i) {
        //NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        std::cout << "Argument " << i << ": " << argv[i] << '\n';
    }
    // QGuiApplication app(argc, argv); // @TODO: To be reviewed

    QQmlApplicationEngine engine;
    engine.loadFromModule("App", "main");
    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }
    return QGuiApplication::exec(); // (readability-static-accessed-through-instance)
}