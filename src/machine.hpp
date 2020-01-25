#ifndef MACHINE_H
#define MACHINE_H

#include <vector>
template <typename DType> class machine {
public:
    void increment_data() { memory.at(current)++; }

    void decrement_data() { memory.at(current)--; }

    void next_data() { 
        if(current_is_end()){
            memory.emplace_back(0);
        }    
        current++; 
    }

    void previous_data() { current--; }

    DType output_current() const { return memory.at(current); }

    void input_current(DType d) { memory.at(current) = d; }

    private:
    std::vector<DType> memory{DType{}};
    size_t current{};

    bool current_is_end() const {
        return current == memory.size()-1;
    }
};
#endif
