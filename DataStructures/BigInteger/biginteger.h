#pragma once
#include <vector>
#include <string>
#include<iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;

class BigInteger;
const BigInteger operator*(const BigInteger &first, const BigInteger &second);

const BigInteger operator+(const BigInteger &first, const BigInteger &second);

const BigInteger operator-(const BigInteger &first, const BigInteger &second);

const BigInteger operator/(const BigInteger &first, const BigInteger &second);

const BigInteger operator%(const BigInteger &first, const BigInteger &second);

bool operator>=(const BigInteger &first, const BigInteger &second);

bool operator<=(const BigInteger &first, const BigInteger &second);
std::ostream& operator<<(std::ostream& os, const BigInteger& tmp);

std::istream &operator >> (std::istream &istr, BigInteger& tmp);
void swap(BigInteger& first, BigInteger& second);



class BigInteger {
public:

    friend std::istream &operator>>(std::istream &istr, BigInteger &tmp);

    friend bool operator==(const BigInteger &first, const BigInteger &second);

    friend bool operator>(const BigInteger &first, const BigInteger &second);

    friend bool operator<(const BigInteger &first, const BigInteger &second);

    BigInteger() : sign(1) {}

    ~BigInteger(){

    }
    BigInteger(const int &n1);


    BigInteger(const BigInteger &tmp);

    explicit operator bool() const;

    string toString() const;

    BigInteger operator=(const BigInteger &other);


    BigInteger operator-() const;

    BigInteger &operator++();

    BigInteger &operator--();

    const BigInteger operator++(int);

    const BigInteger operator--(int);

    BigInteger &operator-=(const BigInteger &other);


    BigInteger &operator+=(const BigInteger &other);

    BigInteger &operator*=(const BigInteger &other1);

    BigInteger &operator%=(const BigInteger &other1);

    BigInteger &operator/=(const BigInteger &other1);

private:
    void swap(int &a, int &b);

    vector<int> buffer;
    int sign;

    int size() const;

    void divide(const BigInteger &first, const BigInteger &second, BigInteger &rest, BigInteger &q);


    void checkIfZero(BigInteger &tmp) const;

    void normalaze(BigInteger &tmp) const;

    void pl(const BigInteger &other);

    void mi(const BigInteger &other);

    void cut(BigInteger &tmp) const;

    bool absEqual(const BigInteger &other) const;
    bool absLess(const BigInteger &other) const;

    bool absGraterEq(const BigInteger &other) const;

    bool absGreater(const BigInteger &other) const;

    bool absLessEq(const BigInteger &other) const;

    void mul(const vector<int> &x, const vector<int> &y, vector<int> &answer);

    void reverse();
};

BigInteger::BigInteger(const int& n1) {
    int n = n1;

    if (n >= 0) {
        sign = 1;
    }
    else {
        sign = -1;
        n *= -1;
    }
    if (n == 0) buffer.push_back(0);
    while (n > 0) {
        buffer.push_back(n % 10);
        n /= 10;
    }
    normalaze(*this);
}


BigInteger::BigInteger(const BigInteger &tmp) {
    sign = tmp.sign;
    buffer = tmp.buffer;
    normalaze(*this);
}

BigInteger:: operator bool() const {
    return *this == 0 ? false : true;
}

string  BigInteger:: toString() const {
    string tmp;
    if (sign == -1)
        tmp.push_back('-');
    for (int i = size() - 1; i >= 0; --i) {
        tmp.push_back(buffer[i] + '0');
    }
    return tmp;
}



BigInteger  BigInteger:: operator=(const BigInteger &other) {
    sign = other.sign;
    buffer = other.buffer;
    normalaze(*this);
    return *this;
}


BigInteger  BigInteger::operator-() const {
    BigInteger tmp;
    tmp.sign = (-1) * sign;
    tmp.buffer = buffer;
    normalaze(tmp);
    return tmp;
}

BigInteger& BigInteger:: operator++() {
    return *this += 1;
}

BigInteger& BigInteger:: operator--() {
    return *this -= 1;
}

const BigInteger  BigInteger::operator++(int) {
    BigInteger old = *this;
    ++(*this);
    return old;
}
const BigInteger  BigInteger::operator--(int) {
    BigInteger old = *this;
    --(*this);
    return old;
}
BigInteger& BigInteger:: operator-=(const BigInteger &other) {
    if (sign * other.sign == 1)
        mi(other);
    else
        pl(-other);
    normalaze(*this);
    return *this;
}


