#include <iostream>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class ranking{
    vector<pair<int, int>> top7;
    unordered_map<int, int> liczba_glosow;

    void dodaj(unordered_set<int> glosy){
        for (const auto& nr_utworu: glosy) {
            auto it = liczba_glosow.find(nr_utworu);
            if (it == liczba_glosow.end())
                liczba_glosow.insert({nr_utworu, 1});
            else
                it->second++;
        }

        //update top7
    }
};

int main() {
    string input_line;
    getline(cin, input_line);
    regex NEW("\\s*NEW\\s+[1-9]([0-9]){0,7}\\s*"); // polecenie
    regex TOP("\\s*TOP\\s*"); // polecenie
    regex EMPTY("\\s*"); // ignored line
    regex VOTE("\\s*[1-9][0-9]{0,7}\\s*(\\s+[1-9][0-9]{0,7}\\s*)*"); // vote -> unordered set

    /*
     * Ranking
     * 2 unordered map
     */

    if(regex_match(input_line, VOTE))
        cout << "si";
    return 0;
}
