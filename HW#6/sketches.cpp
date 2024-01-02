#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <algorithm>
#include <numeric>
#include <cstdint>

using namespace std;

constexpr uint64_t c1 = 0xcc9e2d51ull;
constexpr uint64_t c2 = 0x1b873593ull;
constexpr uint64_t r1 = 15;
constexpr uint64_t r2 = 13;
constexpr uint64_t m = 5;
constexpr uint64_t n = 0xe6546b64ull;

constexpr uint64_t a1 = 97102761753631939 , b1 = 42506983374872291;
constexpr uint64_t a2 = 56842397421741207 , b2 = 18456721873196387;
constexpr uint64_t a3 = 61811894731001917 , b3 = 37217717671130671;
constexpr uint64_t a4 = 31415926535897931 , b4 = 27182818284590453;
constexpr uint64_t a5 = 98765432109876543 , b5 = 57548533468232177;


uint64_t murmur_hash(const std::string& key, uint64_t seed = 0) {
  uint64_t hash = seed;
  const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
  size_t len = key.length();
  for (size_t i = 0; i < len / 8; ++i) {
    uint64_t k = *reinterpret_cast<const uint64_t*>(data + i * 8);
    k *= c1;
    k = (k << r1) | (k >> (64 - r1));
    k *= c2;
    
    hash ^= k;
    hash = ((hash << r2) | (hash >> (64 - r2))) * m + n;
  }
  const uint8_t* tail = data + (len / 8) * 8;
  uint64_t k1 = 0;
  switch (len & 7) {
  case 7:
    k1 ^= static_cast<uint64_t>(tail[6]) << 48;
  case 6:
    k1 ^= static_cast<uint64_t>(tail[5]) << 40;
  case 5:
    k1 ^= static_cast<uint64_t>(tail[4]) << 32;
  case 4:
    k1 ^= static_cast<uint64_t>(tail[3]) << 24;
  case 3:
    k1 ^= static_cast<uint64_t>(tail[2]) << 16;
  case 2:
    k1 ^= static_cast<uint64_t>(tail[1]) << 8;
  case 1:
    k1 ^= static_cast<uint64_t>(tail[0]);
    k1 *= c1;
    k1 = (k1 << r1) | (k1 >> (64 - r1));
    k1 *= c2;
    hash ^= k1;
  }
  hash ^= len;
  hash ^= (hash >> 37);
  hash *= 0x85ebca6bull;
  hash ^= (hash >> 32);
  return hash;
}

uint64_t ax_b_string_func(uint64_t a, uint64_t b, string to_hash){
  
  uint64_t hash = b;
  for(char c : to_hash){
    hash = hash * a + static_cast<uint64_t>(c);
  }
  
  return (uint64_t) hash;
}


class Sketch{
public:
  virtual void add(const std::string& s) = 0;
};

class MembershipSketch : public Sketch{
public:
  virtual bool isMember(string s) = 0;
};

/*A Bloom Filter is a probabilistic data structure used for testing whether an element is a member of a set or
not. It is particularly efficient in terms of space when compared to other data structures like hash tables or sets,
but it comes with a trade-off in terms of accuracy. A Bloom Filter may sometimes produce false positives,
meaning it might mistakenly claim that an element is in the set when it’s not, but it never produces false
negatives (if it says an element is not in the set, it is definitely not in the set). For the homework, your Bloom
Filter class should include a bit array that is made up of uint64 t variables and m independent hash functions.
Usage of Bloom Filter is as follows;
Step 1: Initialization
• Allocate memory for size/64 64-bit unsigned integers where size, i.e., the number of bits the sketch will
use, is given as a parameter during construction. You need to set all these bits to 0.
4
0 0 0 0 0 0 0 0 0
hash_1("apple") = 1
0 1 2 3 4 5 6 7 8
0 1 0 1 0 0 0 1 0 0 1 2 3 4 5 6 7 8
hash_2("apple") = 7
hash_3("apple") = 3
insert("apple")
bit array
bit array
hash_1("ware") = 2
hash_2("ware") = 6
hash_3("ware") = 5
insert("ware")
0 1 1 1 0 1 1 1 0 0 1 2 3 4 5 6 7 8
bit array
hash_1("ware") = 2
hash_2("ware") = 6
hash_3("ware") = 5
isMember("ware")
true positive 
hash_1("astral") = 1
hash_2("astral") = 4
hash_3("astral") = 8
isMember("astral")
hash_1("tuesday") = 5
hash_2("tuesday") = 1
hash_3("tuesday") = 2
isMember("tuesday")
true negative  false positive 
5
• Prepare k = 3 hash functions - use the a, b values above. You can put these pairs in an array to loop over
them (i.e., to loop over the hash functions).
Step 2: Insertion
• To insert an item, acquire k = 3 hash values from independent hash functions (i.e., using different a, b
pairs).
• For each hash value, compute the index of the bit to be modified by taking the modulo of the hash value
w.r.t. size (i.e., the number of bits).
• Set the corresponding bits of the Bloom Filter (to 1) where the bits’ indices are obtained from the above
item.
Step 3: Membership Query
To determine if an item is previously inserted into the bloom filter, simply act similar to Step 2.
• To query an item, acquire k = 3 hash values from independent hash functions (i.e., using different a, b
pairs).
• For each hash value, compute the index of the bit to be modified by taking the modulo of the hash value
w.r.t. size (i.e., the number of bits).
• Check the corresponding bits of Bloom Filter where the bits’ indices are obtained from the above item.
• If all checked bits are 1 then return true. Note that the answer can be wrong.
• If any of the checked bits are 0 then return false. This item is definitely not inserted into the filter
before.
In the HW, you will use 3 hash functions (check the hash function implementation given above and three
possible a, b pairs) to test the accuracy of the Bloom Filter sketch with different sizes on membership queries.
*/
class BloomFilter : public MembershipSketch{
private:
  int k;
  int size;
  int _switch; // select which hash function to use
  uint64_t* filter; // bit array only use bit operations
public:
  BloomFilter(int k, int size, int _switch){
    this->k = k;
    this->size = size;
    this->_switch = _switch;
    filter = new uint64_t[size / 64];
    for(int i = 0; i < size / 64; i++){
      filter[i] = 0;
    }
  }

