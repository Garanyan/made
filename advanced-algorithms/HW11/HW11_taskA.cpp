//
// Created by Ovanes Garanyan on 5/20/20.
//

#include <iostream>
#include <vector>
#include <map>

// Алгоритм Аримуры — Арикавы — Касаи — Ли — Пака
// lcp[x] - длина наибольшего общего префикса суффиксов pos[x] и pos[x + 1]
void kasai(const std::string &text, const std::vector<int> &pos, std::vector<int> &lcp) {
    int n = text.length();
    std::vector<int> rank(n);
    for (int i = 0; i < n; ++i){
        rank[pos[i]] = i;
    }
    for (int i = 0, k = 0; i < n; ++i){
        if (k > 0){
            k--;
        }
        if (rank[i] == n - 1){
            lcp[n - 1] = -1;
            k = 0;
            continue;
        }
        int j = pos[rank[i] + 1];
        while (std::max(i + k, j + k) < text.length() && text[i + k] == text[j + k]){
            k++;
        }
        lcp[rank[i]] = k;
    }

}

std::vector<int> suffix_array(const std::string &line) {
    char terminal = '$';
    std::vector<int> s(line.size(), 0);
    for (int i = 0; i < line.size(); ++i){
        s[i] = line[i];
    }
    s.push_back(terminal);

    int n = s.size(), cnt = 0, previous_number_of_equivalence_classes = 0;
    std::vector<int> previous_equivalence_classes(n), p(n);
    std::map<int, std::vector<int> > t;
    for (int i = 0; i < n; i++)
        t[s[i]].push_back(i);

    for (auto &x : t){
        for (int u : x.second){
            previous_equivalence_classes[u] = previous_number_of_equivalence_classes;
            p[cnt++] = u;
        }
        previous_number_of_equivalence_classes++;
    }

    for (int l = 1; previous_number_of_equivalence_classes < n; l++){
        std::vector<std::vector<int> > a(previous_number_of_equivalence_classes);
        std::vector<int> next_equivalence_classes(n);
        int next_number_of_equivalence_classes = 0;
        int cnt = 0;
        int power_of_two = (1 << l) / 2;

        for (int i = 0; i < n; i++){
            int k = (p[i] - power_of_two + n) % n;
            a[previous_equivalence_classes[k]].push_back(k);
        }

        for (int i = 0; i < previous_number_of_equivalence_classes; i++){
            for (size_t j = 0; j < a[i].size(); j++){
                // |z|c[]|
                // |q|c[]|
                // сравнение класс эквивалентности текущего начала и предыдущего начала, при том же классе эквивалентности конца
                int z = (a[i][j] + power_of_two) % n;
                if (j == 0 ||
                    previous_equivalence_classes[z] != previous_equivalence_classes[(a[i][j - 1] + power_of_two) % n]){
                    next_number_of_equivalence_classes++;
                }

                next_equivalence_classes[a[i][j]] = next_number_of_equivalence_classes - 1;
                p[cnt++] = a[i][j];
            }
        }

        previous_equivalence_classes = next_equivalence_classes;
        previous_number_of_equivalence_classes = next_number_of_equivalence_classes;
    }

    return std::vector<int>(p.begin() + 1, p.end());
}

int main() {
    std::string line;
    std::cin >> line;

    std::vector<int> suffixus;
    suffixus = suffix_array(line);

    std::vector<int> lcp(line.size());
    kasai(line, suffixus, lcp);

    for (auto i: suffixus){
        std::cout << i + 1 << " ";
    }
    std::cout << "\n";
    for (int i = 0; i + 1 < line.size(); ++i){
        std::cout << lcp[i] << " ";
    }
    return 0;
}
