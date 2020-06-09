// task 6: find max flow

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

bool pseudo_dijkstra(vector<vector<vector<int>>> &graph, vector<pair<int, bool>> &way, int &flow) {
    bool flag = false; 
    vector<int> values(graph.size(), 0); // graph.size() == mags_count + 2

    set<int> Q;
    Q.insert(0);
    values[0] = 30000;

    while (Q.size()) {
        int u = *Q.begin();
        int maxval = values[u];

        for (int i : Q)
            if (values[i] > maxval) {
                maxval = values[i];
                u = i;
            }

        for (int v = 0; v < graph.size() - 1; v++) { // - 1
            int min1 = values[u];

            if (graph[u][v][0] != 0) {
                

                if (min1 >= graph[u][v][0] - graph[u][v][1]) 
                    min1 = graph[u][v][0] - graph[u][v][1];

                if (min1 > values[v]) {
                    Q.insert(v);
                    values[v] = min1;
                    way[v].first = u;
                    way[v].second = true; 
                }
            }

            if (graph[v][u][0] > 0) {
                
                if (min1 >= graph[v][u][1])
                    min1 = graph[v][u][1];

                if (min1 > values[v]) {
                    Q.insert(v);
                    values[v] = min1;
                    way[v].first = u;
                    way[v].second = false; 
                }
            }   
        }
        
        if (values[graph.size()-2] != 0) {
            flow = values[graph.size()-2];
            Q.clear();
            return true;
        }

        else
            Q.erase(u);
    }

    return flag;
}


int find_flow(vector<vector<vector<int>>> & graph, int source) {
    int flow = 0;
    int max_stream = 0;
    vector<pair<int, bool>> way(graph.size()); 
    way[source].first = -1;

    while (pseudo_dijkstra(graph, way, flow)) {
        int parent = graph.size()-2;

        while (way[parent].first != -1) {
            if (way[parent].second == true) // прямая
                graph[way[parent].first][parent][1] += flow;
                
            else
                graph[parent][way[parent].first][1] -= flow;

            parent = way[parent].first;
        }

        max_stream += flow;

        flow = 0;
    }
    return max_stream;
}

int main() {
    int mags_count, power, tower, neighbours_count, powerbank = 0;
    ifstream fin("input.txt");
    vector<int> neighbours; // вектор для хранения количества соседей для каждого мага
    vector<int> mag_power; // сколько энергии может принять каждый маг

    fin >> mags_count;

    vector<vector<vector<int>>> graph(mags_count + 2, vector<vector<int>>(mags_count + 2, vector<int>(2, 0)));

    fin >> power >> tower >> neighbours_count;
    mag_power.push_back(power);
    neighbours.push_back(neighbours_count);

    for (int i = 1; i < mags_count; i++) {
        fin >> power >> tower >> neighbours_count;

        powerbank += power;
        neighbours.push_back(neighbours_count);
        mag_power.push_back(power);

        graph[i][mags_count][0] = mag_power[i]; // пропускная способность дуги = количеству энергии мага (вес вершины)
        graph[i][mags_count][1] = 0; // величина потока

        if (tower == 1) {
            graph[i][mags_count + 1][0] = 30000; // маг может атаковать башню (пропускная способность) 
            graph[i][mags_count + 1][1] = 0; // величина потока
        }

    }

    for (int i = 0; i < mags_count; i++){
        int curr_neighbours_count = neighbours[i]; // какое количество соседей у текущего мага

        if (curr_neighbours_count > 0) {
            int mag_id; // кому
            int power; // сколько можно передать

            for (int j = 0; j < curr_neighbours_count; j++) {
                fin >> mag_id >> power;

                graph[i][mag_id][0] = power; // сколько можно передать от мага i магу mag_id (пропускная способность)
                graph[i][mag_id][1] = 0;
            }
        }
    }

    int curr_flow = find_flow(graph, 0); // до фиктивной вершины

    if (curr_flow != powerbank) {
        std::cout << -1;
        return 0;
    }
    
    for (int i = 0; i < mags_count; i++) {
        graph[i][mags_count][0] = graph[i][mags_count + 1][0];
        graph[i][mags_count][1] = graph[i][mags_count + 1][1];

        for (int j = 0; j < mags_count; j++) 
            if (graph[i][j][1] > 0) {
                graph[j][i][0] += graph[i][j][1];
                graph[i][j][0] -= graph[i][j][1];
                graph[i][j][1] = 0;
            }
    }

    int res_flow = find_flow(graph, 0);

    std::cout << res_flow; // в башню

    return 0;
}