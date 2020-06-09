// task 2: stalker

#include <iostream>
#include <fstream>
#include <list>
#include <vector>


// weight - вес дуги
// id - куда дуга приходит
struct vertex {
    int weight;
    int id;

    vertex(int weight = 0, int id = 0) : weight(weight), id(id) { }
};


// добавление дуги в граф
void add_arc(std::vector<std::vector<vertex> >& vertices, int id1, int id2, int buildings)
{
    vertices[id1].push_back(vertex(0, id2));
    vertices[id2].push_back(vertex(0, id1));

    int id1_fake;
    int id2_fake;

    if (id1 % buildings == 0)
        id1_fake = buildings;
    else
        id1_fake = id1 % buildings;
    if (id2 % buildings == 0)
        id2_fake = buildings;
    else
        id2_fake = id2 % buildings;

    // добавление фиктивных дуг
    vertices[id1].push_back(vertex(1, id1_fake));
    vertices[id2].push_back(vertex(1, id2_fake));

    vertices[id1_fake].push_back(vertex(0, id1));
    vertices[id2_fake].push_back(vertex(0, id2));
}


void bfs(std::vector<std::vector<vertex> >& arcs, std::vector<int>& distance, std::vector<bool>& color, int id) {
    distance[id] = 0;
    std::list<int> Q;
    Q.push_back(id);

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop_front();

        if (color[u] == 0) {
            for (auto vert : arcs[u]) {
                auto dist = distance[u] + vert.weight;

                if(distance[vert.id] > dist) {
                    distance[vert.id] = dist;

                    if (vert.weight == 1)
                        Q.push_back(vert.id);
                    else
                        Q.push_front(vert.id);
                }
            }

            color[u] = 1;
        }
    }
}


int main()
{
    int buildings = 0;
    int maps = 0;
    int roads_on_map = 0;
    int road_begin = 0;
    int road_end = 0;

    std::ifstream fin("input.txt");

    fin >> buildings >> maps;

    std::vector<int> distance(buildings * (maps + 1) + 2, INT16_MAX);
    std::vector<bool> color(buildings * (maps + 1) + 2, 0);
    std::vector<std::vector<vertex> > arcs(buildings * (maps + 1) + 2, std::vector<vertex>());

    int map_counter = buildings;
    for (int i = 0; i < maps; ++i) {
        fin >> roads_on_map;

        while (roads_on_map > 0) {
            fin >> road_begin >> road_end;
            add_arc(arcs, road_begin + map_counter, road_end + map_counter, buildings);
            --roads_on_map;
        }

        map_counter += buildings;
    }

    bfs(arcs, distance, color, 1);

    // проверка, можно ли решить задачу (есть ли путь)
    if (distance[buildings] == INT16_MAX) {
        std::cout << "-1";
        return 0;
    }

    std::cout << distance[buildings];

    return 0;
}