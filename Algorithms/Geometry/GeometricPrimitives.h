#pragma once

template <typename TDot>
class Vector {
public:
    TDot x_, y_, z_;
    bool isValid_ = true;
    long double length_;
    long long number_, absNumber_;
public:
    void setAbsNumber(int num){
        absNumber_ = num;
    }

    void setNumber(int num){
        number_ = num;
    }
    Vector() {
        x_ = 0;
        y_ = 0;
        z_ = 0;
        length_ = 0;
        number_ = 0;
    }

    Vector(TDot x, TDot y, TDot z = 0, long long number = 0) {
        x_ = x;
        y_ = y;
        z_ = z;
        length_ = sqrt(x * x + y * y + z * z);
        absNumber_ = number_ = number;
    }
    TDot getNumber(){
        return number_;
    }
    TDot inline getAbsNumber(){
        return absNumber_;
    }

    TDot getZ(){
        return z_;
    }
    TDot getX(){
        return x_;
    }
    TDot getY(){
        return y_;
    }
    Vector inline operator[](const Vector &other) {
        return Vector(y_ * other.z_ - z_ * other.y_,
                      z_ * other.x_ - x_ * other.z_,
                      x_ * other.y_ - y_ * other.x_);
    }

    Vector inline operator+(const Vector& other){
        return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    Vector inline operator-(const Vector& other){
        return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }
    Vector inline operator-(Vector&& other){
        return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }
    long double operator()(const Vector &other) {
        return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
    }

    Vector operator*(TDot k){
        Vector<TDot> tmp = *this;
        tmp.x_ *= k;
        tmp.y_ *= k;
        tmp.z_ *= k;
        tmp.length_*= k;
        return tmp;}


    Vector operator/(TDot k){
        Vector<TDot> tmp = *this;
        tmp.x_ /= k;
        tmp.y_ /= k;
        tmp.z_ /= k;
        tmp.length_ /= k;
        return tmp;
    }
    long double getLenght(){
        return length_;
    }
    long double getCos(Vector& other){
        return this->operator()(other)/length_/other.length_;
    }
    long double getAngle(Vector& other){
        return acos(getCos(other));

    }

    Vector getProjection(Vector<TDot> other){// projection other on this
        TDot k = operator()(other);
        Vector<TDot> tmp= (*this) * (k  / length_ / length_);
        return tmp;
    }
};
template <typename TDot>
Vector<TDot> operator*(TDot k, Vector<TDot>& other){
    return other.operator*(k);

}

template <typename TDot>
Vector<TDot> operator/(TDot k, Vector<TDot>& other){
    return other.operator/(k);
}

template <typename TDot>
class Line;

template <typename TDot>
class Segment {
private:
    bool isVertical_;
    long index_;
    Vector<TDot> begin_, end_;
public:
    Segment<TDot>(const Vector<TDot> &begin, const Vector<TDot> &end, long ind) : begin_(begin), end_(end),
                                                                                  index_(ind) {
        isVertical_ = begin.getX() == end.getX();
    }

    Segment<TDot>(const Segment<TDot> &other) : begin_(other.begin_), end_(other.end_), index_(other.index_) {
        isVertical_ = begin_.getX() == end_.getX();
    }

    double getIndex() const {
        return index_;
    }

    Vector<TDot> getBegin() const {
        return begin_;
    }

    Vector<TDot> getEnd() const {
        return end_;
    }

    bool isVertical() const {
        return isVertical_;
    }

    bool has(const Vector<TDot> &dot) const {
        if (isVertical()) {
            return dot.getX() == begin_.getX() && dot.getY() <= begin_.getY() && dot.getY() >= end_.getY();
        } else {
            return begin_.getX() <= dot.getX() && end_.getX() >= dot.getX() && Line<TDot>(begin_, end_).has(dot);
        }
    }

    bool operator == (const Segment<TDot> &other) const {
        return begin_ == other.begin_ && end_ == other.end_;
    }

    bool operator < (const Segment<TDot> &other) const {
        return begin_.getX() == other.begin_.getX() ? end_.getY() > other.end_.getY() : end_.getX() < other.end_.getX();
    }

    Vector<TDot> getMid() const {
        return Vector<TDot>((begin_.getX() + end_.getX()) / 2, (begin_.getY() + end_.getY()) / 2);
    }
};

template <typename TDot>
class Line{
private:
    double a_, b_, c_;
    const long double EPS  = 0.001;
public:
    Line(Vector<TDot> a, Vector<TDot> b){
        a_ = a.getY() - b.getY();
        b_ = b.getX() - a.getX();
        c_ = a.getX() * b.getY() - b.getX() * a.getY();
    }

    bool has(const Vector<TDot>&  dot) const{
        double res = std::abs(a_*dot.getX() + b_*dot.getY() + c_);
        return  res <= EPS;
    }

    Vector<TDot> getDotWithCertainX(double x) const {
        return Vector<TDot>(x, -(c_ + a_*x)/b_);
    }
};

struct Dot{
    int x,y, number;
    Dot():x(0),y(0),number(0){
    }
    Dot(int x_, int y_, int number_){
        x = x_;
        y = y_;
        number = number_;
    }
};

bool inline isLess(Dot& a, Dot& b, Dot& p0){
    long long tmp = (a.x - p0.x)*(b.y - p0.y) - (a.y - p0.y)*(b.x - p0.x);
    return tmp > 0 || (tmp == 0 && ((a.x - p0.x)*(a.x - p0.x) + (a.y - p0.y)*(a.y - p0.y) <(b.x - p0.x)*(b.x - p0.x) + (b.y - p0.y)*(b.y - p0.y)));
}