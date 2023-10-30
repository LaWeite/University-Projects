#include "application.h"
#include "compare.h"
#include <cstring>

struct database
{
    int count;
    int maxsize;
    application* data;
};

void print(const database& DB);

void add(database& DB, const char* ate, const char* time, const char* ticker, const char* type, const char* amount, const char* price,  bool sort = true);

void sortByDate(database& DB, int start = 0, int end = 0);

void sortBuySell(database& DB, int start, int end);

void sortPrice(database& DB);

int findRecord(database& DB, int id);

void deleteRecord(database& DB, int id);

void clear(database& DB);

void dom(database& DB, const char* ticker);
