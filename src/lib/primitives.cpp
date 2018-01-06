#include <nuschl/primitives.hpp>

std::ostream &nuschl::operator<<(std::ostream &os, const primitive &p) {
    os << p.representation();
    return os;
}
