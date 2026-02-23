#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> par, sz;

int root(int v) {
    if (v == par[v]) return v;
    return par[v] = root(par[v]);
}

void unite(int a, int b) {
    a = root(a);
    b = root(b);
    if (a != b) {
        if (sz[b] > sz[a]) swap(a, b);
        par[b] = a;
        sz[a] += sz[b];
    }
}

string normalize(const string& word) {
    string res;
    for (char s : word) {
        s = tolower(s);
        if ((s >= 'a' && s <= 'z') || s == '\'') {
            res += s;
        }
    }
    return res;
}

bool proof(const string& a, const string& b) {
    if (a.length() == b.length()) {
        int diff = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            if (a[i] != b[i]) {
                diff++;
            }
            if (diff > 1) {
                return false;
            }
        }
        return diff == 1;
    }

    if (abs((int)a.length() - (int)b.length()) != 1) {
        return false;
    }
    string longg, shortt;
    if ((int)a.length() > (int)b.length()) {
        shortt = b;
        longg = a;
    } else {
        shortt = a;
        longg = b;
    }

    if (longg.substr(0, shortt.length()) == shortt) {
        char s = longg.back();
        if (s == 's' || s == 'e') {
            return true;
        }
    }
    return false;
}

bool comp(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second == b.second) {
        return a.first < b.first;
    }
    return a.second > b.second;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int k;
    cin >> k;
    cin.ignore();
    vector<string> lines, words;
    vector<string> u_words;
    unordered_map<string, int> ids;
    string line, word;
    while(getline(cin, line) && !line.empty()) {
        istringstream iss(line);
        while (iss >> word) {
            words.push_back(normalize(word)); // приводим к нужному виду
        }
    }
    // выделяем уникальные слова и присваиваем каждому слову его уникальный номер
    for (string w : words) {
        if (!w.empty() && ids.find(w) == ids.end()) {
            ids[w] = u_words.size();
            u_words.push_back(w);
        }
    }
    if (u_words.empty()) {
        return 0;
    }
    /* делаем снм - это главная идея программы, чтобы быстро объединять множества, 
    а эти множества объединяются через корень => мне сразу пришла на ум такая структура данных как снм
    еще нужно сделать радиус контекстного окна - понятно, что нужно сделать сканлайн с окном размера k
    */
    int n = u_words.size();
    sz.assign(n, 1);
    par.assign(n, 0);
    for (int x = 0; x < n; x++) par[x] = x;
    deque<string> dq;

    for (int idx = 0; idx < (int)words.size(); idx++) {
        string curr = words[idx];
        dq.push_back(curr); // окно размером k, я реализоал через дек
        while ((int)dq.size() > k) {
            dq.pop_front();
        }

        vector<string> a; // тк нужны только непустые слова то я отфильтрую пустые
        for (int j = 0; j < (int)dq.size(); j++) {
            if (!dq[j].empty()) {
                a.push_back(dq[j]);
            }
        }

        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = i+1; j < (int)a.size(); j++) {
                string word1 = a[i];
                string word2 = a[j];
                if (proof(word1, word2)) unite(ids[word1], ids[word2]);
            }
        }
    }


    unordered_map<string, int> freq; // часоты
    for (string w : words) {
        if (!w.empty()) freq[w]++;
    }

    unordered_map<int, vector<string>> groups; // выделяем группы
    for (auto [w, id] : ids) {
        int rot = root(id);
        groups[rot].push_back(w);
    }

    vector<pair<string, int>> res;
    for (auto& [root, words] : groups) {
        int t = 0;
        for (const string& w : words) {
            t += freq[w];
        }
        string best; // ищем лексикографически наименьшее слово в группе
        for (const string& w : words) {
            if (w.length() > 1) {
                if (w < best || best.empty()) best = w;
            }
        }
        if (best.empty() && !words.empty()) { 
            best = words[0];
            for (string w : words) {
                if (w < best) best = w;
            }
        }

        if (t > 0) {
            res.push_back({best, t});
        }
    }

    sort(res.begin(), res.end(), comp); // сортируем группы со своим компаратором

    for (auto [w, fr] : res) { // выводим результат
        cout << w << ": " << fr << "\n";
    }
    return 0;
}

