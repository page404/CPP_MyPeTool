#pragma once
#include "afxcmn.h"
#include <fstream>
#include <vector>

// CSectionTableDlg �Ի���

class CSectionTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionTableDlg)

public:
	CSectionTableDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSectionTableDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SectionTables };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//��ʾ�б�ؼ���ͷ������
	void showListControlHead();

	//---����Dosͷ��Ϣ
	//��Ȼ������NTͷ��Ϣģ��,��������ļ���NTͷ��ϢҪ�õ�Dosͷ��Ϣ�е� e_lfanew ����,
	//����Ҫ�ټ���һ��,�Ա��ȡ�� e_lfanew ��ֵ.
	void loadDosHead();

	//����NTͷ��Ϣ
	void loadNtHead();

	//���� section �������Ϣ(�������������ܻ��ж��)
	void loadSectionHeaders();

public:

	std::fstream m_Stream;

	//Dosͷ��Ϣ����
	IMAGE_DOS_HEADER m_ImageDosHeader;
	//NTͷ��Ϣ����
	IMAGE_NT_HEADERS m_ImageNtHeaders;

	//����� vector
	std::vector<IMAGE_SECTION_HEADER> m_sectionHeaders_vt;

	//���ؼ�
	CListCtrl m_sectionTablesList;
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
