
// MyPeToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "MyPeToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyPeToolDlg 对话框



CMyPeToolDlg::CMyPeToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYPETOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyPeToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyPeToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DosHead, &CMyPeToolDlg::OnBnClickedButtonDoshead)
	ON_BN_CLICKED(IDC_BUTTON_ImageFileHead, &CMyPeToolDlg::OnBnClickedButtonImagefilehead)
	ON_BN_CLICKED(IDC_BUTTON_ImageOptionalHeader, &CMyPeToolDlg::OnBnClickedButtonImageoptionalheader)
	ON_BN_CLICKED(IDC_BUTTON_ImageDataDirectory, &CMyPeToolDlg::OnBnClickedButtonImagedatadirectory)
	ON_BN_CLICKED(IDC_BUTTON_addressChange, &CMyPeToolDlg::OnBnClickedButtonaddresschange)
	ON_BN_CLICKED(IDC_BUTTON_SectionTables, &CMyPeToolDlg::OnBnClickedButtonSectiontables)
END_MESSAGE_MAP()


// CMyPeToolDlg 消息处理程序

BOOL CMyPeToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyPeToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyPeToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyPeToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//点击 Dos头 按钮
void CMyPeToolDlg::OnBnClickedButtonDoshead()
{
	// TODO: 在此添加控件通知处理程序代码
	
	dlg = new CDosHeadDlg(this);
	dlg->Create(IDD_DIALOG_DosHead, this);
	dlg->ShowWindow(SW_SHOW);
}

//点击 映像文件头 按钮
void CMyPeToolDlg::OnBnClickedButtonImagefilehead()
{
	// TODO: 在此添加控件通知处理程序代码

	imageFileHeaderDlg = new CImageFileHeaderDlg(this);
	imageFileHeaderDlg->Create(IDD_DIALOG_ImageFileHeader, this);
	imageFileHeaderDlg->ShowWindow(SW_SHOW);
}

//点击 可选映像头 按钮
void CMyPeToolDlg::OnBnClickedButtonImageoptionalheader()
{
	// TODO: 在此添加控件通知处理程序代码

	imageOptionalHeaderDlg = new CImageOptionalHeaderDlg(this);
	imageOptionalHeaderDlg->Create(IDD_DIALOG_ImageOptionalHeader, this);
	imageOptionalHeaderDlg->ShowWindow(SW_SHOW);
}

//点击 数据目录表 按钮
void CMyPeToolDlg::OnBnClickedButtonImagedatadirectory()
{
	// TODO: 在此添加控件通知处理程序代码

	imageDataDirectoryDlg = new CImageDataDirectoryDlg(this);
	imageDataDirectoryDlg->Create(IDD_DIALOG_ImageDataDirectory, this);
	imageDataDirectoryDlg->ShowWindow(SW_SHOW);
}

//点击 区段表 按钮
void CMyPeToolDlg::OnBnClickedButtonSectiontables()
{
	// TODO: 在此添加控件通知处理程序代码

	sectionTableDlg = new CSectionTableDlg(this);
	sectionTableDlg->Create(IDD_DIALOG_SectionTables, this);
	sectionTableDlg->ShowWindow(SW_SHOW);

}

//点击 虚拟地址与文件偏移地址相互转换 按钮
void CMyPeToolDlg::OnBnClickedButtonaddresschange()
{
	// TODO: 在此添加控件通知处理程序代码

	addressChangeDlg = new CAddressChangeDlg(this);
	addressChangeDlg->Create(IDD_DIALOG_AddressChange, this);
	addressChangeDlg->ShowWindow(SW_SHOW);

}


