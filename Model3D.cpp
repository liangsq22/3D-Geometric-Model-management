/*************************************************************************
【文件名】                 Model3D.cpp
【功能模块和目的】          三维模型类实现
【开发者及日期】            梁思奇 2024/8/5
【更改记录】               梁思奇 2024/8/8 调整bug并增加对点线面的操作
                          梁思奇 2024/8/9 增加模型元素批量合并与移除接口
*************************************************************************/

//自身类头文件
#include "Model3D.hpp"
//Face3D类所属头文件
#include "Face3D.hpp"
//Line3D类所属头文件
#include "Line3D.hpp"
//Point3D类所属头文件
#include "Point3D.hpp"
//std::vector所属头文件
#include <vector>
//size_t所属头文件
#include <cstddef>
//std::shared_ptr类所属头文件
#include <memory>
//std::vector、std::minmax_element所属头文件
#include <algorithm>
//std::vector操作所需仿函数头文件
#include <functional>
//std::string所属头文件
#include <string>

/*************************************************************************
【函数名称】        NO_POINT_OPERATE
【函数功能】        NO_POINT_OPERATE构造函数，用于初始化异常类并设置错误消息
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D::NO_POINT_OPERATE::NO_POINT_OPERATE() : std::logic_error(
    std::string("Model3D Base class has no point operation")){
}

//构造、赋值操作

/*************************************************************************
【函数名称】        Model3D
【函数功能】        默认构造函数，初始化空的三维模型对象
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D::Model3D(){
}

/*************************************************************************
【函数名称】        Model3D
【函数功能】        拷贝构造函数，用另一个Model3D对象初始化当前对象
【参数】            const Model3D& Source：另一个Model3D对象
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D::Model3D(const Model3D& Source){
    //拷贝Face3D和Line3D以及其他数据成员
    m_Faces = Source.Faces;
    m_Lines = Source.Lines;
    m_ullFaceNum = Source.FaceNum;
    m_ullLineNum = Source.LineNum;
    m_ullPointNum = Source.PointNum;
    m_ullElementNum = Source.ElementNum;
    m_rFaceArea_Sum = Source.FaceArea_Sum;
    m_rLineLength_Sum = Source.LineLength_Sum;
    m_rEncaseCuboid_Length = Source.m_rEncaseCuboid_Length;
    m_rEncaseCuboid_Width = Source.m_rEncaseCuboid_Width;
    m_rEncaseCuboid_Height = Source.m_rEncaseCuboid_Height;
    m_rEncaseCuboid_Volume = Source.EncaseCuboid_Volume;
    Name = Source.Name;
    Notes = Source.Notes;
}

/*************************************************************************
【函数名称】        Model3D
【函数功能】        带参构造函数，用给定的面和线集合初始化三维模型对象
【参数】            const std::vector<Face3D>& vFaces：面集合
                   const std::vector<Line3D>& vLines：线集合
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D::Model3D(const std::vector<Face3D>& vFaces, 
    const std::vector<Line3D>& vLines){
    //添加面和线
    for (auto Face : vFaces) {
        AddFace(Face);
    }
    for (auto Line : vLines) {
        AddLine(Line);
    }
}

/*************************************************************************
【函数名称】        operator=
【函数功能】        赋值运算符，将另一个Model3D对象的内容赋值给当前对象
【参数】            const Model3D& Source：另一个Model3D对象
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator=(const Model3D& Source){
    //检查自赋值
    if (this != &Source) {
        m_Faces = Source.Faces;
        m_Lines = Source.Lines;
        m_ullFaceNum = Source.FaceNum;
        m_ullLineNum = Source.LineNum;
        m_ullElementNum = Source.ElementNum;
        m_rFaceArea_Sum = Source.FaceArea_Sum;
        m_rLineLength_Sum = Source.LineLength_Sum;
        m_rEncaseCuboid_Length = Source.m_rEncaseCuboid_Length;
        m_rEncaseCuboid_Width = Source.m_rEncaseCuboid_Width;
        m_rEncaseCuboid_Height = Source.m_rEncaseCuboid_Height;
        m_rEncaseCuboid_Volume = Source.EncaseCuboid_Volume;
        Name = Source.Name;
        Notes = Source.Notes;
    }
    return *this;
}

//Face3D增删改操作

/*************************************************************************
【函数名称】        AddFace
【函数功能】        添加一个指定的Face3D对象到模型中
【参数】            const Face3D& Face1：要添加的Face3D对象
【返回值】          如果成功添加，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::AddFace(const Face3D& Face1){
    //查找是否已经存在相同的Face3D
    auto TempFace = FaceSearcher(Face1);
    if (TempFace != m_Faces.end()) {
        return false;
    }
    //创建新的Face3D对象并添加到模型中
    std::shared_ptr<Face3D> TempPtr{new Face3D{Face1}};
    m_Faces.push_back(TempPtr);
    //面数加1
    m_ullFaceNum++;
    //点数加3
    m_ullPointNum += 3;
    //元素数加1
    m_ullElementNum++;
    //总面积加上新面的面积
    m_rFaceArea_Sum += TempPtr->GetArea();
    //重新计算最小包围长方体
    CalcEncaseCuboid();
    return true;
}

/*************************************************************************
【函数名称】        AddFace
【函数功能】        使用三个Point3D对象来创建并添加一个Face3D对象
【参数】            const Point3D& Point1, const Point3D& Point2, 
                   const Point3D& Point3：构成Face3D的三个点
【返回值】          如果成功添加，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::AddFace(
    const Point3D& Point1, const Point3D& Point2, const Point3D& Point3){
    //调用已有函数，返回成功与否
    return AddFace(Face3D(Point1, Point2, Point3));
}

/*************************************************************************
【函数名称】        operator+=
【函数功能】        将多个Face3D对象添加到模型中
【参数】            const std::vector<Face3D>& vFaces：要添加的Face3D对象集合
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator+=(const std::vector<Face3D>& vFaces){
    //循环添加每一个Face3D对象
    for (auto TempFace : vFaces) {
        AddFace(TempFace);
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator+
【函数功能】        返回一个新模型，新模型由当前模型和多个Face3D对象组成
【参数】            const std::vector<Face3D>& vFaces：要添加的Face3D对象集合
【返回值】          合并后新创建的Model3D对象
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D Model3D::operator+(const std::vector<Face3D>& vFaces) const{
    //创建当前模型的副本
    Model3D Temp{*this};
    //循环添加每一个Face3D对象到副本中
    for (auto TempFace : vFaces) {
        Temp.AddFace(TempFace);
    }
    return Temp;
}

/*************************************************************************
【函数名称】        ChangeFace
【函数功能】        将模型中的指定Face3D对象替换为新的Face3D对象
【参数】            const Face3D& OldFace：要替换的旧Face3D对象
                   const Face3D& NewFace：新的Face3D对象
【返回值】          如果替换成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::ChangeFace(const Face3D& OldFace, const Face3D& NewFace){
    //查找旧的Face3D对象
    auto ItOld = FaceSearcher(OldFace);
    //查找新的Face3D对象是否已经存在
    auto ItNew = FaceSearcher(NewFace);
    if (ItOld == m_Faces.end() || ItNew != m_Faces.end()) {
        return false;
    }
    //更新旧的Face3D为新的Face3D
    *(*ItOld) = NewFace;
    //更新统计数据
    m_rFaceArea_Sum -= OldFace.GetArea();
    m_rFaceArea_Sum += NewFace.GetArea();
    //重新计算最小包围长方体
    CalcEncaseCuboid();
    return true;
}

/*************************************************************************
【函数名称】        ChangeFace
【函数功能】        在指定的Face3D对象中替换一个点
【参数】            const Face3D& Face1：要修改的Face3D对象
                   const Point3D& OldFace：要替换的点
                   const Point3D& NewFace：新的点
【返回值】          如果替换成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::ChangeFace(
    const Face3D& Face1, const Point3D& OldFace, const Point3D& NewFace){
    //检查旧点是否存在，且新点不存在
    if (!Face1.Points.IsExist(OldFace) || Face1.Points.IsExist(NewFace)) {
        return false;
    }
    //创建一个新的Face3D对象替换旧的Face3D对象
    Face3D TempFace{Face1};
    TempFace.ChangePoint(OldFace, NewFace);
    //调用已有函数，返回成功与否
    return ChangeFace(Face1, TempFace);
}

/*************************************************************************
【函数名称】        ChangeFace
【函数功能】        在指定的Face3D对象中替换指定索引处的点
【参数】            const Face3D& Face1：要修改的Face3D对象
                   size_t Index：要替换的点的索引
                   const Point3D& NewPoint：新的点
【返回值】          如果替换成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::ChangeFace(
    const Face3D& Face1, size_t Index, const Point3D& NewPoint){
    //调用已有函数，返回成功与否
    return ChangeFace(Face1, Face1.Points[Index], NewPoint);
}

/*************************************************************************
【函数名称】        DeleteFace
【函数功能】        删除模型中的指定Face3D对象
【参数】            const Face3D& Face1：要删除的Face3D对象
【返回值】          如果删除成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::DeleteFace(const Face3D& Face1){
    //查找要删除的Face3D对象
    auto target = FaceSearcher(Face1);
    if (target == m_Faces.end()) {
        return false;
    }
    //删除对象
    m_Faces.erase(target);
    //面数减1
    m_ullFaceNum--;
    //点数减3
    m_ullPointNum -= 3;
    //元素数减1
    m_ullElementNum--;
    //总面积减去删除面的面积
    m_rFaceArea_Sum -= Face1.GetArea();
    //重新计算最小包围长方体
    CalcEncaseCuboid();
    return true;
}

/*************************************************************************
【函数名称】        DeleteFace
【函数功能】        使用三个Point3D对象来删除模型中的指定Face3D对象
【参数】            const Point3D& Point1, const Point3D& Point2, 
                   const Point3D& Point3：构成Face3D的三个点
【返回值】          如果删除成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::DeleteFace(
    const Point3D& Point1, const Point3D& Point2, const Point3D& Point3){
    return DeleteFace(Face3D(Point1, Point2, Point3));
}

/*************************************************************************
【函数名称】        operator-=
【函数功能】        从模型中删除多个指定的Face3D对象
【参数】            const std::vector<Face3D>& vFaces：要删除的Face3D对象集合
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator-=(const std::vector<Face3D>& vFaces){
    //循环删除每一个Face3D对象
    for (auto TempFace : vFaces) {
        DeleteFace(TempFace);
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator-
【函数功能】        返回一个新模型，新模型由当前模型移除多个Face3D对象后的结果
【参数】            const std::vector<Face3D>& vFaces：要删除的Face3D对象集合
【返回值】          移除指定Face3D对象后新创建的Model3D对象
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D Model3D::operator-(const std::vector<Face3D>& vFaces) const{
    Model3D Temp{*this};
    //循环删除每一个Face3D对象
    for (auto TempFace : vFaces) {
        Temp.DeleteFace(TempFace);
    }
    return Temp;
}

/*************************************************************************
【函数名称】        ClearFaces
【函数功能】        删除模型中的所有Face3D对象
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
void Model3D::ClearFaces(){
    //清空所有Face3D对象
    m_Faces.clear();
    //面积和清零
    m_rFaceArea_Sum = 0.0;
    //元素数减去面数
    m_ullElementNum -= m_ullFaceNum;
    //点数减去3倍面数
    m_ullPointNum -= 3 * m_ullFaceNum;
    //面数清零
    m_ullFaceNum = 0;
    //重新计算最小包围长方体
    CalcEncaseCuboid();
}

//Line3D增删改操作

/*************************************************************************
【函数名称】        AddLine
【函数功能】        添加一个指定的Line3D对象到模型中
【参数】            const Line3D& Line1：要添加的Line3D对象
【返回值】          如果成功添加，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::AddLine(const Line3D& Line1){
    //查找是否已经存在相同的Line3D
    auto TempLine = LineSearcher(Line1);
    if (TempLine != m_Lines.end()) {
        return false;
    }
    //创建新的Line3D对象并添加到模型中
    std::shared_ptr<Line3D> TempPtr{new Line3D{Line1}};
    m_Lines.push_back(TempPtr);
    //线数加1
    m_ullLineNum++;
    //点数加2
    m_ullPointNum += 2;
    //元素数加1
    m_ullElementNum++;
    //总线长加上新线的长度
    m_rLineLength_Sum += TempPtr->GetLength();
    //重新计算最小包围长方体
    CalcEncaseCuboid();
    return true;
}

/*************************************************************************
【函数名称】        AddLine
【函数功能】        使用两个Point3D对象来创建并添加一个Line3D对象
【参数】            const Point3D& Point1, const Point3D& Point2：
                   构成Line3D的两个点
【返回值】          如果成功添加，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::AddLine(const Point3D& Point1, const Point3D& Point2){
    //调用已有函数，返回成功与否
    return AddLine(Line3D(Point1, Point2));
}

/*************************************************************************
【函数名称】        operator+=
【函数功能】        将多个Line3D对象添加到模型中
【参数】            const std::vector<Line3D>& vLines：要添加的Line3D对象集合
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator+=(const std::vector<Line3D>& vLines){
    //循环添加每一个Line3D对象
    for (auto TempLine : vLines) {
        AddLine(TempLine);
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator+
【函数功能】        返回一个新模型，新模型由当前模型和多个Line3D对象组成
【参数】            const std::vector<Line3D>& vLines：要添加的Line3D对象集合
【返回值】          合并后新创建的Model3D对象
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D Model3D::operator+(const std::vector<Line3D>& vLines) const{
    //创建当前模型的副本
    Model3D Temp{*this};
    //循环添加每一个Line3D对象到副本中
    for (auto TempLine : vLines) {
        Temp.AddLine(TempLine);
    }
    return Temp;
}

/*************************************************************************
【函数名称】        ChangeLine
【函数功能】        将模型中的指定Line3D对象替换为新的Line3D对象
【参数】            const Line3D& OldLine：要替换的旧Line3D对象
                   const Line3D& NewLine：新的Line3D对象
【返回值】          如果替换成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::ChangeLine(const Line3D& OldLine, const Line3D& NewLine){
    //查找旧的Line3D对象
    auto ItOld = LineSearcher(OldLine);
    //查找新的Line3D对象是否已经存在
    auto ItNew = LineSearcher(NewLine);
    if (ItOld == m_Lines.end() || ItNew != m_Lines.end()) {
        return false;
    }
    //更新旧的Line3D为新的Line3D
    *(*ItOld) = NewLine;
    //更新统计数据
    m_rLineLength_Sum -= OldLine.GetLength();
    m_rLineLength_Sum += NewLine.GetLength();
    //重新计算最小包围长方体
    CalcEncaseCuboid();
    return true;
}

/*************************************************************************
【函数名称】        ChangeLine
【函数功能】        在指定的Line3D对象中替换一个点
【参数】            const Line3D& Line1：要修改的Line3D对象
                   const Point3D& OldPoint：要替换的点
                   const Point3D& NewPoint：新的点
【返回值】          如果替换成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::ChangeLine(
    const Line3D& Line1, const Point3D& OldPoint, const Point3D& NewPoint){
    //检查旧点是否存在，且新点不存在
    if (!Line1.Points.IsExist(OldPoint) 
        || Line1.Points.IsExist(NewPoint)) {
        return false;
    }
    //创建一个新的Line3D对象替换旧的Line3D对象
    Line3D TempLine{Line1};
    TempLine.ChangePoint(OldPoint, NewPoint);
    //调用已有函数，返回成功与否
    return ChangeLine(Line1, TempLine);
}

/*************************************************************************
【函数名称】        ChangeLine
【函数功能】        在指定的Line3D对象中替换指定索引处的点
【参数】            const Line3D& Line1：要修改的Line3D对象
                   size_t Index：要替换的点的索引
                   const Point3D& NewPoint：新的点
【返回值】          如果替换成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::ChangeLine(
    const Line3D& Line1, size_t Index, const Point3D& NewPoint){
    //调用已有函数，返回成功与否
    return ChangeLine(Line1, Line1.Points[Index], NewPoint);
}

/*************************************************************************
【函数名称】        DeleteLine
【函数功能】        删除模型中的指定Line3D对象
【参数】            const Line3D& Line1：要删除的Line3D对象
【返回值】          如果删除成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::DeleteLine(const Line3D& Line1){
    //查找要删除的Line3D对象
    auto target = LineSearcher(Line1);
    if (target == m_Lines.end()) {
        return false;
    }
    //删除对象
    m_Lines.erase(target);
    //线数减1
    m_ullLineNum--;
    //点数减2
    m_ullPointNum -= 2;
    //元素数减1
    m_ullElementNum--;
    //总线长减去删除线的长度
    m_rLineLength_Sum -= Line1.GetLength();
    //重新计算最小包围长方体
    CalcEncaseCuboid();
    return true;
}

/*************************************************************************
【函数名称】        DeleteLine
【函数功能】        使用两个Point3D对象来删除模型中的指定Line3D对象
【参数】            const Point3D& Point1, const Point3D& Point2：
                   构成Line3D的两个点
【返回值】          如果删除成功，返回true；否则返回false
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
bool Model3D::DeleteLine(const Point3D& Point1, const Point3D& Point2){
    return DeleteLine(Line3D(Point1, Point2));
}

/*************************************************************************
【函数名称】        operator-=
【函数功能】        从模型中删除多个指定的Line3D对象
【参数】            const std::vector<Line3D>& vLines：要删除的Line3D对象集合
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator-=(const std::vector<Line3D>& vLines){
    //循环删除每一个Line3D对象
    for (auto TempLine : vLines) {
        DeleteLine(TempLine);
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator-
【函数功能】        返回一个新模型，新模型由当前模型移除多个Line3D对象后的结果
【参数】            const std::vector<Line3D>& vLines：要删除的Line3D对象集合
【返回值】          移除指定Line3D对象后新创建的Model3D对象
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D Model3D::operator-(const std::vector<Line3D>& vLines) const{
    Model3D Temp{*this};
    //循环删除每一个Line3D对象
    for (auto TempLine : vLines) {
        Temp.DeleteLine(TempLine);
    }
    return Temp;
}

/*************************************************************************
【函数名称】        ClearLines
【函数功能】        删除模型中的所有Line3D对象
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
void Model3D::ClearLines(){
    //清空所有Line3D对象
    m_Lines.clear();
    //线长和清零
    m_rLineLength_Sum = 0.0;
    //元素数减去线数
    m_ullElementNum -= m_ullLineNum;
    //点数减去2倍线数
    m_ullPointNum -= 2 * m_ullLineNum;
    //线数清零
    m_ullLineNum = 0;
    //重新计算最小包围长方体
    CalcEncaseCuboid();
}

//Point3D增删改,在本类不可操作（虚函数，更高级的Model3D派生类可重写）

bool Model3D::AddPoint(const Point3D& APoint){
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

Model3D& Model3D::operator+=(const std::vector<Point3D>& vPoints){
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

Model3D Model3D::operator+(const std::vector<Point3D>& vPoints) const{
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

bool Model3D::ChangePoint(const Point3D& OldPoint, const Point3D& NewPoint){
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

bool Model3D::DeletePoint(const Point3D& APoint){
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

Model3D& Model3D::operator-=(const std::vector<Point3D>& vPoints){
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

Model3D Model3D::operator-(const std::vector<Point3D>& vPoints) const{
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

void Model3D::ClearPoints(){
    //不可在基类操作Point3D
    throw NO_POINT_OPERATE();
}

//模型操作

/*************************************************************************
【函数名称】        ClearAll
【函数功能】        清空模型中的所有Face3D和Line3D对象
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
void Model3D::ClearAll(){
    //清空所有Face3D对象
    ClearFaces();
    //清空所有Line3D对象
    ClearLines();
}

/*************************************************************************
【函数名称】        operator+=
【函数功能】        将另一个Model3D对象中的Face3D和Line3D对象合并到当前模型中
【参数】            const Model3D& Model1：要合并的Model3D对象
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator+=(const Model3D& Model1){
    //循环添加每一个Face3D对象
    for (auto TempFace : Model1.Faces) {
        AddFace(*TempFace);
    }
    //循环添加每一个Line3D对象
    for (auto TempLine : Model1.Lines) {
        AddLine(*TempLine);
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator+
【函数功能】        返回一个新模型，新模型由当前模型和另一个Model3D对象合并组成
【参数】            const Model3D& Model1：要合并的Model3D对象
【返回值】          合并后新创建的Model3D对象
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D Model3D::operator+(const Model3D& Model1) const{
    //创建当前模型的副本
    Model3D Temp{*this};
    //循环添加每一个Face3D对象到副本中
    for (auto TempFace : Model1.Faces) {
        Temp.AddFace(*TempFace);
    }
    //循环添加每一个Line3D对象到副本中
    for (auto TempLine : Model1.Lines) {
        Temp.AddLine(*TempLine);
    }
    return Temp;
}

/*************************************************************************
【函数名称】        operator-=
【函数功能】        从当前模型中移除另一个Model3D对象中的Face3D和Line3D对象
【参数】            const Model3D& Model1：要移除的Model3D对象
【返回值】          当前Model3D对象的引用
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D& Model3D::operator-=(const Model3D& Model1){
    //循环删除每一个Face3D对象
    for (auto TempFace : Model1.Faces) {
        DeleteFace(*TempFace);
    }
    //循环删除每一个Line3D对象
    for (auto TempLine : Model1.Lines) {
        DeleteLine(*TempLine);
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator-
【函数功能】        返回一个新模型，新模型由当前模型移除另一个Model3D对象
                   中的Face3D和Line3D对象后的结果
【参数】            const Model3D& Model1：要移除的Model3D对象
【返回值】          移除指定Face3D和Line3D对象后新创建的Model3D对象
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
Model3D Model3D::operator-(const Model3D& Model1) const{
    //创建当前模型的副本
    Model3D Temp{*this};
    //循环删除每一个Face3D对象
    for (auto TempFace : Model1.Faces) {
        Temp.DeleteFace(*TempFace);
    }
    //循环删除每一个Line3D对象
    for (auto TempLine : Model1.Lines) {
        Temp.DeleteLine(*TempLine);
    }
    return Temp;
}

/*************************************************************************
【函数名称】        FaceSearcher
【函数功能】        查找指定Face3D对象的迭代器
【参数】            const Face3D& Face1：要查找的Face3D对象
【返回值】          如果找到，返回具体迭代器；否则返回尾迭代器
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
std::vector<std::shared_ptr<Face3D>>::iterator Model3D::FaceSearcher(
    const Face3D& Face1){
    //使用Lambda表达式进行查找
    auto Finder =
        [&Face1](std::shared_ptr<Face3D> Ptr)->bool{return *Ptr == Face1;};
    //使用find_if函数进行查找，返回查找结果
    auto Result = find_if(m_Faces.begin(), m_Faces.end(), Finder);
    return Result;
}

/*************************************************************************
【函数名称】        LineSearcher
【函数功能】        查找指定Line3D对象的迭代器
【参数】            const Line3D& Line1：要查找的Line3D对象
【返回值】          如果找到，返回具体迭代器；否则返回尾迭代器
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
std::vector<std::shared_ptr<Line3D>>::iterator Model3D::LineSearcher(
    const Line3D& Line1){
    //使用Lambda表达式进行查找
    auto Finder =
        [&Line1](std::shared_ptr<Line3D>& Ptr)->bool{return *Ptr == Line1;};
    //使用find_if函数进行查找，返回查找结果
    auto Result = find_if(m_Lines.begin(), m_Lines.end(), Finder);
    return Result;
}

/*************************************************************************
【函数名称】        PointSearcher
【函数功能】        查找指定Point3D对象的迭代器
【参数】            const Point3D& Point1：要查找的Point3D对象
【返回值】          如果找到，返回具体迭代器；否则返回尾迭代器
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
std::vector<std::shared_ptr<Point3D>>::iterator Model3D::PointSearcher(
    const Point3D& Point1){
    //使用Lambda表达式进行查找
    auto Finder =
        [&Point1](std::shared_ptr<Point3D> Ptr)->bool{return *Ptr == Point1;};
    //使用find_if函数进行查找，返回查找结果
    auto Result = find_if(m_Points.begin(), m_Points.end(), Finder);
    return Result;
}

/*************************************************************************
【函数名称】        CalcEncaseCuboid
【函数功能】        计算并更新模型的最小包围长方体的尺寸和体积
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/5
【更改记录】         
*************************************************************************/
void Model3D::CalcEncaseCuboid(){
    //如果没有Element(Face3D和Line3D对象)，最小包围长方体尺寸为0
    if (ElementNum == 0) {
        m_rEncaseCuboid_Length = 0.0;
        m_rEncaseCuboid_Width = 0.0;
        m_rEncaseCuboid_Height = 0.0;
        m_rEncaseCuboid_Volume = 0.0;
        return;
    }
    std::vector<Point3D> AllPoints;
    //将所有Face3D对象的点添加到AllPoints
    for (auto & TempFace : Faces) {
        for (size_t i = 0; i < 3; i++) {
            AllPoints.push_back(TempFace->Points[i]);
        }
    }
    //将所有Line3D对象的点添加到AllPoints
    for (auto & TempLine : Lines) {
        for (size_t i = 0; i < 2; i++) {
            AllPoints.push_back(TempLine->Points[i]);
        }
    }

    //定义简化后的迭代器类型
    using PointIt = std::vector<Point3D>::iterator;
    //注：以下3个计算式很长，均拆分为从第2行开始缩进4格的常规形式
    //使用标准算法库找到所有点中X的最大最小值
    std::pair<PointIt, PointIt> MinMax_X = std::minmax_element(
        AllPoints.begin(),
        AllPoints.end(),
        [](const Point3D& Pointlhs, const Point3D& Pointrhs){
        return Pointlhs.GetX() < Pointrhs.GetX();});
    //找到X轴最大最小值的迭代器
    auto Min_X_it = MinMax_X.first;
    auto Max_X_it = MinMax_X.second;

    //使用标准算法库找到所有点中Y的最大最小值
    std::pair<PointIt, PointIt> MinMax_Y = std::minmax_element(
        AllPoints.begin(),
        AllPoints.end(),
        [](const Point3D& Pointlhs, const Point3D& Pointrhs){
        return Pointlhs.GetY() < Pointrhs.GetY();});
    //找到Y轴最大最小值的迭代器
    auto Min_Y_it = MinMax_Y.first;
    auto Max_Y_it = MinMax_Y.second;

    //使用标准算法库找到所有点中Z的最大最小值
    std::pair<PointIt, PointIt> MinMax_Z = std::minmax_element(
        AllPoints.begin(),
        AllPoints.end(),
        [](const Point3D& Pointlhs, const Point3D& Pointrhs){
        return Pointlhs.GetZ() < Pointrhs.GetZ();});
    //找到Z轴最大最小值的迭代器
    auto Min_Z_it = MinMax_Z.first;
    auto Max_Z_it = MinMax_Z.second;

    //更新最小包围长方体的尺寸
    m_rEncaseCuboid_Length = Max_X_it->GetX() - Min_X_it->GetX();
    m_rEncaseCuboid_Width = Max_Y_it->GetY() - Min_Y_it->GetY();
    m_rEncaseCuboid_Height = Max_Z_it->GetZ() - Min_Z_it->GetZ();
    //更新最小包围长方体的面积
    m_rEncaseCuboid_Area 
        = 2 * (m_rEncaseCuboid_Length * m_rEncaseCuboid_Width
        + m_rEncaseCuboid_Width * m_rEncaseCuboid_Height
        + m_rEncaseCuboid_Height * m_rEncaseCuboid_Length);
    //更新最小包围长方体的体积
    m_rEncaseCuboid_Volume 
        = m_rEncaseCuboid_Length 
        * m_rEncaseCuboid_Width 
        * m_rEncaseCuboid_Height;
}
