/*************************************************************************
【文件名】                 Importer3D_obj.hpp
【功能模块和目的】          导入obj文件到Model3D的导入器类声明
【开发者及日期】            梁思奇 2024/8/7
【更改记录】               
*************************************************************************/

#ifndef IMPORTER3D_OBJ_HPP
#define IMPORTER3D_OBJ_HPP

//Importer3D特化类所属头文件
#include "Importer3D.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"
//std::ifstream所属头文件
#include <fstream>

/*************************************************************************
【类名】             Importer3D_obj
【功能】             导入".obj"文件到"Model3D"的导入器类，
【接口说明】         继承自"ImporterBase"，拥有基类的所有接口，
                    提供默认构造函数，指定文件扩展名为 "obj"，
                    实现从文件流导入到"Model3D"对象的接口
【开发者及日期】     梁思奇 2024/8/7
【更改记录】         
*************************************************************************/
class Importer3D_obj : public Importer3D{
public:
    //默认构造函数，文件扩展名为"obj"
    Importer3D_obj();
    //无拷贝构造
    Importer3D_obj(const Importer3D_obj&) = delete;
    //虚析构函数
    ~Importer3D_obj() = default;
    //无赋值运算符
    Importer3D_obj& operator=(const Importer3D_obj&) = delete;
    //从文件流导入到Model3D对象接口override
    Model3D ImportFromStream(std::ifstream& Stream) const override;
};

#endif //IMPORTER3D_OBJ_HPP
