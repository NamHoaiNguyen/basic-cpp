#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using tab_t  = std::vector<std::vector<std::string>>;
using row_t = std::vector<std::string>;

/*test case
Link: https://rosettacode.org/wiki/Hash_join#C.2B.2B*/
tab_t tab1 {
// Age  Name
  {"27", "Jonah"}
, {"18", "Alan"}
, {"28", "Glory"}
, {"18", "Popeye"}
, {"28", "Alan"}
};
 
tab_t tab2 {
// Character  Nemesis
  {"Jonah", "Whales"}
, {"Jonah", "Spiders"}
, {"Alan", "Ghosts"}
, {"Alan", "Zombies"}
, {"Glory", "Buffy"}
};

std::ostream& operator<< (std::ostream& os, const tab_t &obj) {
    for (size_t i = 0; i < obj.size(); i++) {
        os << i << ":";
        for (size_t j = 0; j < obj[i].size(); j++) {
            os << '\t' << obj[i][j] ;
        }
        os << std::endl;
    }
    return os;
}

decltype(auto) join(const tab_t &tab1, int column_a, const tab_t &tab2, int column_b) {
    std::unordered_multimap<std::string, size_t> hashmap;
    tab_t res;

    for (size_t i = 0; i < tab1.size(); i++) {
        hashmap.insert(std::make_pair(tab1[i][column_a], i));
    }

    for (size_t i = 0; i < tab2.size(); i++) {
        auto range = hashmap.equal_range(tab2[i][column_b]);
        for (auto it = range.first; it != range.second; it++) {
            row_t row;
            row.push_back(tab1[it->second][0]);
            row.push_back(tab1[it->second][column_a]);
            row.push_back(tab2[i][column_b]);
            row.push_back(tab2[i][1]);
            res.push_back(std::move(row));
    //         tab_t::value_type row;
    //   row.insert(row.end() , tab1[it->second].begin() , tab1[it->second].end());
    //   row.insert(row.end() , tab2[i].begin()          , tab2[i].end());
    //   res.push_back(std::move(row));
        }
    }

    return res;
}

int main() {
    std::cout << "Table A: " << std::endl << tab1 << std::endl;
    std::cout << "Table B: " << std::endl << tab2 << std::endl;

    auto tab3 = join(tab1, 1, tab2, 0);

    std::cout << "Table res: " << std::endl << tab3 << std::endl;

    return 0;
}