#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE __BASE_FILE__

#include <boost/test/unit_test.hpp>
#include <fstream>
#include "pbc/pbc.hpp"

using namespace std;
using namespace pbc;

const string TYPE_A_PARAM = []() -> string {
    ifstream f("./a.param", ios::in);
    return string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
}();
