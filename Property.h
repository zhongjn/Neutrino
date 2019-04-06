#pragma once
// �������Ե���д
// �����ڲ���ʼ��ʹ��

// �������ṩһ������
#define PROPERTY(type, name)                                                   \
private:                                                                       \
  type m_##name;                                                               \
public:                                                                        \
  void Set##name(type _##name) { m_##name = _##name; }                         \
  const type &Get##name() { return m_##name; }                                 \
private:

// �������ṩһ��ֻ������
#define PROPERTY_READ(type, name)                                              \
private:                                                                       \
  type m_##name;                                                               \
public:                                                                        \
  const type &Get##name() { return m_##name; }                                 \
private:

// �������ṩһ��ֻд����
#define PROPERTY_WRITE(type, name)                                             \
private:                                                                       \
  type m_##name;                                                               \
public:                                                                        \
  void Set##name(type _##name) { m_##name = _##name; }                         \
private: