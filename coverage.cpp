#pragma GCC optimize("Ofast,no-stack-protector,inline")
#pragma GCC target("bmi,bmi2,lzcnt,popcnt,movbe,aes,pclmul,rdrnd")
#pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2")
#define watch(...)
#undef _GLIBCXX_DEBUG
#define NDEBUG
#include <bits/stdc++.h>
namespace {
template<class T>
T maxn(T a, T b) {
    return max(a, b);
}
template<class T, class... U>
T maxn(T x , U... z) {
    return max(x, maxn(z...));
}
template<class T>
T minn(T a, T b) {
    return min(a, b);
}
template<class T, class... U>
T minn(T x , U... z) {
    return min(x, minn(z...));
}
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
template<typename T>
ssize_t ssize(const T& x) { return static_cast<ssize_t>(x.size()); }

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

void compute_coverage() {
    std::vector<unsigned> endpoints(51000000);
    for (std::string s; std::getline(std::cin, s); ) {
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
    for (size_t i = 0; i < endpoints.size(); ++i) {
        current += endpoints[i];
        if (current >= 200)
            std::cout << i << ' ' << current << '\n';
    }
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    compute_coverage();
    return 0;
}
