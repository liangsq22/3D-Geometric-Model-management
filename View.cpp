/*************************************************************************
【文件名】                  View.cpp
【功能模块和目的】          界面类实现
【开发者及日期】            梁思奇 2024/8/11
【更改记录】               梁思奇 2024/8/13 改进功能函数实现方式
*************************************************************************/

//自身类头文件
#include "View.hpp"
//std::iostream所属头文件
#include <iostream>
//std::string、std::stoi所属头文件
#include <string>
//std::shared_ptr所属头文件
#include <memory>
//控制器类所属头文件
#include "Controller.hpp"
//使用std命名空间
using namespace std;

//简化类型名称
using RES = Controller::RES;
using ModelInfo = Controller::Info_Model3D;
using ModelInfoList = Controller::List_Model3DInfo;
using PointInfo = Controller::Info_Point3D;
using PointInfoList = Controller::List_Point3DInfo;
using LineInfo = Controller::Info_Line3D;
using LineInfoList = Controller::List_Line3DInfo;
using FaceInfo = Controller::Info_Point3D;
using FaceInfoList = Controller::List_Face3DInfo;

/*************************************************************************
【函数名称】        View
【函数功能】        默认构造函数，调用显示主菜单
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/11
【更改记录】        
*************************************************************************/
View::View(){
    //直接进入主菜单界面
    cout << MainMenu() << endl;
}

/*************************************************************************
【函数名称】        MainMenu
【函数功能】        显示主菜单，处理用户输入并调用相应功能转到相应菜单
【参数】            无
【返回值】          返回字符串结语string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】        梁思奇 2024/8/13 改进输入处理逻辑
*************************************************************************/
string View::MainMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户所输入的选择
    string UserInput;
    //用户选择的操作数，初始化为0（无效值）
    int InputNumber = 0;
    //循环显示主菜单，直到用户选择退出
    while (InputNumber != 7) {
        cout << "-----Construct 3D World-----" << endl;
        cout << "Welcome to Liang Siqi's 3D World Builder" << endl;
        cout << "Please choose the operation number:" << endl;
        cout << "1. Create New Model" << endl;
        cout << "2. Import Model" << endl;
        cout << "3. Export Model" << endl;
        cout << "4. Show All models Info" << endl;
        cout << "5. Choose Model" << endl;
        cout << "6. Modify Chosen Model" << endl;
        cout << "7. Exit"   << endl << "(1/2/3/4/5/6/7):";
        //读取用户输入并转换为整数
        getline(cin, UserInput);
        try {
            InputNumber = stoi(UserInput);
        } catch (...) {
            //对于无效输入，赋值为无效值0，重新显示主菜单
            InputNumber = 0; 
        }
        //根据用户输入，调用相应的功能函数
        switch (InputNumber) {
            case 1 :
                //创建新的空模型
                cout << CreateNewModelMenu() << endl;
                break;
            case 2 :
                //导入模型
                cout << ImportModelMenu() << endl;
                break;
            case 3 :
                //导出模型
                cout << ExportModelMenu() << endl;
                break;
            case 4 :
                //列出所有模型信息
                cout << ShowAllModelsMenu() << endl;
                break;
            case 5 :
                //选择要操作的模型
                cout << ChooseModelMenu() << endl;
                break;
            case 6 :
                //编辑当前模型
                cout << ModifyModelMenu() << endl;
                break;
            case 7 :
                //退出操作循环，结束程序
                cout << "Goodbye!" << endl;
                break; 
            default :
                //对于无效输入，给出提示，重新显示菜单
                cout << "Invalid input, please enter 1-7!" << endl;
                //重置输入数为无效值0
                InputNumber = 0;
                break;
        }
    }
    //返回结语
    return "Liang Siqi's 3D World Builder | 2024 Summer";
}

