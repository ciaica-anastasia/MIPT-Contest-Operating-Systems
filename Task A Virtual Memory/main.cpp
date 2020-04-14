#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

unsigned int FIFO(vector<pair<unsigned long long, unsigned long long>>& queries, unsigned long long size)
{
    deque<unsigned long long> cache;
    unsigned int miss_count = 0;

    for (pair<unsigned long long, unsigned long long> query : queries)
    {
        cout << query.first;
        if (find(cache.begin(), cache.end(), query.first) == cache.end())
        {
            if (cache.size() == size)
            {
                cache.pop_front();
            }
            cache.push_back(query.first);

            cout << "-";

            ++miss_count;
        }
        else
        {
            cout << "+";
        }
        cout << " ";
    }
    cout << endl;

    return miss_count;
}
unsigned int LRU(vector<pair<unsigned long long, unsigned long long>>& queries, unsigned long long size)
{
    list<unsigned long long> cache;
    unsigned int miss_count = 0;

    for (pair<unsigned long long, unsigned long long> query : queries)
    {
        cout << query.first;

        list<unsigned long long>::iterator it = find(cache.begin(), cache.end(), query.first);

        if (it == cache.end())
        {
            if (cache.size() == size)
            {
                cache.pop_front();
            }
            cache.push_back(query.first);

            cout << "-";

            ++miss_count;
        }
        else
        {
            cache.push_back(*it);
            cache.erase(it);

            cout << "+";
        }
        cout << " ";
    }
    cout << endl;

    return miss_count;
}
unsigned int LFU(vector<pair<unsigned long long, unsigned long long>>& queries, unsigned long long size)
{
    multimap<unsigned long long, unsigned long long> freq;
    unordered_map<unsigned long long, multimap<unsigned long long, unsigned long long>::iterator> cache;

    unsigned int miss_count = 0;

    for (pair<unsigned long long, unsigned long long> query : queries)
    {
        cout << query.first;
        unsigned long long cache_size = cache.size();

        auto found = cache.find(query.first);
        multimap<unsigned long long, unsigned long long>::iterator prev = cache[query.first];
        
        if (cache_size < size)
        {
            cout << "- ";
            ++miss_count;
            cache[query.first] = freq.emplace_hint(freq.cbegin(), 1, query.first);
        }
        else if (found == cache.end()) // if element not found
        {
            cout << "- ";

            ++miss_count;

            if (cache.size() == size + 1) // cache full
            {
                auto up = freq.upper_bound(freq.begin()->first);
                if (up != freq.begin())
                    --up;

                //auto up = freq.begin();

                cache.erase(up->second);
                freq.erase(up);
            }
            cache[query.first] = freq.emplace_hint(freq.cbegin(), 1, query.first);
        }
        else // else if element found
        {
            auto updated_elem = make_pair(prev->first + 1, prev->second);
            
            // update the previous value
            freq.erase(prev);
            cache[query.first] = freq.emplace_hint(freq.cend(), move(updated_elem));

            cout << "+ ";
        }
    }
    cout << endl;

    return miss_count;
}

bool pred(const pair<unsigned long long, unsigned long long>& left,
    const pair<unsigned long long, unsigned long long>& right)
{
    return left.second < right.second;
}

unsigned int OPT(vector<pair<unsigned long long, unsigned long long>>& queries,    // (page, distance_to_next_page)
                unsigned long long size)
{
    unsigned int miss_count = 0;

    unordered_map<unsigned long long, unsigned long long> cache;
    unordered_map<unsigned long long, unsigned long long>::iterator it;

    for (pair<unsigned long long, unsigned long long> query : queries)
    {
        // find element in cache
        it = cache.find(query.first);

        cout << query.first;

        if (it == cache.end()) // if element not found
        {
            cout << "- ";
            ++miss_count;

            if (cache.size() == size) // if cache full... remove far element
            {
                cache.erase(max_element(cache.begin(), cache.end(), pred));
            }

            cache.insert(query); // push to cache
        }
        else // element found
        {
            it->second = query.second;
            cout << "+ ";
        }
    }
    cout << endl;

    return miss_count;
}


void task_A()
{
    vector<pair<unsigned long long, unsigned long long>> queries; // (page, distance_to_next_page)
    map<unsigned long long, unsigned long long> mp; // (page, index)
    pair<map<unsigned long long, unsigned long long>::iterator, bool> ret;

    unsigned long long size = 0;
    unsigned long long test_cases = 0;

    unsigned int FIFO_miss = 0;
    unsigned int LRU_miss = 0;
    unsigned int LFU_miss = 0;
    unsigned int OPT_miss = 0;

    cin >> size >> test_cases;

    for (unsigned long long i(0); i < test_cases; ++i)
    {
        unsigned long long query = 0;
        cin >> query;

        ret = mp.insert(pair<unsigned long long, unsigned long long>(query, i));

        if (!ret.second) // if KEY exist
        {
            //++ret.first->second.first;
            queries[ret.first->second].second = i;
            ret.first->second = i;
        }
        queries.push_back(pair<unsigned long long, unsigned long long>(query, (unsigned long long) - 1));
    }
    FIFO_miss = FIFO(queries, size);
    LRU_miss = LRU(queries, size);
    LFU_miss = LFU(queries, size);
    OPT_miss = OPT(queries, size);

    cout << FIFO_miss << " " << LRU_miss << " " << LFU_miss << " " << OPT_miss;
}

int main(int argc, char** argv)
{
    task_A();

    return 0;
}
