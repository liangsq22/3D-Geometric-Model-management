/*************************************************************************
【文件名】                  Exporter3D_obj.cpp
【功能模块和目的】          导出Model3D到obj文件的导出器类实现
【开发者及日期】            梁思奇 2024/8/7
【更改记录】               
*************************************************************************/

//自身类头文件
#include "Exporter3D_obj.hpp"
//Exporter3D特化类所属头文件
#include "Exporter3D.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"
//size_t所属头文件
#include <cstddef>
//Point3D类所属头文件
#include "Point3D.hpp"
//Line3D类所属头文件
#include "Line3D.hpp"
//Face3D类所属头文件
#include "Face3D.hpp"
//std::ofstream所属头文件
#include <fstream>
//Point3DGroup类所属头文件
#include "Group_Point3D.hpp"
//std::vector、size()所属头文件
#include <vector>

/*************************************************************************
【函数名称】          Exporter3D_obj
【函数功能】          默认构造函数，调用基类带参构造函数，文件扩展名为"obj"
【参数】              无
【返回值】            无
【开发者及日期】      梁思奇 2024/8/7
【更改记录】         
*************************************************************************/
Exporter3D_obj::Exporter3D_obj() : Exporter3D(std::string{"obj"}){
}

/*************************************************************************
【函数名称】          ExportToStream
【函数功能】          将Model3D类型对象导出到文件流的实现函数
【参数】              std::ofstream& Stream：输出文件流
                     const Model3D& Model1：要导出的Model3D对象
【返回值】            无
【开发者及日期】      梁思奇 2024/8/7
【更改记录】         
*************************************************************************/
void Exporter3D_obj::ExportToStream(
    std::ofstream& Stream, const Model3D& Model1) const{
    //注释
    for (size_t i = 0; i < Model1.Notes.size(); i++) {
        Stream << "# " << Model1.Notes[i] << std::endl;
    }
    //模型名
    Stream << "g " << Model1.Name << std::endl;

    //导出需要去除重复点
    Point3DGroup NoRepeatPoints;
    //将所有Face3D中的点添加到非重复点集
    for (auto TempFace : Model1.Faces) {
        for (size_t i = 0; i < 3; i++) {
            try {
                NoRepeatPoints.Add(TempFace->Points[i]);
            } catch (const Point3DGroup::REPEAT_ELEMENT& e) {
                //发现重复点，跳过
            }
        }
    }
    //将所有Line3D中的点添加到非重复点集
    for (auto TempLine : Model1.Lines) {
        for (size_t i = 0; i < 2; i++) {
            try {
                NoRepeatPoints.Add(TempLine->Points[i]);
            } catch (const Point3DGroup::REPEAT_ELEMENT& e) {
                //发现重复点，跳过
            }
        }
    }
    //导出点的容器
    std::vector<Point3D> ExportPoints;
    //导出点（一行3个double类型坐标值）
    for (size_t i = 0; i < NoRepeatPoints.GetCountSize(); i++) {
        Stream << "v " << NoRepeatPoints[i] << std::endl;
        ExportPoints.push_back(NoRepeatPoints[i]);
    }
    //导出线（一行2个索引值）
    for (auto TempLine : Model1.Lines) {
        Stream << "l ";
        for (size_t i = 0; i < 2; i++) {
            auto ItLine = std::find(
                ExportPoints.begin(),
                ExportPoints.end(),
                TempLine->Points[i]);
            //空格分隔
            Stream << ItLine - ExportPoints.begin() + 1 << ' ';
        }
        //换行
        Stream << std::endl;
    }
    //导出面（一行3个索引值）
    for (auto TempFace : Model1.Faces) {
        Stream << "f ";
        for (size_t i = 0; i < 3; i++) {
            auto ItFace = std::find(
                ExportPoints.begin(),
                ExportPoints.end(),
                TempFace->Points[i]);
            //空格分隔
            Stream << ItFace - ExportPoints.begin() + 1 << ' ';
        }
        //换行
        Stream << std::endl;
    }
}
