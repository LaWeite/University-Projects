#include <iostream>
#include <fstream>

struct Edge {
    int src, dest, weight;
};

struct Graph {
    int V, E;
    Edge* edge;
};

struct subset {
    int parent;
    int rank;
};

static void merge(Edge arr[], Edge temp[], int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (arr[i].weight <= arr[j].weight) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= m) temp[k++] = arr[i++];
    while (j <= r) temp[k++] = arr[j++];
    for (i = l; i <= r; i++) arr[i] = temp[i];
}

static void mergeSort(Edge arr[], Edge temp[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, temp, l, m);
        mergeSort(arr, temp, m + 1, r);
        merge(arr, temp, l, m, r);
    }
}

static Graph* createGraph(int V, int E) {
    Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}

static int find(subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

static void Union(subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

static int KruskalMST(Graph* graph) {
    int V = graph->V;

    Edge* temp = new Edge[graph->E];
    mergeSort(graph->edge, temp, 0, graph->E - 1);
    delete[] temp;

    subset* subsets = new subset[(V * sizeof(subset))];

    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int minWeight = 0;
    for (int i = 0; i < graph->E; ++i)
    {
        int U = graph->edge[i].src;
        int V = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (find(subsets, U) != find(subsets, V))
        {
            minWeight += weight;
            Union(subsets, U, V);
        }
    }

    delete[] subsets;

    return minWeight;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        return 1;
    }

    int V, E;
    infile >> V >> E;

    Graph* graph = createGraph(V, E);

    for (int i = 0; i < E; ++i) {
        infile >> graph->edge[i].src >> graph->edge[i].dest >> graph->edge[i].weight;
    }

    infile.close();

    std::cout << KruskalMST(graph);

    delete[] graph->edge;
    delete graph;

    return 0;
}
