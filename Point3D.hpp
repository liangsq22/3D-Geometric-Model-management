/*************************************************************************
【文件名】                 Point3D.hpp
【功能模块和目的】         由Vector3D派生出三维点类，声明
【开发者及日期】           梁思奇 2024/7/29
【更改记录】               
*************************************************************************/

#ifndef POINT3D_HPP
#define POINT3D_HPP

//Vecor3D基类模版所属头文件
#include "Vector3D.hpp"
//std::string所属头文件
#include <string>
//std::istream、std::ostream所属头文件
#include <iostream>

/*************************************************************************
【类名】             Point3D
【功能】             三维点类
【接口说明】         继承自Vector3D，含构造、析构、赋值函数，
                    新增坐标点数据成员，
                    实现了三维点的坐标访问，
                    派生类接口：获取和设置现实属性
【开发者及日期】      梁思奇 2024/7/29
【更改记录】         梁思奇 2024/8/5 改进写法
*************************************************************************/
class Point3D : public Vector3D<double>{
public:
    //默认构造函数
    Point3D();
    //拷贝构造函数
    Point3D(const Point3D& Source);
    //带参数基类构造函数
    Point3D(const Vector3D<double>& Source);
    //带三坐标点参数构造函数
    Point3D(double XValue, double YValue, double ZValue);
    //虚析构函数（可能做基类），无动态构造成员，默认实现
    virtual ~Point3D() = default;
    //赋值运算符
    Point3D& operator=(const Point3D Source);
    
    //Setter
    //X点坐标Setter
    void SetX(double Value);
    //Y点坐标Setter
    void SetY(double Value);
    //Z点坐标Setter
    void SetZ(double Value);
    //三维点坐标Setter
    void SetXYZ(double XValue, double YValue, double ZValue);
    //流提取运算符（泛用，不针对Model3D）
    friend std::istream& operator>>(
        std::istream& Stream, Point3D& Point1);

    //Getter
    //X点坐标Getter
    double GetX() const;
    //Y点坐标Getter
    double GetY() const;
    //Z点坐标Getter
    double GetZ() const;  
    //三维点坐标Getter  
    std::array<double, 3> GetXYZ() const;
    //流插入运算符（泛用，不针对Model3D）
    friend std::ostream& operator<<(
        std::ostream& Stream, const Point3D& Point1);

protected:
    //新增虚拟空函数接口，获取和设置现实属性，用于派生类
    //Setter
    //设置颜色
    virtual void SetColor(const std::string& Color1); 
    //设置半径
    virtual void SetRadius(double Radius1);
    //设置材质
    virtual void SetMaterial(const std::string& Material1); 

    //Getter
    //获取颜色
    virtual std::string GetColor() const;  
    //获取半径
    virtual double GetRadius() const;     
    //获取材质
    virtual std::string GetMaterial() const; 

private:
    //数据成员，三维点坐标
    //X坐标，向量第0分量
    double& m_rX{Component[0]};
    //Y坐标，向量第1分量
    double& m_rY{Component[1]};
    //Z坐标，向量第2分量
    double& m_rZ{Component[2]};
};

#endif //POINT3D_HPP
