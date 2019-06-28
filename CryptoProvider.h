#pragma once

#include "AES.h"
#include "Base64.h"
#include <Windows.h>
#include <wincrypt.h>
#include <string>
#include <stdexcept>
#pragma comment(lib,"crypt32.lib")

using namespace std;

class CryptoProvider {
public:
    virtual string Encrypt(const string& str) = 0;
    virtual string Decrypt(const string& str) = 0;
};

class AES256CryptoProvider : public CryptoProvider {
    string _key = "asdfwetyhjuytrfd";
    string _iv = "gfdertfghjkuyrtgasd";
public:
    explicit AES256CryptoProvider(const string& key) {
        _key = key + "aaasdasdjhasldhasjkdhas";
    }
    string Encrypt(const string& src) {
        size_t length = src.length();
        int block_num = length / BLOCK_SIZE + 1;
        //明文
        char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
        memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
        strcpy(szDataIn, src.c_str());

        //进行PKCS7Padding填充。
        int k = length % BLOCK_SIZE;
        int j = length / BLOCK_SIZE;
        int padding = BLOCK_SIZE - k;
        for (int i = 0; i < padding; i++)
        {
            szDataIn[j * BLOCK_SIZE + k + i] = padding;
        }
        szDataIn[block_num * BLOCK_SIZE] = '\0';

        //加密后的密文
        char* szDataOut = new char[block_num * BLOCK_SIZE + 1];
        memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

        //进行进行AES的CBC模式加密
        AES aes;
        aes.MakeKey(_key.c_str(), _iv.c_str(), 16, 16);
        aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
        string str = base64_encode((unsigned char*)szDataOut,
            block_num * BLOCK_SIZE);
        delete[] szDataIn;
        delete[] szDataOut;
        return str;
    }

    string Decrypt(const string & str) {
        string strData = base64_decode(str);
        size_t length = strData.length();
        //密文
        char* szDataIn = new char[length + 1];
        memcpy(szDataIn, strData.c_str(), length + 1);
        //明文
        char* szDataOut = new char[length + 1];
        memcpy(szDataOut, strData.c_str(), length + 1);

        //进行AES的CBC模式解密
        AES aes;
        aes.MakeKey(_key.c_str(), _iv.c_str(), 16, 16);
        aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

        //去PKCS7Padding填充
        if (0x00 < szDataOut[length - 1] <= 0x16)
        {
            int tmp = szDataOut[length - 1];
            for (int i = length - 1; i >= length - tmp; i--)
            {
                if (szDataOut[i] != tmp)
                {
                    memset(szDataOut, 0, length);
                    throw logic_error("解密失败");
                    break;
                }
                else
                    szDataOut[i] = 0;
            }
        }
        string strDest(szDataOut);
        delete[] szDataIn;
        delete[] szDataOut;
        return strDest;
    }
};

class Win32CryptoProvider : public CryptoProvider {

    string key = "asda@#%soiduo123123"; // 这个密码可以内置，因为Windows内部也有密码

    bool EncryptData(const void* cbDataIn, const int nLen, const void* key, const int lenKey, void** encData, int* encLen)
    {
        if (!cbDataIn || 0 == nLen)
            return false;
        if (*encData)
            free(*encData);
        DATA_BLOB DataIn;
        DATA_BLOB DataOut;
        DATA_BLOB BlobKey;
        DataIn.pbData = (BYTE*)const_cast<void*>(cbDataIn);
        DataIn.cbData = nLen;
        if (key)
        {
            BlobKey.pbData = (BYTE*)const_cast<void*>(key);
            BlobKey.cbData = lenKey;
        }
        if (!CryptProtectData(&DataIn, L"敏感数据", key ? &BlobKey : NULL, NULL, NULL, 0, &DataOut))
            return false;
        *encLen = DataOut.cbData;
        *encData = malloc(DataOut.cbData);
        memcpy(*encData, DataOut.pbData, DataOut.cbData);
        LocalFree(DataOut.pbData);
        return true;
    }

    bool DecryptData(const void* encData, const int encLen, const void* key, const int lenKey, void** cbDataIn, int* nLen)
    {
        if (!encData || 0 == encLen)
            return false;
        DATA_BLOB DataIn;
        DATA_BLOB DataOut;
        DATA_BLOB BlobKey;
        LPWSTR pDescrOut = NULL;
        if (key)
        {
            BlobKey.pbData = (BYTE*)const_cast<void*>(key);
            BlobKey.cbData = lenKey;
        }
        DataIn.pbData = (BYTE*)const_cast<void*>(encData);
        DataIn.cbData = encLen;

        CRYPTPROTECT_PROMPTSTRUCT promp;
        promp.cbSize = sizeof(CRYPTPROTECT_PROMPTSTRUCT);
        promp.szPrompt = L"测试解密";
        promp.dwPromptFlags = CRYPTPROTECT_PROMPT_ON_UNPROTECT;
        promp.hwndApp = NULL;

        if (!CryptUnprotectData(&DataIn, &pDescrOut, key ? &BlobKey : NULL, NULL, &promp, 0, &DataOut))
            return false;

        *nLen = DataOut.cbData;
        *cbDataIn = malloc(DataOut.cbData);
        memcpy(*cbDataIn, DataOut.pbData, DataOut.cbData);
        LocalFree(DataOut.pbData);
        return true;
    }

public:
    string Encrypt(const string& str) {
        void* r_data = nullptr;
        int r_len = 0;
        EncryptData(str.c_str(), str.length(), key.c_str(), key.length(), &r_data, &r_len);
        string ret = base64_encode((uint8_t*)r_data, r_len);
        free(r_data);
        return ret;
    }

    string Decrypt(const string& cipher) {
        string str_cipher = base64_decode(cipher);
        void* r_data = nullptr;
        int r_len = 0;
        DecryptData(str_cipher.c_str(), str_cipher.length(), key.c_str(), key.length(), &r_data, &r_len);
        char* str = new char[r_len + 1];
        memcpy(str, r_data, r_len);
        str[r_len] = 0;
        string ret(str);
        free(str);
        return ret;
    }
};