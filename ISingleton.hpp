#pragma once
#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <mutex>


/******************************************************************************/
/**********************ufgnix0802:单例宏定义（线程安全）*************************/
/******************************************************************************/

#define SINGLETON_CLASS_DEFAULT(Class)                                          \
    private:                                                                    \
        Class() {}                                                              \
        ~Class() {}                                                             \
        Class(Class &other) {}                                                  \
        Class(const Class &other) {}                                            \
        Class& operator=(Class &other) {}                                       \
        Class& operator=(const Class &other) {}                                 \


#define SINGLETON_CLASS_FUNC_DECL(Class)                                        \
    public:                                                                     \
        static Class& Ins();                                                    \
    private:                                                                    \
        static void InitSingleton();                                            \
                                                                                \
        static void Release();                                                  \
                                                                                \
        SINGLETON_CLASS_DEFAULT(Class)                                          \
private:                                                                        \
    static std::once_flag m_singletonFlag;                                      \
    static Class* m_ins;

/* https://liam.page/2020/10/27/implement-a-singleton-class-template-in-cxx/ */                         
/* https://liam.page/2017/01/17/layers-and-operation-system/#CPU-%E5%8A%A8%E6%80%81%E8%B0%83%E5%BA%A6 */

#define SINGLETON_CLASS_FUNC_IMPL(Class)                                        \
    Class& Class::Ins() {                                                       \
        /* 使用双重检查，外层检查是为了避免锁住过大的面积，从而导致锁的竞争特别频繁；*/\
        /* 内层检查是为了确保只在其它线程没有抢占锁完成初始化工作而设计。*/           \
        /* 这种做法在Java下是正确的，但是在C++下则没有保证。*/                      \
        /* std::once_flag 和 std::call_once：它们内部利用互斥量和条件变量组合，*/   \
        /* 实现这样的语义。值得一提的是，如果执行过程中抛出异常，标准库的设施不认为 */ \
        /* 这是一次「成功的执行」。于是其他线程可以继续抢占锁来执行函数。 */          \
        /* std:call_once确保函数或代码片段在多线程环境下，只需要执行一次。 */        \
        std::call_once(m_singletonFlag, &Class::InitSingleton);                 \
        assert(m_ins != NULL);                                                  \
        return *m_ins;                                                          \
    }                                                                           \
                                                                                \
    void Class::InitSingleton() {                                               \
        m_ins = new Class;                                                      \
        if (m_ins) {                                                            \
        /* C 库函数 int atexit(void (*func)(void)) 当程序正常终止时，调用指定的*/  \
        /* 函数func。您可以在任何地方注册你的终止函数，*/                           \
        /* 但它会在程序终止的时候被调用。*/                                        \
                ::atexit(Release);                                              \
        }                                                                       \
    }                                                                           \
                                                                                \
    void Class::Release() {                                                     \
         if (m_ins) {                                                           \
              delete m_ins;                                                     \
              m_ins = NULL;                                                     \
         }                                                                      \
    }                                                                   


#define SINGLETON_CLASS_VARIABLE_IMPL(Class)                                    \
        std::once_flag Class::m_singletonFlag;                                  \
        Class* Class::m_ins = NULL;


#define SINGLETON_CLASS_IMPL(Class)                                             \
        SINGLETON_CLASS_VARIABLE_IMPL(Class)                                    \
        SINGLETON_CLASS_FUNC_IMPL(Class)                                        