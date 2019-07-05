#include <QCoreApplication>
#include "dbmongo.h"
#include <chrono>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DBMongo db;
    std::string url = "mongodb://192.168.0.68:27018";
    std::string db_name  = "example";
    std::string coll_name = "test";
    db.setConnect(url);
    db.open();
    auto begin = std::chrono::system_clock::now();
    std::vector<std::string> vect_data;
    for (int i = 10000; i < 15000; i ++)
    {
        json json_data;
        json_data[i] = std::to_string(i);
        vect_data.push_back(json_data.dump());
        //db.insert(db_name,coll_name,json_data.dump());
    }
    auto center = std::chrono::system_clock::now();
    std::cout <<  "center time :" <<double(std::chrono::duration_cast<std::chrono::seconds>(center - begin).count()) << std::endl;
    db.insertMutli(db_name,coll_name,vect_data);

    auto end = std::chrono::system_clock::now();
    std::cout <<  "end time :" <<double(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()) << std::endl;

    return a.exec();
}
