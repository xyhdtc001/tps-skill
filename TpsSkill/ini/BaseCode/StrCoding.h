#pragma once
class strCoding
{
public:
    strCoding(void);
    ~strCoding(void);
   
    static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);//utf_8תΪgb2312
    static void GB2312ToUTF_8(string& pOut,const char *pText, int pLen); //gb2312 תutf_8
    static string UrlGB2312( const char * str);                           //urlgb2312����
    static string UrlUTF8(const char * str);                             //urlutf8 ����
    static string UrlUTF8Decode(string str);                  //urlutf8����
    static string UrlGB2312Decode(string str);                //urlgb2312����
	static bool IsDBCSLeadByte(unsigned char ch);
    static uint32 GetCharBytes(unsigned char ch);

	static void SetIsAcp(bool isAcp);
private:
    typedef wchar_t WCHAR;
    static void Gb2312ToUnicode(WCHAR* pOut,const char *gbBuffer);
    static void UTF_8ToUnicode(WCHAR* pOut,char *pText);
    static void UnicodeToUTF_8(char* pOut,WCHAR* pText);
    static void UnicodeToGB2312(char* pOut,WCHAR uData);
    static char  CharToInt(char ch);
    static char StrToBin(char *str);

	static bool s_isAcp;

};
 

 

 




