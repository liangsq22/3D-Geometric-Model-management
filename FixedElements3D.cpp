/*************************************************************************
【文件名】                 FixedElements3D.cpp
【功能模块和目的】          特定数量三维点元素集合类的实现
【开发者及日期】            梁思奇 2024/7/31
【更改记录】               梁思奇 2024/8/6 完善类架构
*************************************************************************/

//自身类头文件
#include "FixedElements3D.hpp"
//std::invalid_argument、std::logic_error所属头文件
#include <stdexcept>
//std::string所属头文件
#include <string>
//std::ostream、std::istream所属头文件
#include <iostream>

//内嵌异常类实现

/*************************************************************************
【函数名称】        REPEAT_ELEMENT
【函数功能】        内嵌异常类构造函数，抛出重复元素异常
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
FixedElements3D::REPEAT_ELEMENT::REPEAT_ELEMENT() : std::invalid_argument(
    std::string("Element already exists in the group")){
}

/*************************************************************************
【函数名称】        NO_MATCH_COUNT
【函数功能】        内嵌异常类构造函数，抛出元素数量不匹配异常
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
FixedElements3D::NO_MATCH_COUNT::NO_MATCH_COUNT() : std::invalid_argument(
    std::string("Point count mismatch with the specified max count")){
}

/*************************************************************************
【函数名称】        FIXED_POINTS_COUNT
【函数功能】        内嵌异常类构造函数，抛出不能修改元素数量异常
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
//注：注释块并没有80字符（只有74），这里并没有超出80字符
FixedElements3D::FIXED_POINTS_COUNT::FIXED_POINTS_COUNT() : std::logic_error(
    std::string("Point count is fixed and cannot be changed")){
}

//构造、赋值函数实现

/*************************************************************************
【函数名称】        FixedElements3D
【函数功能】        默认构造函数，初始化对象
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
FixedElements3D::FixedElements3D() : m_Points(0), m_MaxPointsNum(0){
}


/*************************************************************************
【函数名称】        FixedElements3D
【函数功能】        带参构造函数，用给定的点集合和最大点数初始化对象
【参数】            const std::vector<Point3D>& Source：初始化点集合
                   size_t MaxPointsNum：最大点数
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
FixedElements3D::FixedElements3D(
    const std::vector<Point3D>& Source, size_t MaxPointsNum)
    : m_Points(MaxPointsNum), m_MaxPointsNum(MaxPointsNum){
    //Source包含点数与MaxPointsNum不符，则抛出异常
    if (Source.size() != MaxPointsNum) {
        throw NO_MATCH_COUNT();
    }
    //添加所有点
    for (const auto& TempPoint : Source) {
        m_Points.Add(TempPoint);
    }
    //复制后包含不重复点数与MaxPointCount不符
    if (Points.GetCountSize() != MaxPointsNum) {
        throw NO_MATCH_COUNT();
    }
}

/*************************************************************************
【函数名称】        FixedElements3D
【函数功能】        拷贝构造函数，用另一个FixedElements3D对象初始化当前对象
【参数】            const FixedElements3D& Source：另一个FixedElements3D对象
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
FixedElements3D::FixedElements3D(const FixedElements3D& Source)
    : m_Points(Source.m_Points), m_MaxPointsNum(Source.m_MaxPointsNum){
}

/*************************************************************************
【函数名称】        operator=
【函数功能】        赋值运算符，将另一个FixedElements3D对象的内容赋值给当前对象
【参数】            const FixedElements3D& Source：另一个FixedElements3D对象
【返回值】          当前FixedElements3D对象的引用
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
FixedElements3D& FixedElements3D::operator=(const FixedElements3D& Source){
    //检查自赋值
    if (this != &Source) {
        m_Points = Source.m_Points;
        m_MaxPointsNum = Source.m_MaxPointsNum;
    }
    return *this;
}

//Setter函数实现

/*************************************************************************
【函数名称】        ChangePoint
【函数功能】        修改集合中的一个三维点
【参数】            const Point3D& OldElement：要修改的旧点
                   const Point3D& NewElement：新的三维点值
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
void FixedElements3D::ChangePoint(
    const Point3D& OldElement, const Point3D& NewElement){
    //旧点不存在，抛出异常
    if (m_Points.IsExist(NewElement)) {
        throw REPEAT_ELEMENT();
    }
    m_Points.Change(OldElement, NewElement);
}

//Getter函数实现

/*************************************************************************
【函数名称】        operator==
【函数功能】        判断两个FixedElements3D对象是否相等
【参数】            const FixedElements3D& Element1：另一个FixedElements3D对象
【返回值】          如果相等返回true，否则返回false
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
bool FixedElements3D::operator==(const FixedElements3D& Element1) const{
    return m_Points == Element1.m_Points;
}

/*************************************************************************
【函数名称】        operator!=
【函数功能】        判断两个FixedElements3D对象是否不相等
【参数】            const FixedElements3D& Element1：另一个FixedElements3D对象
【返回值】          如果不相等返回true，否则返回false
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
bool FixedElements3D::operator!=(const FixedElements3D& Element1) const{
    return !(*this == Element1);
}

/*************************************************************************
【函数名称】        operator<<
【函数功能】        流插入运算符，将FixedElements3D对象输出到流中
【参数】            std::ostream& Stream：输出流
                   const FixedElements3D& Element1：
                   要输出的FixedElements3D对象
【返回值】          输出流的引用
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
std::ostream& operator<<(
    std::ostream& Stream, const FixedElements3D& Element1){
    //输出所有点，每个点之间用空格隔开
    for (size_t i = 0; i < Element1.PointNum - 1; i++) {
        Stream << Element1.Points[i] << ' ';
    }
    Stream << Element1.Points[Element1.PointNum - 1];
    return Stream;
}

//Setter函数（待派生override）

/*************************************************************************
【函数名称】        AddPoint
【函数功能】        向集合中添加一个三维点
【参数】            const Point3D& Point1：要添加的三维点
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
void FixedElements3D::AddPoint(const Point3D& Point1){
    //本类不允许添加点，抛出异常
    throw FIXED_POINTS_COUNT();
}

/*************************************************************************
【函数名称】        RemovePoint
【函数功能】        从集合中删除一个三维点
【参数】            const Point3D& Point1：要删除的三维点
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
void FixedElements3D::RemovePoint(const Point3D& Point1){
    //本类不允许删除点，抛出异常
    throw FIXED_POINTS_COUNT();
}

/*************************************************************************
【函数名称】        ClearAllPoints
【函数功能】        清空集合中的所有三维点
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
void FixedElements3D::ClearAllPoints(){
    //本类不允许清空点，抛出异常
    throw FIXED_POINTS_COUNT();
}

/*************************************************************************
【函数名称】        Read
【函数功能】        从输入流中读取三维点集合
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/31
【更改记录】         
*************************************************************************/
void FixedElements3D::Read(std::istream& Stream){
    //本类不允许输入点，抛出异常
    throw FIXED_POINTS_COUNT();
}