/*************************************************************************
【函数名称】        CreateNewModelMenu
【函数功能】        显示创建模型的菜单，用户可以创建一个新的空模型，
                   并将其设为当前操作对象；
                   用户可以选择返回主菜单或继续创建更多的空模型。
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::CreateNewModelMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示创建模型菜单
        cout << endl << "-----Create New Empty Model-----" << endl;
        //调用控制器功能创建空模型
        CtrlerPtr->SetEmptyModel();
        //显示创建成功信息
        cout << "1 new empty model was created successfully" << endl;
        //显示当前选择的模型
        cout << "Current model is Model[" 
            << CtrlerPtr->ChosenModelTag << "]" << endl;
        //清空输入
        UserInput.clear();
        //当前菜单循环显示，用户不跳转菜单时停留此界面
        while (UserInput != "Y" && UserInput != "N" 
            && UserInput != "y" && UserInput != "n") {
            cout << endl << "Go Back to main menu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            //提示用户是否继续创建新模型
            cout << "Enter N(No) to create another new model" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到主菜单
    return "";
}

/*************************************************************************
【函数名称】        ImportModelMenu
【函数功能】        显示导入模型菜单，如果没有模型则自动创建一个新模型
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】         
*************************************************************************/
string View::ImportModelMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //模型信息列表
    ModelInfoList InfoList;
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //调用控制器功能获取模型信息
        CtrlerPtr->ShowAllModels(InfoList);
        //显示导入模型菜单
        cout << endl << "-----Import Model From File-----" << endl;
        //如果没有模型可以导入
        if (InfoList.size() == 0) {
            //没有已存在的模型，创建一个新的空模型
            cout << "No model exists, creating a new model..." << endl;
            //调用控制器功能创建空模型
            CtrlerPtr->SetEmptyModel();
            //更新模型信息
            CtrlerPtr->ShowAllModels(InfoList);
            //显示新模型创建成功信息
            cout << "New model Model[" << CtrlerPtr->ChosenModelTag 
                 << "] has been created and set as current." << endl;
        }
        //提示用户输入文件名
        cout << "Please enter file name to import:";
        //用户输入文件名
        string FileName;
        //获取用户输入
        getline(cin, FileName);
        //显示当前选择的模型
        cout << "Current model is Model[" 
            << CtrlerPtr->ChosenModelTag << "]" << endl;
        //提示用户选择要导入的模型标签
        cout << "Choose the model tag to import" << endl;
        cout << "Model tag[0~" << (InfoList.size() - 1) << "]:";
        size_t Tag;
        cin >> Tag;
        cin.get();
        RES Temp = CtrlerPtr->ImportModel(FileName, Tag);
        //如果导入失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //否则显示导入成功信息
        else {
            cout << "The file(" << FileName 
                << ") has been imported to Model[" << Tag << "]" << endl;
        }
        //清空输入
        UserInput.clear();
        //当前菜单循环显示，用户不跳转菜单时停留此界面
        while (UserInput != "Y" && UserInput != "N" 
            && UserInput != "y" && UserInput != "n") {
            cout << endl << "Go Back to main menu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            //提示用户是否继续导入新模型
            cout << "Enter N(No) to import another model" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到主菜单
    return "";
}

