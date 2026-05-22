#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;

// ---- Levenshtein Distance ----
int levenshtein(const string &a, const string &b)
{
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            dp[i][j] = (a[i - 1] == b[j - 1]) ? dp[i - 1][j - 1] : 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
    return dp[m][n];
}

// ---- Lowercase helper ----
string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// ---- Check if text contains any keyword (exact or fuzzy) ----
struct Match
{
    string keyword;
    string matchedWord;
    int distance;
};

vector<Match> findMatches(const string &text, const vector<string> &keywords)
{
    vector<Match> matches;
    string lowerText = toLower(text);

    // Split text into words
    vector<string> words;
    string word;
    for (char c : lowerText)
    {
        if (isalpha(c))
            word += c;
        else if (!word.empty())
        {
            words.push_back(word);
            word = "";
        }
    }
    if (!word.empty())
        words.push_back(word);

    for (const string &kw : keywords)
    {
        string lowerKw = toLower(kw);

        // Exact substring match first
        if (lowerText.find(lowerKw) != string::npos)
        {
            matches.push_back({kw, kw, 0});
            continue;
        }

        // Fuzzy match against individual words
        // threshold scales with keyword length
        if (lowerKw.length() <= 6)
            continue;
        int threshold = (lowerKw.length() <= 9) ? 1 : 2;
        for (const string &w : words)
        {
            {
                if (abs((int)w.length() - (int)lowerKw.length()) > threshold)
                    continue;
                int dist = levenshtein(w, lowerKw);
                if (dist > 0 && dist <= threshold)
                {
                    matches.push_back({kw, w, dist});
                    break;
                }
            }
        }
    }
    return matches;
}
// ---- Load keywords from file ----
vector<string> loadKeywords(const string &path)
{
    vector<string> keywords;
    ifstream file(path);
    if (!file.is_open())
    {
        cout << "Error: Could not open keywords file at " << path << endl;
        return keywords;
    }
    string line;
    while (getline(file, line))
    {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#')
            continue;
        keywords.push_back(line);
    }
    cout << "Loaded " << keywords.size() << " keywords." << endl;
    return keywords;
}

// ---- Scan comments file ----
void scanComments(const string &path, const vector<string> &keywords, json &report)
{
    ifstream file(path);
    if (!file.is_open())
    {
        cout << "Error: Could not open " << path << endl;
        return;
    }

    json data = json::parse(file);
    file.close();

    int flagged = 0;
    for (auto &entry : data)
    {
        // Skip entries with no Comment field
        if (!entry["string_map_data"].contains("Comment"))
            continue;

        string comment = entry["string_map_data"]["Comment"]["value"];
        string owner = entry["string_map_data"]["Media Owner"]["value"];
        int timestamp = entry["string_map_data"]["Time"]["timestamp"];

        vector<Match> matches = findMatches(comment, keywords);
        if (!matches.empty())
        {
            flagged++;
            json flaggedEntry;
            flaggedEntry["type"] = "comment";
            flaggedEntry["comment"] = comment;
            flaggedEntry["owner"] = owner;
            flaggedEntry["timestamp"] = timestamp;

            for (auto &m : matches)
            {
                json matchInfo;
                matchInfo["keyword"] = m.keyword;
                matchInfo["matched"] = m.matchedWord;
                matchInfo["distance"] = m.distance;
                flaggedEntry["matches"].push_back(matchInfo);
            }
            report["flagged"].push_back(flaggedEntry);
        }
    }
    cout << "Comments scanned. Flagged: " << flagged << endl;
}

int main()
{
    // ---- Load keywords ----
    vector<string> keywords = loadKeywords("../keywords/keywords.txt");
    if (keywords.empty())
    {
        cout << "No keywords loaded. Exiting." << endl;
        return 1;
    }

    // ---- Init report ----
    json report;
    report["flagged"] = json::array();

    // ---- Scan files ----
    scanComments("../Actual Data (gitignore)/your_instagram_activity/comments/post_comments_1.json", keywords, report);

    // ---- Write report ----
    ofstream output("../flagged_report.json");
    output << report.dump(4);
    output.close();

    cout << "Report written to flagged_report.json" << endl;
    cout << "Total flagged: " << report["flagged"].size() << endl;

    return 0;
}