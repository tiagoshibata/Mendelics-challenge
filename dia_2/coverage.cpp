#include <fstream>
#include <iostream>
#include <limits>
#include <span>
#include <sstream>
#include <vector>

#define all(x) begin(x), end(x)

namespace {
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

void plot(const std::span<int>& data, const char *filename) {
    // Plot to a ppm image
    std::ofstream stream(filename);
    stream << "P6\n" << data.size() << " 1\n255\n";
    for (auto pixel : data) {
        stream << '\0' << static_cast<char>(std::min(std::max(pixel - 255, 0), 255)) << static_cast<char>(std::min(pixel, 255));
    }
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
        std::string filename = region.id + ".ppm";
        plot(std::span<int>(&coverage[region.start], region.end - region.start), filename.c_str());
        region_min_coverage.emplace_back(min_coverage, region.id);
    }

    sort(all(region_min_coverage));
    std::cout << "Minimum coverage in each region:\n";
    for (const auto& [coverage, id] : region_min_coverage) {
        std::cout << coverage << ' ' << id << '\n';
    }

    return 0;
}
