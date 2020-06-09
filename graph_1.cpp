// task 1: alphabet

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <fstream>

struct vertex;

struct my_time;

std::vector<std::pair<vertex, std::list<int> > > vertices;
bool contoured = false;

enum color {
    white = 0,
    gray,
    black
};

struct my_time {
    int start = 0;
    int finish = 0;

    my_time(int start = 0, int finish = 0) : start(start), finish(finish) { }
};

struct vertex {
    int id;
    char value;
    color m_color;
    my_time m_time;
    int parent;
    int distance;

    vertex(int id = -1, char value = 'S', color c = color::white, my_time t = {0, 0}, int p = -1, int d = -1) 
        : id(id), value(value), m_color(c), m_time(t), parent(p), distance(d) { }

    vertex(const vertex& v) : id(v.id), value(v.value), m_color(v.m_color), m_time(v.m_time), parent(v.parent), distance(v.distance) { }
};

vertex get_vertex(int id) {
    return vertices[id].first;
}

std::list<int> get_adj(int id) {
    return vertices[id].second;
}

bool is_connected(int id1, int id2) {
    std::list<int> l = get_adj(id1);
    std::list<int>::iterator it = std::find(std::begin(l), std::end(l), id2);
    return it != std::end(l);
}

std::string color_to_string(color c){
    switch(c){
    case color::white:
        return "white";
    case color::gray:
        return "gray";
    default:
        return "black";
    }
}

void print(){
    std::cout << "GRAPH: size: " << vertices.size() << std::endl;
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << i << ": " << vertices[i].first.value << " (" << color_to_string(vertices[i].first.m_color) << "), {" << vertices[i].first.m_time.start 
            << "," <<  vertices[i].first.m_time.finish << "}: ";
        for (auto elem : vertices[i].second)
            std::cout << vertices[elem].first.value << ", ";
        std::cout << std::endl;
    }
}


//______________________________________


void add_vertex(vertex v){
    if (v.id > vertices.size())
        return;

    if (v.id < 0) {
        v.id = vertices.size();
    }

    if (v.id == vertices.size())
        vertices.push_back(std::make_pair(v, std::list<int>()));
}

void add_arc(vertex start, vertex finish){
    add_vertex(start);
    add_vertex(finish);
    add_vertex(start);

    vertices[start.id].second.push_back(finish.id);
}

//_____________________________________


void add_unique_vertex(vertex &v) {
    auto it = std::find_if(std::begin(vertices), std::end(vertices), [&v](const std::pair<vertex, std::list<int> >& pair) {
        return pair.first.value == v.value;
    });

    if (it != std::end(vertices)) {
        //std::cout << "vertex " << v.value << " with id == " << v.id << " is already in graph\n";
        v = it->first;
        return; 
    }

    if (v.id < 0) {
        v.id = vertices.size();
        //std::cout << "vertex " << v.value << " takes id == " << v.id << std::endl;
    }

    vertices.push_back(std::make_pair(v, std::list<int>()));

    //std::cout << std::endl;
    //std::cout << "vertex " << v.value << " with id == " << v.id << " is added\n";
}

void add_unique_arc(vertex &start, vertex &finish) {
    //std::cout << "add_unique_arc: " << start.value << ", " << finish.value << std::endl;
    add_unique_vertex(start);
    add_unique_vertex(finish);

    if (!is_connected(start.id, finish.id)) {
        //std::cout << "OK: arc is added\n";
        vertices[start.id].second.push_back(finish.id);
    }
}

void add_all(std::string &str){
    for (int i = 0; i < str.size(); ++i) {
        vertex tmp = vertex(-1, str[i]);
        add_unique_vertex(tmp);
    }
}

//______________________________________

//  ТОПОЛОГИЧЕСКАЯ СОРТИРОВКА

void topological_visit(vertex& v, int &t, std::list<char>& out_list){
    t += 1;
    v.m_time.start = t;
    v.m_color = color::gray;

    for (auto u : vertices[v.id].second){
        if (vertices[u].first.m_color == color::white){
            vertices[u].first.parent = v.id;
            topological_visit(vertices[u].first, t, out_list);
        }
        else if (vertices[u].first.m_color == color::gray) {
            contoured = true;
        }
    }

    v.m_color = color::black;
    t += 1;
    v.m_time.finish = t;
    out_list.push_front(v.value);
}

std::list<char> topological_sort() {
    std::list<char> out_list;
    int time;
    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i].first.m_color = color::white;
        vertices[i].first.parent = -1;
    }
    time = 0;

    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i].first.m_color == color::white) {
            topological_visit(vertices[i].first, time, out_list);
        }
    }

    return out_list;
}

//______________________________________


void alphabet(std::vector<std::string> &strings) {
    vertex v1;
    vertex v2;
    for (int i = 0; i < strings.size() - 1; ++i) {
        std::string str1 = strings[i];
        std::string str2 = strings[i + 1];
        //std::cout << "STR1 == " << str1 << std::endl;
        //std::cout << "STR2 == " << str2 << std::endl;

        for (int j = 0; j < str1.size() && j < str2.size(); ++j) {
            if (str1[j] != str2[j]) {
                v1.value = str1[j];
                v2.value = str2[j];
                add_unique_arc(v1, v2);
                break;
            }
        }   
    }
}

//______________________________________

int main() {
    std::ifstream fin("input.txt");
    std::vector<std::string> strings;
    std::string str;

    int size;
    fin >> size;

    fin >> str;
    strings.push_back(str);

    char first_letter = str[0];
    vertex fl(-1, first_letter);
    add_unique_vertex(fl);

    while (fin.peek() != EOF) {
        fin >> str;
        add_all(str);
        
        if (str[0] == first_letter) {
            strings.push_back(str);
            
        }
        else {
            alphabet(strings);
            strings.clear();
            vertex nextv(-1, str[0]);
            strings.push_back(str);
            add_unique_arc(fl, nextv);
            fl = nextv;
            first_letter = str[0];
        
        }

        if (fin.peek() == EOF) {
            alphabet(strings);
        }
    }

    //print();

    std::list<char> res = topological_sort();
    if (contoured) {
        std::cout << "-";
    }

    else {
        for (auto elem : res) {
            std::cout << elem;
        }
    }

    return 0;
}