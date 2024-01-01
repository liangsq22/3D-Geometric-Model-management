/*************************************************************************
【文件名】                 Vector3D.hpp
【功能模块和目的】         三维向量模版(派生)
【开发者及日期】           梁思奇 2024/7/29
【更改记录】               
*************************************************************************/

#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

//size_t所属头文件
#include <cstddef>
//Vector基类模版所属头文件
#include "Vector.hpp"

/*************************************************************************
【类模版名】          Vector3D
【功能】             三维向量模版(派生)
【接口说明】         继承自Vector<T, 3>，构造、析构、赋值函数，
                    实现了三维向量的叉积
【开发者及日期】      梁思奇 2024/7/29
【更改记录】         梁思奇 2024/8/5 重构
*************************************************************************/
template <class T>
class Vector3D : public Vector<T, 3>{
public:
    //继承基类构造函数
    using Vector<T, 3>::Vector;
    //默认拷贝构造函数
    Vector3D(const Vector3D& Source) = default;
    //带基类参数的构造函数
    Vector3D(const Vector<T, 3>& Source);
    //带3个参数的构造函数
    Vector3D(T XValue, T YValue, T ZValue);
    //虚析构函数（作基类）
    virtual ~Vector3D() = default;
    //赋值运算符
    Vector3D& operator=(const Vector3D& Source);
protected:
    //三维向量叉乘实现函数重载
    Vector<T,3> CrossProduct_Protected(const Vector<T, 3>& Vector1) 
        const override;
};

/*************************************************************************
【函数名称】        Vector3D
【函数功能】        三维向量带参基类构造函数
【参数】            const Vector<T, 3>& Source
【返回值】          无
【开发者及日期】    梁思奇 2024/7/29
【更改记录】
*************************************************************************/
template <class T>
Vector3D<T>::Vector3D(const Vector<T, 3>& Source) : Vector<T, 3>(Source){
}

/*************************************************************************
【函数名称】        Vector3D
【函数功能】        三维向量带三参数构造函数
【参数】            T XValue, T YValue, T ZValue
【返回值】          无
【开发者及日期】    梁思奇 2024/7/29
【更改记录】
*************************************************************************/
template <class T>
Vector3D<T>::Vector3D(T XValue, T YValue, T ZValue) 
    : Vector<T, 3>(std::array<T, 3>{XValue, YValue, ZValue}){
}

/*************************************************************************
【函数名称】        operator=
【函数功能】        三维向量赋值运算符
【参数】            const Vector3D& Source
【返回值】          三维向量自身引用值
【开发者及日期】    梁思奇 2024/7/29
【更改记录】       梁思奇 2024/8/5 重写
*************************************************************************/
template <class T>
Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& Source){
    //检查自赋值
    if (this != &Source) {
        //调用基类赋值运算符
        Vector<T, 3>::operator=(Source);
    }
    return *this;
}

/*************************************************************************
【函数名称】        CrossProduct_Protected
【函数功能】        三维向量叉乘实现函数重载
【参数】            const Vector<T, 3>& Vector1
【返回值】          叉乘结果向量
【开发者及日期】    梁思奇 2024/7/29
【更改记录】       
*************************************************************************/
template <class T>
Vector<T,3> Vector3D<T>::CrossProduct_Protected(
    const Vector<T, 3>& Vector1) const{
    //使用数组存储每个分量的计算结果
    std::array<T, 3> Result;
    Result[0] 
        = this->Component[1] * Vector1.Component[2] 
        - this->Component[2] * Vector1.Component[1];
    Result[1] 
        = this->Component[2] * Vector1.Component[0] 
        - this->Component[0] * Vector1.Component[2];
    Result[2] 
        = this->Component[0] * Vector1.Component[1] 
        - this->Component[1] * Vector1.Component[0];
    //返回包含结果的Vector对象
    return Vector<T, 3>(Result);
};

#endif //VECTOR3D_HPP
