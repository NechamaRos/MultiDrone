
#include <algorithm>
#include <vector>
#include <map>

class HashTable
{
protected:
    
	int table_number=20;
	int key_size=10;
	int multi_probe_level=2;
	//what hash table?
	typedef int DataType;
	// What is stored in the hash 
	typedef uint8_t Featuretype;
	// key type from which we can get a bucket back in an hash table
	typedef unsigned int BucketKey;
	//a bucken in a hash table
    typedef std::map<int, std::vector<int>>Bucket;
	//mask for
	std::vector<size_t> mask_;
public:
    std::vector<size_t> xor_masks;
	size_t getKey(std::vector<int> descrip);
    std::vector<Bucket>hash_table;
	const std::vector<int> getBucketFromKey(BucketKey key,int index);
    inline std::vector<size_t> generateRandomMask(size_t feature_size, unsigned int key_size);
    void fill_xor_mask(int key, int lowest_index, unsigned int level, std::vector<size_t>& xor_masks);
 
};
class ResultSet {
private:
    int capacity;
    float worst_dist;
    std::vector<std::pair<float, int>> results;

public:
    ResultSet(int capacity) : capacity(capacity) {}
    void addPoint(float distance, int index);
    std::vector<int> getIndices() {
        std::vector<int> indices;
        for (const auto& result : results) {
            indices.push_back(result.second);
        }
        return indices;
    }

    std::vector<float> getDistances() {
        std::vector<float> distances;
        for (const auto& result : results) {
            distances.push_back(result.first);
        }
        return distances;
    }
};
