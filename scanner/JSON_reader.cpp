#include <iostream>
#include <fstream>
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;
int main()
{
    ifstream file0("D:/Insta Cleaner V1/test_data/liked_post.json");
    if (!file0.is_open())
    {
        cout << "Error: Could not open File.";
        return 1;
    }
    json data0 = json::parse(file0);
    cout << data0["likes_media_likes"][0]["title"] << endl;
    file0.close();

    ifstream file1("D:/Insta Cleaner V1/test_data/following.json");
    if (!file1.is_open())
    {
        cout << "Error: Could not open File.";
        return 1;
    }
    json data1 = json::parse(file1);
    cout << data1["relationships_following"][0]["title"] << endl;

    file1.close();
    ifstream file2("D:/Insta Cleaner V1/test_data/post_comments.json");
    if (!file2.is_open())
    {
        cout << "Error: Could not open File.";
        return 1;
    }
    json data2 = json::parse(file2);
    cout << data2["comments_media_comments"][0]["title"] << endl;
    file2.close();
}