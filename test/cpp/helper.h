#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <fstream>
#include <string>
#include "pbc/pbc.hpp"

using namespace std;
using namespace pbc;
using namespace Catch::Matchers;

struct Fixture {
    Fixture()
    {
        ifstream f("./a.param", ios::in);
        type_a_param =
            string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
        pairing = Pairing::init_from_param_str(type_a_param);
    }
    ~Fixture() {}

    string type_a_param;
    PairingPtr pairing;
};
