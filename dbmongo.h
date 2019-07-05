#ifndef DBMONGO_H
#define DBMONGO_H
#include<string>
#include<vector>
#include <mongocxx/client.hpp>

class DBMongo
{
public:
    DBMongo();
    ~DBMongo();

public:
    void setConnect(const std::string& url);

    bool open();

public:
    bool insert(const std::string& db_name, const std::string& coll_name, const std::string& content);

    bool insertMutli(const std::string& db_name, const std::string& coll_name, const std::vector<std::string>& vect_content);

    bool update(const std::string& db_name, const std::string& coll_name, const std::string &condition, const std::string& content);

    bool updateMutli(const std::string& db_name, const std::string& coll_name, const std::vector<std::string> &vect_condition, const std::vector<std::string>& vect_content);

    bool delData(const std::string& db_name, const std::string& coll_name, const std::string& content);

    bool delDataMutli(const std::string& db_name, const std::string& coll_name, const std::vector<std::string>& vect_content);

    bool getData(const std::string& db_name, const std::string& coll_name, const std::string &condition, std::vector<std::string>& vect_data);

private:
    std::string url_;
    mongocxx::client cli_;

};

#endif // DBMONGO_H
