#ifndef _EHOME_CONVERT_H_
#define _EHOME_CONVERT_H_


BOOL A2UTF8(const char* pCont, const DWORD dwInSize, char* pOut, DWORD dwOutSize, DWORD* pOutLen);
BOOL UTF82A(const char* pCont, const DWORD dwInSize, char* pOut, DWORD dwOutSize, DWORD* pOutLen);

BOOL A2UTF8(const char* pCont, char* pOut, DWORD dwOutSize, DWORD* pOutLen);
BOOL UTF82A(const char* pCont, char* pOut, DWORD dwOutSize, DWORD* pOutLen);



#endif


