#ifndef MACHINE_H
#define MACHINE_H
#include <sstream>
#include <vector>
template <typename DType> class machine {
public:
    void increment_data() { memory.at(current)++; }

    void decrement_data() { memory.at(current)--; }

    void next_data() {
        if (current_is_end()) {
            memory.emplace_back(0);
        }
        current++;
    }

    void previous_data() {
        if (current == 0) {
            memory.insert(memory.cbegin(), DType{});
        } else {
            current--;
        }
    }

    bool current_is_zero() const noexcept { return memory.at(current) == 0; }

    DType output_current() const { return memory.at(current); }

    size_t current_memory_location() const { return current; }

    void input_current(DType d) { memory.at(current) = d; }

    bool operator!=(const machine<DType> &rhs) const { return !(*this == rhs); }

    bool operator==(const machine<DType> &rhs) const { return current == rhs.current && memory == rhs.memory; }

    std::string get_memory() const {
        std::stringstream ss;
        for (size_t i = 0; i < memory.size(); ++i) {
            if (i == current) {
                ss << "->\t";
            } else {
                ss << "\t";
            }
            ss << "[" << memory.at(i) << "]\n";
        }
        return ss.str();
    }

private:
    std::vector<DType> memory{DType{}};
    size_t current{};

    bool current_is_end() const noexcept { return current == memory.size() - 1; }
};

#endif
