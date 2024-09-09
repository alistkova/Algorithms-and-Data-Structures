#include <fstream>
#include <iostream>

struct Edge {
    int start;
    int end;
    int w;
};

template <typename T, typename Comp>
void sort(T* data, int start, int end, Comp comp) {
    if (start >= end) {
        return;
    }

    int i = start;
    int j = end;
    T pivot = data[(i + j) / 2];

    while (i <= j) {
        while (comp(data[i], pivot)) {
            ++i;
        }
        while (comp(pivot, data[j])) {
            --j;
        }
        if (i <= j) {
            std::swap(data[i], data[j]);
            ++i;
            --j;
        }
    }
    sort(data, start, j, comp);
    sort(data, i, end, comp);
}

class DSU {
   private:
    int* data_;
    int* rank_;

   public:
    DSU(int n) : data_(new int[n]), rank_(new int[n]) {
        for (int i = 0; i < n; ++i) {
            data_[i] = i;
            rank_[i] = 0;
        }
    }

    ~DSU() {
        delete[] data_;
        delete[] rank_;
    }

    int find(int x) {
        if (data_[x] == x) {
            return x;
        }
        return data_[x] = find(data_[x]);
    }

    void intersect(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx != ry) {
            if (rank_[rx] < rank_[ry]) {
                std::swap(rx, ry);
            }
            data_[ry] = rx;
            if (rank_[rx] == rank_[ry]) {
                ++rank_[rx];
            }
        }
    }
};

int kruskal(Edge* edges, int nv, int ne) {
    DSU dsu(nv);
    sort(edges, 0, ne - 1, [](Edge& l, Edge& r) { return l.w < r.w; });
    int w = 0;
    for (int i = 0; i < ne; ++i) {
        int start = edges[i].start;
        int end = edges[i].end;
        if (dsu.find(start) != dsu.find(end)) {
            w += edges[i].w;
            dsu.intersect(start, end);
        }
    }
    return w;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Wrong number of arguments given" << std::endl;
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cout << "Failed to open input file" << std::endl;
        return 1;
    }

    int nv;
    int ne;
    input >> nv >> ne;
    Edge* edges = new Edge[ne];
    for (int i = 0; i < ne; ++i) {
        int start, end, w;
        input >> start >> end >> w;
        edges[i] = Edge{start, end, w};
    }

    int w = kruskal(edges, nv, ne);
    std::cout << w;

    input.close();
    delete[] edges;

    return 0;
}
