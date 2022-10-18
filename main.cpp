#include <iostream>
#include <regex>

using namespace std;

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