BigInteger&  BigInteger::operator+=(const BigInteger &other) {
    if (sign * other.sign == 1)
        pl(other);
    else
        mi(-other);
    normalaze(*this);
    return *this;
}

BigInteger&  BigInteger::operator*=(const BigInteger &other1) {
    BigInteger other = other1;
    int sig = other.sign * sign;
    if (absGraterEq(other))
        mul(other.buffer, buffer, buffer);
    else
        mul(buffer, other.buffer, buffer);
    sign = sig;
    normalaze(*this);
    return *this;
}

BigInteger&  BigInteger::operator%=(const BigInteger &other1) {
    int sig = sign;
    BigInteger other = other1;
    other.sign = 1;
    BigInteger r;
    divide(*this, other, r,*this);
    *this = r;
    sign = sig;
    normalaze(*this);
    return *this;
}

BigInteger&  BigInteger::operator/=(const BigInteger &other1) {
    BigInteger other = other1;
    int sig = other.sign * sign;
    other.sign = 1;
    BigInteger r;
    divide(*this, other, r,*this);
    sign = sig;
    normalaze(*this);
    return *this;
}

void   BigInteger::swap(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
}

int  BigInteger::size() const {
    return (int)buffer.size();
}

void  BigInteger:: divide(const BigInteger &first, const BigInteger &second, BigInteger &rest, BigInteger & q) {
    BigInteger d;
    int k = 0;
    while (first.size() > k) {
        ++k;
        rest.buffer.push_back(first.buffer[first.size() - k]);
        for (int i = rest.size() - 2; i >= 0; --i)
            swap(rest.buffer[i], rest.buffer[i + 1]);

        int a = 5;
        while (rest.absGreater(second * a))
            ++a;
        while (rest.absLess(second * a))
            --a;
        d.buffer.push_back(a);
        rest -= second * a;
        if (rest.buffer.back() == 0)
            rest.buffer.pop_back();
    }
    d.reverse();
    if (rest.size() == 0)
        rest.buffer.push_back(0);
    q = d;
}


void  BigInteger::checkIfZero(BigInteger &tmp) const {
    if (tmp.size() == 1 && tmp.buffer.back() == 0)
        tmp.sign = 1;
}

void  BigInteger::normalaze(BigInteger &tmp) const {
    cut(tmp);
    checkIfZero(tmp);
}

void  BigInteger::pl(const BigInteger &other) {
    if (absGraterEq(other)) {
        int d = 0;
        for (int i = 0; i < other.size(); ++i) {
            buffer[i] = (buffer[i] + other.buffer[i] + d);
            d = buffer[i] / 10;
            buffer[i] %= 10;
        }
        for (int i = other.size(); i < size(); ++i) {
            buffer[i] = (buffer[i] + d);
            d = buffer[i] / 10;
            buffer[i] %= 10;
        }
        if (d)
            buffer.push_back(d);
    }
    else {
        int d = 0;
        for (int i = 0; i < size(); ++i) {
            buffer[i] = (buffer[i] + other.buffer[i] + d);
            d = buffer[i] / 10;
            buffer[i] %= 10;;
        }
        for (int i = size(); i < other.size(); ++i) {
            buffer.push_back((other.buffer[i] + d));
            d = buffer[i] / 10;
            buffer[i] %= 10;
        }
        if (d)
            buffer.push_back(d);
        sign = other.sign;

    }
}

void  BigInteger::mi(const BigInteger &other) {
    if (absGraterEq(other)) {
        int d = 0;
        for (int i = 0; i < other.size(); ++i) {
            buffer[i] = (buffer[i] - other.buffer[i] + d);
            if (buffer[i] < 0) {
                d = -1;
                buffer[i] += 10;
            }
            else
                d = 0;
        }
        for (int i = other.size(); i < size(); ++i) {
            buffer[i] = (buffer[i] + d);
            if (buffer[i] < 0) {
                d = -1;
                buffer[i] += 10;
            }
            else
                d = 0;
        }
    }
    else {
        int d = 0;
        for (int i = 0; i < size(); ++i) {
            buffer[i] = -buffer[i] + other.buffer[i] + d;
            if (buffer[i] < 0) {
                d = -1;
                buffer[i] += 10;
            }
            else
                d = 0;
        }
        for (int i = size(); i < other.size(); ++i) {
            buffer.push_back(other.buffer[i] + d);
            if (buffer[i] < 0) {
                d = -1;
                buffer[i] += 10;
            }
            else
                d = 0;
        }
        sign = other.sign * (-1);
    }
}

