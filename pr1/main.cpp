#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// report error and terminate
#define DIE(...) do { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); } while (0)

struct TestData {
    int x, y;
    std::vector<int> steps;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        DIE("usage: %s [test file]\n", argv[0]);
    }
    std::ifstream f;
    f.open(argv[1]);
    if (!f) {
        DIE("cannot open file \"%s\"", argv[1]);
    }
    std::string line;
    while (std::getline(f, line)) {
        std::stringstream hey;
        hey << line;
        std::string op;
        TestData test;
        if (!(hey >> op)) continue;
        if (!(hey >> test.x)) DIE("missing initial x coordinate\n");
        if (!(hey >> test.y)) DIE("missing initial y coordinate\n");
        int n;
        while (hey >> n) {
            if (n > 9) DIE("step too big\n");
            if (n < 0) DIE("step too small\n");
            test.steps.push_back(n);
        }
        if (op == "BFS") {
        }
        else if (op == "IDS") {
        }
        else if (op == "A*") {
        }
        else {
            DIE("unknown search strategy \"%s\"\n", op.c_str());
        }
    }
    f.close();
    return 0;
}
