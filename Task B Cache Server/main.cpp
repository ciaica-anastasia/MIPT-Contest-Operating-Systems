#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>

using namespace std;


bool pred(const pair<unsigned long long, unsigned long long>& left,
	const pair<unsigned long long, unsigned long long>& right)
{
	return left.second < right.second;
}

unsigned int OPT(map<unsigned long long, unsigned long long>& mp,	// (page, index)
	vector<pair<unsigned long long, unsigned long long>>& queries,	// (page, distance_to_next_page)
	unsigned long long size)
{
	unsigned int miss_count = 0;

	unordered_map<unsigned long long, unsigned long long> cache;
	unordered_map<unsigned long long, unsigned long long>::iterator it;

	for (pair<unsigned long long, unsigned long long> query : queries)
	{
		// find element in cache
		it = cache.find(query.first);

		//cout << query.first;

		if (it == cache.end()) // if element not found
		{
			//cout << "- ";
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
			//cout << "+ ";
		}
	}

	return miss_count;
}


void task_B()
{
	vector<pair<unsigned long long, unsigned long long>> queries; // (page, distance_to_next_page)
	map<unsigned long long, unsigned long long> mp; // (page, index)
	pair<map<unsigned long long, unsigned long long>::iterator, bool> ret;

	unsigned long long size = 0;
	unsigned long long test_cases = 0;

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

	cout << OPT(mp, queries, size);
}

int main(int argc, char** argv)
{
	task_B();

	return 0;
}
