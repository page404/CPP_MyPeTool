#pragma once
#include "afxcmn.h"
#include <fstream>
#include <vector>

// CSectionTableDlg 对话框

class CSectionTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionTableDlg)

public:
	CSectionTableDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSectionTableDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SectionTables };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//显示列表控件的头部标题
	void showListControlHead();

	//---加载Dos头信息
	//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
	//所以要再加载一次,以便获取到 e_lfanew 的值.
	void loadDosHead();

	//加载NT头信息
	void loadNtHead();

	//加载 section 区块表信息(区块表的数量可能会有多个)
	void loadSectionHeaders();

public:

	std::fstream m_Stream;

	//Dos头信息定义
	IMAGE_DOS_HEADER m_ImageDosHeader;
	//NT头信息定义
	IMAGE_NT_HEADERS m_ImageNtHeaders;

	//区块表 vector
	std::vector<IMAGE_SECTION_HEADER> m_sectionHeaders_vt;

	//表格控件
	CListCtrl m_sectionTablesList;
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
