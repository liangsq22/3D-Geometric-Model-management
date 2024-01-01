/*************************************************************************
【文件名】                 FixedElements3D.hpp
【功能模块和目的】         特定数量三维点元素集合类声明
【开发者及日期】           梁思奇 2024/7/31
【更改记录】               梁思奇 2024/8/6 完善类架构
*************************************************************************/

#ifndef FIXEDELEMENTS3D_HPP
#define FIXEDELEMENTS3D_HPP

//size_t所属头文件
#include <cstddef>
//std::invalid_argument、std::logic_error所属头文件
#include <stdexcept>
//std::vector所属头文件
#include <vector>
//Point3D类所属头文件
#include "Point3D.hpp"
//Point3DGroup类所属头文件
#include "Group_Point3D.hpp"

/*************************************************************************
【类名】             FixedElements3D
【功能】             特定数量三维点元素集合类
【接口说明】         特定元素数量三维点集合类，包含构造、析构、赋值行为，
                    对元素的增删改清操作，以及获取元素数量、判断是否相等
                    等必要函数
【开发者及日期】      梁思奇 2024/7/31
【更改记录】         梁思奇 2024/8/6 完善类架构
*************************************************************************/
class FixedElements3D{
public:
    //内嵌异常类组
    //异常：加入重复元素
    class REPEAT_ELEMENT : std::invalid_argument{
    public:
        REPEAT_ELEMENT();
    };

    //异常：元素数量不匹配
    class NO_MATCH_COUNT : std::invalid_argument{
    public:
        NO_MATCH_COUNT();
    };

    //异常：不能修改元素数量
    class FIXED_POINTS_COUNT : std::logic_error{
    public:
        FIXED_POINTS_COUNT();
    };

public:
    //默认构造函数
    FixedElements3D(); 
    //带参构造函数
    FixedElements3D(const std::vector<Point3D>& Source, 
        size_t MaxPointsNum);
    //拷贝构造函数
    FixedElements3D(const FixedElements3D& Source);
    //虚析构函数
    virtual ~FixedElements3D() = default;
    //赋值运算符
    FixedElements3D& operator=(const FixedElements3D& Source);

    //Setter
    //修改点
    virtual void ChangePoint(const Point3D& OldElement, 
        const Point3D& NewElement);
    
    //Getter
    //判断三维点元素是否相等
    bool operator==(const FixedElements3D& Element1) const;
    //判断三维点元素是否不等
    bool operator!=(const FixedElements3D& Element1) const;
    //求面积，纯虚函数
    virtual double GetArea() const = 0;
    //求长度，纯虚函数
    virtual double GetLength() const = 0;
    //流插入输出运算符
    friend std::ostream& operator<<(std::ostream& Stream,
        const FixedElements3D& Element1);

    //Getter数据成员
    //获取三维点元素
    const Point3DGroup& Points{m_Points};
    //固定三维点数量
    const size_t PointNum{m_Points.GetCountSize()};

protected:
    //可能派生的Setter（本类不需要）
    //添加点
    virtual void AddPoint(const Point3D& Point1);
    //删除点
    virtual void RemovePoint(const Point3D& Point1);
    //清空点
    virtual void ClearAllPoints();
    //流提取输入
    virtual void Read(std::istream& Stream);

private:
    //私有数据成员：三维点实际储存
    Point3DGroup m_Points;
    //私有数据成员：三维点最大数量
    size_t m_MaxPointsNum;
};

#endif //FIXEDELEMENTS3D_HPP
