#pragma once
#include <string>
using namespace std;

class MailAddress {
	string m_Addr;
public:
	static bool Validate(const string& addr); // TODO: ��֤����Ϸ���
	MailAddress(const string& addr); // TODO: ��֤�Ϸ��ԣ�Ȼ�󴴽�
};