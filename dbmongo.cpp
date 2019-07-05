#include "dbmongo.h"
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/types.hpp>

#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <iostream>

mongocxx::instance instance{};

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

DBMongo::DBMongo()
{

}

DBMongo::~DBMongo()
{

}

void DBMongo::setConnect(const std::string &url)
{
    url_  = url;
}

bool DBMongo::open()
{
    mongocxx::uri uri(url_);
    mongocxx::client cli(uri);
    cli_= std::move(cli);
    return true;
}

bool DBMongo::insert(const std::string &db_name, const std::string &coll_name, const std::string &content)
{
    mongocxx::database db = cli_[db_name];
    mongocxx::collection coll = db[coll_name];
    coll.insert_one(bsoncxx::from_json(content).view());
    return true;
}

bool DBMongo::insertMutli(const std::string &db_name, const std::string &coll_name, const std::vector<std::string> &vect_content)
{
    mongocxx::database db = cli_[db_name];
    mongocxx::collection coll = db[coll_name];

    std::vector<bsoncxx::document::value> vect_doc;
    for(int i = 0; i < vect_content.size(); i++)
    {
        vect_doc.push_back(bsoncxx::from_json(vect_content[i]));
    }
    auto begin = std::chrono::system_clock::now();
    coll.insert_many(vect_doc);
    auto end = std::chrono::system_clock::now();
    std::cout <<  "insert time :" <<double(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()) << std::endl;
    return true;
}

bool DBMongo::update(const std::string &db_name, const std::string &coll_name,const std::string& condition ,const std::string &content)
{
    mongocxx::database db = cli_[db_name];
    mongocxx::collection coll = db[coll_name];
    bsoncxx::document::value doc_content = document{}  << "$set" << bsoncxx::from_json(content) << finalize;

    bsoncxx::stdx::optional<mongocxx::result::update> result =
            coll.update_one(bsoncxx::from_json(condition), bsoncxx::from_json(content));
    if(result)
    {
        return true;
    }

    return false;
}

bool DBMongo::updateMutli(const std::string &db_name, const std::string &coll_name, const std::vector<std::string>& vect_condition, const std::vector<std::string> &vect_content)
{
    /*    bsoncxx::stdx::optional<mongocxx::result::update> result =
         coll.update_many(
                 document{} << "i" << open_document <<
                 "$lt" << 100 << close_document << finalize,
                 document{} << "$inc" << open_document <<
                 "i" << 100 << close_document << finalize);

     if(result) {
         std::cout << result->modified_count() << "\n";
     }
*/

    return true;
}

bool DBMongo::delData(const std::string &db_name, const std::string &coll_name, const std::string &content)
{
    mongocxx::database db = cli_[db_name];
    mongocxx::collection coll = db[coll_name];
    coll.delete_one(bsoncxx::from_json(content));
    return true;
}

bool DBMongo::delDataMutli(const std::string &db_name, const std::string &coll_name, const std::vector<std::string> &vect_content)
{
    /*    bsoncxx::stdx::optional<mongocxx::result::delete_result> result_del = coll.delete_many(
                    document{} << "i" << open_document <<
                    "$gte" << 100 << close_document << finalize);

        if(result_del) {
            std::cout << result_del->deleted_count() << "\n";
        }
*/
    return true;
}

bool DBMongo::getData(const std::string &db_name, const std::string &coll_name, const std::string& condition, std::vector<std::string> &vect_data)
{
    mongocxx::database db = cli_[db_name];
    mongocxx::collection coll = db[coll_name];
    mongocxx::cursor cursor = coll.find(bsoncxx::from_json(condition));

    for(auto doc : cursor)
    {
        vect_data.push_back(bsoncxx::to_json(doc));
    }
    return true;
}
