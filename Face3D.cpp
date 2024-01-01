/*************************************************************************
【文件名】                 Face3D.cpp
【功能模块和目的】          三维三角形面类实现
【开发者及日期】            梁思奇 2024/8/2
【更改记录】               梁思奇 2024/8/2 完善类架构
*************************************************************************/

//自身类头文件
#include "Face3D.hpp"
//Point3D类所属头文件
#include "Point3D.hpp"
//std::invalid_argument、std::logic_error所属头文件
#include <stdexcept>
//std::string所属头文件
#include <string>
//std::cmath所属头文件
#include <cmath>
//std::tuple所属头文件
#include <tuple>

/*************************************************************************
【函数名称】        Face3D
【函数功能】        拷贝构造函数
【参数】            const Face3D& Source
【返回值】          无
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
Face3D::Face3D(const Face3D& Source) : FixedElements3D(Source){
}

/*************************************************************************
【函数名称】        Face3D
【函数功能】        带参构造函数，根据Point1、Point2和Point3初始化三角形
【参数】            const Point3D& Point1, const Point3D& Point2, 
                   const Point3D& Point3
【返回值】          无
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
//按规则第一行以下均缩进4格
Face3D::Face3D(const Point3D& Point1, const Point3D& Point2, 
    const Point3D& Point3) 
    : FixedElements3D(std::vector<Point3D>{Point1, Point2, Point3}, 3){
}

//Setter成员函数

/*************************************************************************
【函数名称】        operator=
【函数功能】        赋值运算符
【参数】            const Face3D& Source
【返回值】          Face3D对象的引用
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
Face3D& Face3D::operator=(const Face3D& Source){
    //检查自赋值
    if (this != &Source) {
        FixedElements3D::operator=(Source);
    }
    return *this;
}

/*************************************************************************
【函数名称】        SetPoints
【函数功能】        设置新的三角形顶点
【参数】            const Point3D& Point1, const Point3D& Point2, 
                   const Point3D& Point3
【返回值】          无
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
void Face3D::SetPoints(
    const Point3D& Point1, const Point3D& Point2, const Point3D& Point3){
    //检查三个点是否有重复
    if (Point1 == Point2 || Point2 == Point3 || Point1 == Point3) {
        throw std::invalid_argument(
            "Cannot set duplicate points in a triangle");
    }
    //清空原有点集
    FixedElements3D::ClearAllPoints();
    //添加新的三个点
    FixedElements3D::AddPoint(Point1);
    FixedElements3D::AddPoint(Point2);
    FixedElements3D::AddPoint(Point3);
}

/*************************************************************************
【函数名称】        operator>>
【函数功能】        流提取运算符，从输入流中提取Face3D对象
【参数】            std::istream& Stream, Face3D& Face1
【返回值】          std::istream&，提取后的流
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
std::istream& operator>>(std::istream& Stream, Face3D& Face1){
    //临时变量存储三个点
    Point3D Point1, Point2, Point3;
    //Point3D类已经重载了流提取运算符
    Stream >> Point1 >> Point2 >> Point3;
    //设置三角形的三个顶点
    Face1.SetPoints(Point1, Point2, Point3);
    return Stream;
}

//Getter成员函数

/*************************************************************************
【函数名称】        GetLength const
【函数功能】        求长度override
【参数】            无
【返回值】          double，三角形边长之和
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
double Face3D::GetLength() const{
    //三角形周长
    return (Points[0] - Points[1]).Length()
        + (Points[1] - Points[2]).Length()
        + (Points[0] - Points[2]).Length();
}

/*************************************************************************
【函数名称】        Area const
【函数功能】        求面积override
【参数】            无
【返回值】          double，面积值
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
double Face3D::GetArea() const{
    //三边长
    double a = (Points[0] - Points[1]).Length();
    double b = (Points[1] - Points[2]).Length();
    double c = (Points[0] - Points[2]).Length();
    //海伦公式求三角形面积
    double p = (a + b + c) * 0.5;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

/*************************************************************************
【函数名称】        GetPoints const
【函数功能】        获取三角形的三个顶点
【参数】            无
【返回值】          std::tuple<Point3D, Point3D, Point3D>，三个顶点
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
std::tuple<Point3D, Point3D, Point3D> Face3D::GetPoints() const{
    return std::make_tuple(Points[0], Points[1], Points[2]);
}

/*************************************************************************
【函数名称】        operator== const
【函数功能】        判断三角形是否与其他三角形相等
【参数】            const Face3D& Face1
【返回值】          bool，相等返回true，否则返回false
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
bool Face3D::operator==(const Face3D& Face1) const{
    //获取当前三角形的顶点
    const Point3D& A1 = Points[0];
    const Point3D& B1 = Points[1];
    const Point3D& C1 = Points[2];
    //获取另一个三角形的顶点
    const Point3D& A2 = Face1.Points[0];
    const Point3D& B2 = Face1.Points[1];
    const Point3D& C2 = Face1.Points[2];
    //检查所有可能的顶点排列组合
    return ((A1 == A2 && B1 == B2 && C1 == C2)
        || (A1 == A2 && B1 == C2 && C1 == B2)
        || (A1 == B2 && B1 == A2 && C1 == C2)
        || (A1 == B2 && B1 == C2 && C1 == A2)
        || (A1 == C2 && B1 == A2 && C1 == B2)
        || (A1 == C2 && B1 == B2 && C1 == A2));
}

/*************************************************************************
【函数名称】        operator!= const
【函数功能】        判断三角形是否与其他三角形不等
【参数】            const Face3D& Face1
【返回值】          bool，不等返回true，否则返回false
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
bool Face3D::operator!=(const Face3D& Face1) const{
    //调用==运算符
    return !(*this == Face1);
}

/*************************************************************************
【函数名称】        operator<<
【函数功能】        流插入运算符，将Face3D对象插入输出流中
【参数】            std::ostream& Stream, const Face3D& Face1
【返回值】          std::ostream&，插入后的流
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
std::ostream& operator<<(std::ostream& Stream, const Face3D& Face1){
    //输出三角形的三个顶点，中间用逗号隔开，外围用[]括起
    Stream << "Face3D: [" 
        << Face1.Points[0] << ", "
        << Face1.Points[1] << ", "
        << Face1.Points[2] << "]";
    return Stream;
}

//虚函数默认实现，用于派生类重写

/*************************************************************************
【函数名称】        SetColor
【函数功能】        设置三角形颜色，派生类可重写
                   基类中调用此函数将抛出异常
【参数】            const std::string& Color1
【返回值】          无
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
void Face3D::SetColor(const std::string& Color1){
    // 抛出异常，表示此操作在基类中无效，只能对派生的现实三角形类进行操作
    throw std::logic_error(
        "SetColor is not applicable to Face3D base");
}

/*************************************************************************
【函数名称】        SetThickness
【函数功能】        设置三角形厚度，派生类可重写
                    基类中调用此函数将抛出异常
【参数】            double Thickness1
【返回值】          无
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
void Face3D::SetThickness(double Thickness1){
    // 抛出异常，表示此操作在基类中无效，只能对派生的现实三角形类进行操作
    throw std::logic_error(
        "SetThickness is not applicable to Face3D base");
}

/*************************************************************************
【函数名称】        SetMaterial
【函数功能】        设置三角形材质，派生类可重写
                    基类中调用此函数将抛出异常
【参数】            const std::string& Material1
【返回值】          无
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
void Face3D::SetMaterial(const std::string& Material1){
    // 抛出异常，表示此操作在基类中无效，只能对派生的现实三角形类进行操作
    throw std::logic_error(
        "SetMaterial is not applicable to Face3D base");
}

/*************************************************************************
【函数名称】        GetColor
【函数功能】        获取三角形颜色，默认返回"White"
【参数】            无
【返回值】          std::string，默认颜色"White"
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
std::string Face3D::GetColor() const{
    // 非现实三角形类，默认颜色白色
    return "White";
}

/*************************************************************************
【函数名称】        GetThickness
【函数功能】        获取三角形厚度，默认返回0.0
【参数】            无
【返回值】          double，默认厚度0.0
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
double Face3D::GetThickness() const{
    // 非现实三角形类，默认厚度0.0
    return 0.0;
}

/*************************************************************************
【函数名称】        GetMaterial
【函数功能】        获取三角形材质，默认返回"None"
【参数】            无
【返回值】          std::string，默认材质"None"
【开发者及日期】    梁思奇 2024/8/2
【更改记录】         
*************************************************************************/
std::string Face3D::GetMaterial() const{
    // 非现实三角形类，默认材质为None
    return "None";
}
