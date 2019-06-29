#pragma once

#include "AES.h"
#include "Base64.h"
#include <Windows.h>
#include <wincrypt.h>
#include <string>
#include <stdexcept>
#pragma comment(lib,"crypt32.lib")

using namespace std;

// 密码服务基类
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
        int len = src.length();
        int n_blocks = len / BLOCK_SIZE + 1;
        char* plain = new char[n_blocks * BLOCK_SIZE + 1];
        memset(plain, 0x00, n_blocks * BLOCK_SIZE + 1);
        strcpy(plain, src.c_str());
        char* cipher = new char[n_blocks * BLOCK_SIZE + 1];
        memset(cipher, 0, n_blocks * BLOCK_SIZE + 1);
        AES aes;
        aes.MakeKey(_key.c_str(), _iv.c_str(), 16, 16);
        aes.Encrypt(plain, cipher, n_blocks * BLOCK_SIZE, AES::CBC);
        string str = base64_encode((unsigned char*)cipher, n_blocks * BLOCK_SIZE);
        delete[] plain;
        delete[] cipher;
        return str;
    }

    string Decrypt(const string & str) {
        string strData = base64_decode(str);
        int len = strData.length();
        char* cipher = new char[len + 1];
        memcpy(cipher, strData.c_str(), len + 1);
        char* plain = new char[len + 1];
        memcpy(plain, strData.c_str(), len + 1);
        AES aes;
        aes.MakeKey(_key.c_str(), _iv.c_str(), 16, 16);
        aes.Decrypt(cipher, plain, len, AES::CBC);
        string strDest(plain);
        delete cipher;
        delete plain;
        return strDest;
    }
};

class Win32CryptoProvider : public CryptoProvider {

    string key = "asda@#%soiduo123123"; // 这个密码可以内置，因为Windows内部也有密码

    bool Win32Encrypt(const void* cbDataIn, const int nLen, const void* key, const int lenKey, void** encData, int* encLen)
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
        if (!CryptProtectData(&DataIn, L"保护凭据", key ? &BlobKey : NULL, NULL, NULL, 0, &DataOut))
            return false;
        *encLen = DataOut.cbData;
        *encData = malloc(DataOut.cbData);
        memcpy(*encData, DataOut.pbData, DataOut.cbData);
        LocalFree(DataOut.pbData);
        return true;
    }

    bool Win32Decrypt(const void* encData, const int encLen, const void* key, const int lenKey, void** cbDataIn, int* nLen)
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

        if (!CryptUnprotectData(&DataIn, &pDescrOut, key ? &BlobKey : NULL, NULL, NULL, 0, &DataOut))
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
        Win32Encrypt(str.c_str(), str.length(), key.c_str(), key.length(), &r_data, &r_len);
        string ret = base64_encode((uint8_t*)r_data, r_len);
        free(r_data);
        return ret;
    }

    string Decrypt(const string& cipher) {
        string str_cipher = base64_decode(cipher);
        void* r_data = nullptr;
        int r_len = 0;
        Win32Decrypt(str_cipher.c_str(), str_cipher.length(), key.c_str(), key.length(), &r_data, &r_len);
        char* str = new char[r_len + 1];
        memcpy(str, r_data, r_len);
        str[r_len] = 0;
        string ret(str);
        free(str);
        return ret;
    }
};