/*************************************************************************
【函数名称】        ExportModelMenu
【函数功能】        显示导出模型菜单，允许用户选择模型并导出到文件
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】         
*************************************************************************/
string View::ExportModelMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //模型信息列表
    ModelInfoList InfoList;
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //调用控制器功能获取模型信息
        CtrlerPtr->ShowAllModels(InfoList);
        //显示导出模型菜单
        cout << endl << "-----Export Model To File-----" << endl;
        //如果没有模型可以导出
        if (InfoList.size() == 0) {
            //当前菜单循环显示，用户不跳转菜单时停留此界面
            while (UserInput != "Y" && UserInput != "N" 
                && UserInput != "y" && UserInput != "n") {
                cout << "No model exists" << endl; 
                cout << "please create or import new model first!" << endl;
                cout << "Go back to main menu to create a new model?" << endl;
                cout << "Enter Y(Yes) to go back, N(No) to stay here" << endl;
                cout << "(Y/N):";
                //获取用户输入
                getline(cin, UserInput);
            }
            //如果用户选择返回主菜单
            if (UserInput == "Y" || UserInput == "y") {
                //返回空字符串，回到主菜单
                return "";  
            } 
            else {
                //清空输入，继续留在当前界面
                UserInput.clear();
            }
        }
        //提示用户输入文件名
        cout << "Please enter file name to export:";
        //用户输入文件名
        string FileName;
        //获取用户输入
        getline(cin, FileName);
        //显示当前选择的模型
        cout << "Current model is Model[" 
            << CtrlerPtr->ChosenModelTag << "]" << endl;
        //提示用户选择要导出的模型标签
        cout << "Choose the model tag to export" << endl;
        cout << "Model tag[0~" << (InfoList.size() - 1) << "]:";
        size_t Tag;
        cin >> Tag;
        cin.get();
        RES Temp = CtrlerPtr->ExportModel(FileName, Tag);
        //如果导出失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //否则显示导出成功信息
        else {
            cout << "The file(" << FileName 
                << ") has been exported from Model[" << Tag << "]" << endl;
        }
        //清空输入
        UserInput.clear();
        //当前菜单循环显示，用户不跳转菜单时停留此界面
        while (UserInput != "Y" && UserInput != "N" 
            && UserInput != "y" && UserInput != "n") {
            cout << endl << "Go Back to main menu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            //提示用户是否继续导出新模型
            cout << "Enter N(No) to export another model" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到主菜单
    return "";
}

/*************************************************************************
【函数名称】        ShowAllModelsMenu
【函数功能】        显示列出所有模型信息的菜单，用户查看当前所有创建的模型信息，
                   用户可以选择返回主菜单或再次查看模型信息
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】         
*************************************************************************/
string View::ShowAllModelsMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //模型信息列表
    ModelInfoList InfoList;
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //获取模型信息
        CtrlerPtr->ShowAllModels(InfoList);
        //显示全部模型信息
        cout << endl << "-----Show All Models' Infomation-----" << endl;
        cout << InfoList.size() << " Model(s)" << endl;
        size_t TempTag = 0;
        for (auto TempModel : InfoList) {
            cout << "Model[" << TempTag << "]:" << endl;
            cout << "Name:" << TempModel.Name << endl;
            cout << "FaceNumber:" << TempModel.FaceNumber << endl;
            cout << "AreaSum: " << TempModel.AreaSum << endl;
            cout << "LineNumber: " << TempModel.LineNumber << endl;
            cout << "LengthSum: " << TempModel.LengthSum << endl;
            cout << "PointNumber: " << TempModel.PointNumber << endl;
            cout << "EncaseBoxVolume: " << TempModel.Volume << endl;
            cout << endl;
            TempTag++;
        }
        //清空输入
        UserInput.clear();
        //当前菜单循环显示，用户不跳转菜单时停留此界面
        while (UserInput != "Y" && UserInput != "N"
            && UserInput != "y" && UserInput != "n") {
            cout << endl << "Go Back to main menu?" << endl;
            cout << "Enter Y(Yes) to go back, N(No) to stay here" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到主菜单
    return "";
}

