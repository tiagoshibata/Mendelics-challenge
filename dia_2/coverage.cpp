#include <bits/stdc++.h>
#include <limits>
namespace {
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)

template <class ...Args> auto &read_var(Args& ...args) { return (std::cin >> ... >> args); }
template <class T> auto &operator>>(std::istream &is, std::vector<T> &v) {
    for (auto &x : v)
        is >> x;
    return is;
}
template <typename T1, typename T2> auto &operator<<(std::ostream &os, const std::pair<T1, T2> &p);
template <class T> auto &operator<<(std::ostream &os, const std::vector<T> &v) {
    for (const auto& x : v)
        os << x << '\n';
    return os;
}
template <typename T1, typename T2> auto &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
    os << p.first << ' ' << p.second;
    return os;
}

#ifdef TIAGOSHIBATA_DEBUG
template<typename T> void _watch(string name, T arg) {
    cerr << name << " is " << arg << '\n';
}
template<typename T1, typename... T2> void _watch(string names, T1 arg, T2... args) {
    cerr << names.substr(0, names.find(',')) << " is " << arg << " | ";
    _watch(names.substr(names.find(',') + 2), args...);
}
#define watch(...) _watch(#__VA_ARGS__, __VA_ARGS__)
#endif

std::vector<int> compute_coverage(const char *filename) {
    std::ifstream stream(filename);
    std::vector<int> endpoints(51000000);
    for (std::string s; std::getline(stream, s); ) {
        if (s[0] == '@')
            continue;
        std::istringstream ss(s);

        // https://en.wikipedia.org/wiki/SAM_(file_format)#Format
        std::string qname;
        int flag;
        std::string rname;
        int pos;
        int mapq;
        std::string cigar;  // TODO currently assuming 101 length
        ss >> qname >> flag >> rname >> pos >> mapq >> cigar;

        if (pos) {
            ++endpoints[pos - 1];
            --endpoints[pos + 100];
        }
    }

    unsigned current{};
    for (auto& delta : endpoints) {
        current += delta;
        delta = current;
    }

    return endpoints;
}

struct bed_entry {
    std::string id;
    int start, end;
};

std::vector<bed_entry> read_bed(const char *filename) {
    std::ifstream stream(filename);
    std::vector<bed_entry> entries;
    for (std::string s; std::getline(stream, s); ) {
        std::string chromossome;
        int start, end;
        std::string id;

        std::istringstream ss(s);
        ss >> chromossome >> start >> end >> id;
        entries.push_back({id, start, end});
    }
    return entries;
}
}

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);

    if (argc != 3) {
        std::cerr << "Usage: " << *argv << " SAM_file BED_file\n";
        return 1;
    }

    auto coverage = compute_coverage(argv[1]);
    auto regions = read_bed(argv[2]);

    std::vector<std::pair<int, std::string>> region_min_coverage;
    region_min_coverage.reserve(regions.size());
    for (const auto& region : regions) {
        int max_coverage = 0, min_coverage = std::numeric_limits<int>::max();
        for (int i = region.start; i < region.end; ++i) {
            max_coverage = std::max(max_coverage, coverage[i]);
            min_coverage = std::min(min_coverage, coverage[i]);
        }
        std::cout << "Region " << region.id << ": min coverage = " << min_coverage << ", max coverage = " << max_coverage << '\n';
        region_min_coverage.emplace_back(min_coverage, region.id);
    }

    sort(all(region_min_coverage));
    std::cout << "Minimum coverage in each region:\n";
    for (const auto& [coverage, id] : region_min_coverage) {
        std::cout << coverage << ' ' << id << '\n';
    }

    return 0;
}