  ~BloomFilter(){
    delete[] filter;
  }

  void add(const std::string& s){
    uint64_t hash;
    if(_switch == 1)
      hash = ax_b_string_func(a1, b1, s);
    else
      hash = murmur_hash(s);
    for(int i = 0; i < k; i++){
      int index = hash % size;
      filter[index / 64] |= (1ULL << (index % 64));
      hash = hash >> 8;
    }
  }

  bool isMember(string s){
    uint64_t hash;
    if(_switch == 1)
      hash = ax_b_string_func(a2, b2, s);
    else
      hash = murmur_hash(s);
    for(int i = 0; i < k; i++){
      int index = hash % size;
      if((filter[index / 64] & (1ULL << (index % 64))) == 0){
  return false;
      }
      hash = hash >> 8;
    }
    return true;
  }

  int get_size(){
    return size;
  }

  int get_k(){
    return k;
  }
};

class CardinalitySketch : public Sketch{
public:
  virtual double estimateCardinality() = 0;
};


class HyperLogLog : public CardinalitySketch{
  private:
    int b; // number of buckets
    int _switch; // select which hash function to use
    uint64_t* buckets; // bit array
  public:
    HyperLogLog(int b, int _switch){
      this->b = b;
      this->_switch = _switch;
      buckets = new uint64_t[b];
      for(int i = 0; i < b; i++){
  buckets[i] = 0;
      }
    }

    ~HyperLogLog(){
      delete[] buckets;
    }

    void add(const std::string& s){
      uint64_t hash;
      if(_switch == 1)
  hash = ax_b_string_func(a3, b3, s);
      else
  hash = murmur_hash(s);
      int index = hash % b;
      // use clz to find the number of leading zeros
      int leading_zeros = __builtin_clzll(hash);
      buckets[index] = max(buckets[index], (uint64_t) leading_zeros + 1);
    }

    double estimateCardinality(){
      double sum = 0;
      for(int i = 0; i < b; i++){
  sum += 1.0 / pow(2, buckets[i]);
      }
      double estimate = alpha(b) * b * b / sum;
      return estimate;

    }

    double alpha(int b){
      if(b == 16)
  return 0.673;
      else if(b == 32)
  return 0.697;
      else if(b == 64)
  return 0.709;
      else
  return 0.7213 / (1 + 1.079 / b);
    }
};

/*-MinVal Sketch
Let’s go back to the first approach, MinVal: as mentioned before, only storing the first minimum value can yield
extremely off estimates since this value can be small. However, to reduce the error, we can store the smallest k
hash values as the following k-MinVal recipe describes:
Step 1: Initialization
You start by initializing a k-MinVal sketch which contains an array of uint64 t to store the k minimum hash
values found so far. Assume that k = 3 (i.e., we keep 3 minimum values)
[−1, −1, −1]
(-1 denotes that the corresponding entry is initially empty).
3
Step 2: Hashing Data Elements
For each data element in your stream, apply the hash function and calculate its hash value.
Example Data Stream: ["apple", "banana", "cherry", "date", "elderberry"]
Hash Values:
apple− > 112
banana− > 90
cherry− > 150
date− > 75
elderberry− > 130
Step 3: Updating the Minimum Values Array
After computing each hash, update the array to maintain only the ’k’ smallest unique hash values.
Add hash of "apple": [112, −1, −1]
Add hash of "banana": [90, 112, −1]
Add hash of "cherry": [90, 112, 150]
Add hash of "date": [75, 90, 112]
Add hash of "elderberry": [75, 90, 112] (130 is not in the top 3 minimum values).
Step 4: Estimating the Number of Distinct Elements
Once all elements have been processed, use the values in the array to estimate the total number of distinct
elements in the data stream. The formula often used is:
DistinctCount ≈
(k − 1) × Maximum possible hash value
k’th minimum hash value
• k’th minimum hash value = 112 (last value in the array)
• Maximum possible hash value: (depends on hash function implementation - you need to use the appropriate value from climits)
• Distinct Count Estimation: Assuming the maximum possible hash value = 28 − 1 (8-bit unsigned hash
values are assumed for demonstration)
DistinctCount = (3 − 1) ∗ (28 − 1)/112 ≈ 4.55
*/
class KMinVal : public CardinalitySketch{
private:
  int K;
  int _switch; // select which hash function to use
  uint64_t* vals; // array
public:
  KMinVal(int K, int _switch){
    this->K = K;
    this->_switch = _switch;
    vals = new uint64_t[K];
    for(int i = 0; i < K; i++){
      vals[i] = ULLONG_MAX;
    }
  }

