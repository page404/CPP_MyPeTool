#pragma once
#include <fstream>
#include <vector>

// CImageDataDirectoryDlg 对话框

class CImageDataDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDataDirectoryDlg)

public:
	CImageDataDirectoryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageDataDirectoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageDataDirectory };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

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

	//区块表 vector
	std::vector<IMAGE_SECTION_HEADER> m_sectionHeaders;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClicked2();
	afx_msg void OnClose();
};
