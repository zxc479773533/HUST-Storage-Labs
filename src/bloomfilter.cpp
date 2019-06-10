/* 
 * bloomfilter.cpp
 * 
 * TEST for HUST IOT Storage lab
 * 
 * Crated at 2019-06-08
 */

#include <iostream>
#include <fstream>
#include "pylib/debug.h"
#include "pylib/sys/time.h"

using namespace pylib;
using namespace std;

class BloomFilter {
private:
  int size;
  int *tables;
  int Hasher1(std::string str) {
    int ret = 0;
    for (int i = 0; i < str.length(); i++)
      ret += str[i];
    return ret % size;
  }
  int Hasher2(std::string str) {
    int ret = 1;
    for (int i = 0; i < str.length(); i++) {
      ret = ret * 33 + str[i];
    }
    if (ret < 0) ret = -ret;
    return ret % size;
  }
  int Hasher3(std::string str) {
    int ret = str.length();
    for (int i = 0; i < str.length(); i += 2) {
      ret = ((ret >> 28) ^ (ret << 4)) ^ str[i];
    }
    if (ret < 0) ret = -ret;
    return ret % size;
  }
  int Hasher4(std::string str) {
    int ret = 1;
    int p = (int)2166136261L;
    for (int i = 0; i < str.length(); i += 2) {
      ret = (ret ^ str[i]) * p;
    }
    ret += ret << 13;
    ret ^= ret >> 7;
    ret += ret << 3;
    ret ^= ret >> 17;
    ret += ret << 5;
    if (ret < 0) ret = -ret;
    return ret % size;
  }
  int Hasher5(std::string str) {
    int ret = 0;
    int a = 63689;
    int b = 378551;
    for (int i = 0; i < str.length(); i += 2) {
      ret = ret * a + str[i];
      a = a * b;
    }
    if (ret < 0) ret = -ret;
    return ret % size;
  }
  
public:
  BloomFilter(int size) {
    this->size = size;
    tables = new int[this->size];
    if (tables == nullptr)
      err_msg("New table error");
    for (int i = 0; i < this->size; i++)
      tables[i] = 0;
  }
  ~BloomFilter() { delete[] tables; }
  bool Add(std::string str) {
    int ret[5];
    ret[0] = Hasher1(str);
    ret[1] = Hasher2(str);
    ret[2] = Hasher3(str);
    ret[3] = Hasher4(str);
    ret[4] = Hasher5(str);
    for (int i = 0; i < 5; i++) {
      tables[ret[i]]++;
    }
    return true;
  }
  bool Find(std::string str) {
    int find = true;
    int ret[5];
    ret[0] = Hasher1(str);
    ret[1] = Hasher2(str);
    ret[2] = Hasher3(str);
    ret[3] = Hasher4(str);
    ret[4] = Hasher5(str);
    for (int i = 0; i < 5; i++) {
      if (tables[ret[i]] == 0) {
        find = false;
        break;
      }
    }
    return find;
  }
  bool Delete(std::string str) {
    int ret[5];
    ret[0] = Hasher1(str);
    ret[1] = Hasher2(str);
    ret[2] = Hasher3(str);
    ret[3] = Hasher4(str);
    ret[4] = Hasher5(str);
    for (int i = 0; i < 5; i++) {
      tables[ret[i]]--;
    }
    return true;
  }
};

int main(int argc, const char** argv) {
  Timer Start;

  ifstream datafile, testfile, deletefile;
  datafile.open("data.txt");
  testfile.open("test.txt");
  deletefile.open("data.txt");

  BloomFilter bf(7500);
  string str;

  while(getline(datafile, str)) {
    bf.Add(str);
  }
  Timer Afteradd;

  int counter = 0;
  while(getline(testfile, str)) {
    if (bf.Find(str) == true)
      counter++;
  }
  Timer Afterfind;

  while(getline(deletefile, str)) {
    bf.Delete(str);
  }
  Timer Afterdelete;

  cout << "Results" << endl;
  cout << "      Add time: " << (Afteradd - Start).AccurateMicrosecond() << endl;
  cout << "     Find time: " << (Afterfind - Afteradd).AccurateMicrosecond() << endl;
  cout << "   Delete time: " << (Afterdelete - Afterfind).AccurateMicrosecond() << endl;
  cout << "False Positive: " << counter << endl;

  return 0;
}