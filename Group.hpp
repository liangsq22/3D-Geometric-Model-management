/*************************************************************************
【文件名】                 Group.hpp
【功能模块和目的】         通用集合类模版
【开发者及日期】           梁思奇 2024/7/30
【更改记录】              梁思奇 2024/8/5 参考范老师Demo的Set进行大改
*************************************************************************/

#ifndef GROUP_HPP
#define GROUP_HPP

//size_t、SIZE_MAX所属头文件
#include <cstddef>
//std::invalid_argument、std::out_of_range所属头文件
#include <stdexcept>
//std::string所属头文件
#include <string>
//std::vector所属头文件
#include <vector>
//std::find所属头文件
#include <algorithm>

/*************************************************************************
【类模版名】          Group
【功能】             通用集合类模版
【接口说明】         包含通用集合类的构造、析构、赋值函数，
                    重载了集合的交、并、差、异或、加、减、赋值运算符，
                    重载了集合的[]、==、!=、IsExist、IsEmpty等运算符，
                    包含了集合的增、删、改、清空等操作
【开发者及日期】      梁思奇 2024/7/30
【更改记录】         梁思奇 2024/8/5 参考范老师Demo的Set进行大改
*************************************************************************/
template<class T>
class Group{
public:
    //内嵌异常类组
    //异常：加入重复元素
    class REPEAT_ELEMENT : std::invalid_argument{
    public:
        REPEAT_ELEMENT(): std::invalid_argument(
            std::string("New element already exists in the group")){
        }
    };

    //异常：索引超出范围
    class INDEX_OUT_OF_RANGE : std::out_of_range{
    public:
        INDEX_OUT_OF_RANGE(): std::out_of_range(
            std::string("Index out of range")){
        }
    };

    //异常：集合元素个数最大值为0
    class MAXSIZE_0 : std::invalid_argument{
    public:
        MAXSIZE_0(): std::invalid_argument(
            std::string("Group's max size must be > 0")){
        }
    };

    //异常：集合已满，无法添加新元素
    class FULL_GROUP : std::out_of_range{
    public:
        FULL_GROUP(): std::out_of_range(
            std::string("Group is full")){
        }
    };
    
    //异常：集合中没有指定元素
    class ELEMENT_NOT_FOUND : std::invalid_argument{
    public:
        ELEMENT_NOT_FOUND(): std::invalid_argument(
            std::string("This element not found in the group")){
        }
    };

    //异常：容量小于当前元素个数
    class NEW_SIZE_LIMIT : std::invalid_argument{
    public:
        NEW_SIZE_LIMIT(): std::invalid_argument(
            std::string("New size is smaller than elements' number")){
        }
    };

public:
    //默认构造函数
    Group();
    //拷贝构造函数
    Group(const Group& Source);
    //带参蓝图构造函数
    Group(const std::vector<T>& Source);
    //带参数构造函数
    Group(size_t MaxSize);
    //虚析构函数（通用类可做基类）
    virtual ~Group() = default;

    //Setter
    //赋值运算符
    Group& operator=(const Group& Source);
    //取和集（并集）
    Group& operator+=(const Group& Group1);
    //取差集
    Group& operator-=(const Group& Group1);
    //取交集
    Group& operator&=(const Group& Group1);
    //取异或集
    Group& operator^=(const Group& Group1);
    //改对应索引元素
    T& operator[](size_t Index);
    //改容量大小
    void ReSize(size_t NewSize);
    //添加元素
    void Add(const T& Element);
    //修改元素
    void Change(const T& OldElement, const T& NewElement);
    //删除元素
    void Remove(const T& Element);
    //清空集合
    void EraseAll();
    //友元函数：流提取运算符（输入）
    template<class U>
    friend std::istream& operator>>(
        std::istream& Stream, Group<U>& Group1);

    //Getter
    //和集（并集）
    Group operator+(const Group& Group1) const;    
    //差集
    Group operator-(const Group& Group1) const;
    //交集
    Group operator&(const Group& Group1) const;
    //异或集
    Group operator^(const Group& Group1) const;    
    //判断是否相等
    bool operator==(const Group& Group1) const;
    //判断是否不等
    bool operator!=(const Group& Group1) const;
    //取元素值
    T operator[](size_t Index) const;    
    //判断元素是否存在
    bool IsExist(const T& Element) const;
    //判断集合是否为空
    bool IsEmpty() const;
    //获取集合元素个数
    size_t GetCountSize() const;
    //友元函数：流插入运算符（输出）
    template<class U>
    friend std::ostream& operator<<(
        std::ostream& Stream, const Group<U>& Group1);

    //非静态Getter    
    //获取集合元素个数最大值
    const size_t& MaxSize{m_MaxSize};
    //静态数据成员：默认最大元素个数容量
    static constexpr size_t DEFAULT_MAXSIZE{SIZE_MAX - 1};

private:
    //数据成员：集合元素
    std::vector<T> m_Elements;
    //数据成员：集合最大元素个数容量
    size_t m_MaxSize;
};

/*************************************************************************
【函数名称】        Group
【函数功能】        Group模版默认构造函数
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>::Group() : m_Elements{}, m_MaxSize(DEFAULT_MAXSIZE){
    //检查最大容量是否为0
    if (m_MaxSize == 0) {
        throw MAXSIZE_0();
    }
}

/*************************************************************************
【函数名称】        Group
【函数功能】        Group模版拷贝构造函数
【参数】            const Group<T>& Source：另一个Group对象的引用
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>::Group(const Group<T>& Source)
    : m_Elements(Source.m_Elements), m_MaxSize(Source.m_MaxSize){
}

/*************************************************************************
【函数名称】        Group
【函数功能】        Group模版带参蓝图构造函数
【参数】            const std::vector<T>& Source：初始元素的向量
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>::Group(const std::vector<T>& Source)
    : m_Elements{}, m_MaxSize(Source.size()){
    //检查最大容量是否为0    
    if (m_MaxSize == 0) {
        throw MAXSIZE_0();
    }
    //遍历添加元素
    for (const auto& Element : Source) {
        if (!IsExist(Element)) {
            m_Elements.push_back(Element);
        }
    }
    //更新最大容量并检查是否为0
    m_MaxSize = m_Elements.size();
    if (m_MaxSize == 0) {
        throw MAXSIZE_0();
    }
}

/*************************************************************************
【函数名称】        Group
【函数功能】        Group模版带参数构造函数
【参数】            size_t MaxSize：集合的最大元素个数
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>::Group(size_t MaxSize) : m_Elements{}, m_MaxSize(MaxSize){
    //检查最大容量是否为0
    if (m_MaxSize == 0) {
        throw MAXSIZE_0();
    }
}

//Setter函数实现

/*************************************************************************
【函数名称】        operator=
【函数功能】        赋值运算符Setter，将另一个Group对象的内容赋值给当前对象
【参数】            const Group<T>& Source：另一个Group对象的引用
【返回值】          当前Group对象的引用
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>& Group<T>::operator=(const Group<T>& Source){
    //检查自赋值
    if (this != &Source) {
        m_Elements = Source.m_Elements;
        m_MaxSize = Source.m_MaxSize;
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator+=
【函数功能】        取和集（并集）Setter，将另一个集合中的元素添加到当前集合
【参数】            const Group& Group1：另一个集合的引用
【返回值】          当前集合的引用Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>& Group<T>::operator+=(const Group& Group1){
    //遍历另一个集合的元素，不在当前集合中的元素添加到当前集合
    for (auto Element : Group1.m_Elements) {
        if (!m_Elements.IsExist(Element)) {
            m_Elements.Add(Element);
        }
    }
    return *this;
}

/*************************************************************************
【函数名称】        operator-=
【函数功能】        取差集Setter，将当前集合中存在于另一个集合中的元素移除
【参数】            const Group& Group1：另一个集合的引用
【返回值】          当前集合的引用Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>& Group<T>::operator-=(const Group& Group1){
    //临时集合
    std::vector<T> Temp;
    //遍历当前集合元素，不在另一个集合中的元素添加到临时集合
    for (const auto& Element : m_Elements) {
        if (!Group1.IsExist(Element)) {
            Temp.push_back(Element);
        }
    }
    //将临时集合赋值给当前集合
    m_Elements = std::move(Temp);
    return *this;
}

/*************************************************************************
【函数名称】        operator&=
【函数功能】        取交集Setter，只保留当前集合中存在于另一个集合中的元素
【参数】            const Group& Group1：另一个集合的引用
【返回值】          当前集合的引用Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>& Group<T>::operator&=(const Group& Group1){
    //临时集合
    std::vector<T> Temp;
    //遍历当前集合元素，存在于另一个集合中的元素添加到临时集合
    for (const auto& Element : Group1.m_Elements) {
        if (IsExist(Element)) {
            Temp.push_back(Element);
        }
    }
    //将临时集合赋值给当前集合
    m_Elements = std::move(Temp);
    return *this;
}

/*************************************************************************
【函数名称】        operator^=
【函数功能】        取异或集Setter，保留当前集合中不存在于另一个集合中的元素
【参数】            const Group& Group1：另一个集合的引用
【返回值】          当前集合的引用Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T>& Group<T>::operator^=(const Group& Group1){
    //创建临时副本以保存当前集合
    Group<T> TempCopy = *this;
    //计算并集
    *this += Group1;
    //计算交集
    TempCopy &= Group1;
    //将并集减去交集
    *this -= TempCopy;
    return *this;
}

/*************************************************************************
【函数名称】        operator[]
【函数功能】        改变指定索引的元素Setter
【参数】            size_t Index：元素的索引
【返回值】          元素的引用T&
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
T& Group<T>::operator[](size_t Index){
    //检查索引是否越界
    if (Index >= m_Elements.size()) {
        throw INDEX_OUT_OF_RANGE();
    }
    return m_Elements[Index];
}

/*************************************************************************
【函数名称】        ReSize
【函数功能】        修改集合的最大容量Setter
【参数】            size_t NewSize：新的最大容量
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
void Group<T>::ReSize(size_t NewSize){
    //检查新容量是否为0
    if (NewSize == 0) {
        throw MAXSIZE_0();
    }
    //检查新容量是否小于当前元素个数
    else if (NewSize < m_Elements.size()) {
        throw NEW_SIZE_LIMIT();
    }
    m_MaxSize = NewSize;
}

/*************************************************************************
【函数名称】        Add
【函数功能】        向集合中添加元素Setter
【参数】            const T& Element：要添加的元素
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
void Group<T>::Add(const T& Element){
    //检查集合是否已满
    if (m_Elements.size() == m_MaxSize) {
        throw FULL_GROUP();
    }
    //检查元素是否已存在
    else if (IsExist(Element)) {
        throw REPEAT_ELEMENT();
    }
    //若无异常则添加元素
    m_Elements.push_back(Element);
}

/*************************************************************************
【函数名称】        Change
【函数功能】        修改集合中的元素Setter
【参数】            const T& OldElement：要修改的旧元素
                   const T& NewElement：新的元素值
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
void Group<T>::Change(const T& OldElement, const T& NewElement){
    //检查新元素是否已存在
    if (IsExist(NewElement)) {
        throw REPEAT_ELEMENT();
    }
    //查找旧元素并替换
    auto Target 
    = std::find(m_Elements.begin(), m_Elements.end(), OldElement);
    if (Target != m_Elements.end()) {
        *Target = NewElement;
    } 
    //未找到旧元素则抛出异常
    else {
        throw ELEMENT_NOT_FOUND();
    }
}

/*************************************************************************
【函数名称】        Remove
【函数功能】        删除集合中的指定元素Setter
【参数】            const T& Element：要删除的元素
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
void Group<T>::Remove(const T& Element){
    //查找元素并删除
    auto Target 
    = std::find(m_Elements.begin(), m_Elements.end(), Element);
    if (Target != m_Elements.end()) {
        m_Elements.erase(Target);
    } 
    //未找到元素则抛出异常
    else{
        throw ELEMENT_NOT_FOUND();
    }
}

/*************************************************************************
【函数名称】        EraseAll
【函数功能】        清空集合Setter
【参数】            无
【返回值】          无
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
void Group<T>::EraseAll(){
    m_Elements.clear();
}

/*************************************************************************
【函数名称】        operator>>
【函数功能】        流提取运算符（输入），用于从流中读取数据到Group对象
【参数】            std::istream& Stream：输入流
                   Group<U>& Group1：要赋值的Group对象
【返回值】          输入流的引用std::istream&
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class U>
std::istream& operator>>(std::istream& Stream, Group<U>& Group1){
    U Element;
    //清空当前的Group
    Group1.EraseAll(); 
    char Ch;
    //读取 '{'
    Stream >> Ch; 
    while (Stream >> Element) {
        Group1.Add(Element);
        //读取元素后的空格或 '}'
        Stream >> Ch; 
        if (Ch == '}') {
            break;
        }
    }
    return Stream;
}

//Getter函数实现

/*************************************************************************
【函数名称】        operator+
【函数功能】        返回当前集合与另一个集合的并集Getter
【参数】            const Group& Group1：另一个集合
【返回值】          并集后的集合Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T> Group<T>::operator+(const Group& Group1) const{
    //创建临时集合
    Group<T> Temp{*this};
    //将Group1的元素逐一添加到Temp中，如果不存在就添加
    for (const auto& Element : Group1.m_Elements) {
        if (!Temp.IsExist(Element)) {
            Temp.m_Elements.push_back(Element);
        }
    }
    return Temp;
}

/*************************************************************************
【函数名称】        operator-
【函数功能】        返回当前集合与另一个集合的差集Getter
【参数】            const Group& Group1：另一个集合
【返回值】          差集后的集合Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T> Group<T>::operator-(const Group& Group1) const{
    //创建临时集合
    Group<T> Temp;
    //将当前集合中的元素添加到Temp中，如果不在Group1中就保留
    for (const auto& Element : m_Elements) {
        if (!Group1.IsExist(Element)) {
            Temp.m_Elements.push_back(Element);
        }
    }
    return Temp;
}

/*************************************************************************
【函数名称】        operator&
【函数功能】        返回当前集合与另一个集合的交集Getter
【参数】            const Group& Group1：另一个集合
【返回值】          交集后的集合Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T> Group<T>::operator&(const Group& Group1) const{
    //创建临时集合
    Group<T> Temp;
    //只保留当前集合中同时存在于Group1中的元素
    for (const auto& Element : m_Elements) {
        if (Group1.IsExist(Element)) {
            Temp.m_Elements.push_back(Element);
        }
    }
    return Temp;
}

/*************************************************************************
【函数名称】        operator^
【函数功能】        返回当前集合与另一个集合的异或集Getter
【参数】            const Group& Group1：另一个集合
【返回值】          异或集后的集合Group
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
Group<T> Group<T>::operator^(const Group& Group1) const{
    //计算并集
    Group<T> Union = *this + Group1;
    //计算交集
    Group<T> Intersection = *this & Group1;
    //异或集 = 并集 - 交集
    return (Union - Intersection);
}

/*************************************************************************
【函数名称】        operator[]
【函数功能】        取元素值Getter，根据指定的索引返回集合中的元素值
【参数】            size_t Index：元素的索引
【返回值】          指定索引处的元素值T
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
T Group<T>::operator[](size_t Index) const{
    //检查索引是否越界
    if (Index >= m_Elements.size()) {
        throw INDEX_OUT_OF_RANGE();
    }
    return m_Elements[Index];
}

/*************************************************************************
【函数名称】        operator==
【函数功能】        判断两个集合是否相等Getter
【参数】            const Group& Group1：另一个集合
【返回值】          如果相等，返回true；否则返回false
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
bool Group<T>::operator==(const Group& Group1) const{
    //如果两个集合元素个数不同，直接返回false
    if (m_Elements.size() != Group1.m_Elements.size()) {
        return false;
    }
    //遍历当前集合元素，任意一个不在另一集合中就返回false
    for (const auto& Element : m_Elements) {
        if (!Group1.IsExist(Element)) {
            return false;
        }
    }
    //完全相同则返回true
    return true;
}

/*************************************************************************
【函数名称】        operator!=
【函数功能】        判断两个集合是否不相等Getter
【参数】            const Group& Group1：另一个集合
【返回值】          如果不相等，返回true；否则返回false
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
bool Group<T>::operator!=(const Group& Group1) const{
    //调用==运算符取反
    return !(*this == Group1);
}

/*************************************************************************
【函数名称】        IsExist
【函数功能】        判断集合中是否存在指定元素Getter
【参数】            const T& Element：要查找的元素
【返回值】          如果存在，返回true；否则返回false
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
bool Group<T>::IsExist(const T& Element) const{
    return std::find(m_Elements.begin(), m_Elements.end(), Element) 
        != m_Elements.end();
}

/*************************************************************************
【函数名称】        IsEmpty
【函数功能】        判断集合是否为空Getter
【参数】            无
【返回值】          如果集合为空，返回true；否则返回false
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
bool Group<T>::IsEmpty() const{
    return m_Elements.empty();
}

/*************************************************************************
【函数名称】        operator<<
【函数功能】        流插入运算符（输出），用于将Group对象输出到流
【参数】            std::ostream& Stream：输出流
                   const Group<U>& Group1：要输出的Group对象
【返回值】          输出流的引用std::ostream&
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class U>
std::ostream& operator<<(std::ostream& Stream, const Group<U>& Group1){
    Stream << "{ ";
    //每输出一元素后接空格
    for (const auto& Element : Group1.m_Elements) {
        Stream << Element << " ";
    }
    Stream << "}";
    return Stream;
}

/*************************************************************************
【函数名称】        GetCountSize
【函数功能】        获取集合元素个数Getter
【参数】            无
【返回值】          集合元素个数size_t
【开发者及日期】    梁思奇 2024/7/30
【更改记录】
*************************************************************************/
template<class T>
size_t Group<T>::GetCountSize() const{
    return m_Elements.size();
}

#endif //GROUP_HPP
