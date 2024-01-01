/*************************************************************************
【文件名】                 Controller.hpp
【功能模块和目的】          控制器类声明
【开发者及日期】            梁思奇 2024/8/8
【更改记录】               梁思奇 2024/8/10 改进功能函数实现方式
*************************************************************************/

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

//Model3D类所属头文件
#include "Model3D.hpp"
//Point3D类所属头文件
#include "Point3D.hpp"
//Face3D类所属头文件
#include "Face3D.hpp"
//Line3D类所属头文件
#include "Line3D.hpp"
//size_t所属头文件
#include <cstddef>
//std::vector所属头文件
#include <vector>
//std::shared_ptr所属头文件
#include <memory>
//std::string所属头文件
#include <string>

//定义常量NO_TAG_NUMBER，表示无标签号
const size_t NO_TAG_NUMBER = SIZE_MAX;

/*************************************************************************
【类名】             Controller
【功能】             控制器类，提供用户可调用的全部层级功能接口
【接口说明】         包括模型的创建、选择、删除、导入、导出操作；
                    面、线、点的增删改操作；
                    以及模型相关信息的获取接口
【开发者及日期】     梁思奇 2024/8/8
【更改记录】         梁思奇 2024/8/10 改进功能函数实现方式
*************************************************************************/
class Controller{
private:
    //私有默认构造函数，只能存在一个实例Controller
    Controller();
public:
    //无拷贝构造函数
    Controller(const Controller& Source) = delete;
    //虚析构函数,考虑派生出高级控制器
    virtual ~Controller() = default;
    //无赋值运算符
    Controller& operator=(const Controller Source) = delete;

    //内嵌信息类：不同层次获取信息

    //点信息类
    class Info_Point3D{
    public:
        //X轴点坐标
        double X;
        //Y轴点坐标
        double Y;
        //Z轴点坐标
        double Z;
    };
    //点信息表列表
    using List_Point3DInfo = std::vector<Info_Point3D>;
    
    //线信息类
    class Info_Line3D{
    public:
        //线段两端点信息
        Info_Point3D PointsInfo[3];
        //线段长度
        double Length;
    };
    //线信息表列表
    using List_Line3DInfo = std::vector<Info_Line3D>;

    //面信息类
    class Info_Face3D{
    public:
        //面三个顶点信息
        Info_Point3D PointsInfo[3];
        //面积
        double Area;
    };
    //面信息表列表
    using List_Face3DInfo = std::vector<Info_Face3D>;
    
    //模型信息类
    class Info_Model3D{
    public:
        //模型名
        std::string Name;
        //重复点数
        size_t PointNumber;
        //线段数
        size_t LineNumber;
        //面数
        size_t FaceNumber;
        //线段总长度
        double LengthSum;
        //面积总和
        double AreaSum;
        //最小包围长方体体积
        double Volume;
    };
    //模型信息表列表
    using List_Model3DInfo = std::vector<Info_Model3D>;

    //Controller类返回值枚举（成功或错误类型）
    enum class RES : size_t{
        SUCCESS             = 0,
        UNKNOWN_ERROR       = 1,
        REPEAT_ELEMENT      = 2,
        REPEAT_POINT        = 3,
        TAG_OUT_OF_RANGE    = 4,
        FAIL_TO_IMPORT      = 5,
        FAIL_TO_EXPORT      = 6
    };
    //静态常量字符串数组数据成员：RES枚举类名称
    static const std::string RESNAME[];
    
    //Setter

    //导入文件到模型
    RES ImportModel(const std::string& FileName, size_t ModelTag);
    //导出模型到文件
    RES ExportModel(const std::string& FileName, size_t ModelTag);
    //创建设置空模型
    RES SetEmptyModel();
    //选择模型作为操作对象
    RES ChooseModel(size_t ModelTag);
    //删除指定模型
    RES DeleteModel(size_t ModelTag);

    //加入元素
    //向当前模型添加指定线
    RES ModelAddLine(const Line3D& Line1);
    //向当前模型添加指定面
    RES ModelAddFace(const Face3D& Face1);

    //修改元素    
    //修改当前模型指定线中的指定点
    RES ModelChangeLinePoint(
        size_t LineTag,
        size_t PointTag,
        const Point3D& Point1);
    //修改当前模型指定面中的指定点
    RES ModelChangeFacePoint(
        size_t FaceTag,
        size_t PointTag,
        const Point3D& Point1);

    //删除元素
    //删除当前模型中的指定线
    RES ModelDeleteLine(size_t LineTag);
    //删除当前模型中的指定面
    RES ModelDeleteFace(size_t FaceTag);

    //Getter

    //静态函数：获取控制器实例指针
    static std::shared_ptr<Controller> GetControllerPtr();
    //列出所有模型信息
    RES ShowAllModels(List_Model3DInfo& InfoList);
    //列出指定模型信息
    RES ShowModle(size_t ModelTag, Info_Model3D& Info);
    //列出当前模型信息
    RES ShowThisModel(Info_Model3D& Info);
    //列出当前模型中所有面信息
    RES ShowModelAllFaces(List_Face3DInfo& InfoList);
    //列出当前模型中所有线信息
    RES ShowModelAllLines(List_Line3DInfo& InfoList);
    //列出当前模型指定面中所有点信息
    RES ShowModelFaceAllPoints(
        size_t FaceTag,
        List_Point3DInfo& InfoList);
    //列出当前模型指定线中所有点信息
    RES ShowModelLineAllPoints(
        size_t LineTag,
        List_Point3DInfo& InfoList);
    //非静态常引用数据成员：当前模型标签
    const size_t& ChosenModelTag{m_ChosenModelTag};
    
private:
    //所有Model3D模型实例指针列表
    std::vector<std::shared_ptr<Model3D>> m_Models{};
    //私有数据成员：当前模型标签，初始化为size_t最大值表示暂无标签
    size_t m_ChosenModelTag{NO_TAG_NUMBER};
    //静态私有数据成员：控制器实例指针
    static std::shared_ptr<Controller> m_pControllerIntance;
};

#endif //CONTROLLER_HPP
