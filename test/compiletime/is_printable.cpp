#include <nuschl/unittests/is_printable.hpp>

#include <string>

struct unprintable {};

struct printable {
    printable() = default;
    printable(const printable &) = delete;
};

std::ostream &operator<<(std::ostream &, const printable &);

int main() {

    static_assert(nuschl::tmetap::is_printable<int>::value, "Integer should be "
                                                            "printabl"
                                                            "e");
    static_assert(nuschl::tmetap::is_printable<std::string>::value,
                  "std::string should be printable");

    static_assert(nuschl::tmetap::is_printable<printable>::value,
                  "std::string should be printable");

    static_assert(!nuschl::tmetap::is_printable<unprintable>::value,
                  "unprintable should not be printable");
}
