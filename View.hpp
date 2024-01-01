/*************************************************************************
【文件名】                  View.hpp
【功能模块和目的】          界面类声明
【开发者及日期】            梁思奇 2024/8/11
【更改记录】               梁思奇 2024/8/13 改进功能函数实现方式
*************************************************************************/

#ifndef VIEW_HPP
#define VIEW_HPP

//std::string所属头文件
#include <string>

/*************************************************************************
【类名】             View
【功能】             界面类，显示菜单、与用户交换数据、调用Controller功能
【接口说明】         各类菜单、列出模型、选择模型、创建模型、导入导出模型，
                    增删改各类元素等控制器功能的显示窗口
【开发者及日期】     梁思奇 2024/8/11
【更改记录】         梁思奇 2024/8/13 改进功能函数实现方式
*************************************************************************/
class View{
public:
    //默认构造函数
    View();
    //虚析构函数，考虑派生出高级界面类
    virtual ~View() = default;

    //显示菜单功能函数Getter

    //显示主菜单
    std::string MainMenu() const;

    //主菜单次级功能菜单

    //显示创建模型菜单
    std::string CreateNewModelMenu() const;
    //显示导入模型菜单
    std::string ImportModelMenu() const;
    //显示导出模型菜单
    std::string ExportModelMenu() const;
    //显示列出所有模型菜单
    std::string ShowAllModelsMenu() const;
    //显示选择模型菜单
    std::string ChooseModelMenu() const;
    //显示编辑模型菜单
    std::string ModifyModelMenu() const;

    //编辑模型菜单次级功能菜单

    //显示列出模型信息菜单
    std::string ListModelInfoMenu() const;
    //面的增删改
    //显示添加面菜单
    std::string AddFaceMenu() const;
    //显示修改面上点菜单
    std::string ChangeFacePointMenu() const;
    //显示删除面菜单
    std::string DeleteFaceMenu() const;
    //线的增删改
    //显示添加线菜单
    std::string AddLineMenu() const;
    //显示修改线上点菜单
    std::string ChangeLinePointMenu() const;
    //显示删除线菜单
    std::string DeleteLineMenu() const;
};

#endif //VIEW_HPP
