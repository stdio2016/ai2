#include <chrono>
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

// search algorithms
struct Point {
    int x,y;
    Point() {}
    Point(int x, int y): x(x), y(y) {}
};

struct BfsNode {
    Point pos;
    int prev;
    BfsNode() {}
    BfsNode(int x, int y, int prev): pos(x,y), prev(prev) {}
};

void showPath(const std::vector<Point> &path, const TestData &t) {
    std::cout << "initial (0,0)" << std::endl;
    for (size_t i = 1; i < path.size(); i++) {
        Point now = path[i];
        Point prev = path[i-1];
        if (now.x > prev.x) std::cout << "(x+) ";
        else if (now.y > prev.y) std::cout << "(y+) ";
        else if (now.x < prev.x) std::cout << "(x-) ";
        else if (now.y < prev.y) std::cout << "(y-) ";
        else std::cout << "(S)  ";
        std::cout << t.steps[i-1] << "  (" << now.x << ',' << now.y << ')' << std::endl;
    }
}

void bfs(const TestData &test) {
    std::vector<BfsNode> state;
    int i;
    const int n = test.steps.size();
    size_t now = 0;
    state.emplace_back(0,0,0); // initial state
    for (i = 0; i <= n; i++) {
        const size_t end = state.size();
        const int step = i == n ? 0 : test.steps[i];
        for (now = now; now < end; now++) {
            BfsNode st = state[now];
            const int x = st.pos.x, y = st.pos.y;
            if (x == test.x && y == test.y) break;
            if (i < n) {
                state.emplace_back(x+step, y, now); // (x+)
                state.emplace_back(x, y+step, now); // (y+)
                state.emplace_back(x-step, y, now); // (x-)
                state.emplace_back(x, y-step, now); // (y-)
                state.emplace_back(x, y, now); // (S)
            }
        }
        if (now < end) break;
    }
    if (i <= n) { // show path
        std::cout << "Steps: " << i << std::endl;
        std::vector<Point> path(i+1);
        path[0] = Point(0,0);
        while (i > 0) {
            path[i] = state[now].pos;
            now = state[now].prev;
            i--;
        }
        showPath(path, test);
    }
    else {
        std::cout << "No solution" << std::endl;
    }
    std::cout << "Reached node count: " << state.size() << std::endl;
}

struct DfsNode { // a linked list
    Point pos;
    DfsNode *next; // next step
    DfsNode() {}
    DfsNode(int x, int y, DfsNode *next): pos(x, y), next(next) {}
};

static int idsReachedCount;
static DfsNode *idsHelper(const TestData &test, int depth, int x, int y, int i) {
    idsReachedCount++;
    if (depth == i) { // maximum depth reached
        if (x == test.x && y == test.y) { // get target
            return new DfsNode(x,y,NULL);
        }
        else return NULL;
    }
    int k = test.steps[i];
    DfsNode *s;
    // if condition is true, it means we found a solution
    if (s = idsHelper(test, depth, x+k, y, i+1)) return new DfsNode(x,y,s); //(x+)
    if (s = idsHelper(test, depth, x, y+k, i+1)) return new DfsNode(x,y,s); //(y+)
    if (s = idsHelper(test, depth, x-k, y, i+1)) return new DfsNode(x,y,s); //(x-)
    if (s = idsHelper(test, depth, x, y-k, i+1)) return new DfsNode(x,y,s); //(y-)
    if (s = idsHelper(test, depth, x, y, i+1)) return new DfsNode(x,y,s); //(S)
    return NULL; // no solution
}

void ids(const TestData &test) {
    idsReachedCount = 0;
    const int n = test.steps.size();
    int i;
    DfsNode *s = NULL;
    for (i = 0; i <= n; i++) {
        s = idsHelper(test, i, 0, 0, 0);
        if (s != NULL) break; // solution found
    }
    if (s != NULL) {
        std::cout << "Steps: " << i << std::endl;
        std::vector<Point> path;
        for (DfsNode *p = s; p != NULL; p = p->next) {
            path.push_back(p->pos);
        }
        showPath(path, test);
    }
    else {
        std::cout << "No solution" << std::endl;
    }
    std::cout << "Reached node count: " << idsReachedCount << std::endl;
    while (s != NULL) { // cleanup
        DfsNode *p = s;
        s = s->next;
        delete p;
    }
}

void astar(const TestData &test) {
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
        typedef std::chrono::time_point<std::chrono::steady_clock> TimeType;
        TimeType start = std::chrono::steady_clock::now();
        std::cout << op << ' ';
        test.show();
        if (op == "BFS") {
            bfs(test);
        }
        else if (op == "IDS") {
            ids(test);
        }
        else if (op == "A*") {
            astar(test);
        }
        else {
            std::cout << "unknown search strategy \"" << op << "\"" << std::endl;
        }
        TimeType end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Time used: " << diff.count() << " seconds" << std::endl;
    }
    f.close();
    return 0;
}
