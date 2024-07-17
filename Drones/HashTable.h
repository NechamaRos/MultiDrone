
#include <algorithm>
template<typename ElementType>
class HashTable
{
private:
	int table_number;
	int key_size;
	int multi_probe_level;
public:
	//HashTable(int table_number, int key_size, int multi_probe_level) :
	//	table_number(table_number), key_size(key_size), multi_probe_level(multi_probe_level) {};
	size_t getKey(const ElementType* discrip) { return 0; };

};
template<typename DistanceType>
class ResultSet 
{
private:

	int capacity;
public:
	ResultSet(int capacity) : capacity(capacity) {};

};
