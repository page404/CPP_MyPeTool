#pragma once
#include <fstream>

// CImageOptionalHeaderDlg 对话框

class CImageOptionalHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageOptionalHeaderDlg)

public:
	CImageOptionalHeaderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageOptionalHeaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageOptionalHeader };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();

	//---加载Dos头信息
	//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
	//所以要再加载一次,以便获取到 e_lfanew 的值.
	void loadDosHead();

	//加载NT头信息
	void loadNtHead();

public:
	std::fstream m_Stream;
	//Dos头信息定义
	IMAGE_DOS_HEADER m_ImageDosHeader;
	//NT头信息定义
	IMAGE_NT_HEADERS m_ImageNtHeaders;
};
