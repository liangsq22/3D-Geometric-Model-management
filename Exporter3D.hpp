/*************************************************************************
【文件名】                 Exporter3D.hpp
【功能模块和目的】         三维模型Model3D导出器特化类
【开发者及日期】           梁思奇 2024/8/7
【更改记录】              
*************************************************************************/

#ifndef EXPORTER3D_HPP
#define EXPORTER3D_HPP

//ExporterBase基类模版所属头文件
#include "ExporterBase.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"

//三维模型Model3D导出器特化类
using Exporter3D = ExporterBase<Model3D>;

#endif //EXPORTER3D_HPP
