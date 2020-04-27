#include "app/SharedMemoryApplication.h"

int main(int argc, const char **argv) {
    SharedMemoryApplication app(argc, argv);
    return app.run();
}
