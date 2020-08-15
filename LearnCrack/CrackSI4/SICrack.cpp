#pragma comment(lib, "crypt32.lib")

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#include "TestEncryptDecrypt.h"

DWORD VerifySignature(BYTE* pbData, DWORD dwDataLen, BYTE* pbSignature, DWORD dwSigLen);
int Base64Decode(char* szSignature, BYTE* ptr_pbSignature, DWORD* ptr_dwSigLen);
void Base64Encode(BYTE* bSig/*size=256*/, char* szSig/*size=345 contains null-end*/);
BYTE* ReadBinary(char* szFilePath, DWORD* cbSize);

char* sig4_data = "<!--\
SourceInsight4.xLicenseFile\
DONOTEDITTHISFILE.Doingsowillrenderitunusable.\
Thislicensewascreatedfor:\
xxx\
51asm\
xxx@xxx.com\
-->\
<SourceInsightLicense>\
<Header\
Value=\"1\"\
/>\
<LicenseProperties\
LicensedUser=\"51asm\"\
ActId=\"9930152826\"\
HWID=\"ZM6QWPSW-MNFUHVF5\"\
Serial=\"S4SG-KRGM-YD7Q-RCFY\"\
Organization=\"\"\
Email=\"xxx@xxx.com\"\
Type=\"Standard\"\
Version=\"4\"\
MinorVersion=\"0\"\
Date=\"2020-08-15\"\
Expiration=\"2030-08-15\"\
/>";

void main()
{
    // ----------------------------------------------------
    // ��һ���֣�ǩ�� Sign

    TestSignVerify edc;
    edc.InitializeProviderForSigner(NULL, PROV_RSA_FULL);
    // ������Կ
    edc.ExportX509PEMPublicKey("public.key");

    // ǩ������
    BYTE* bSig = NULL;
    DWORD szSigLen = 0;
    if (!edc.SignMessage(
        CALG_SHA1,
        (BYTE*)sig4_data,   // ��ǩ���ַ������ݣ���Դ��si4.lic �ļ��� <Signature>��ǩ֮ǰ���������ݣ�ȥ�����е� \r\n\t�Ϳո�
        strlen(sig4_data),  // ��ǩ���ַ������ݳ���
        (BYTE**)&bSig,      // ����������������ǩ������
        &szSigLen))         // ����������������ǩ�����ݴ�С
        return;
    
    // Base64���������ǩ����תΪ�ַ���ǩ�����洢�� szSig��
    char szSig[345] = { 0 };
    Base64Encode(bSig, szSig);
    printf("%s\r\n", szSig);

    // ----------------------------------------------------
    // �ڶ����֣�У�� Verify

    // Base64�����ַ���ǩ����תΪ������ǩ��
    BYTE bSigBuff[0x2000] = { 0 };
    DWORD dwSigLen = 0;
    int nConvertLen = Base64Decode(szSig, bSigBuff, &dwSigLen);

    // У��ǩ��
    DWORD dwRet = VerifySignature(
        (BYTE*)sig4_data, 
        strlen(sig4_data), 
        bSigBuff, 
        dwSigLen);

    if (dwRet == 0xC8)
        printf("success\r\n");
    else
        printf("failed\r\n");

    return;
}


void Base64Encode(BYTE* bSig/*size=256*/, char* szSig/*size=345 contains null-end*/)
{
    char cvtTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };

    unsigned char cvtGroup[3] = { 0 };      //ǩ���е�3���ֽڹ���24λ��ת��4��6λ��������Ϊ�±�ȥcvtTable��ȡ��ת�����ַ�
    for (int i = 0; i < 85; i++)
    {
        memcpy(cvtGroup, bSig + i * 3, 3);

        int nCvtIdx1 = cvtGroup[0] >> 2;
        int nCvtIdx2 = ((cvtGroup[0] & 3)   << 4) | (cvtGroup[1] >> 4);
        int nCvtIdx3 = ((cvtGroup[1] & 0xF) << 2) | (cvtGroup[2] >> 6);
        int nCvtIdx4 = (cvtGroup[2] & 0x3F);

        szSig[i * 4] = cvtTable[nCvtIdx1];
        szSig[i * 4 + 1] = cvtTable[nCvtIdx2];
        szSig[i * 4 + 2] = cvtTable[nCvtIdx3];
        szSig[i * 4 + 3] = cvtTable[nCvtIdx4];
    }

    szSig[340] = cvtTable[bSig[255] >> 2];
    szSig[341] = cvtTable[(bSig[255] & 3) << 4];
    szSig[342] = '=';
    szSig[343] = '=';
}

BYTE* ReadBinary(char* szFilePath, DWORD* cbSize)
{
    *cbSize = 0;
    HANDLE hFile = CreateFile(szFilePath, FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == NULL)
        return NULL;

    DWORD dwFileSize = GetFileSize(hFile, NULL);
    BYTE* buff = (BYTE*)malloc(dwFileSize);

    if (buff != NULL)
    {
        ZeroMemory(buff, dwFileSize);

        DWORD dwBytesRead = 0;
        if (!ReadFile(hFile, buff, dwFileSize, &dwBytesRead, NULL))
            free(buff);
        else
            *cbSize = dwBytesRead;
    }
    CloseHandle(hFile);
    return buff;
}

