/*************************************************************************
【文件名】                 Controller.cpp
【功能模块和目的】          控制器类实现
【开发者及日期】            梁思奇 2024/8/8
【更改记录】               梁思奇 2024/8/10 改进功能函数实现方式
*************************************************************************/

//自身类头文件
#include "Controller.hpp"
//Model3D类所属头文件
#include "Model3D.hpp"
//Point3D类所属头文件
#include "Point3D.hpp"
//Face3D类所属头文件
#include "Line3D.hpp"
//Importer3D_obj所属头文件
#include "Importer3D_obj.hpp"
//Exporter3D_obj所属头文件
#include "Exporter3D_obj.hpp"
//size_t所属头文件
#include <cstddef>
//std::vector所属头文件
#include <vector>
//std::shared_ptr所属头文件
#include <memory>
//std::string所属头文件
#include <string>

//控制器本身类操作函数实现

//静态私有数据成员：控制器实例指针初始化为空
std::shared_ptr<Controller> Controller::m_pControllerIntance{nullptr};

/*************************************************************************
【函数名称】          GetControllerPtr
【函数功能】          获取控制器实例指针，如果实例不存在则创建
【参数】              无
【返回值】            std::shared_ptr<Controller>：控制器实例指针
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
std::shared_ptr<Controller> Controller::GetControllerPtr(){
    if (m_pControllerIntance == nullptr) {
        m_pControllerIntance 
            = std::shared_ptr<Controller>(new Controller());
    }
    return m_pControllerIntance;
}

/*************************************************************************
【函数名称】          Controller
【函数功能】          控制器类构造函数，添加具体导入器与导出器
【参数】              无
【返回值】            无
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::Controller(){
    //添加具体obj导入器类
    Importer3D::AddImporter<Importer3D_obj>();
    //添加具体obj导出器类
    Exporter3D::AddExporter<Exporter3D_obj>();
}

//功能函数实现

//所有控制器功能函数返回值对应的名称字符串
const std::string Controller::RESNAME[] {
    "SUCCESS",
    "UNKNOWN_ERROR",
    "REPEAT_ELEMENT",
    "REPEAT_POINT",
    "TAG_OUT_OF_RANGE",
    "FAIL_TO_IMPORT",
    "FAIL_TO_EXPORT"
};

/*************************************************************************
【函数名称】          ImportModel
【函数功能】          导入指定文件到指定模型
【参数】              const std::string& FileName：文件名
                     size_t ModelTag：模型标记
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ImportModel
(const std::string& FileName, size_t ModelTag){
    if (ModelTag >= m_Models.size()) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    try {
        //获取具体导入器指针
        std::shared_ptr<Importer3D> ImporterPtr 
            = Importer3D::GetImporter(FileName);
        //导入文件到模型
        (*m_Models[ModelTag]) = ImporterPtr->ImportFromFile(FileName);
    }
    //文件导入失败
    catch (const Importer3D::FAIL_TO_IMPORT& e) {
        return RES::FAIL_TO_IMPORT;
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ExportModel
【函数功能】          导出指定模型到指定文件
【参数】              const std::string& FileName：文件名
                     size_t ModelTag：模型标记
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ExportModel
(const std::string& FileName, size_t ModelTag){
    if (ModelTag >= m_Models.size()) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    try {
        //获取具体导出器指针
        std::shared_ptr<Exporter3D> ExporterPtr
            = Exporter3D::GetExporter(FileName);
        //导出模型到文件
        ExporterPtr->ExportToFile(FileName, (*m_Models[ModelTag]));
    }
    //文件导出失败
    catch (const Exporter3D::FAIL_TO_EXPORT& e) {
        return RES::FAIL_TO_EXPORT;
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          SetEmptyModel
【函数功能】          创建空模型并设为当前操作模型
【参数】              无
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::SetEmptyModel(){
    //添加新模型到模型类列表
    m_Models.push_back(std::shared_ptr<Model3D>(new Model3D()));
    //指定当前模型
    m_ChosenModelTag = m_Models.size() - 1;
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ChooseModel
【函数功能】          选择指定模型为当前操作模型
【参数】              size_t ModelTag：模型标记
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ChooseModel(size_t ModelTag){
    if (ModelTag >= m_Models.size()) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //设定当前模型
    m_ChosenModelTag = ModelTag;
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          DeleteModel
【函数功能】          删除指定模型
【参数】              size_t ModelTag：模型标记
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::DeleteModel(size_t ModelTag){
    if (ModelTag >= m_Models.size()) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //删除模型
    m_Models.erase(m_Models.begin() + ModelTag);
    //若无模型，更新标签为无模型
    if (m_Models.size() == 0) {
        m_ChosenModelTag = NO_TAG_NUMBER;
    }
    //若删除当前模型，则更新为0号标签
    else if (ModelTag == m_ChosenModelTag) {
        m_ChosenModelTag = 0;
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ModelAddFace
【函数功能】          添加指定面到当前模型
【参数】              const Face3D& AFace：面对象
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ModelAddFace(const Face3D& AFace){
    if (m_Models[m_ChosenModelTag]->AddFace(AFace)) {
        //若没有遇到异常错误，则返回“成功”
        return RES::SUCCESS;
    }
    else {
        //重复元素异常
        return RES::REPEAT_ELEMENT;
    }
}

/*************************************************************************
【函数名称】          ModelDeleteFace
【函数功能】          删除当前模型指定面的对象
【参数】              size_t FaceTag：面标记
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ModelDeleteFace(size_t FaceTag){
    if (FaceTag >= m_Models[m_ChosenModelTag]->FaceNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //删除指定面
    Face3D TempFace = *(m_Models[m_ChosenModelTag]->Faces[FaceTag]);
    m_Models[m_ChosenModelTag]->DeleteFace(TempFace);
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ModelAddLine
【函数功能】          添加指定线到当前模型
【参数】              const Line3D& ALine：线对象
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ModelAddLine(const Line3D& ALine){
    if (m_Models[m_ChosenModelTag]->AddLine(ALine)) {
        //若没有遇到异常错误，则返回“成功”
        return RES::SUCCESS;
    }
    else {
        //重复元素异常
        return RES::REPEAT_ELEMENT;
    }
}

/*************************************************************************
【函数名称】          ModelDeleteLine
【函数功能】          删除当前模型指定线对象
【参数】              size_t LineTag：线标记
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ModelDeleteLine(size_t LineTag){
    if (LineTag >= m_Models[m_ChosenModelTag]->LineNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //删除指定线
    Line3D TempLine = *(m_Models[m_ChosenModelTag]->Lines[LineTag]);
    m_Models[m_ChosenModelTag]->DeleteLine(TempLine);
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ModelChangeFacePoint
【函数功能】          修改当前模型指定面的指定点
【参数】              size_t FaceTag：面标记
                     size_t PointTag：点标记
                     const Point3D& Point1：点对象
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ModelChangeFacePoint
(size_t FaceTag, size_t PointTag, const Point3D& Point1){
    if (FaceTag >= m_Models[m_ChosenModelTag]->FaceNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    if (PointTag >= m_Models[m_ChosenModelTag]->Faces[FaceTag]->PointNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //修改点
    try {
        //这里按从第2行开始均缩进4格的规范
        m_Models[m_ChosenModelTag]->Faces[FaceTag]->ChangePoint(
            m_Models[m_ChosenModelTag]->Faces[FaceTag]->Points[PointTag],
            Point1);
    }
    catch (Point3DGroup::REPEAT_ELEMENT) {
        //重复点错误
        return RES::REPEAT_POINT;
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ModelChangeLinePoint
【函数功能】          修改当前模型指定线上的指定点
【参数】              size_t LineTag：线标记
                     size_t PointTag：点标记
                     const Point3D& Point1：点对象
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ModelChangeLinePoint
(size_t LineTag, size_t PointTag, const Point3D& Point1){
    if (LineTag >= m_Models[m_ChosenModelTag]->LineNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    if (PointTag >= m_Models[m_ChosenModelTag]->Lines[LineTag]->PointNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //修改
    try {
        //这里按从第2行开始均缩进4格的规范
        m_Models[m_ChosenModelTag]->Lines[LineTag]->ChangePoint(
            m_Models[m_ChosenModelTag]->Lines[LineTag]->Points[PointTag], 
            Point1);
    }
    catch (Point3DGroup::REPEAT_ELEMENT) {
        //重复点错误
        return RES::REPEAT_POINT;
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

//显示信息函数实现

/*************************************************************************
【函数名称】          ShowAllModels
【函数功能】          列出所有模型信息
【参数】              List_Model3DInfo& InfoList：信息列表（会被清空列表重写）
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowAllModels(List_Model3DInfo& InfoList){
    //清空列表
    InfoList.clear();
    //逐一生成写入
    for (auto TempModel : m_Models) {
        InfoList.push_back(Info_Model3D{
            //模型名
            TempModel->Name,
            //重复点数
            TempModel->FaceNum * 3 + TempModel->LineNum * 2,
            //线数
            TempModel->LineNum,
            //面数
            TempModel->FaceNum,
            //线段总长
            TempModel->LineLength_Sum,
            //面积总和
            TempModel->FaceArea_Sum,
            //包围盒体积
            TempModel->EncaseCuboid_Volume});
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ShowModle
【函数功能】          列出指定模型的信息
【参数】              size_t ModelTag：模型标记
                     Info_Model3D& Info：信息列表
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowModle(size_t ModelTag, Info_Model3D& Info){
    if (ModelTag >= m_Models.size()) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //生成信息
    Info = Info_Model3D{
        //模型名
        m_Models[ModelTag]->Name,
        //重复点数
        m_Models[ModelTag]->FaceNum * 3 
            + m_Models[ModelTag]->LineNum * 2,
        //线数
        m_Models[ModelTag]->LineNum,
        //面数
        m_Models[ModelTag]->FaceNum,
        //线段总长
        m_Models[ModelTag]->LineLength_Sum,
        //面积总和
        m_Models[ModelTag]->FaceArea_Sum,
        //包围盒体积
        m_Models[ModelTag]->EncaseCuboid_Volume
    };
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ShowThisModel
【函数功能】          列出当前模型的信息
【参数】              Info_Model3D& Info：信息列表
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowThisModel(Info_Model3D& Info){
    //调用ShowModle函数简化
    return ShowModle(m_ChosenModelTag, Info);
}

/*************************************************************************
【函数名称】          ShowModelAllFaces
【函数功能】          列出当前模型中的所有面信息
【参数】              List_Face3DInfo& InfoList：信息列表（会被清空列表重写）
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowModelAllFaces(List_Face3DInfo& InfoList){
    //清空列表
    InfoList.clear();
    //逐一生成写入
    for (auto TempFace : m_Models[m_ChosenModelTag]->Faces) {
        //临时信息储存
        Info_Face3D TempInfo;
        //每个点的信息
        for (size_t i = 0; i < 3; i++) {
            TempInfo.PointsInfo[i].X = TempFace->Points[i].GetX();
            TempInfo.PointsInfo[i].Y = TempFace->Points[i].GetY();
            TempInfo.PointsInfo[i].Z = TempFace->Points[i].GetZ();
        }
        TempInfo.Area = TempFace->GetArea();
        InfoList.push_back(TempInfo);
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ShowModelAllLines
【函数功能】          列出当前模型中的所有线信息
【参数】              List_Line3DInfo& InfoList：信息列表（会被清空列表重写）
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowModelAllLines(List_Line3DInfo& InfoList){
    //清空列表
    InfoList.clear();
    //逐一生成写入
    for (auto TempLine : m_Models[m_ChosenModelTag]->Lines) {
        //临时信息储存
        Info_Line3D TempInfo;
        //每个点的信息
        for (size_t i = 0; i < 2; i++) {
            TempInfo.PointsInfo[i].X = TempLine->Points[i].GetX();
            TempInfo.PointsInfo[i].Y = TempLine->Points[i].GetY();
            TempInfo.PointsInfo[i].Z = TempLine->Points[i].GetZ();
        }
        TempInfo.Length = TempLine->GetLength();
        InfoList.push_back(TempInfo);
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ShowModelFaceAllPoints
【函数功能】          列出当前模型指定面中的所有点信息
【参数】              size_t FaceTag：面标记
                     List_Point3DInfo& InfoList：信息列表（会被清空列表重写）
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowModelFaceAllPoints(
    size_t FaceTag, List_Point3DInfo& InfoList){
    if (FaceTag >= m_Models[m_ChosenModelTag]->FaceNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //清空列表
    InfoList.clear();
    //逐一生成写入
    for (size_t i = 0; i < 3; i++) {
        //这里按从第2行开始均缩进4格的规范
        InfoList.push_back(
            Info_Point3D{
            m_Models[m_ChosenModelTag]->Faces[FaceTag]->Points[i].GetX(),
            m_Models[m_ChosenModelTag]->Faces[FaceTag]->Points[i].GetY(),
            m_Models[m_ChosenModelTag]->Faces[FaceTag]->Points[i].GetZ()}
        );
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}

/*************************************************************************
【函数名称】          ShowModelLineAllPoints
【函数功能】          列出当前模型指定线中所有点信息
【参数】              size_t LineTag：线标记
                     List_Point3DInfo& InfoList：信息列表（会被清空列表重写）
【返回值】            RES：执行结果，成功返回RES::SUCCESS
【开发者及日期】      梁思奇 2024/8/8
【更改记录】         
*************************************************************************/
Controller::RES Controller::ShowModelLineAllPoints(
    size_t LineTag, List_Point3DInfo& InfoList){
    if (LineTag >= m_Models[m_ChosenModelTag]->LineNum) {
        //标签越界错误
        return RES::TAG_OUT_OF_RANGE;
    }
    //清空列表
    InfoList.clear();
    //逐一生成写入
    for (size_t i = 0; i < 2; i++) {
        //这里按从第2行开始均缩进4格的规范
        InfoList.push_back(
            Info_Point3D{
            m_Models[m_ChosenModelTag]->Lines[LineTag]->Points[i].GetX(),
            m_Models[m_ChosenModelTag]->Lines[LineTag]->Points[i].GetY(),
            m_Models[m_ChosenModelTag]->Lines[LineTag]->Points[i].GetZ()}
        );
    }
    //若没有遇到异常错误，则返回“成功”
    return RES::SUCCESS;
}
