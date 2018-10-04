#pragma once
#include <fstream>
#include <vector>

// CImageDataDirectoryDlg �Ի���

class CImageDataDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDataDirectoryDlg)

public:
	CImageDataDirectoryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageDataDirectoryDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageDataDirectory };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//---����Dosͷ��Ϣ
	//��Ȼ������NTͷ��Ϣģ��,��������ļ���NTͷ��ϢҪ�õ�Dosͷ��Ϣ�е� e_lfanew ����,
	//����Ҫ�ټ���һ��,�Ա��ȡ�� e_lfanew ��ֵ.
	void loadDosHead();

	//����NTͷ��Ϣ
	void loadNtHead();

public:
	std::fstream m_Stream;

	//Dosͷ��Ϣ����
	IMAGE_DOS_HEADER m_ImageDosHeader;
	//NTͷ��Ϣ����
	IMAGE_NT_HEADERS m_ImageNtHeaders;

	//����� vector
	std::vector<IMAGE_SECTION_HEADER> m_sectionHeaders;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClicked2();
	afx_msg void OnClose();
};
