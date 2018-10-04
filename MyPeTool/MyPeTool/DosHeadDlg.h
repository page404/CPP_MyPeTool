#pragma once
#include <string>
#include "afxwin.h"
#include <fstream>

// CDosHeadDlg �Ի���

class CDosHeadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDosHeadDlg)

public:
	CDosHeadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDosHeadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DosHead };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	//����Dosͷ��Ϣ
	void loadDosHead();

public:
	std::fstream m_Stream;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
