/*************************************************************************
【文件名】                 Face3D.hpp
【功能模块和目的】          三维三角形面类声明
【开发者及日期】            梁思奇 2024/8/2
【更改记录】               梁思奇 2024/8/7 完善类架构
*************************************************************************/

#ifndef FACE3D_HPP
#define FACE3D_HPP

//特定数量三维点基类所属头文件
#include "FixedElements3D.hpp"
//std::tuple所属头文件
#include <tuple>
//std::string所属头文件
#include <string>

/*************************************************************************
【类名】             Face3D
【功能】             三维三角形类（非重复三维点集合容量3）
【接口说明】         继承自FixedElements3D，含构造、析构、赋值行为，
                    实现了求三角形面积、求周长、判断相同等功能，
                    派生类接口：用于获取和设置现实属性
【开发者及日期】      梁思奇 2024/8/2
【更改记录】         梁思奇 2024/8/7 增加功能和改进类架构
*************************************************************************/
class Face3D : public FixedElements3D{
public:
    //无默认构造函数，三角形面类必须有三个不同顶点
    Face3D() = delete;
    //拷贝构造函数
    Face3D(const Face3D& Source);
    //带参构造函数
    Face3D(const Point3D& Point1, 
        const Point3D& Point2, const Point3D& Point3);
    //虚析构函数
    virtual ~Face3D() = default;
    
    //Setter
    //赋值运算符
    Face3D& operator=(const Face3D& Source);
    //设置新的三角形顶点
    void SetPoints(const Point3D& Point1, 
        const Point3D& Point2, const Point3D& Point3);
    //流提取运算符（泛用，不针对Model3D）
    friend std::istream& operator>>(std::istream& Stream,
        Face3D& Face1);

    //Getter
    //求长度override
    double GetLength() const override;
    //求面积override
    double GetArea() const override;
    //获取三角形的三个顶点
    std::tuple<Point3D, Point3D, Point3D> GetPoints() const;
    //判断三角形是否与其他三角形相等
    bool operator==(const Face3D& Face1) const;
    //判断三角形是否不等于其他三角形
    bool operator!=(const Face3D& Face1) const;
    //流插入运算符（泛用，不针对Model3D）
    friend std::ostream& operator<<(std::ostream& Stream,
        const Face3D& Face1);
    
protected:
    //新增虚拟空函数接口，获取和设置现实属性，用于派生类
    //Setter
    //设置颜色
    virtual void SetColor(const std::string& Color1); 
    //设置厚度
    virtual void SetThickness(double Thickness1);  
    //设置材质
    virtual void SetMaterial(const std::string& Material1); 

    //Getter
    //获取颜色
    virtual std::string GetColor() const;  
    //获取厚度
    virtual double GetThickness() const;     
    //获取材质
    virtual std::string GetMaterial() const; 

private:
    //私有数据成员：三维三角形顶点
    Point3D m_Point1;
    Point3D m_Point2;
    Point3D m_Point3;
};

#endif //FACE3D_HPP
