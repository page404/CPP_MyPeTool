#pragma once
#include <fstream>
#include <vector>

// CAddressChangeDlg 对话框

class CAddressChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddressChangeDlg)

public:
	CAddressChangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddressChangeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AddressChange };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCalculation();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnSetfocusEditVa();
	afx_msg void OnEnSetfocusEditRva();
	afx_msg void OnEnSetfocusEditRaw();

	//---加载Dos头信息
	//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
	//所以要再加载一次,以便获取到 e_lfanew 的值.
	void loadDosHead();

	//加载NT头信息
	void loadNtHead();

	//加载 section 区块表信息(区块表的数量可能会有多个)
	void loadSectionHeaders();

	//--------地址转换相关

	//VA 转 RVA
	void VAToRVA(DWORD address);

public:

	std::fstream m_Stream;

	//Dos头信息定义
	IMAGE_DOS_HEADER m_ImageDosHeader;
	//NT头信息定义
	IMAGE_NT_HEADERS m_ImageNtHeaders;

	//区块表 vector
	std::vector<IMAGE_SECTION_HEADER> m_sectionHeaders_vt;
	
	//哪个文本框正获得焦点(这里从1开始)
	int m_WhichTextEditGetFocus;
};
