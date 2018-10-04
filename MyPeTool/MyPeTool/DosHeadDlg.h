#pragma once
#include <string>
#include "afxwin.h"
#include <fstream>

// CDosHeadDlg 对话框

class CDosHeadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDosHeadDlg)

public:
	CDosHeadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDosHeadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DosHead };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	//加载Dos头信息
	void loadDosHead();

public:
	std::fstream m_Stream;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
