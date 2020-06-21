#ifndef __LinearVector__
#define __LinearVector__

#include <cassert>
#include <vector>

using namespace std;

template <typename T>
vector<T> operator+(const vector<T> &a, const vector<T> &b) {
  assert(a.size() == b.size());

  vector<T> result(a.size());
  for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = a[i] + b[i];

  return result;
}

template <typename T>
T operator*(const vector<T> &a, const vector<T> &b){
	assert(a.size() == b.size());
	
	T result = a[0]*b[0];
  for (int i = 1; i < static_cast<int>(a.size()); i++) result += a[i]*b[i];

  return result;
}

template <typename T>
vector<T> operator*(const T &a, const vector<T> &b) {

  vector<T> result(b.size());
  for (int i = 0; i < static_cast<int>(b.size()); i++) result[i] = a*b[i];

  return result;
}

template <typename T>
vector<T> operator*(const vector<T> &b, const T &a) {

  vector<T> result(b.size());
  for (int i = 0; i < static_cast<int>(b.size()); i++) result[i] = a*b[i];

  return result;
}

#endif //__LinearVector__