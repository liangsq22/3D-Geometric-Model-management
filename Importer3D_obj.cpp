/*************************************************************************
【文件名】                 Importer3D_obj.cpp
【功能模块和目的】          导入obj文件到Model3D的导入器类实现
【开发者及日期】            梁思奇 2024/8/7
【更改记录】               
*************************************************************************/

//自身类头文件
#include "Importer3D_obj.hpp"
//Importer3D特化类所属头文件
#include "Importer3D.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"
//Point3D类所属头文件
#include "Point3D.hpp"
//Line3D类所属头文件
#include "Line3D.hpp"
//Face3D类所属头文件
#include "Face3D.hpp"
//std::ifstream所属头文件
#include <fstream>
//std::vector、push_back所属头文件
#include <vector>
//size_t所属头文件
#include <cstddef>

/*************************************************************************
【函数名称】          Importer3D_obj
【函数功能】          默认构造函数，调用基类带参构造函数，文件扩展名为"obj"
【参数】              无
【返回值】            无
【开发者及日期】      梁思奇 2024/8/7
【更改记录】         
*************************************************************************/
Importer3D_obj::Importer3D_obj() : Importer3D(std::string("obj")){
}

/*************************************************************************
【函数名称】          ImportFromStream
【函数功能】          从文件流导入到Model3D对象的实现函数
【参数】              std::ifstream& Stream：输入文件流
【返回值】            返回Model3D对象，包含从obj文件读取的数据
【开发者及日期】      梁思奇 2024/8/7
【更改记录】         
*************************************************************************/
Model3D Importer3D_obj::ImportFromStream(std::ifstream& Stream) const{
    //储存Model3D
    Model3D StoreModel;
    //定义点、线、面队列
    std::vector<Point3D> vPoints;
    std::vector<Line3D> vLines;
    std::vector<Face3D> vFaces;
    //存储文件中每行的标头，以便读取特定数据
    char Label;
    //存储注释内容
    std::string Notes;
    //存储点的三维坐标
    double Point_X{0};
    double Point_Y{0};
    double Point_Z{0};
    //存储点的索引值
    size_t Index1{0};
    size_t Index2{0};
    size_t Index3{0};
    //读取文件流中的数据，直到文件流结束
    while (!Stream.eof()) {
        //标头初始为N（None）
        Label = 'N';
        //每一轮读一行
        //读取标头
        Stream >> Label;
        //吃掉空格
        Stream.get();
        //根据标头读取数据
        switch (Label) {
            case '#' : //注释
                //读取注释内容
                getline(Stream, Notes);
                //加入Notes队列
                StoreModel.Notes.push_back(Notes);
                break;
            case 'g' : //模型名称
                //读取模型名称
                getline(Stream, StoreModel.Name);
                break;
            case 'v' : //点数据
                //读取点的三维坐标
                Stream >> Point_X >> Point_Y >> Point_Z;
                //吃掉换行符
                Stream.get();
                //加入点队列
                vPoints.push_back(Point3D(Point_X, Point_Y, Point_Z));
                break;
            case 'l' : //线数据
                //读取两点索引
                Stream >> Index1 >> Index2;
                //吃掉换行符
                Stream.get();
                //通过找点创建Line3D对象加入线队列，注意索引减1
                vLines.push_back(
                    Line3D(vPoints[Index1 - 1], vPoints[Index2 - 1]));
                break;
            case 'f' : //面数据
                //读取三点索引
                Stream >> Index1 >> Index2 >> Index3;
                //吃掉换行符
                Stream.get();
                //通过找点创建Face3D对象加入面队列，注意索引减1
                vFaces.push_back(Face3D(
                    vPoints[Index1 - 1],
                    vPoints[Index2 - 1],
                    vPoints[Index3 - 1]));
                break;
            default :
                break;
        }
    }
    //将读取的所有Face3D对象逐个添加到临时模型中
    for (const auto& TempFace : vFaces) {
        StoreModel.AddFace(TempFace);
    }
    //将读取的所有Line3D对象逐个添加到临时模型中
    for (const auto& TempLine : vLines) {
        StoreModel.AddLine(TempLine);
    }
    return StoreModel;
}
