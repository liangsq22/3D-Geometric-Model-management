/*************************************************************************
【文件名】                 Importer3D.hpp
【功能模块和目的】         三维模型Model3D导入器特化类
【开发者及日期】           梁思奇 2024/8/7
【更改记录】              
*************************************************************************/

#ifndef IMPORTER3D_HPP
#define IMPORTER3D_HPP

//ImporterBase基类模版所属头文件
#include "ImporterBase.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"

//三维模型Model3D导入器特化类
using Importer3D = ImporterBase<Model3D>;

#endif //IMPORTER3D_HPP
