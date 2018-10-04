#pragma once
#include <fstream>

// CImageOptionalHeaderDlg �Ի���

class CImageOptionalHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageOptionalHeaderDlg)

public:
	CImageOptionalHeaderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageOptionalHeaderDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageOptionalHeader };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();

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
};
