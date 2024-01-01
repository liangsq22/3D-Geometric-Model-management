/*************************************************************************
【文件名】                 Model3D.hpp
【功能模块和目的】          三维模型类声明
【开发者及日期】            梁思奇 2024/8/5
【更改记录】               梁思奇 2024/8/8 调整bug并增加对点线面的操作
                          梁思奇 2024/8/9 增加模型元素批量合并与移除接口
*************************************************************************/

#ifndef MODEL3D_HPP
#define MODEL3D_HPP

//Face3D类所属头文件
#include "Face3D.hpp"
//Line3D类所属头文件
#include "Line3D.hpp"
//Point3D类所属头文件
#include "Point3D.hpp"
//std::vector类所属头文件
#include <vector>
//size_t所属头文件
#include <cstddef>
//std::shared_ptr所属头文件
#include <memory>
//std::string所属头文件
#include <string>

/*************************************************************************
【类名】             Model3D
【功能】             三维模型类，包含点、线、面
【接口说明】         包括构造、析构、赋值操作，模型合并与移除，点、线、面
                    的增删改操作，并提供相关的获取接口
【开发者及日期】      梁思奇 2024/8/5
【更改记录】         梁思奇 2024/8/8 调整bug并增加对点线面的操作
                    梁思奇 2024/8/9 增加模型元素批量合并与移除接口
*************************************************************************/
class Model3D{
public:
    //内嵌异常类:不能在基类对单独的点进行操作
    class NO_POINT_OPERATE : std::logic_error{
    public:
        NO_POINT_OPERATE();
    };

    //默认构造函数
    Model3D();
    //拷贝构造函数
    Model3D(const Model3D& Source);
    //带参构造函数
    Model3D(const std::vector<Face3D>& vFaces, 
        const std::vector<Line3D>& vLines);
    //虚析构函数（可能做基类）
    virtual ~Model3D() = default;
    //赋值运算符
    Model3D& operator=(const Model3D& Source);
    
    //Face3D增删改操作（除Getter标注外全Setter）

    //添加一个Face3D
    bool AddFace(const Face3D& Face1);
    bool AddFace(const Point3D& Point1, 
        const Point3D& Point2, const Point3D& Point3);
    //运算符重载：模型合并Face3D
    Model3D& operator+=(const std::vector<Face3D>& vFaces);
    //Getter
    Model3D operator+(const std::vector<Face3D>& vFaces) const;

    //修改一个Face3D
    bool ChangeFace(const Face3D& OldFace, const Face3D& NewFace);
    bool ChangeFace(const Face3D& Face1, 
        const Point3D& OldFace, const Point3D& NewFace);
    bool ChangeFace(const Face3D& Face1, 
        size_t Index, const Point3D& NewFace);

    //删除一个Face3D
    bool DeleteFace(const Face3D& Face1);
    bool DeleteFace(const Point3D& Point1, 
        const Point3D& Point2, const Point3D& Point3);
    //运算符重载：模型移除Face3D
    Model3D& operator-=(const std::vector<Face3D>& vFaces);
    //Getter
    Model3D operator-(const std::vector<Face3D>& vFaces) const;
    //删除所有 Face3D
    void ClearFaces();

    //Line3D增删改操作（除Getter标注外全Setter）

    //添加一个Line3D
    bool AddLine(const Line3D& Line1);
    bool AddLine(const Point3D& Point1, const Point3D& Point2);
    //运算符重载：模型合并Line3D
    Model3D& operator+=(const std::vector<Line3D>& vLines);
    //Getter
    Model3D operator+(const std::vector<Line3D>& vLines) const;

    //修改一个Line3D
    bool ChangeLine(const Line3D& OldLine, const Line3D& NewLine);
    bool ChangeLine(const Line3D& Line1, 
        const Point3D& OldLine, const Point3D& NewLine);
    bool ChangeLine(const Line3D& Line1, 
        size_t Index, const Point3D& NewFace);

    //删除一个Line3D
    bool DeleteLine(const Line3D& Line1);
    bool DeleteLine(const Point3D& Point1, const Point3D& Point2);
    //运算符重载：模型移除Line3D
    Model3D& operator-=(const std::vector<Line3D>& vLines);
    //Getter
    Model3D operator-(const std::vector<Line3D>& vLines) const;
    //删除所有 Line3D
    void ClearLines();
    
    //Point3D增删改操作，Model3D基类不用（考虑派生更复杂的模型类）

    //添加一个Point3D（虚拟函数，派生类可重写）
    virtual bool AddPoint(const Point3D& Point1);
    //运算符重载：模型合并Point3D（虚拟函数，派生类可重写）
    virtual Model3D& operator+=(const std::vector<Point3D>& vPoints);
    //Getter
    virtual Model3D operator+(const std::vector<Point3D>& vPoints) const;