void  BigInteger::cut(BigInteger &tmp) const {
    while (tmp.buffer[tmp.size() - 1] == 0 && tmp.size() != 1)
        tmp.buffer.pop_back();

}


bool  BigInteger::absLess(const BigInteger &other) const {
    if (size() > other.size())
        return false;
    else if (size() < other.size())
        return true;
    else {
        for (int i = size() - 1; i >= 0; --i) {
            if (buffer[i] < other.buffer[i])
                return true;
            else if (buffer[i] > other.buffer[i])
                return false;
        }
        return false;
    }
}

bool BigInteger:: absGraterEq(const BigInteger &other) const {
    return !absLess(other);
}
bool BigInteger::absGreater(const BigInteger &other) const {
    if (size() > other.size())
        return true;
    else if (size() < other.size())
        return false;
    else {
        for (int i = size() - 1; i >= 0; --i) {
            if (buffer[i] < other.buffer[i])
                return false;
            else if (buffer[i] > other.buffer[i])
                return true;

        }
        return false;
    }
}

bool  BigInteger::absLessEq(const BigInteger &other) const {
    return !absGreater(other);
}

bool  BigInteger::absEqual(const BigInteger &other) const {
    if (size() != other.size())
        return false;
    for (int i = 0; i < size(); ++i)
        if (buffer[i] != other.buffer[i])
            return false;
    return true;
}


void BigInteger:: mul(const vector<int> &x, const vector<int> &y, vector <int> &answer) {
    int len = x.size() + y.size();
    vector<int> res(len);

    for (int i = 0; i < (int)x.size(); ++i) {
        for (int j = 0; j < (int)y.size(); ++j) {
            res[i + j] += x[i] * y[j];
            res[i + j + 1] += res[i + j] / 10;
            res[i + j] %= 10;
        }
    }
    answer = res;
}


void  BigInteger::reverse() {
    for (int i = 0; i < size() / 2; ++i)
        swap(buffer[i], buffer[size() - 1 - i]);
}

std::ostream& operator<<(std::ostream& os, const BigInteger& tmp)
{
    os << (tmp).toString();
    return os;
}

std::istream &operator >> (std::istream &istr, BigInteger& tmp)
{
    tmp.buffer.clear();
    char p = '0';
    istr.get(p);
    while ((p<'0' || p>'9') && p != '-' && !istr.eof())
        istr.get(p);
    if (p == '-') {
        tmp.sign = -1;
        istr.get(p);
    }
    else
        tmp.sign = 1;

    while (!((p<'0' || p>'9') && p != '-')&& !istr.eof())
    {
        tmp.buffer.push_back(p - '0');
        istr.get(p);
    }
    tmp.reverse();
    tmp.normalaze(tmp);
    return istr;
}

const BigInteger operator+ (const BigInteger & a, const BigInteger& b) {
    BigInteger tmp = a;
    return tmp += b;
}
const BigInteger operator- (const BigInteger & a, const BigInteger& b) {
    BigInteger tmp = a;
    return tmp -= b;
}
const BigInteger operator*(const BigInteger& first, const BigInteger& second) {
    BigInteger tmp = first;
    return tmp *= second;
}

const BigInteger operator/(const BigInteger& first, const BigInteger& second) {
    BigInteger tmp = first;
    return tmp /= second;
}
const BigInteger operator%(const BigInteger& first, const BigInteger& second) {
    BigInteger tmp = first;
    return tmp %= second;
}

bool operator<(const BigInteger& first, const BigInteger& second) {
    if (first.sign > second.sign)
        return false;
    if (first.sign < second.sign)
        return true;
    return first.sign == 1 ? first.absLess(second) : first.absGreater(second);
}

bool operator>=(const BigInteger& first, const BigInteger& second) {
    return !(first<second);
}

bool operator>(const BigInteger& first, const BigInteger& second) {
    if (first.sign > second.sign)
        return true;
    if (first.sign < second.sign)
        return false;
    return first.sign == 1 ? first.absGreater(second) : first.absLess(second);
}

bool operator<=(const BigInteger& first, const BigInteger& second) {
    return !(first > second);
}

bool operator==(const BigInteger& first, const BigInteger& second) {
    if (first.sign != second.sign)
        return false;
    return first.absEqual(second);
}

bool operator!=(const BigInteger& first, const BigInteger& second) {
    return !(first == second);
}

void swap(BigInteger& first, BigInteger& second){
    BigInteger tmp = first;
    first = second;
    second = tmp;
}