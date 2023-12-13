#include <config.h>

std::ostream& operator<<(std::ostream& os, const Directory& dir) {
    return os << fmt::format("Dir[path={}, passphrase={}]", dir.path, dir.passphrase);
}