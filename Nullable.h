#pragma once

class NullValueException {};

template <typename T> class Nullable {
  bool m_HasValue = false;
  T m_Value;

public:
  Nullable() {}

  Nullable(T value) { SetValue(value); }

  bool HasValue() const { return m_HasValue; }

  void SetValue(T value) {
    m_HasValue = true;
    m_Value = value;
  }

  const T &GetValue() const {
    if (!m_HasValue)
      throw NullValueException();
    return m_Value;
  }

  void SetNull() {
    if (m_HasValue) {
      m_HasValue = false;
      m_Value.~T();
    }
  }

  ~Nullable() { SetNull(); }
};
