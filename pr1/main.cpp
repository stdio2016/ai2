#include <cstdlib>
#include <cstdio>
#include <queue>
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
    void show() const {
        std::cout << x << ' ' << y;
        for (int i: steps) {
            std::cout << ' ' << i;
        }
        std::cout << std::endl;
    }
};

struct RunState {
    int x, y;
    RunState *parent;
    RunState() {}
    RunState(int x, int y, RunState *prev): x(x), y(y), parent(prev) {}
    void showPath(const TestData &t) const {
        std::vector<const RunState *> tmp;
        const RunState *p = this;
        while (p != NULL) {
            tmp.push_back(p);
            p = p->parent;
        }
        std::cout << "initial (0,0)" << std::endl;
        const RunState *prev = tmp[tmp.size()-1];
        for (int i = tmp.size()-2; i>=0 ;i--) {
            const RunState *s = tmp[i];
            if (s->x > prev->x) std::cout << "(x+) ";
            else if (s->y > prev->y) std::cout << "(y+) ";
            else if (s->x < prev->x) std::cout << "(x-) ";
            else if (s->y < prev->y) std::cout << "(y-) ";
            else std::cout << "(S)  ";
            std::cout << t.steps[tmp.size() - i - 2] << "  (" << s->x << ',' << s->y << ')' << std::endl;
            prev = s;
        }
    }
};

// search algorithms
void bfs(const TestData &test) {
    test.show();
    std::vector<RunState*> state;
    int i;
    int n = test.steps.size();
    size_t now = 0;
    state.push_back(new RunState(0,0,NULL)); // initial state
    for (i = 0; i <= n; i++) {
        const size_t end = state.size();
        const int step = i == n ? 0 : test.steps[i];
        for (now = now; now < end; now++) {
            RunState *st = state[now];
            if (st->x == test.x && st->y == test.y) break;
            const int x = st->x, y = st->y;
            state.push_back(new RunState(x+step, y, st)); // (x+)
            state.push_back(new RunState(x, y+step, st)); // (y+)
            state.push_back(new RunState(x-step, y, st)); // (x-)
            state.push_back(new RunState(x, y-step, st)); // (y-)
            state.push_back(new RunState(x, y, st)); // (S)
        }
        if (now < end) break;
    }
    if (i <= n) {
        std::cout << "Steps: " << i << std::endl;
        state[now]->showPath(test);
    }
    else {
        std::cout << "No solution" << std::endl;
    }
    for (RunState *i : state) { // clean up
        delete i;
    }
}

// start here
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
    // read each line
    while (std::getline(f, line)) {
        std::stringstream hey;
        hey << line;
        std::string op;
        TestData test;
        // parse line
        if (!(hey >> op)) continue;
        if (!(hey >> test.x)) DIE("missing initial x coordinate\n");
        if (!(hey >> test.y)) DIE("missing initial y coordinate\n");
        int n;
        while (hey >> n) {
            if (n > 9) DIE("step too big\n");
            if (n < 0) DIE("step too small\n");
            test.steps.push_back(n);
        }
        // search
        if (op == "BFS") {
            std::cout << op << ' ';
            bfs(test);
        }
        else if (op == "IDS") {
            std::cout << op << ' ';
        }
        else if (op == "A*") {
            std::cout << op << ' ';
        }
        else {
            DIE("unknown search strategy \"%s\"\n", op.c_str());
        }
    }
    f.close();
    return 0;
}
