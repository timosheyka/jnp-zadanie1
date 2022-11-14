#include <iostream>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class ranking {
    //<nr_utworu, liczba glosow>
    unordered_map<int, int> liczba_glosow_map;

    unordered_map<int, int> liczba_punktow_map;

    // banned numbers
    unordered_set<int32_t> banned_nums;
    unordered_set<int> last_nums;

    // constant top N
    const int top_n = 7;

    bool czy_jest_utwor(int nr_utworu) {
        auto it = liczba_glosow_map.find(nr_utworu);
        return it != liczba_glosow_map.end();
    }

    bool czy_jest_utwor_punkty(int nr_utworu) {
        auto it = liczba_punktow_map.find(nr_utworu);
        return it != liczba_punktow_map.end();
    }

    void dodaj_utwor(int nr_utworu) {
        liczba_glosow_map.insert({nr_utworu, 0});
    }

    void dodaj_utwor_punkty(int nr_utworu) {
        liczba_punktow_map.insert({nr_utworu, 0});
    }

    //utwor musi istniec
    //Zwraza aktualna liczbe glosow
    int zwieksz_licznik_glosow(int nr_utworu) {
        auto it = liczba_glosow_map.find(nr_utworu);
        it->second++;
        return it->second;
    }

    int zwieksz_licznik_punkty(int nr_utworu, int punkty) {
        auto it = liczba_punktow_map.find(nr_utworu);
        it->second += punkty;
        return it->second;
    }

    //zwraca aktualna liczbe glosow na utwor
    int dodaj_glos(int nr_utworu) {
        if (!czy_jest_utwor(nr_utworu)) {
            dodaj_utwor(nr_utworu);
        }

        return zwieksz_licznik_glosow(nr_utworu);
    }

    int dodaj_punkty(int nr_utworu, int punkty) {
        if (!czy_jest_utwor_punkty(nr_utworu)) {
            dodaj_utwor_punkty(nr_utworu);
        }

        return zwieksz_licznik_punkty(nr_utworu, punkty);
    }

    int min_top7_votes() {
        if (top7.size() < top_n)
            return 0;

        return top7[top7.size() - 1].first;
    }

    int min_top7_points() {
        if (top7_points.size() < top_n)
            return 0;

        return top7_points[top7_points.size() - 1].first;
    }

    //zwraca -1 jezeli utworu nie ma w top7
    int pozycja_w_rankingu(int nr_utworu) {
        for (int i = 0; i < top7.size(); i++)
            if (top7[i].second == nr_utworu)
                return i;

        return -1;
    }

    //zwraca -1 jezeli utworu nie ma w top7
    int pozycja_w_rankingu_punkty(int nr_utworu) {
        for (int i = 0; i < top7_points.size(); i++)
            if (top7_points[i].second == nr_utworu)
                return i;

        return -1;
    }


    void usun_z_rankingu_jak_istnieje(int nr_utworu) {
        int pozycja = pozycja_w_rankingu(nr_utworu);
        if (pozycja != -1)
            top7.erase(top7.begin() + pozycja);
    }

    void usun_z_podsumowania_jak_istnieje(int nr_utworu) {
        int pozycja = pozycja_w_rankingu_punkty(nr_utworu);
        if (pozycja != -1)
            top7_points.erase(top7_points.begin() + pozycja);
    }

    void dodaj_glos_i_zaktualizuj_ranking(int nr_utworu) {
        int glosy = dodaj_glos(nr_utworu);
        if (glosy > min_top7_votes()) {
            usun_z_rankingu_jak_istnieje(nr_utworu);
            top7.emplace_back(glosy, nr_utworu);
            std::sort(top7.begin(), top7.end(), [](auto &left, auto &right) {
                return left.first > right.first || (left.first == right.first && left.second < right.second);
            });
            while (top7.size() > 7)
                top7.pop_back();
        }
    }

    void dodaj_punkty_i_zaktualizuj_ranking(int nr_utworu, int punkty) {
        punkty = dodaj_punkty(nr_utworu, punkty);
        if (punkty > min_top7_points()) {
            usun_z_podsumowania_jak_istnieje(nr_utworu);
            top7_points.emplace_back(punkty, nr_utworu);
            std::sort(top7_points.begin(), top7_points.end(), [](auto &left, auto &right) {
                return left.first > right.first || (left.first == right.first && left.second < right.second);
            });
            while (top7_points.size() > 7)
                top7_points.pop_back();
        }
    }

public:
    //<nr utworu, miejsce>
    unordered_map<int, int> ostatnie_miejsce;
    //<nr utworu, miejsce>
    unordered_map<int, int> ostatnie_notowanie;
    unordered_map<int, int> ostatnie_punkty;
    //<liczba glosow, nr utworu>
    vector<pair<int, int>> top7;
    vector<pair<int, int>> top7_points;

    void zamknij_notowanie() {
        for (int i = 0; i < top7.size(); i++) {
            dodaj_punkty_i_zaktualizuj_ranking(top7[i].second, 7 - i);
        }
        ostatnie_notowanie = ostatnie_miejsce;
        liczba_glosow_map = unordered_map<int, int>();
        ban();
        top7 = vector<pair<int, int>>();
        ostatnie_miejsce = unordered_map<int, int>();
    }

    void dodaj_zbior_glosow(const unordered_set<int> &glosy) {
        for (const auto &nr_utworu: glosy) {
            dodaj_glos_i_zaktualizuj_ranking(nr_utworu);
        }
    }


    bool banned(int nr_utworu) {
        if (banned_nums.find(nr_utworu) != banned_nums.end()) // works for c++20
            return true;
        return false;
    }

    void ban() {
        unordered_set<int> actual_nums;
        for (auto &i: top7)
            actual_nums.insert(i.second);

        for (const auto &nr_utworu: last_nums) {
            if (actual_nums.find(nr_utworu) == actual_nums.end())
                banned_nums.insert(nr_utworu);
        }

        last_nums = actual_nums;
    }

    void aktualizuj_pozycje() {
        ostatnie_miejsce = unordered_map<int, int>();
        for (int i = 0; i < top7.size(); i++)
            ostatnie_miejsce.insert({top7[i].second, i});
    }

    void aktualizuj_pozycje_punkty() {
        ostatnie_punkty = unordered_map<int, int>();
        for (int i = 0; i < top7_points.size(); i++)
            ostatnie_punkty.insert({top7_points[i].second, i});
    }
};


ranking r = ranking();


string findNum(string &line, regex &expression) {
    smatch match;
    regex_search(line, match, expression);
    string num = match.str(0);
    line = match.suffix().str();
    return num;
}

void errorLine(int32_t lineCounter, string &input_line) {
    cerr << "Error in line " << lineCounter << ": " << input_line << '\n';
}

void NEW(int32_t lineCounter, string &input_line, regex &expression, int32_t &Max) {

    string lineCopy = input_line;
    int32_t newMax = (int32_t) stol(findNum(input_line, expression));
    if (Max > newMax) {
        errorLine(lineCounter, lineCopy);
        return;
    }

    unordered_map<int, int> last = r.ostatnie_notowanie;
    r.aktualizuj_pozycje();

    for (int i = 0; i < r.top7.size(); i++) {
        cout << r.top7[i].second << " ";
        auto it = last.find(r.top7[i].second);
        if (it != last.end()) {
            cout << it->second - i;
        } else {
            cout << "-";
        }
        cout << "\n";
    }

    r.zamknij_notowanie();

    //r.new_ranking();
    /*
    * instructions :
    *  closeCurrRecord
    *  coutCurrRecord
    *  startNewRecord
    *  setPoints
    */
    Max = newMax;
}

void TOP() {
    unordered_map<int, int> last = r.ostatnie_punkty;
    r.aktualizuj_pozycje_punkty();

    for (int i = 0; i < r.top7_points.size(); i++) {
        cout << r.top7_points[i].second << " ";
        auto it = last.find(r.top7_points[i].second);
        if (it != last.end()) {
            cout << it->second - i;
        } else {
            cout << "-";
        }
        cout << "\n";
    }
    /*
     * instructions :
     *  coutRecordSummary
     */
}

void VOTE(int32_t lineCounter, string &input_line, regex &expression, int32_t max) {
    string lineCopy = input_line;
    string number;
    unordered_set<int32_t> validator;

    while (!(number = findNum(input_line, expression)).empty()) {
        auto num = (int32_t) stol(number);
        if (num > max || validator.find(num) != validator.end() || r.banned(num)) {
            errorLine(lineCounter, lineCopy);
            return;
        }
        validator.insert(num);
    }
    r.dodaj_zbior_glosow(validator);
}

int main() {

    string input_line;
    regex NEW_regex("\\s*NEW\\s+0*[1-9]([0-9]){0,7}\\s*"); // instruction
    regex TOP_regex("\\s*TOP\\s*"); // instruction
    regex EMPTY_regex("\\s*"); // ignored line
    regex VOTE_regex("\\s*0*[1-9][0-9]{0,7}\\s*(\\s+0*[1-9][0-9]{0,7}\\s*)*"); // vote
    regex NUM_regex("[1-9][0-9]{0,7}"); // number

    int32_t lineCounter = 0;
    int32_t Max = 0;

    while (getline(cin, input_line)) {
        lineCounter++;
        if (regex_match(input_line, EMPTY_regex)) { continue; }
        if (regex_match(input_line, NEW_regex)) {
            NEW(lineCounter, input_line, NUM_regex, Max);
            continue;
        }
        if (regex_match(input_line, TOP_regex)) {
            TOP();
            continue;
        }
        if (regex_match(input_line, VOTE_regex)) {
            VOTE(lineCounter, input_line, NUM_regex, Max);
            continue;
        }
        errorLine(lineCounter, input_line);
    }
    return 0;
}
