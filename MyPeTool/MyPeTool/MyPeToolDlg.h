
// MyPeToolDlg.h : 头文件
//

#pragma once
#include "DosHeadDlg.h"
#include "ImageFileHeaderDlg.h"
#include "ImageOptionalHeaderDlg.h"
#include "ImageDataDirectoryDlg.h"
#include "AddressChangeDlg.h"
#include "SectionTableDlg.h"

// CMyPeToolDlg 对话框
class CMyPeToolDlg : public CDialogEx
{
// 构造
public:
	CMyPeToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPETOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDoshead();
	afx_msg void OnBnClickedButtonImagefilehead();
	afx_msg void OnBnClickedButtonImageoptionalheader();
	afx_msg void OnBnClickedButtonImagedatadirectory();
	afx_msg void OnBnClickedButtonaddresschange();
	afx_msg void OnBnClickedButtonSectiontables();

public:
	CDosHeadDlg *dlg;
	CImageFileHeaderDlg *imageFileHeaderDlg;
	CImageOptionalHeaderDlg *imageOptionalHeaderDlg;
	CImageDataDirectoryDlg *imageDataDirectoryDlg;
	CAddressChangeDlg *addressChangeDlg;
	CSectionTableDlg *sectionTableDlg;
};
