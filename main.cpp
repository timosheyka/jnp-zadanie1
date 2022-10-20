#include <iostream>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class ranking {
    //<liczba glosow, nr utworu>
    vector<pair<int, int>> top7;
    //<nr_utworu, liczba glosow>
    unordered_map<int, int> liczba_glosow_map;

    bool czy_jest_utwor(int nr_utworu) {
        auto it = liczba_glosow_map.find(nr_utworu);
        return it != liczba_glosow_map.end();
    }

    void dodaj_utwor(int nr_utworu) {
        liczba_glosow_map.insert({nr_utworu, 0});
    }

    //utwor musi istniec
    //Zwraza aktualna liczbe glosow
    int zwieksz_licznik_glosow(int nr_utworu) {
        auto it = liczba_glosow_map.find(nr_utworu);
        it->second++;
        return it->second;
    }

    //zwraca aktualna liczbe glosow na utwor
    int dodaj_glos(int nr_utworu) {
        if (!czy_jest_utwor(nr_utworu)) {
            dodaj_utwor(nr_utworu);
        }

        return zwieksz_licznik_glosow(nr_utworu);
    }

    int min_top7_votes() {
        if (top7.size() < 7)
            return 0;

        return top7[top7.size() - 1].first;
    }


    //zwraca -1 jezeli utworu nie ma w top7
    int pozycja_w_rankingu(int nr_utworu) {
        for (int i = 0; i < top7.size(); i++)
            if (top7[i].second == nr_utworu)
                return i;

        return -1;
    }

    void usun_z_rankingu_jak_istnieje(int nr_utworu) {
        int pozycja = pozycja_w_rankingu(nr_utworu);
        if (pozycja != -1)
            top7.erase(top7.begin() + pozycja);
    }

    void dodaj_glos_i_zaktualizuj_ranking(int nr_utworu) {
        int glosy = dodaj_glos(nr_utworu);
        if (glosy > min_top7_votes()) {
            usun_z_rankingu_jak_istnieje(nr_utworu);
            top7.emplace_back(glosy, nr_utworu);
            std::sort(top7.begin(), top7.end(), [](auto &left, auto &right) {
                return left.first > right.first || (left.first == right.first && left.second < right.second);
            });


        }
    }

public:
    void dodaj_zbior_glosow(const unordered_set<int> &glosy) {
        for (const auto &nr_utworu: glosy) {
            dodaj_glos_i_zaktualizuj_ranking(nr_utworu);
        }
    }

    pair<int, int> podaj_przeboj(int nr_w_rankingu) {
        return top7[nr_w_rankingu - 1];
    }
};

string findNum(string& line, regex& expression) {
    smatch match;
    regex_search(line, match, expression);
    string num = match.str(0);
    line = match.suffix().str();
    return num;
}

void errorLine(int32_t lineCounter, string& input_line) {
    cerr << "Error in line " << lineCounter << ": " << input_line << '\n';
}

void NEW(int32_t lineCounter, string& input_line, regex& expression, int32_t& Max) {
    string lineCopy = input_line;
    int32_t newMax = (int32_t) stol(findNum(input_line, expression));
    if (Max > newMax) { errorLine(lineCounter, lineCopy); return; }
    if (Max > 0) {
        /*
        * instructions :
        *  closeCurrRecord
        *  coutCurrRecord
        *  startNewRecord
        *  setPoints
        */
    }
}

void TOP() {
    /*
     * instructions :
     *  coutRecordSummary
     */
}

void VOTE(int32_t lineCounter, string& input_line) {
    /*
     * instructions :
     *  isValid (if differentVotesInLine == set.size())
     *  convertToSet
     */
}

int main() {

    string input_line;
    regex NEW_regex("\\s*NEW_regex\\s+0*[1-9]([0-9]){0,7}\\s*"); // instruction
    regex TOP_regex("\\s*TOP_regex\\s*"); // instruction
    regex EMPTY_regex("\\s*"); // ignored line
    regex VOTE_regex("\\s*0*[1-9][0-9]{0,7}\\s*(\\s+0*[1-9][0-9]{0,7}\\s*)*"); // vote
    regex NUM_regex("[1-9][0-9]{0,7}"); // number

    /*
    ranking r = ranking();
    r.dodaj_zbior_glosow({2, 1, 3, 4, 5, 6, 7});
    r.dodaj_zbior_glosow({4});
    cout << r.podaj_przeboj(1).second;
    cout << r.podaj_przeboj(7).second;
    */
    int32_t lineCounter = 0;
    int32_t Max = 0;
    while(getline(cin, input_line)) {
        lineCounter++;
        if (regex_match(input_line, EMPTY_regex)) { continue; }
        if (regex_match(input_line, NEW_regex)) { NEW(lineCounter, input_line, NUM_regex, Max); continue; }
        if (regex_match(input_line, TOP_regex)) { TOP(); continue; }
        if (regex_match(input_line, VOTE_regex)) { VOTE(lineCounter, input_line); continue; }
        errorLine(lineCounter, input_line);
    }
    return 0;
}
