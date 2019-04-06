#pragma once
// 简化类属性的书写
// 在类内部开始处使用

// 在类内提供一个属性
#define PROPERTY(type, name)                                                   \
private:                                                                       \
  type m_##name;                                                               \
public:                                                                        \
  void Set##name(type _##name) { m_##name = _##name; }                         \
  const type &Get##name() { return m_##name; }                                 \
private:

// 在类内提供一个只读属性
#define PROPERTY_READ(type, name)                                              \
private:                                                                       \
  type m_##name;                                                               \
public:                                                                        \
  const type &Get##name() { return m_##name; }                                 \
private:

// 在类内提供一个只写属性
#define PROPERTY_WRITE(type, name)                                             \
private:                                                                       \
  type m_##name;                                                               \
public:                                                                        \
  void Set##name(type _##name) { m_##name = _##name; }                         \
private: