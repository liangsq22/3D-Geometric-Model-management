/*************************************************************************
【文件名】                 Vector.hpp
【功能模块和目的】          N维度通用向量类模版
【开发者及日期】           梁思奇 2024/7/28
【更改记录】               梁思奇 2024/8/4 增加模版类功能函数
                          梁思奇 2024/8/5 修改错误；改进叉积派生类接口
*************************************************************************/

#ifndef VECTOR_HPP
#define VECTOR_HPP

//size_t所属头文件
#include <cstddef>
//std::invalid_argument、std::logic_error所属头文件
#include <stdexcept>
//std::array所属头文件
#include <array>
//std::istream、std::ostream所属头文件
#include <iostream>
//std::numeric_limits函数所属头文件
#include <limits>  
//std::abs、std::sqrt函数所属头文件
#include <cmath>

/*************************************************************************
【类模版名】           Vector
【功能】              N维度通用向量类模版
【接口说明】          包含构造、析构、赋值；加法、减法、乘法（标乘、叉乘、点乘）、
                     除法等运算符的重载、范数计算、数据流输入输出等函数；
                     数据成员包含向量分量数组和维数
【开发者及日期】       梁思奇 2024/7/28
【更改记录】          梁思奇 2024/8/4 改进模版类功能函数架构
*************************************************************************/
template <class T, size_t N>
class Vector{
    //维度必须大于0
    static_assert(N > 0, "Dimension shouldn't be <= 0");

public:
    //默认构造函数
    Vector() = default;
    //带参构造函数
    Vector(const std::array<T, N>& Source);
    //默认拷贝构造函数
    Vector(const Vector& Source) = default;
    //默认虚析构函数
    virtual ~Vector() = default;

    //Setter
    //默认赋值运算符
    Vector& operator=(const Vector& Source) = default;
    //向量取反向运算符，引用修改值
    Vector& Invert();
    //加法运算符，引用修改值
    Vector& operator+=(const Vector& Vector1);
    //减法运算符，引用修改值
    Vector& operator-=(const Vector& Vector1);
    //标量乘法运算符，引用修改值
    Vector& operator*=(T Scalar);
    //标量除法运算符，引用修改值
    Vector& operator/=(T Scalar);
    //向量叉乘运算符重载，引用修改值
    Vector& operator*=(const Vector& Vector1);
    //对应下标分量运算符，引用修改值
    T& operator[](size_t Index);
    //输入流运算符
    template <class U, size_t N1>
    friend std::istream& operator>>(
        std::istream& Content, Vector<U, N1>& Vector1);

    //Getter
    //判断向量相等运算符
    bool operator==(const Vector& Vector1) const;
    //判断向量不等运算符
    bool operator!=(const Vector& Vector1) const;
    //取反向量运算符
    Vector opposite() const;
    //向量加法运算符
    Vector operator+(const Vector& Vector1) const;
    //向量减法运算符
    Vector operator-(const Vector& Vector1) const;
    //向量标乘运算符
    Vector operator*(T Scalar) const;
    //向量标除运算符
    Vector operator/(T Scalar) const;
    //向量叉乘运算符重载
    Vector operator*(const Vector& Vector1) const;
    //返回下标分量运算符
    T operator[](size_t Index) const;
    //向量点乘运算符
    T DotProduct(const Vector& Vector1) const;
    //模长
    T Length() const;
    //0范数
    size_t Norm0() const;
    //1范数
    T Norm1() const;
    //2范数，等同于模长
    T Norm2() const;
    //无穷范数
    T InfiniteNorm() const;
    //P范数
    T NormP(double P) const;
    //输出流运算符
    template <class U, size_t N1>
    friend std::ostream& operator<<(
        std::ostream& Content, const Vector<U, N1>& Vector1);

    //数据成员
    //使用std::array存储向量分量Components
    std::array<T, N> Component;
    //静态数据成员：维数（常量N）
    static constexpr size_t DIMENSION{N};

protected:
    //虚拟叉乘函数，用于派生类重载
    virtual Vector CrossProduct_Protected(const Vector& Vector1) const;
};

