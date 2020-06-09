// task 4: find size of second-best spanning tree

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <fstream>
#include <list>
#include <set>

using edge = std::pair<int, int>;

void add_edge(std::vector<std::pair<int, edge> > &edges, int &weight, int &u, int &v) {
    edges.push_back(std::make_pair(weight, std::make_pair(u, v)));
}

void print_edges(std::vector<std::pair<int, edge> > &edges) {
    for (auto pair : edges) {
        std::cout << pair.first << " [" << pair.second.first << "," << pair.second.second << "] \n";
    }
}

unsigned long sum_edges(std::vector<std::pair<int, edge> > &edges){
    unsigned long s = 0;

    for (auto edge : edges) {
        s += edge.first;
    }

    return s;
}

bool find_set(std::list<std::set<int>> &sets, int u, int v){
    auto it1 = std::find_if(sets.begin(), sets.end(), [u](const std::set<int>& set){
        return set.find(u) != set.end();
    });

    auto it2 = std::find_if(sets.begin(), sets.end(), [v](const std::set<int>& set){
        return set.find(v) != set.end();
    });

    if (it1 == it2) {
        return true;
    }

    it1->insert(it2->begin(), it2->end());
    sets.erase(it2);

    return false;
}

std::vector<std::pair<int, edge> > kruskal(std::vector<std::pair<int, edge> > &edges) {
    std::vector<std::pair<int, edge> > mst;
    mst.reserve(edges.size());
    std::list<std::set<int>> sets;

    for (int i = 1; i <= edges.size() + 1; i++) {
        sets.push_back(std::set<int>());
        sets.back().insert(i);
    }

    std::sort(edges.begin(), edges.end());

    for (auto edge : edges) {
        if (!find_set(sets, edge.second.first, edge.second.second)) {
            mst.push_back(std::make_pair(edge.first, std::make_pair(edge.second.first, edge.second.second)));
        }
    }

    return mst;
}

unsigned long second_best_mst(std::vector<std::pair<int, edge> > &edges){
    std::vector<std::pair<int, edge> > mst = kruskal(edges);
    std::vector<std::pair<int, edge> > cur_mst;
    unsigned long cur_sum;
    std::list<unsigned long> sums;

    //std::cout << "min sum = " << sum_edges(mst) << "\n";

    for (auto edge : mst) {

        auto it = std::find(edges.begin(), edges.end(), edge);
        auto edge_tmp = *it;

        edges.erase(std::find(edges.begin(), edges.end(), edge));

        cur_mst = kruskal(edges);

        if (cur_mst.size() == mst.size()) {
            cur_sum = sum_edges(cur_mst);      
            //std::cout << "cur sum = " << cur_sum << " ";   
            sums.push_back(cur_sum);
        }

        edges.push_back(edge_tmp); 
    }

    //sums.sort();
    return *std::min_element(sums.begin(), sums.end());;

}


int main() {

    std::ifstream fin("input.txt");
    int n, m, id1, id2, weight;

    fin >> n >> m;

    std::vector<std::pair<int, edge> > edges;
    edges.reserve(m);

    for (int i = 0; i < m; i++) {
        fin >> id1 >> id2 >> weight;
        add_edge(edges, weight, id1, id2);
    }

    std::cout << second_best_mst(edges);

    fin.close();

    return 0;
}
