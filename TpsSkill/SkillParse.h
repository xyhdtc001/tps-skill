#pragma once
#include <string>
#include "CSKillData.h"

using namespace std;
using namespace tinyxml2;

class CIniLoad;
class tinyxml2::XMLDocument;
class tinyxml2::XMLNode;
class tinyxml2::XMLElement;

class SkillParse
{
public:
	SkillParse();
	~SkillParse();

	
	bool SetWorkDir(string strPath);

	bool SetSelfWorkDir(string strPath);
	
	void ParseHero(string strHeroID);


	void outPutTmeInfo();

	void outPutStateTmeInfo();

	void outPutErrorInfo();

	//����ͨ��TME
	void ProceeCommonTme();

	void outCommonPath();

	//����lua�е� tme.
	void ProcessLuaTme();
protected:

	void InitConfigFile();

	void InitXmlFile();

	void InitHeroSkillMap();

	void CreateHeroSkillInfoByXml(MAPSKILLINFO&,string strHeroID);



	//xml������.
	void LoopXMlForInfo(XMLNode* pSourceNode, map<string, XMLNode*>&mapInfo);

	//�ݹ�Ѱ��TreeNodePresentation
	void LoopForTreeNodePresentation(XMLNode *pRoot,set<string>& setName);

	//����setName��ȡ����ʵ�ʱ���.

	void GetSkillPresentByName(XMLDocument *pdoc,set<string>& setName, map<string, XMLNode*> &mapInfoP,XMLNode* pDesNode,const string &strSkillID, map<string, XMLNode*> &mapInfoPSelf, map<string, XMLNode*> &mapInfoPLink);
	
	string m_strWorkDir; 

	void GetEventInfo(XMLNode *pParent, const char* pszKeyname,map<string, XMLNode*>&mapInfo,string &str24000,string &str25000,string &str26000,string &str30000,bool blower,bool bheroID=true);


	//��ȡ����INI��ֵ
	void GetIniKeyAndValue(INIKEY &iniKeyValue,CIniLoad* pIniFile,string strSection,string xmlNodeName="");

	//�����ֵ
	void GetIniKeyAndValueAndRecoardToXml(INIKEY &iniKeyValue,CIniLoad* pIniFile,string strSection,const string& strIniName);

	void InsertKeyNameToXMl(const string &  strIniName,const string & Keyname);


	//����state.ini,damage.ini.��ȡ����,(Ŀǰȡ���ܹ�Ч.)
	void ProcessStateAndDamageInfo();
	void ProcessStateAndDamageInfoByIni(CIniLoad* pIni,STATETMEMAP& mapData);

	//״̬��Ч�������.
	void ProceeStateTme(string presentName,string strSkllID,map<string, XMLNode*> &mapInfoP);

	bool GetSkillPresentTmePathByName(string strSkllID,XMLNode* xmlNode);

	void InsetToHeroSkillMap(string skillID,int nVo,bool bCoreSKill=false);

	//����ʣ�µ��뵱ǰӢ���йصĹ�Ч.
	void ProcessReMainStateTme(map<string, XMLNode*>& ,map<string, XMLNode*>*p=NULL);

	//����unitdata�еļ������.
	void ProcessUnitDataSkillPresent(map<string, XMLNode*>&,map<string, XMLNode*>*p=NULL);


	
	//prt�ļ��е�tmeֱ�Ӽ���ͨ������.
	void AddTmePathInPrt(string strPath);
	void AddTmeFile(string strPath);


	//��������ͨ�ù�Ч.
	void LoadAllCommonTmeInfo();

	//����Ƥ��ID��ȡӢ������.
	int  GetHeroTypeByIni(string strRoleID);

	//��ȡ����ְҵ������Ϣ.

	//�����ļ�
	CIniLoad* m_pHeroSkill;//��ʹ�õ�ID
	CIniLoad* m_pIniSkill;//ȫ��ID
	CIniLoad *m_pIniSkiPresent;//���ܶ�Ӧ����.
	CIniLoad *m_pIniLanguage;//��������.
	CIniLoad *m_pDamage; //�˺�����.
	CIniLoad *m_pStateIni; //state.ini

	CIniLoad *m_pRoleInfoIni; //state.ini

	CIniLoad *m_pUnitData;

	XMLDocument *m_pXmlRole;

	XMLDocument *m_pXmlHero1101_C;
	XMLDocument *m_pXmlHero1101_P;

	XMLDocument *m_pXmlHero1201_C;
	XMLDocument *m_pXmlHero1201_P;

	XMLDocument *m_pXmlHero1301_C;
	XMLDocument *m_pXmlHero1301_P;

	XMLDocument *m_pXmlHero1401_C;
	XMLDocument *m_pXmlHero1401_P;

	XMLDocument *m_pXmlHero1501_C;
	XMLDocument *m_pXmlHero1501_P;

	XMLDocument *m_pXmlHero1801_C;
	XMLDocument *m_pXmlHero1801_P;


	//iniKey XMl
	XMLDocument *m_pDocXmlKeyName;


	//����ְҵ���ܱ�.
	MAPSKILLINFO m_Hero1101;
	MAPSKILLINFO m_Hero1201;
	MAPSKILLINFO m_Hero1301;
	MAPSKILLINFO m_Hero1401;
	MAPSKILLINFO m_Hero1501;
	MAPSKILLINFO m_Hero1801;

	MAPSKILLINFO* m_cuHeroInfo;


	//buf..

	string m_curHeroID;  //Ӣ��roleID
	string m_curRoleLinkID; //Ӣ��linkID
	string m_curProceeHeroID;//��ǰ���ڴ����Ӣ��ID��m_curHeroID m_curRoleLinkID 2 ѡ1 ��

	STATETMEMAP m_mapStateTme;//״̬��Ч.
	map<string,string>m_mapSKillIDAndname;//damage.ini������Ϣ.


	//����ͨ�ù�Чmap
	map<string, XMLNode*>m_mapCommonInfo;
	XMLDocument *m_pDocComonp;

	//��ʽ���淶��Ϣ.
	map<string,vector<string>> m_mapErrorInfo;

	//���ܶ�Ӧ��Ч.
	VEC_SKILLTME m_vecSkillTme;

	//������Ĺ�Ч.
	map<string,string>m_mapProcessedTmePath;

	//ͨ�ù�Ч
	std::set<const string> m_CommonTmeVec;

	//�����ڵĹ�Ч.
	std::set<const string> m_NoexistTmeVec;

	set<const string> m_luaTme;

	//exe ����·��.
	string m_curSelfRunPath;
};

