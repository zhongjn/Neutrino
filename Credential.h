#pragma once
#include <string>
#include "MailAddress.h"
using namespace std;

class Credential {
	MailAddress m_Addr;
	string m_Username;
	string m_Password;
	// TODO: 其他字段，对接网络邮箱用
public:
	// TOOD: 访问函数
};