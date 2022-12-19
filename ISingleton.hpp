#pragma once
#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <mutex>


/******************************************************************************/
/**********************ufgnix0802:�����궨�壨�̰߳�ȫ��*************************/
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
        /* ʹ��˫�ؼ�飬�������Ϊ�˱�����ס�����������Ӷ��������ľ����ر�Ƶ����*/\
        /* �ڲ�����Ϊ��ȷ��ֻ�������߳�û����ռ����ɳ�ʼ����������ơ�*/           \
        /* ����������Java������ȷ�ģ�������C++����û�б�֤��*/                      \
        /* std::once_flag �� std::call_once�������ڲ����û�����������������ϣ�*/   \
        /* ʵ�����������塣ֵ��һ����ǣ����ִ�й������׳��쳣����׼�����ʩ����Ϊ */ \
        /* ����һ�Ρ��ɹ���ִ�С������������߳̿��Լ�����ռ����ִ�к����� */          \
        /* std:call_onceȷ�����������Ƭ���ڶ��̻߳����£�ֻ��Ҫִ��һ�Ρ� */        \
        std::call_once(m_singletonFlag, &Class::InitSingleton);                 \
        assert(m_ins != NULL);                                                  \
        return *m_ins;                                                          \
    }                                                                           \
                                                                                \
    void Class::InitSingleton() {                                               \
        m_ins = new Class;                                                      \
        if (m_ins) {                                                            \
        /* C �⺯�� int atexit(void (*func)(void)) ������������ֹʱ������ָ����*/  \
        /* ����func�����������κεط�ע�������ֹ������*/                           \
        /* �������ڳ�����ֹ��ʱ�򱻵��á�*/                                        \
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