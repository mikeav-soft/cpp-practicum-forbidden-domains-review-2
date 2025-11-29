#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <fstream>
#include <algorithm> 

using namespace std;

class Domain {
public:
    Domain(const string& name)
        : name_{ name } {
        reverse(name_.begin(), name_.end());
    }

    bool operator==(const Domain& other) const {
        return name_ == other.name_;
    }

    bool operator<(const Domain& other) const {
        return name_ < other.name_;
    }

    bool IsSubdomain(const Domain& domain) const {
        return name_.starts_with(domain.name_ + ".");
    }

private:
    string name_;
};

class DomainChecker {
public:
    template<typename It>
    DomainChecker(It from, It to)
        : sorted_domains_{ from,to } {
        std::sort(
            sorted_domains_.begin(),
            sorted_domains_.end(),
            [](const Domain& a, const Domain& b) {
                return a < b;
            });
        auto it = std::unique(
            sorted_domains_.begin(),
            sorted_domains_.end(),
            [](const Domain& a, const Domain& b) {
                return b == a || b.IsSubdomain(a);
            });
        sorted_domains_.erase(it, sorted_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
         auto it = std::upper_bound(
             sorted_domains_.begin(),
             sorted_domains_.end(),
             domain);

         if (it == sorted_domains_.begin()) {
             return false;
         }
         --it;
         return (*it) == domain || domain.IsSubdomain(*it);
    }

private:
    vector<Domain> sorted_domains_;
};

std::vector<Domain> ReadDomains(istream& input, size_t lines_count) {
    vector<Domain> domains;
    for (size_t ix = 0; ix < lines_count; ++ix) {
        string line;
        getline(input, line);
        std::istringstream(line) >> line;
        domains.push_back({ line });
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    istream& is = cin;

    const std::vector<Domain> forbidden_domains = ReadDomains(is, ReadNumberOnLine<size_t>(is));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(is, ReadNumberOnLine<size_t>(is));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}