//Setter函数实现

/*************************************************************************
【函数名称】        Vector
【函数功能】        向量带参构造函数
【参数】            const std::array<T, N>& Source
【返回值】          无
【开发者及日期】    梁思奇 2024/7/28
【更改记录】
*************************************************************************/
template <class T, size_t N>
Vector<T, N>::Vector(const std::array<T, N>& Source) : Component(Source){
}


/*************************************************************************
【函数名称】        Invert
【函数功能】        向量取反Setter
【参数】            无
【返回值】          向量自身引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】
*************************************************************************/
template <class T, size_t N>
Vector<T, N>& Vector<T, N>::Invert(){
    //逐一分量取反
    for (size_t i = 0; i < N; i++) {
        Component[i] = -Component[i];
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator+=
【函数功能】        向量加向量Setter
【参数】            待相加向量const Vector& Vector1
【返回值】          向量自身引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N>& Vector<T, N>::operator+=(const Vector& Vector1){
    //逐一分量+=
    for (size_t i = 0; i < N; i++) {
        Component[i] += Vector1.Component[i];
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator-=
【函数功能】        向量减向量Setter
【参数】            待相减向量const Vector& Vector1
【返回值】          向量自身引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N>& Vector<T, N>::operator-=(const Vector& Vector1){
    //逐一分量-=
    for (size_t i = 0; i < N; i++) {
        Component[i] -= Vector1.Component[i];
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator*=
【函数功能】        向量标量乘法Setter
【参数】            标量值T Scalar
【返回值】          向量自身引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N>& Vector<T, N>::operator*=(T Scalar){
    //逐一分量*=
    for (size_t i = 0; i < N; i++) {
        Component[i] *= Scalar;
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator/=
【函数功能】        向量标量除法Setter
【参数】            标量值T Scalar
【返回值】          向量自身引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N>& Vector<T, N>::operator/=(T Scalar){
    //定义一个非常小的阈值
    const T Epsilon = std::numeric_limits<T>::epsilon();
    //检查除数是否接近于零
    if (std::abs(Scalar) < Epsilon) {
        throw std::invalid_argument("Division by 0 is not allowed");
    }
    //逐一分量/=
    for (size_t i = 0; i < N; i++) {
        Component[i] /= Scalar;
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator*=
【函数功能】        向量叉乘Setter
【参数】            向量const Vector& Vector1
【返回值】          向量自身引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        梁思奇 2024/8/5 改进写法
*************************************************************************/
template <class T, size_t N>
Vector<T, N>& Vector<T, N>::operator*=(const Vector& Vector1){
    //调用Protected_CrossProduct
    return operator=(CrossProduct_Protected(Vector1));
}

/*************************************************************************
【函数名称】        operator>>
【函数功能】        流输入Setter
【参数】           输入流std::istream& Content, 向量Vector<T, N>& Vector1
【返回值】          输入流引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        梁思奇 2024/8/5 改进写法
*************************************************************************/
template <class U, size_t N>
std::istream& operator>>(std::istream& Content, Vector<U, N>& Vector1){
    //根据向量维度，逐一提取元素
    for (size_t i = 0; i < N; i++) {
        Content >> Vector1.Component[i];
    }
    return Content;
}

/*************************************************************************
【函数名称】        operator[]
【函数功能】        向量对应下标分量Setter
【参数】            入口size_t Index
【返回值】          T类型分量值，引用修改值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        梁思奇 2024/8/5 改进写法
*************************************************************************/
template <class T, size_t N>
T& Vector<T, N>::operator[](size_t Index){
    return Component[Index];
}

//Getter函数实现

/*************************************************************************
【函数名称】        operator==
【函数功能】        向量相等判断Getter
【参数】            向量const Vector& Vector1
【返回值】          bool类型结果值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
bool Vector<T, N>::operator==(const Vector& Vector1) const{
    return Component == Vector1.Component;
}

/*************************************************************************
【函数名称】        operator!=
【函数功能】        向量不等判断Getter
【参数】            向量const Vector& Vector1
【返回值】          bool类型结果值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
bool Vector<T, N>::operator!=(const Vector& Vector1) const{
    //逐一比较各个分量，一出现不等则返回true
    for (size_t i = 0; i < N; i++) {
        if (this->Component[i] != Vector1.Component[i]){
            return true;  
        }
    }
    return false; 
}

/*************************************************************************
【函数名称】        opposite const
【函数功能】        向量取反Getter
【参数】            无
【返回值】          结果反向量
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::opposite() const{
    //创建一个新的向量来存储结果
    Vector<T, N> Result;  
    //对每个分量取反
    for (size_t i = 0; i < N; i++) {
        Result.Component[i] = -this->Component[i];  
    }
    return Result;
}

/*************************************************************************
【函数名称】        operator+ const
【函数功能】        向量加法Getter
【参数】            向量const Vector& Vector1
【返回值】          加法结果向量
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::operator+(const Vector& Vector1) const{
    //创建一个新的向量来存储结果
    Vector<T, N> Result;  
    //对每个分量进行相加
    for (size_t i = 0; i < N; i++) {
        Result.Component[i] = this->Component[i] + Vector1.Component[i];  
    }
    return Result;
}

/*************************************************************************
【函数名称】        operator- const
【函数功能】        向量减法Getter
【参数】            向量const Vector& Vector1
【返回值】          减法结果向量
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::operator-(const Vector& Vector1) const{
    //创建一个新的向量来存储结果
    Vector<T, N> Result;  
    //对每个分量进行相减
    for (size_t i = 0; i < N; i++) {
        Result.Component[i] = this->Component[i] - Vector1.Component[i];  
    }
    return Result;
}

/*************************************************************************
【函数名称】        operator* const
【函数功能】        向量标乘Getter
【参数】            标量T Scalar
【返回值】          标乘结果向量
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::operator*(T Scalar) const{
    //创建一个新的向量来存储结果
    Vector<T, N> Result;  
    //对每个分量进行标量乘法运算
    for (size_t i = 0; i < N; i++) {
        Result.Component[i] = this->Component[i] * Scalar;  
    }
    return Result;
}

/*************************************************************************
【函数名称】        operator/ const
【函数功能】        向量标除Getter
【参数】            标量T Scalar
【返回值】          标除结果向量
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::operator/(T Scalar) const{
    //定义一个非常小的阈值
    const T Epsilon = std::numeric_limits<T>::epsilon();
    //检查除数是否接近于零
    if (std::abs(Scalar) < Epsilon) {
        throw std::invalid_argument("Division by zero is not allowed");
    }
    //创建一个新的向量来存储结果
    Vector<T, N> Result;  
    //对每个分量进行标量除法运算
    for (size_t i = 0; i < N; i++) {
        Result.Component[i] = this->Component[i] / Scalar;  
    }
    return Result;
}

/*************************************************************************
【函数名称】        operator* const
【函数功能】        向量叉乘Getter
【参数】            向量const Vector& Vector1
【返回值】          叉乘结果向量
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::operator*(const Vector& Vector1) const{
    //调用Protected_CrossProduct
    return Protected_CrossProduct(Vector1);
}

/*************************************************************************
【函数名称】        operator[] const
【函数功能】        向量对应下标分量Getter
【参数】            入口size_t Index
【返回值】          T类型分量值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::operator[](size_t Index) const{
    return Component[Index];
}

/*************************************************************************
【函数名称】        DotProduct const
【函数功能】        向量点乘Getter
【参数】            向量const Vector& Vector1
【返回值】          点乘结果值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::DotProduct(const Vector& Vector1) const{
    T Result = 0;  
    //逐一元素相乘并累加
    for (size_t i = 0; i < N; i++) {
        Result += this->Component[i] * Vector1.Component[i];  
    }
    return Result;
}

/*************************************************************************
【函数名称】        Length const
【函数功能】        求模长Getter，同2范数
【参数】            无
【返回值】          所有分量平方和的算数平方根，T类型值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::Length() const{
    T Sum = 0;  
    //逐一元素平方累加
    for (size_t i = 0; i < N; i++) {
        Sum += this->Component[i] * this->Component[i];  
    }
    return static_cast<T>(std::sqrt(Sum));
}

/*************************************************************************
【函数名称】        Norm0 const
【函数功能】        0范数Getter
【参数】            无
【返回值】          不为0分量的个数，size_t类型值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
size_t Vector<T, N>::Norm0() const{
    size_t Result = 0;  
    //逐一累加不为0的分量个数
    for (size_t i = 0; i < N; i++) {
        Result += this->Component[i] != 0;  
    }
    return Result;
}

/*************************************************************************
【函数名称】        Norm1 const
【函数功能】        1范数Getter
【参数】            无
【返回值】          所有分量的绝对值之和，T类型值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::Norm1() const{
    T Result = 0;  
    //逐一累加各分量的绝对值
    for (size_t i = 0; i < N; i++) {
        Result += std::abs(this->Component[i]);  
    }
    return Result;
}

/*************************************************************************
【函数名称】        Norm2 const
【函数功能】        2范数Getter，同模长
【参数】            无
【返回值】          所有分量的绝对值之和，T类型值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::Norm2() const{
    return Length();
}

/*************************************************************************
【函数名称】        InfiniteNorm const
【函数功能】        无穷范数Getter
【参数】            无
【返回值】          最大分量值，T类型值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::InfiniteNorm() const{
    T Result = this->Component[0];  
    //遍历所有分量，找到最大值
    for (size_t i = 1; i < N; i++) {
        if (this->Component[i] > Result) {
            Result = this->Component[i];
        }
    }
    return Result;
}

/*************************************************************************
【函数名称】        NormP const
【函数功能】        P范数Getter
【参数】            double P
【返回值】          所有分量绝对值的P次方和的1/P次方，T类型值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        
*************************************************************************/
template <class T, size_t N>
T Vector<T, N>::NormP(double P) const{
    //P必须>=1
    if (P < 1) {
        throw std::invalid_argument("The norm order P must be >= 1");
    }
    //用double类型累加器提高精度
    double Sum = 0;
    //逐一分量绝对值的P次方，累加
    for (size_t i = 0; i < N; i++) {
        Sum += std::pow(std::abs(Component[i]), P);
    }
    return static_cast<T>(std::pow(Sum, 1.0 / P));
}

/*************************************************************************
【函数名称】        operator<<
【函数功能】        流输出Getter
【参数】           输出流std::ostream& Content, 向量const Vector<T, N>& Vector1
【返回值】          输出流引用值
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        梁思奇 2024/8/5 改进写法
*************************************************************************/
template <class U, size_t N>
std::ostream& operator<<(std::ostream& Content, const Vector<U, N>& Vector1){
    //前N-1个分量逐个输出并接空格
    for (size_t i = 0; i < N - 1; i++) {
        Content << Vector1.Component[i] << ' ';  
    }
    //最后无空格
    Content << Vector1.Component[N - 1];  
    return Content;
}

//Protected函数实现

/*************************************************************************
【函数名称】        CrossProduct_Protected const
【函数功能】        虚拟叉乘函数，用于派生类重载
【参数】            向量const Vector& Vector1
【返回值】          无，若没派生实现则抛出异常
【开发者及日期】    梁思奇 2024/7/28
【更改记录】        梁思奇 2024/8/5 改进写法
*************************************************************************/
template <class T, size_t N>
Vector<T, N> Vector<T, N>::CrossProduct_Protected(const Vector& Vector1) const{
    throw std::logic_error(
        "Cross product not defined for this dimensional type of vector");
}

#endif //VECTOR_HPP
