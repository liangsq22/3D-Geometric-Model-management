/*************************************************************************
【文件名】                  Exporter3D_obj.hpp
【功能模块和目的】          导出Model3D到obj文件的导出器类声明
【开发者及日期】            梁思奇 2024/8/7
【更改记录】               
*************************************************************************/

#ifndef EXPORTER3D_OBJ_HPP
#define EXPORTER3D_OBJ_HPP

//Exporter3D特化类所属头文件
#include "Exporter3D.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"
//std::ofstream所属头文件
#include <fstream>

/*************************************************************************
【类名】             Exporter3D_obj
【功能】             导出"Model3D"到".obj"文件的导出器类，
【接口说明】         继承自"Exporter3D"，拥有基类的所有接口，
                    提供默认构造函数，指定文件扩展名为 "obj"，
                    实现将"Model3D"对象去除重复点导出非重复点的导出接口
【开发者及日期】     梁思奇 2024/8/7
【更改记录】         
*************************************************************************/
class Exporter3D_obj : public Exporter3D{
public:
    //默认构造函数，文件扩展名为"obj"
    Exporter3D_obj();
    //无拷贝构造
    Exporter3D_obj(const Exporter3D_obj&) = delete;
    //虚析构函数
    ~Exporter3D_obj() = default;
    //无赋值运算符
    Exporter3D_obj& operator=(const Exporter3D_obj&) = delete;
    //将Modle3D类型对象导出到文件流的接口override
    void ExportToStream(
        std::ofstream& Stream, const Model3D& Model1) const override;
};

#endif //EXPORTER3D_OBJ_HPP