/*************************************************************************
【函数名称】        ChooseModelMenu
【函数功能】        显示选择模型的菜单，允许用户选择一个模型作为当前操作对象，
                   用户可以查看当前模型、选择新的模型，
                   或在无模型时提示用户创建模型
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::ChooseModelMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //模型信息列表
    ModelInfoList InfoList;
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //调用控制器功能获取模型信息
        CtrlerPtr->ShowAllModels(InfoList);
        //显示模型信息
        cout << endl << "-----Choose Current Model-----" << endl;
        //如果没有模型被选择
        if (CtrlerPtr->ChosenModelTag == NO_TAG_NUMBER) {
            cout << "No model is choose" << endl;
        }
        //否则显示当前选择的模型
        else {
            cout << "Current model is: Model[" 
                << CtrlerPtr->ChosenModelTag << "]" << endl;
        }
        //如果没有模型可以选择
        if (InfoList.size() == 0) {
            cout << "No model can be choose" << endl;
            cout << "please create or import new model!" << endl;
        }
        //否则进行模型选择
        else {
            //提示用户选择模型标签
            cout << "Model tag[0~" << (InfoList.size() - 1) << "]:";
            size_t Tag;
            cin >> Tag;
            cin.get();
            RES Temp = CtrlerPtr->ChooseModel(Tag);
            //如果选择失败，显示错误信息
            if (Temp != RES::SUCCESS) {
                cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
            }
        }
        //清空输入
        UserInput.clear();
        //当前菜单循环显示，用户不跳转菜单时停留此界面
        while (UserInput != "Y" && UserInput != "N" 
            && UserInput != "y" && UserInput != "n") {
            cout << endl << "Go Back to main menu?" << endl;
            cout << "Enter Y(Yes) to go back, N(No) to stay here" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到主菜单
    return "";
}

/*************************************************************************
【函数名称】        ModifyModelMenu
【函数功能】        显示编辑当前模型的菜单，提供模型信息查看、面和线的
                   添加、修改、删除功能接口，并根据用户的选择执行相应操作；
                   如果未选择模型，提示用户先选择模型，并询问是否返回主菜单
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::ModifyModelMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //用户选择的操作数，初始化为0（无效值）
    int InputNumber = 0;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (InputNumber != 8) {
        //显示编辑模型菜单
        cout << "-----Modify Current Model-----" << endl;
        //如果没有选择模型
        if (CtrlerPtr->ChosenModelTag == NO_TAG_NUMBER) {
            //当前菜单循环显示，用户不跳转菜单时停留此界面
            while (UserInput != "Y" && UserInput != "N" 
                && UserInput != "y" && UserInput != "n") {
                cout << "No model been chosen" << endl; 
                cout << "please choose a model first!" << endl;
                cout << "Go back to main menu to choose a model?" << endl;
                cout << "Enter Y(Yes) to go back, N(No) to stay here" << endl;
                cout << "(Y/N):";
                //获取用户输入
                getline(cin, UserInput);
            }
            //如果用户选择返回主菜单
            if (UserInput == "Y" || UserInput == "y") {
                //返回空字符串，回到主菜单
                return "";  
            } 
            else {
                //清空输入，继续留在当前界面
                UserInput.clear();
            }
        }
        //否则显示编辑模型的次级菜单功能
        cout << "1. Current Model Information" << endl;
        cout << "Face Operation:" << endl;
        cout << "2. Add Face" << endl;        
        cout << "3. Change Face Points" << endl;
        cout << "4. Delete Face" << endl;
        cout << "Line Operation:" << endl;
        cout << "5. Add Line" << endl;        
        cout << "6. Change Line Points" << endl;
        cout << "7. Delete Line" << endl;
        cout << "8. Go Back Main Menu" << endl;
        cout << "Please choose the operation number:" << endl;
        cout << "(1/2/3/4/5/6/7/8):";
        //获取用户输入
        getline(cin, UserInput);
        //尝试将用户输入转换为整数
        try {
            InputNumber = stoi(UserInput);
        } catch (...) {
            //无效输入，重置为0以继续显示菜单
            InputNumber = 0;  
        }
        //根据用户输入，调用相应的功能函数
        switch (InputNumber) {
            case 1 :
                cout << ListModelInfoMenu() << endl;
                break;
            case 2 :
                cout << AddFaceMenu() << endl;
                break;
            case 3 :
                cout << ChangeFacePointMenu() << endl;
                break;
            case 4 :
                cout << DeleteFaceMenu() << endl;
                break;
            case 5 :
                cout << AddLineMenu() << endl;
                break;
            case 6 :
                cout << ChangeLinePointMenu() << endl;
                break;
            case 7 :
                cout << DeleteLineMenu() << endl;
                break;
            case 8 :
                //返回主菜单
                break; 
            default :
                cout << "Invalid input, please enter a number between 1-8!"
                    << endl;
                //无效输入，重置为0以继续显示菜单
                InputNumber = 0; 
                break;
        }
        //清空输入
        UserInput.clear();
    }
    //返回空字符串，回到主菜单
    return "";
}

/*************************************************************************
【函数名称】        ListModelInfoMenu
【函数功能】        显示列出当前模型的详细信息菜单，包括模型的面和线的
                   详细信息。用户可以选择返回上一级菜单或留在当前界面
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::ListModelInfoMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示列出模型信息菜单
        cout << "-----List Current Model All Infomation-----" << endl;
        //模型信息列表
        ModelInfoList InfoList;
        //调用控制器功能获取模型信息
        CtrlerPtr->ShowAllModels(InfoList);
        //当前模型标签
        cout << "Current model: Model[" << CtrlerPtr->ChosenModelTag 
            << "]" << endl;
        cout << "Name: "
            << InfoList[CtrlerPtr->ChosenModelTag].Name << endl;
        cout << "Count face number: "
            << InfoList[CtrlerPtr->ChosenModelTag].FaceNumber << endl;
        cout << "Sum face area: "
            << InfoList[CtrlerPtr->ChosenModelTag].AreaSum << endl;
        cout << "Count line number: "
            << InfoList[CtrlerPtr->ChosenModelTag].LineNumber << endl;
        cout << "Sum line length: "
            << InfoList[CtrlerPtr->ChosenModelTag].LengthSum << endl;
        cout << "EncaseBox volume: "
            << InfoList[CtrlerPtr->ChosenModelTag].Volume << endl;
        //面信息列表
        FaceInfoList FaceInfoList;
        //调用控制器功能得到模型所有面信息
        CtrlerPtr->ShowModelAllFaces(FaceInfoList);
        size_t i = 0;
        //每个面显示其面积和三个顶点坐标
        for (auto e : FaceInfoList) {
            cout << "Face[" << i << "] (Area = " << e.Area << "):" << endl;
            cout << "Point0:(" << e.PointsInfo[0].X << ", " 
                << e.PointsInfo[0].Y << ", " <<e.PointsInfo[0].Z << ") ";
            cout << "Point1:(" << e.PointsInfo[1].X << ", "
                << e.PointsInfo[1].Y << ", " <<e.PointsInfo[1].Z << ") ";
            cout << "Point2:(" << e.PointsInfo[2].X << ", "
                << e.PointsInfo[2].Y << ", " <<e.PointsInfo[2].Z << ")"
                << endl;
            i++;
        };
        //线信息列表
        LineInfoList LineInfoList;
        //调用控制器功能得到模型所有线信息
        CtrlerPtr->ShowModelAllLines(LineInfoList);
        //重置i为0
        i = 0;
        //每个线显示其长度和两个顶点坐标
        for (auto e : LineInfoList) {
            cout << "Line[" << i << "] (Length = " << e.Length << "):" << endl;
            cout << "Point0:(" << e.PointsInfo[0].X << ", " 
                << e.PointsInfo[0].Y << ", " <<e.PointsInfo[0].Z << ") ";
            cout << "Point1:(" << e.PointsInfo[1].X << ", "
                << e.PointsInfo[1].Y << ", " <<e.PointsInfo[1].Z << ")"
                << endl;
            i++;
        };
        //清空输入
        UserInput.clear();
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back, N(No) to stay here" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}

/*************************************************************************
【函数名称】        AddFaceMenu
【函数功能】        显示添加面操作的菜单，用户输入三个顶点的坐标值，
                   并将此面添加到当前模型中；用户可以选择返回上一
                   级菜单或继续添加新的面
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::AddFaceMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示添加面菜单
        cout << "-----Add Face In Model-----" << endl;
        //临时储存添加点坐标
        double Point0_X;
        double Point0_Y;
        double Point0_Z;
        double Point1_X;
        double Point1_Y;
        double Point1_Z;
        double Point2_X;
        double Point2_Y;
        double Point2_Z;
        //提示用户输入三个顶点坐标
        cout << "Please enter 3 points' coordinates:" << endl;
        cout << "The first point:" << endl;
        cout << "Point[0].X :";
        cin >> Point0_X;
        cout << "Point[0].Y :";
        cin >> Point0_Y;
        cout << "Point[0].Z :";
        cin >> Point0_Z;
        cout << "The second point:" << endl;
        cout << "Point[1].X :";
        cin >> Point1_X;
        cout << "Point[1].Y :";
        cin >> Point1_Y;
        cout << "Point[1].Z :";
        cin >> Point1_Z;
        cout << "The third point:" << endl;
        cout << "Point[2].X :";
        cin >> Point2_X;
        cout << "Point[2].Y :";
        cin >> Point2_Y;
        cout << "Point[2].Z :";
        cin >> Point2_Z;
        cin.get();
        //调用控制器功能添加面
        //注：根据常规缩进规范，这里采用从第2行开始均缩进4格的形式
        RES Temp = CtrlerPtr->ModelAddFace(
            Face3D({Point0_X, Point0_Y, Point0_Z}, 
            {Point1_X, Point1_Y, Point1_Z}, 
            {Point2_X, Point2_Y, Point2_Z}));
        //如果添加失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //清空输入
        UserInput.clear();
        //用户选择是否返回上一级菜单
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            cout << "Enter N(No) to add another face" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}

/*************************************************************************
【函数名称】        ChangeFacePointMenu
【函数功能】        显示并处理修改面上点操作的菜单，用户输入面和点的标签后，
                   修改当前模型中的指定点的坐标；如果没有面可以修改，则提示用户
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::ChangeFacePointMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示编辑面上点菜单
        cout << "-----Change Point In Face-----" << endl;
        //临时储存修改面的标签
        size_t Tag;
        //面信息列表
        FaceInfoList InfoList;
        //调用控制器功能得到模型所有面信息
        CtrlerPtr->ShowModelAllFaces(InfoList);
        //如果没有面可以修改
        if (InfoList.size() == 0) {
            cout << "No face in model" << endl;
            //返回空字符串，回到ModifyModelMenu
            return "";
        }
        //提示用户输入要修改的面标签
        cout << "Please enter the face tag to change:" << endl;
        cout << "Face tag[0~" << InfoList.size() - 1 << "]:";
        cin >> Tag;
        cin.get();
        //面上点信息列表
        PointInfoList PointList;
        //调用控制器功能得到面上所有点信息
        RES Temp = CtrlerPtr->ShowModelFaceAllPoints(Tag, PointList);
        //如果获取失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //否则输出面上所有点信息
        else {
            cout << "Face[" << Tag << "] points infomation:" << endl;
            cout << "Point[0]: (" << PointList[0].X << ", " 
                << PointList[0].Y << ", " << PointList[0].Z << ")";
            cout << "Point[1]: (" << PointList[1].X << ", "
                << PointList[1].Y << ", " << PointList[1].Z << ")";
            cout << "Point[2]: (" << PointList[2].X << ", "
                << PointList[2].Y << ", " << PointList[2].Z << ")" << endl;
            //临时储存修改点的标签
            size_t PointTag;
            //提示用户输入要修改的点标签
            cout << "Please enter the point tag to change:" << endl;
            cout << "Point tag[0~2]:";
            cin >> PointTag;
            //临时储存修改点的坐标
            double Temp_X;
            double Temp_Y;
            double Temp_Z;
            //提示用户输入新的点坐标
            cout << "Please enter the new point coordinates:" << endl;
            cout << "New X:";
            cin >> Temp_X;
            cout << "New Y:";
            cin >> Temp_Y;
            cout << "New Z:";
            cin >> Temp_Z;
            cin.get();
            //调用控制器功能修改面上的点
            Temp = CtrlerPtr->ModelChangeFacePoint(
                Tag, PointTag, {Temp_X, Temp_Y, Temp_Z});
            //如果修改失败，显示错误信息
            if (Temp != RES::SUCCESS) {
                cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)];
                cout << endl;
            }
        }
        //清空输入
        UserInput.clear();
        //用户选择是否返回上一级菜单
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            cout << "Enter N(No) to change another face" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}

/*************************************************************************
【函数名称】        DeleteFaceMenu
【函数功能】        显示并处理删除面操作的菜单，用户输入面标签后，
                   删除当前模型中的指定面；如果没有面可以删除，则提示用户
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::DeleteFaceMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示删除面菜单
        cout << "-----Delete Face In Model-----" << endl;
        //临时储存要删除的面标签
        size_t Tag;
        //面信息列表
        FaceInfoList InfoList;
        //调用控制器功能得到模型所有面信息
        CtrlerPtr->ShowModelAllFaces(InfoList);
        //如果没有面可以删除
        if (InfoList.size() == 0) {
            cout << "No face in model" << endl;
            //返回空字符串，回到ModifyModelMenu
            return "";
        }
        //提示用户输入要删除的面标签
        cout << "Please enter the face tag to delete:" << endl;
        cout << "Face tag[0~" << InfoList.size() - 1 << "]:" << endl;
        cin >> Tag;
        cin.get();
        //调用控制器功能删除面
        RES Temp = CtrlerPtr->ModelDeleteFace(Tag);
        //如果删除失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //清空输入
        UserInput.clear();
        //用户选择是否返回上一级菜单
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            cout << "Enter N(No) to delete another face" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}

/*************************************************************************
【函数名称】        AddLineMenu
【函数功能】        显示添加线操作的菜单，用户输入两端点的坐标值，
                   并将此线添加到当前模型中；用户可以选择返回上一
                   级菜单或继续添加新的线段
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::AddLineMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示添加线菜单
        cout << "-----Add Line In Model-----" << endl;
        //临时储存添加点坐标
        double Point0_X;
        double Point0_Y;
        double Point0_Z;
        double Point1_X;
        double Point1_Y;
        double Point1_Z;
        //提示用户输入两个顶点坐标
        cout << "Please enter 2 points' coordinates:" << endl;
        cout << "The first point:" << endl;
        cout << "Point[0].X :";
        cin >> Point0_X;
        cout << "Point[0].Y :";
        cin >> Point0_Y;
        cout << "Point[0].Z :";
        cout << "The second point:" << endl;
        cin >> Point0_Z;
        cout << "Point[1].X :";
        cin >> Point1_X;
        cout << "Point[1].Y :";
        cin >> Point1_Y;
        cout << "Point[1].Z :";
        cin >> Point1_Z;
        cin.get();
        //调用控制器功能添加线
        //注：根据常规缩进规范，这里采用从第2行开始均缩进4格的形式
        RES Temp = CtrlerPtr->ModelAddLine(
            Line3D({Point0_X, Point0_Y, Point0_Z}, 
            {Point1_X, Point1_Y, Point1_Z}));
        //如果添加失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //清空输入
        UserInput.clear();
        //用户选择是否返回上一级菜单
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            cout << "Enter N(No) to add another line" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}

/*************************************************************************
【函数名称】        ChangeLinePointMenu
【函数功能】        显示并处理修改线上点操作的菜单，用户输入线和点的标签后，
                   修改当前模型中的指定点的坐标；如果没有线可以修改，则提示用户
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::ChangeLinePointMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示编辑线上点菜单
        cout << "-----Change Point In Line-----" << endl;
        //临时储存修改线的标签
        size_t Tag;
        //线信息列表
        LineInfoList InfoList;
        //调用控制器功能得到模型所有线信息
        CtrlerPtr->ShowModelAllLines(InfoList);
        //如果没有线可以修改
        if (InfoList.size() == 0) {
            cout << "No Line in model" << endl;
            //返回空字符串，回到ModifyModelMenu
            return "";
        }
        //提示用户输入要修改的线标签
        cout << "Please enter the line tag to change:" << endl;
        cout << "Line tag[0~" << InfoList.size() - 1 << "]:";
        cin >> Tag;
        cin.get();
        //线上点信息列表
        PointInfoList PointList;
        //调用控制器功能得到线上所有点信息
        RES Temp = CtrlerPtr->ShowModelLineAllPoints(Tag, PointList);
        //如果获取失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //否则输出线上所有点信息
        else {
            cout << "Line[" << Tag << "] points infomation:" << endl;
            cout << "Point[0]: (" << PointList[0].X << ", " 
                << PointList[0].Y << ", " << PointList[0].Z << ")";
            cout << "Point[1]: (" << PointList[1].X << ", "
                << PointList[1].Y << ", " << PointList[1].Z << ")" << endl;
            //临时储存修改点的标签
            size_t PointTag;
            //提示用户输入要修改的点标签
            cout << "Please enter the point tag to change:" << endl;
            cout << "Point tag[0~1]:";
            cin >> PointTag;
            //临时储存修改点的坐标
            double Temp_X;
            double Temp_Y;
            double Temp_Z;
            //提示用户输入新的点坐标
            cout << "Please enter the new point coordinates:" << endl;
            cout << "New X:";
            cin >> Temp_X;
            cout << "New Y:";
            cin >> Temp_Y;
            cout << "New Z:";
            cin >> Temp_Z;
            cin.get();
            //调用控制器功能修改线上的点
            Temp = CtrlerPtr->ModelChangeFacePoint(
                Tag, PointTag, {Temp_X, Temp_Y, Temp_Z});
            //如果修改失败，显示错误信息
            if (Temp != RES::SUCCESS) {
                cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
            }
        }
        //清空输入
        UserInput.clear();
        //用户选择是否返回上一级菜单
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            cout << "Enter N(No) to change another line" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}

/*************************************************************************
【函数名称】        DeleteLineMenu
【函数功能】        显示并处理删除线操作的菜单，用户输入线标签后，
                   删除当前模型中的指定线；如果没有线可以删除，则提示用户
【参数】            无
【返回值】          空字符串string
【开发者及日期】    梁思奇 2024/8/11
【更改记录】
*************************************************************************/
string View::DeleteLineMenu() const{
    //获取控制器实例指针
    shared_ptr<Controller> CtrlerPtr = Controller::GetControllerPtr();
    //用户输入
    string UserInput;
    //当前菜单循环显示，用户不跳转菜单时停留此界面
    while (UserInput != "Y" && UserInput != "y") {
        //显示删除线菜单
        cout << "-----Delete Line In Model-----" << endl;
        //临时储存要删除的线标签
        size_t Tag;
        //线信息列表
        LineInfoList InfoList;
        //调用控制器功能得到模型所有线信息
        CtrlerPtr->ShowModelAllLines(InfoList);
        //如果没有线可以删除
        if (InfoList.size() == 0) {
            cout << "No line in model" << endl;
            //返回空字符串，回到ModifyModelMenu
            return "";
        }
        //提示用户输入要删除的线标签
        cout << "Please enter the line tag to delete:" << endl;
        cout << "Line tag[0~" << InfoList.size() - 1 << "]:" << endl;
        cin >> Tag;
        cin.get();
        //调用控制器功能删除线
        RES Temp = CtrlerPtr->ModelDeleteLine(Tag);
        //如果删除失败，显示错误信息
        if (Temp != RES::SUCCESS) {
            cout << CtrlerPtr->RESNAME[static_cast<size_t>(Temp)] << endl;
        }
        //清空输入
        UserInput.clear();
        //用户选择是否返回上一级菜单
        while (UserInput != "Y" && UserInput != "y" 
            && UserInput != "N" && UserInput != "n") {
            cout << endl << "Go back to ModifyModelMenu?" << endl;
            cout << "Enter Y(Yes) to go back" << endl;
            cout << "Enter N(No) to delete another line" << endl;
            cout << "(Y/N):";
            //获取用户输入
            getline(cin, UserInput);
        }
    }
    //返回空字符串，回到ModifyModelMenu
    return "";
}