  ~KMinVal(){
    delete[] vals;
  }

  void add(const std::string& s){
    uint64_t hash;
    if(_switch == 1)
      hash = ax_b_string_func(a4, b4, s);
    else
      hash = murmur_hash(s);
    for(int i = 0; i < K; i++){
      if(hash < vals[i]){
        for(int j = K - 1; j > i; j--){
          vals[j] = vals[j - 1];
        }
        vals[i] = hash;
        break;
      }
    }
  }

  double estimateCardinality(){
    double estimate = (K - 1) * (pow(2, 64) - 1) / vals[K - 1];
    return estimate;
  }

};


void evaluate_bloom(vector<string> words, BloomFilter *bloom, int i){

  int true_positives = 0;
  int false_positives = 0;
  
  for(int i = 0; i < 100; i++){
    true_positives += (int)bloom->isMember(words[i]);
  }

  for(int i = 0; i < 100; i++){
    false_positives += (int)bloom->isMember(words[words.size() - i - 1]);
  }  
  
  cout << "size: " << bloom->get_size() << " k: " << bloom->get_k() << " Inserted: " << i << " TP: " << true_positives << " FP: " << false_positives << endl;
}

int main(int argc, char** argv){

  string fname;
  fname = argv[1];
  
  ifstream file(fname);
  if(!file.is_open()){
    cout << "Error opening file: " << fname << endl;
    return 1;
  }

  vector<string> words;
  string word;

  //Reading each line into vector of strings
  while(std::getline(file, word)){
    words.push_back(word);
  }

  cout << "#######################################################################################" << endl;
  cout << "HyperLogLog Using ax+b hash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    HyperLogLog loglog(an, 1);
    for(int i = 0; i < words.size(); i++){
      loglog.add(words[i]);
    }
    
    cout << "buckets: " << an << " HyperLogLog estimate: " << loglog.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "HyperLogLog Using MurMurHash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    HyperLogLog loglog(an, 2);
    for(int i = 0; i < words.size(); i++){
      loglog.add(words[i]);
    }
    
    cout << "buckets: " << an << " HyperLogLog estimate: " << loglog.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "KMinVal Using ax + b hash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    KMinVal kminval(an, 1);
    for(int i = 0; i < words.size(); i++){
      kminval.add(words[i]);
    }
    cout << "buckets: " << an << " KMinVal estimate: " << kminval.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "KMinVal Using MurMurHash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    KMinVal kminval(an, 2);
    for(int i = 0; i < words.size(); i++){
      kminval.add(words[i]);
    }
    cout << "buckets: " << an << " KMinVal estimate: " << kminval.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "BloomFilter using ax + b hash" << endl;
  for(int i = 64; i <= 131072; i *= 2){
    BloomFilter *bloom = new BloomFilter(5, i, 1); //Note that we are using 5 hashes with ax + b
    for(int i = 0; i < words.size() - 100; i++){ //We do not add last 100 words to the bloom filter to be able to check if they return as false positives
      bloom->add(words[i]);
      if(i == 100)
	evaluate_bloom(words, bloom, i);
      if(i == 500)
	evaluate_bloom(words, bloom, i);
      if(i == 1000)
	evaluate_bloom(words, bloom, i);
      if(i == 100000)
	evaluate_bloom(words, bloom, i);
      if(i == 1000000)
	evaluate_bloom(words, bloom, i);
    }
    delete bloom;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "BloomFilter using MurMurHash" << endl;
  for(int i = 64; i <= 131072; i *= 2){
    BloomFilter *bloom = new BloomFilter(5, i, 2); //Note that we are using 5 hashes with ax + b
    for(int i = 0; i < words.size() - 100; i++){ //We do not add last 100 words to the bloom filter to be able to check if they return as false positives
      bloom->add(words[i]);
      if(i == 100)
	evaluate_bloom(words, bloom, i);
      if(i == 500)
	evaluate_bloom(words, bloom, i);
      if(i == 1000)
	evaluate_bloom(words, bloom, i);
      if(i == 100000)
	evaluate_bloom(words, bloom, i);
      if(i == 1000000)
	evaluate_bloom(words, bloom, i);
    }
    delete bloom;
  }
  cout << "#######################################################################################" << endl;

    
}
