#pragma once
#include <string>
using namespace std;

class MailAddress {
	string m_Addr;
public:
	static bool Validate(const string& addr); // TODO: 验证邮箱合法性
	MailAddress(const string& addr); // TODO: 验证合法性，然后创建
};