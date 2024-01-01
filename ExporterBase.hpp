/*************************************************************************
【文件名】                 ExporterBase.hpp
【功能模块和目的】          导出器基类模版声明与实现
【开发者及日期】            梁思奇 2024/8/3
【更改记录】               梁思奇 2024/8/7 参考范老师Demo进行代码结构调整
*************************************************************************/

#ifndef EXPORTERBASE_HPP
#define EXPORTERBASE_HPP

//std::shared_ptr所属头文件
#include <memory>
//std::string、find_last_of所属头文件
#include <string>
//std::ofstream所属头文件
#include <fstream>
//std::vector所属头文件
#include <vector>
//std异常基类所属头文件
#include <stdexcept>

/*************************************************************************
【类名】             ExporterBase
【功能】             导出器基类模版
【接口说明】         提供文件导出接口及派生接口，派生添加具体导出器类函数，
                    并根据文件名获取派生出的导出器实例指针
【开发者及日期】      梁思奇 2024/8/3
【更改记录】         梁思奇 2024/8/7 参考范老师Demo进行代码结构调整
*************************************************************************/
template<class T>
class ExporterBase{
protected:
    //默认构造函数删除，避免无扩展名
    ExporterBase() = delete;
    //带参构造函数，初始化扩展名
    ExporterBase(const std::string& FileSuffix);
    //拷贝构造函数删除，避免拷贝
    ExporterBase(const ExporterBase& Source) = delete;
    //赋值运算符删除，避免拷贝
    ExporterBase& operator=(const ExporterBase& Source) = delete;
    //虚析构函数，默认实现
    virtual ~ExporterBase() = default;

public:
    //导出函数接口
    //将T类型对象导出到文件
    void ExportToFile(const std::string& FileName, const T& T1) const;
    //从T类型对象导出到文件流接口，纯虚函数，需要派生类实现
    virtual void ExportToStream(
        std::ofstream& Stream, const T& T1) const = 0;

    //静态函数接口
    //派生添加具体导出器类实例
    template<class DERIVED>
    static void AddExporter();
    //根据指定文件名，获取派生导出器实例指针
    static std::shared_ptr<ExporterBase<T>> GetExporter(
        const std::string& FileName);

    //数据成员常引用：文件扩展名尾缀
    const std::string& FileSuffix{m_FileSuffix};

private:
    //可导出的文件扩展名尾缀
    std::string m_FileSuffix;
    //静态数据成员：派生导出器实例指针列表
    static std::vector<std::shared_ptr<ExporterBase<T>>> m_pExporters;

public:
    //内嵌：文件导出异常类
    class FAIL_TO_EXPORT : public std::invalid_argument {
    public:
        FAIL_TO_EXPORT(): std::invalid_argument(
            std::string("Fail to export, ")
            + std::string("please check the file type or file path")){
        }
    };
};

//私有、静态数据成员初始化
template<class T>
std::vector<std::shared_ptr<ExporterBase<T>>> ExporterBase<T>::m_pExporters{
};

//函数实现部分

/*************************************************************************
【函数名称】        ExporterBase
【函数功能】        带参构造函数，初始化扩展名
【参数】            const std::string& FileSuffix
【返回值】          无
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
ExporterBase<T>::ExporterBase(const std::string& FileSuffix)
    : m_FileSuffix(FileSuffix){
    //验证扩展名是否为空
    if (FileSuffix == "") {
        throw FAIL_TO_EXPORT();
    }
}

/*************************************************************************
【函数名称】        ExportToFile
【函数功能】        将T类型对象导出到文件
【参数】            const std::string& FileName, const T& T1
【返回值】          无
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
void ExporterBase<T>::ExportToFile(
    const std::string& FileName, const T& T1) const{
    //提取扩展名
    size_t Position = FileName.find_last_of('.');
    std::string ExtInFileName 
        = (Position == std::string::npos || Position == 0) 
        ? std::string{""} : FileName.substr(Position + 1);
    //验证扩展名
    if (ExtInFileName != m_FileSuffix) {
        throw FAIL_TO_EXPORT();
    }
    //验证文件是否可以打开
    std::ofstream File(FileName);
    if (!File.is_open()) {
        throw FAIL_TO_EXPORT();
    }
    //将数据保存到文件流中
    ExportToStream(File, T1);
    //关闭文件流
    File.close();
}

/*************************************************************************
【函数名称】        AddExporter
【函数功能】        添加具体导出器类
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
template<class DERIVED>
void ExporterBase<T>::AddExporter(){
    //验证是否派生自ExporterBase<T>
    static_assert(std::is_base_of<ExporterBase<T>, DERIVED>::value, 
        "Must be derived from ExporterBase<T>");
    //创建导出器实例指针
    std::shared_ptr<ExporterBase<T>> PDerivedExporter(new DERIVED());
    //验证是否已经存在
    for (auto PExporter : m_pExporters) {
        if (PExporter->FileSuffix == PDerivedExporter->FileSuffix) {
            throw FAIL_TO_EXPORT();
        }
    }
    //添加具体导出器类
    m_pExporters.push_back(PDerivedExporter);
}

/*************************************************************************
【函数名称】        GetExporter
【函数功能】        根据指定文件名，获取导出器实例指针
【参数】            const std::string& FileName
【返回值】          std::shared_ptr<ExporterBase<T>> 导出器实例指针
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
std::shared_ptr<ExporterBase<T>> ExporterBase<T>::GetExporter(
    const std::string& FileName){
    //提取扩展名，验证是否为空
    size_t Position = FileName.find_last_of('.');
    std::string ExtInFileName 
        = (Position == std::string::npos || Position == 0) 
        ? std::string{""} : FileName.substr(Position + 1);
    //遍历导出器实例指针列表
    for (auto PExporter : m_pExporters) {
        //返回匹配的导出器实例指针
        if (PExporter->FileSuffix == ExtInFileName) {
            return PExporter;
        }
    }
    //未找到匹配的导出器实例指针，抛出异常
    throw FAIL_TO_EXPORT();
}

#endif //EXPORTERBASE_HPP
