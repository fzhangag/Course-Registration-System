#include "register_common.h"
#include "term.h"
#include "register_system.h"

int main(int argc, char **argv) {
    Term terminal;
    RegisterSystem sys(&terminal);
    sys.exec();
    return 0;
}