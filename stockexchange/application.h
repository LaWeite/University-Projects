#include <iostream>
#include <fstream>
#include <iomanip>

struct application {
    int id_;
    char time_[9];
    bool buy;
    int amount_;
    char ticker_[16];
    double price_;
    char date_[11];
};
