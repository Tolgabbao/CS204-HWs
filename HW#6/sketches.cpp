// Tolga Toker -- 32639
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
    filter = new uint64_t[size/64];
    for(int i = 0; i < size/64; i++){
      filter[i] = 0;
    }
  }

  ~BloomFilter(){
    delete[] filter;
  }

  void add(const std::string& s){
    uint64_t hash;
    uint64_t hash_1, hash_2, hash_3, hash_4, hash_5;
    int index_1, index_2, index_3, index_4, index_5;
    // use k different hash functions from independent hash functions, ie different a, b pairs stated above like a1 b1, a2 b2, a3 b3
    if(_switch == 1){
      for (int i = 0; i < k; i++){
        switch(i){
        case 0:
          hash_1 = ax_b_string_func(a1, b1, s);
          index_1 = hash_1 % size;
          break;
        case 1:
          hash_2 = ax_b_string_func(a2, b2, s);
          index_2 = hash_2 % size;
          break;
        case 2:
          hash_3 = ax_b_string_func(a3, b3, s);
          index_3 = hash_3 % size;
          break;
        case 3:
          hash_4 = ax_b_string_func(a4, b4, s);
          index_4 = hash_4 % size;
          break;
        case 4:
          hash_5 = ax_b_string_func(a5, b5, s);
          index_5 = hash_5 % size;
          break;
        }
      }
    }
    else {
      for (int i = 0; i < k; i++){
        switch(i){
        case 0:
          hash_1 = murmur_hash(s);
          index_1 = hash_1 % size;
          break;
        case 1:
          hash_2 = murmur_hash(s);
          index_2 = hash_2 % size;
          break;
        case 2:
          hash_3 = murmur_hash(s);
          index_3 = hash_3 % size;
          break;
        case 3:
          hash_4 = murmur_hash(s);
          index_4 = hash_4 % size;
          break;
        case 4:
          hash_5 = murmur_hash(s);
          index_5 = hash_5 % size;
          break;
        }
      }
    }

    // set the corresponding bits of the Bloom Filter (to 1) where the bits’ indices are obtained from the above item

    filter[index_1/64] |= (1ULL << (index_1 % 64));
    filter[index_2/64] |= (1ULL << (index_2 % 64));
    filter[index_3/64] |= (1ULL << (index_3 % 64));
    filter[index_4/64] |= (1ULL << (index_4 % 64));
    filter[index_5/64] |= (1ULL << (index_5 % 64));

  }

  bool isMember(string s){
    uint64_t hash;
    uint64_t hash_1, hash_2, hash_3, hash_4, hash_5;
    int index_1, index_2, index_3, index_4, index_5;
    // use k different hash functions from independent hash functions, ie different a, b pairs stated above like a1 b1, a2 b2, a3 b3
    if(_switch == 1){
      for (int i = 0; i < k; i++){
        switch(i){
        case 0:
          hash_1 = ax_b_string_func(a1, b1, s);
          index_1 = hash_1 % size;
          break;
        case 1:
          hash_2 = ax_b_string_func(a2, b2, s);
          index_2 = hash_2 % size;
          break;
        case 2:
          hash_3 = ax_b_string_func(a3, b3, s);
          index_3 = hash_3 % size;
          break;
        case 3:
          hash_4 = ax_b_string_func(a4, b4, s);
          index_4 = hash_4 % size;
          break;
        case 4:
          hash_5 = ax_b_string_func(a5, b5, s);
          index_5 = hash_5 % size;
          break;
        }
      }
    }
    else {
      for (int i = 0; i < k; i++){
        switch(i){
        case 0:
          hash_1 = murmur_hash(s);
          index_1 = hash_1 % size;
          break;
        case 1:
          hash_2 = murmur_hash(s);
          index_2 = hash_2 % size;
          break;
        case 2:
          hash_3 = murmur_hash(s);
          index_3 = hash_3 % size;
          break;
        case 3:
          hash_4 = murmur_hash(s);
          index_4 = hash_4 % size;
          break;
        case 4:
          hash_5 = murmur_hash(s);
          index_5 = hash_5 % size;
          break;
        }
      }
    }

    // check the corresponding bits of Bloom Filter where the bits’ indices are obtained from the above item
    // if all checked bits are 1 then return true. Note that the answer can be wrong.
    if((filter[index_1/64] & (1ULL << (index_1 % 64))) == 0){
      return false;
    }
    if((filter[index_2/64] & (1ULL << (index_2 % 64))) == 0){
      return false;
    }
    if((filter[index_3/64] & (1ULL << (index_3 % 64))) == 0){
      return false;
    }
    if((filter[index_4/64] & (1ULL << (index_4 % 64))) == 0){
      return false;
    }
    if((filter[index_5/64] & (1ULL << (index_5 % 64))) == 0){
      return false;
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


class KMinVal : public CardinalitySketch{
private:
  int K;
  int _switch; // select which hash function to use
  uint64_t* vals; // array
public:
  // K is the number of minimum values to keep track of

  // step 1: initialize the array to all -1's
  // step 2: for each element, hash it and find the index of the minimum value
  // step 3: if the hashed value is smaller than the minimum value, replace it

  // step 4: estimate the cardinality using distinctCount = (k-1) * maximumpossiblevalue / k'th minimum value
  // maximumpossiblevalue = take it from climits

  KMinVal(int K, int _switch){
    this->K = K;
    this->_switch = _switch;
    vals = new uint64_t[K];
    for(int i = 0; i < K; i++){
      vals[i] = -1;
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
    // after computing the hash, update the array to maintain only the K smallest unique hash values
    int index = -1;
    for(int i = 0; i < K; i++){
      if(vals[i] == hash){
  return;
      }
      if(vals[i] == -1){
  index = i;
  break;
      }
    }
    if(index == -1){
      int max_index = 0;
      for(int i = 1; i < K; i++){
  if(vals[i] > vals[max_index]){
    max_index = i;
  }
      }
      if(hash < vals[max_index]){
  vals[max_index] = hash;
      }
    }
    else{
      vals[index] = hash;
    }
  }


  double estimateCardinality(){
    sort(vals, vals + K);
    double estimate = (K - 1) * (double) ULLONG_MAX / (double) vals[K-1];
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
