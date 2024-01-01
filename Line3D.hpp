/*************************************************************************
【文件名】                 Line3D.hpp
【功能模块和目的】          三维直线段类声明
【开发者及日期】            梁思奇 2024/8/2
【更改记录】               梁思奇 2024/8/7 完善类架构
*************************************************************************/

#ifndef LINE3D_HPP
#define LINE3D_HPP

//特定数量三维点基类所属头文件
#include "FixedElements3D.hpp"
//std::string所属头文件
#include <string>
//std::istream、std::ostream所属头文件
#include <iostream>

/*************************************************************************
【类名】             Line3D
【功能】             三维直线段类（非重复三维点集合容量2）
【接口说明】         继承自FixedElements3D，含构造、析构、赋值行为，
                    实现了求直线段长度、求面积、判断相同等功能，
                    派生类接口：用于获取和设置现实属性
【开发者及日期】      梁思奇 2024/8/2
【更改记录】         梁思奇 2024/8/7 增加功能和改进类架构
*************************************************************************/
class Line3D : public FixedElements3D{
public:
    //无默认构造函数，线段需要两个不同端点
    Line3D() = delete;
    //拷贝构造函数
    Line3D(const Line3D& Source);
    //带参构造函数
    Line3D(const Point3D& Point1, const Point3D& Point2);
    //虚析构函数
    virtual ~Line3D() = default;
    
    //Setter
    //赋值运算符
    Line3D& operator=(const Line3D& Source);
    //设置新的直线段端点
    void SetPoints(const Point3D& Point1, const Point3D& Point2);
    //流提取运算符（泛用，不针对Model3D）
    friend std::istream& operator>>(
        std::istream& Stream, Line3D& Line1);

    //Getter
    //求长度override
    double GetLength() const override;
    //求面积override
    double GetArea() const override;
    //获取直线段的两个端点
    std::pair<Point3D, Point3D> GetPoints() const;
    //判断直线段是否与其他直线段相等
    bool operator==(const Line3D& Line1) const;
    //判断直线段是否不等于其他直线段
    bool operator!=(const Line3D& Line1) const;
    //流插入运算符（泛用，不针对Model3D）
    friend std::ostream& operator<<(
        std::ostream& Stream, const Line3D& Line1);

protected:
    //新增虚拟空函数接口，获取和设置现实属性，用于派生类
    //Setter
    //设置颜色
    virtual void SetColor(const std::string& Color1); 
    //设置线的粗细
    virtual void SetWidth(double Width1);  
    //设置材质
    virtual void SetMaterial(const std::string& Material1); 

    //Getter
    //获取颜色
    virtual std::string GetColor() const;  
    //获取线的粗细
    virtual double GetWidth() const;     
    //获取材质
    virtual std::string GetMaterial() const; 

private:
    //私有数据成员：三维直线段端点
    Point3D m_Point1;
    Point3D m_Point2;
};

#endif //LINE3D_HPP
