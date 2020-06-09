// task 3: find matrix of largest paths from each to each

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

enum class color {
    white, black
};


void print_matrix(std::vector<std::vector<int>> &m, std::vector<int> weights) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m.size(); j++){
            if (m[i][j] < 0) {
                std::cout << 0 << " ";
            }

            else {
                m[i][j] += weights[j];
                std::cout << m[i][j] << " ";
            }

        }
        std::cout << "\n";
    }
}

void topological_visit(int i, std::vector<std::vector<int> > &a, std::vector<color> &colors, std::vector<int> &out_list){
    colors[i] = color::black;

    for (int j = 0; j < a.size(); j++)
        if (a[i][j] != -30000 && colors[j] == color::white)
            topological_visit(j, a, colors, out_list);

    out_list.push_back(i);
}

std::vector<int> topological_sort(std::vector<std::vector<int> > &a) {
    std::vector<int> out_list;
    std::vector<color> colors(a.size());

    for (int i = 0; i < colors.size(); i++) 
        colors[i] = color::white;
    
    for (int i = 0; i < a.size(); ++i)
        if (colors[i] == color::white) 
            topological_visit(i, a, colors, out_list);

    std::reverse(out_list.begin(), out_list.end());

    return out_list;
}

void get_matrix(std::vector<std::vector<int> > &a, std::vector<int> sor) {
    for (int i = 0; i < a.size(); i++) 
        for (int k = i+1; k < a.size(); k++) 
            for (int j = k+1; j < a.size(); j++) 
                if (a[sor[i]][sor[j]] < a[sor[i]][sor[k]] + a[sor[k]][sor[j]]) 
                    a[sor[i]][sor[j]] = a[sor[i]][sor[k]] + a[sor[k]][sor[j]];
}

int main() {
    std::ifstream fin("input.txt");

    int size;
    int w;

    fin >> size;

    std::vector<int> weights(size);
    
    std::vector<std::vector<int> > matrix(size, std::vector<int>(size));

    for (int i = 0; i < size; i++) {
        fin >> w;
        weights[i] = w;
    }

    for (int k = 0; k < size; k++) {
        for (int col = 0; col < size; col++){
            fin >> w;

            if (w == 1) 
                matrix[k][col] = weights[k];
            else {
                matrix[k][col] = -30000;
            }
        }
    }

    std::vector<int> sorted = topological_sort(matrix);
    get_matrix(matrix, sorted);
    print_matrix(matrix, weights);

}