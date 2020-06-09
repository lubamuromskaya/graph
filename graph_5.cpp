// task 5: find shortest way, but longest for all the friends

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <list>

struct vertex {
    std::vector<int> parents;
    int distance;
    int label;
    std::vector<int> labels;

    vertex(int d = INT16_MAX) 
        : parents(1, -1), distance(d), label(-1) { }
};

// elem.first - weight, elem.second - id
void print(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << i << ": (" << vertices[i].first.distance << ") " <<  " ---> ";

        for (auto elem : vertices[i].second)
            std::cout << "[" << elem.second << ", " << elem.first << "], ";
            
        std::cout << std::endl;
    }
}

// формирование графа
void add_arc_in_vertices(std::vector<std::pair<vertex, std::list<std::pair<int, int> > > > &vertices, int v1, int v2, int weight){
    vertices[v1].second.push_back(std::make_pair(weight, v2));
    vertices[v2].second.push_back(std::make_pair(weight, v1));
}

// проверка: parent in v.parents 
bool in_parents(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, int v, int parent) {
    for (int par : vertices[v].first.parents)
        if (par == parent)
            return true;
    return false;
}

// проверка: label in v.labels
bool in_labels(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, int v, int label) {
    for (int l : vertices[v].first.labels)
        if (l == label)
            return true;
    return false;
}

void relax(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, int u, std::pair<int, int> &v, std::list<std::pair<int, int>> &Q){
    // v: v.first = weight(u,v)
    //    v.second = id(v)

    if (vertices[v.second].first.distance > vertices[u].first.distance + v.first){
        Q.remove({vertices[v.second].first.distance, v.second}); // удаляем из очереди старую пару со старой дистаницей

        vertices[v.second].first.distance = vertices[u].first.distance + v.first; // изменяем дистанцию вершины на новую
        Q.push_back(std::make_pair(vertices[v.second].first.distance, v.second)); // добавляем в очередь новую пару {дистанция, вершина}

        if (!in_parents(vertices, v.second, u))
            vertices[v.second].first.parents[0] = u;

    }
}

// алгоритм Дейкстры
void dijkstra(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices){
    vertices[0].first.distance = 0;

    std::list<std::pair<int, int>> Q;
    // здесь пара это <distance, id>

    for (int i = 0; i < vertices.size(); i++)
        Q.push_back(std::make_pair(vertices[i].first.distance, i));

    while (!Q.empty()) {
        Q.sort();
        int u = Q.front().second; // first - distance, second - id
        Q.pop_front();
        
        for (auto v : vertices[u].second)
            relax(vertices, u, v, Q);
    }
}

// составляем граф кратчайших путей (фактически - заполняем вектор parents для каждой вершины)
void add_parents(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > >  &vertices){
    for (int i = 0; i < vertices.size(); i++) {
        for (auto pair : vertices[i].second) {
            if (vertices[i].first.distance + pair.first == vertices[pair.second].first.distance && !in_parents(vertices, pair.second, i))
                vertices[pair.second].first.parents.push_back(i);
        }
    }
}

// вспомогательная функция к функции add_all_labels()
// добавить метку label в вектор labels предка
void add_labels(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, int label, std::vector<int> &parents){
    if (parents[0] == -1)
        return;
    
    for (int parent : parents) {
        if (!in_labels(vertices, parent, label))
            vertices[parent].first.labels.push_back(label);

        //if (vertices[parent].first.label == prev_label)
        //    vertices[parent].first.label = label;
        
        add_labels(vertices, label, vertices[parent].first.parents);
    }
}

// СТАРЫЙ ВАРИАНТ: проходим от всех лабораторий по предкам, ставим их метки
// ИЗМЕНЕНО: добавляем в вектор labels предка каждую метку, от которой идем
void add_all_labels(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, std::vector<int> &platforms) {

    // СТАРЫЙ ВАРИАНТ с использование int label, которую мы просто изменяем каждый раз.

    // для 1 платформы (перебираем всех ее предков, ставим им ее метку)

    //for (int parent : vertices[platforms[0]].first.parents) 
    //        vertices[parent].first.label = platforms[0];
    //
    //vertices[platforms[0]].first.label = platforms[0];

    // для платформ со 2 до последней используем алгоритм:

    //for (int i = 1; i < platforms.size(); i++) {
    //    int prev_label = platforms[i-1];
    //    int current_label = platforms[i];
    
    //    vertices[current_label].first.label = current_label;

    //    add_labels(vertices, current_label, vertices[current_label].first.parents, prev_label);
    //} 

    // НОВЫЙ ВАРИАНТ с использованием вектора labels и добавлением в него меток:

    for (int platform : platforms) {
        if (!in_labels(vertices, platform, platform))
            vertices[platform].first.labels.push_back(platform);

        add_labels(vertices, platform, vertices[platform].first.parents);
    }
}


// СТАРЫЙ ВАРИАНТ: если у какой-то вершины стоит последний label, добавляем ее дистанцию
// ИЗМЕНЕНО: если количество лэйблов в векторе labels предка такое же, как и количество platforms, 
// значит у этого предка есть все метки, и мы добавляем его distance
void check_distance(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, std::vector<int> &parents, std::vector<int> &distances, std::vector<int> &platforms) {
    if (parents[0] == -1)
        return;

    for (int parent : parents) {
        if (vertices[parent].first.labels.size() == platforms.size()) {
            distances.push_back(vertices[parent].first.distance);
        }

        check_distance(vertices, vertices[parent].first.parents, distances, platforms);
    }

}

// СТАРЫЙ ВАРИАНТ: проходим по всем вершинам, ищем те, у которых стоят метки последней лаборатории, выбираем из них ту, у которой дистанция максимальна
// ИЗМЕНЕНО: прохоим по всем вершинам, ищем те, у которых в labels находятся метки всех лабораторий, выбираем из них ту, у которой дистанция максимальна
int find_general_parent(std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > &vertices, std::vector<int> &platforms) {
    std::vector<int> distances(1, 0);
    int last_label = platforms[platforms.size()-1];

    for (int platform : platforms)
        check_distance(vertices, vertices[platform].first.parents, distances, platforms);

    return *std::max_element(distances.begin(), distances.end());
}

int main()
{
    int n, m, k, p, id1, id2, weight;
    std::vector<int> platforms;

    std::ifstream fin("input.txt");
    fin >> n >> m >> k;

    std::vector<std::pair<vertex, std::list<std::pair<int, int>> > > vertices(n);

    for (int i = 0; i < k; i++){
        fin >> p;
        platforms.push_back(p);
    }

    for (int i = 0; i < m; i++){
        fin >> id1 >> id2 >> weight;
        add_arc_in_vertices(vertices, id1, id2, weight);
        
    }

    dijkstra(vertices);
    add_parents(vertices);
    add_all_labels(vertices, platforms);
    std::cout << find_general_parent(vertices, platforms);
    
    return 0;
}
