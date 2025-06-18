#include "databaseorganizer.h"

int main() {
    databaseorganizer db;
    db.parsedata("data.txt");
    db.parsecommands("commands.txt");
    db.write_log("output.txt");
    return 0;
}