    //修改一个Point3D（虚拟函数，派生类可重写）
    virtual bool ChangePoint(const Point3D& OldPoint, 
        const Point3D& NewPoint);

    //删除一个Point3D（虚拟函数，派生类可重写）
    virtual bool DeletePoint(const Point3D& Point1);
    //运算符重载：模型移除Point3D（虚拟函数，派生类可重写）
    virtual Model3D& operator-=(const std::vector<Point3D>& vPoints);
    //Getter
    virtual Model3D operator-(const std::vector<Point3D>& vPoints) const;
    //删除所有 Point3D（虚拟函数，派生类可重写）
    virtual void ClearPoints();   

    //模型操作

    //删除所有 Face3D、Line3D 和 Point3D
    void ClearAll();
    //模型合并运算符重载
    Model3D& operator+=(const Model3D& Model1);
    Model3D operator+(const Model3D& Model1) const;
    //模型移除运算符重载
    Model3D& operator-=(const Model3D& Model1);
    Model3D operator-(const Model3D& Model1) const;
    
    //数据成员Getter
    //模型名
    std::string& Name{m_sName};
    //模型注释
    std::vector<std::string>& Notes{m_Notes};
    //Face3D对象指针列表
    const std::vector<std::shared_ptr<Face3D>>& Faces{m_Faces};
    //Line3D对象指针列表
    const std::vector<std::shared_ptr<Line3D>>& Lines{m_Lines};
    //Point3D对象指针列表（Model3D基类不需要，为派生类考虑）
    const std::vector<std::shared_ptr<Point3D>>& Points{m_Points};
    //Face3D总数
    const size_t& FaceNum{m_ullFaceNum};
    //Line3D总数
    const size_t& LineNum{m_ullLineNum};
    //Point3D总数（重复计算）
    const size_t& PointNum{m_ullPointNum};
    //Point3D总数（不重复计算）
    const size_t& PointTureNum{m_ullPointTureNum};
    //Element3D总数（Face3D + Line3D）
    const size_t& ElementNum{m_ullElementNum};
    //Face3D总面积
    const double& FaceArea_Sum{m_rFaceArea_Sum};
    //Line3D总长度
    const double& LineLength_Sum{m_rLineLength_Sum};
    //最小包围长方体X边长度
    const double& EncaseCuboid_Length{m_rEncaseCuboid_Length};
    //最小包围长方体Y边长度
    const double& EncaseCuboid_Width{m_rEncaseCuboid_Width};
    //最小包围长方体Z边长度
    const double& EncaseCuboid_height{m_rEncaseCuboid_Height};
    //最小包围长方体面积
    const double& EncaseCuboid_Area{m_rEncaseCuboid_Area};
    //最小包围长方体体积
    const double& EncaseCuboid_Volume{m_rEncaseCuboid_Volume};
    
private:
    //私有成员函数
    //指定Face3D迭代器搜索
    std::vector<std::shared_ptr<Face3D>>::iterator FaceSearcher(
        const Face3D& Face1);
    //指定Line3D迭代器搜索
    std::vector<std::shared_ptr<Line3D>>::iterator LineSearcher(
        const Line3D& Line1);
    //指定Point3D迭代器搜索
    std::vector<std::shared_ptr<Point3D>>::iterator PointSearcher(
        const Point3D& Point1);
    //计算更新最小包围长方体
    void CalcEncaseCuboid();

    //私有数据成员
    //模型名，默认为"NONE"
    std::string m_sName{std::string{"NONE"}};
    //模型注释
    std::vector<std::string> m_Notes;
    //Face3D对象指针列表
    std::vector<std::shared_ptr<Face3D>> m_Faces{};
    //Line3D对象指针列表
    std::vector<std::shared_ptr<Line3D>> m_Lines{};
    //Point3D对象指针列表
    std::vector<std::shared_ptr<Point3D>> m_Points{};
    //Face3D总数
    size_t m_ullFaceNum{0};
    //Line3D总数
    size_t m_ullLineNum{0};
    //Point3D总数（重复计算）
    size_t m_ullPointNum{0};
    //Point3D总数（不重复计算）
    size_t m_ullPointTureNum{0};
    //Element3D总数（Face3D + Line3D）
    size_t m_ullElementNum{0};
    //Face3D总面积
    double m_rFaceArea_Sum{0};
    //Line3D总长度
    double m_rLineLength_Sum{0};
    //最小包围长方体X边长度
    double m_rEncaseCuboid_Length{0};
    //最小包围长方体Y边长度
    double m_rEncaseCuboid_Width{0};
    //最小包围长方体Z边长度
    double m_rEncaseCuboid_Height{0};
    //最小包围长方体面积
    double m_rEncaseCuboid_Area{0};
    //最小包围长方体体积
    double m_rEncaseCuboid_Volume{0};
};

#endif /* MODEL3D_HPP */
