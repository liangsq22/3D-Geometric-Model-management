/*************************************************************************
【文件名】                 ImporterBase.hpp
【功能模块和目的】          导入器基类模版声明与实现
【开发者及日期】            梁思奇 2024/8/3
【更改记录】               梁思奇 2024/8/7 参考范老师Demo进行代码结构调整
*************************************************************************/

#ifndef IMPORTERBASE_HPP
#define IMPORTERBASE_HPP

//std::shared_ptr所属头文件
#include <memory>
//std::string、find_last_of所属头文件
#include <string>
//std::ifstream所属头文件
#include <fstream>
//std::vector所属头文件
#include <vector>
//std::invalid_argument所属头文件
#include <stdexcept>

/*************************************************************************
【类名】             ImporterBase
【功能】             导入器基类模版
【接口说明】         提供文件导入接口及派生接口，派生添加具体导入器类函数，
                    并根据文件名获取派生出的导入器实例指针
【开发者及日期】      梁思奇 2024/8/3
【更改记录】         梁思奇 2024/8/7 参考范老师Demo进行代码结构调整
*************************************************************************/
template<class T>
class ImporterBase{
protected:
    //默认构造函数删除，避免无扩展名
    ImporterBase() = delete;
    //带参构造函数，初始化扩展名
    ImporterBase(const std::string& FileSuffix);
    //拷贝构造函数删除，避免拷贝
    ImporterBase(const ImporterBase& Source) = delete;
    //虚析构函数，默认实现
    virtual ~ImporterBase() = default;
    //赋值运算符删除，避免拷贝
    ImporterBase& operator=(const ImporterBase& Source) = delete;

public:
    //导入函数接口
    //从文件导入到T类型对象
    T ImportFromFile(const std::string& FileName) const;
    //从文件导入给T1(T类型)对象
    void ImportFromFile(const std::string& FileName, T& T1) const;
    //从文件流导入到T类型对象接口，纯虚函数，需要派生类实现
    virtual T ImportFromStream(std::ifstream& Stream) const = 0;

    //静态函数接口
    //派生添加具体导入器类实例
    template<class DERIVED>
    static void AddImporter();
    //根据指定文件名，获取派生导入器实例指针
    static std::shared_ptr<ImporterBase<T>> GetImporter
        (const std::string& FileName);
    
    //数据成员常引用：文件扩展名尾缀
    const std::string& FileSuffix{m_sFileSuffix}; 

private:
    //可导入的文件扩展名尾缀
    std::string m_sFileSuffix;  
    //静态数据成员：派生导入器实例指针列表
    static std::vector<std::shared_ptr<ImporterBase<T>>> m_pImporters;

public:
    //内嵌：文件导入异常类
    class FAIL_TO_IMPORT : public std::invalid_argument{
    public:
        FAIL_TO_IMPORT(): std::invalid_argument(
            std::string("Fail to import, ")
            + std::string("please check the file type or file path")){
        }
    };
};

//私有、静态数据成员初始化
template<class T>
std::vector<std::shared_ptr<ImporterBase<T>>> ImporterBase<T>::m_pImporters{
};

//函数实现部分

/*************************************************************************
【函数名称】        ImporterBase
【函数功能】        带参构造函数，初始化扩展名
【参数】            const std::string& FileSuffix
【返回值】          无
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
ImporterBase<T>::ImporterBase(const std::string& FileSuffix)
    : m_sFileSuffix(FileSuffix){
    //验证扩展名是否为空
    if (FileSuffix == "") {
        throw FAIL_TO_IMPORT();
    }
}

/*************************************************************************
【函数名称】        ImportFromFile
【函数功能】        从文件导入到T类型对象
【参数】            const std::string& FileName
【返回值】          T类型对象
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
T ImporterBase<T>::ImportFromFile(const std::string& FileName) const{
    //提取扩展名
    size_t Position = FileName.find_last_of('.');
    std::string ExtInFileName 
        = (Position == std::string::npos || Position == 0) 
        ? std::string{""} : FileName.substr(Position + 1);
    //验证扩展名
    if (ExtInFileName != m_sFileSuffix) {
        throw FAIL_TO_IMPORT();
    }
    //验证文件是否可以打开
    std::ifstream File(FileName);
    if (!File.is_open()) {
        throw FAIL_TO_IMPORT();
    }
    //从文件流中加载数据
    return ImportFromStream(File);
}

/*************************************************************************
【函数名称】        ImportFromFile
【函数功能】        从文件导入给T类型对象T1
【参数】            const std::string& FileName, T& T1
【返回值】          无
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
void ImporterBase<T>::ImportFromFile(
    const std::string& FileName, T& T1) const{
    //调用导入函数
    T Temp = ImportFromFile(FileName);
    T1 = Temp;
}

/*************************************************************************
【函数名称】        AddImporter
【函数功能】        添加具体导入器类
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
template<class DERIVED>
void ImporterBase<T>::AddImporter(){
    //验证是否派生自ImporterBase<T>
    static_assert(std::is_base_of<ImporterBase<T>, DERIVED>::value, 
        "Must be derived from ImporterBase<T>");
    //创建导入器实例指针
    std::shared_ptr<ImporterBase<T>> PDerivedImporter(new DERIVED());
    //验证是否已经存在
    for (auto PImporter : m_pImporters) {
        if (PImporter->FileSuffix == PDerivedImporter->FileSuffix) {
            throw FAIL_TO_IMPORT();
        }
    }
    //添加具体导入器类
    m_pImporters.push_back(PDerivedImporter);
}

/*************************************************************************
【函数名称】        GetImporter
【函数功能】        根据指定文件名，获取导入器实例指针
【参数】            const std::string& FileName
【返回值】          std::shared_ptr<ImporterBase<T>> 导入器实例指针
【开发者及日期】    梁思奇 2024/8/3
【更改记录】         
*************************************************************************/
template<class T>
std::shared_ptr<ImporterBase<T>> ImporterBase<T>::GetImporter(
    const std::string& FileName){
    //提取扩展名，验证是否为空
    size_t Position = FileName.find_last_of('.');
    std::string ExtInFileName 
        = (Position == std::string::npos || Position == 0) 
        ? std::string{""} : FileName.substr(Position + 1);
    //遍历导入器实例指针列表
    for (auto PImporter : m_pImporters) {
        //返回匹配的导入器实例指针
        if (PImporter->FileSuffix == ExtInFileName) {
            return PImporter;
        }
    }
    //未找到匹配的导入器实例指针，抛出异常
    throw FAIL_TO_IMPORT();
}

#endif //IMPORTERBASE_HPP
