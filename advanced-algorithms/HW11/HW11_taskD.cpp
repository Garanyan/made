//
// Created by Ovanes Garanyan on 5/24/20.
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
        if (pos[i] < n)
            rank.at(pos[i]) = i;
    }
    for (int i = 0, k = 0; i < n; ++i){
        if (k > 0){
            k--;
        }
        if (rank[i] == n - 1){
            lcp.at(n - 1) = -1;
            k = 0;
            continue;
        }
        int j = pos.at(rank[i] + 1);
        while (std::max(i + k, j + k) < text.length() && text[i + k] == text[j + k]){
            k++;
        }
        lcp[rank[i]] = k;
    }

}
//void calcLcp(std::string& s, std::vector<int> p, pos){
////lcp[n]
//int prev = 0;
//    for (int i = 0; i<  n; ++i)
//    {
//        j = p[pos[i] - 1]
//        cur = prev
//        while (s[i + cur] = s[j + cur]){
//            cur++
//        }
//        lcp[pos[i]] = cur
//        prev = std::max(cur - 1, 0);
//    }
//return lcp
//}

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
    std::string pattern;
    std::cin >> line;
    size_t first_line_size = line.size();
    std::cin >> pattern;
    line += "#";
    line += pattern;
    std::vector<int> suffixus = suffix_array(line);

    std::vector<int> lcp(suffixus.size() + 1);
    kasai(line, suffixus, lcp);

    int substr_size = 0;
    int substr_pos = -1;
    for (int i = 0; i + 1 < suffixus.size(); ++i){
        bool in_first_stirng = suffixus[i] < first_line_size;
        bool in_second_string = !(suffixus[i + 1] < first_line_size);

        bool next_in_first_stirng = suffixus[i + 1] < first_line_size;
        bool next_in_second_string = !(suffixus[i] < first_line_size);
        if (in_first_stirng && in_second_string || next_in_first_stirng && next_in_second_string){
            if (lcp[i] > substr_size){
                substr_pos = suffixus[i];
                substr_size = lcp[i];
            }
        }
    }


    for (int i = 0; i < substr_size; ++i){
        std::cout << line[substr_pos + i];
    }
    std::cout << std::endl;


    return 0;
}
