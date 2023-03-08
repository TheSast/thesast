// this is a replacement of another library used in the original code for privacy concerns
#include <cstdlib>

#include <iostream>
#include <iomanip>

#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <list>

#include <chrono>
#include <thread>

namespace rep {  
    int getInt(const std::string promptForInput, const int lw, const int hi) {
        int x;
        if(lw > hi) {return getInt(promptForInput, hi, lw);}
        std::cout << promptForInput;
        std::cin >> x;
        if (std::cin.fail()) {
            std::cout << "⚠ Input must be an integer\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<int>::max(), '\n');
            return getInt(promptForInput, lw, hi);
        }
        std::cout << std::flush;
        return (x >= lw && x <= hi) ? x
                                        : getInt(promptForInput, lw, hi);
    }
}