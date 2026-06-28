#ifndef __MY_SKILL_WINDOW_H__
#define __MY_SKILL_WINDOW_H__

class MySkillWindow
{
public:
    MySkillWindow();
    virtual ~MySkillWindow();

public:
    enum eMode
    {
        eSkillMode = 0,
        eTrainingMode,
        eCeremonyMode,
    };

public:
    void SetMode( char nMode );
    void NextPage();
    void PreviousPage();
	int  GetCurrentPage();

	void UpdateSkillInfo();
    void UpdateTrainingInfo();
    void UpdateCeremonyInfo();

protected:
    char  m_nCurrentMode;

    int   m_nCurrentPage;
    int   m_nTotalPage;
    int   m_nEquipCount;
    int   m_nTotalCount;
};

#endif

