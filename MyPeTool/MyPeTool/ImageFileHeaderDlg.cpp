// ImageFileHeaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "ImageFileHeaderDlg.h"
#include "afxdialogex.h"


// CImageFileHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CImageFileHeaderDlg, CDialogEx)

BOOL CImageFileHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//打开PE文件
	m_Stream.open(AnalysisExeName, std::fstream::binary | std::fstream::in);

	//加载Dos头信息
	loadDosHead();

	//加载NT头信息
	loadNtHead();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

CImageFileHeaderDlg::CImageFileHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ImageFileHeader, pParent)
{

}

CImageFileHeaderDlg::~CImageFileHeaderDlg()
{
}

void CImageFileHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageFileHeaderDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImageFileHeaderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImageFileHeaderDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//---加载Dos头信息
//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
//所以要再加载一次,以便获取到 e_lfanew 的值.
void CImageFileHeaderDlg::loadDosHead() {
	
	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//加载NT头信息
void CImageFileHeaderDlg::loadNtHead() {

	//DOS头信息中的e_lfanew值为NT头的开始位置
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	memset(&m_ImageNtHeaders, 0, sizeof(IMAGE_NT_HEADERS));

	//NT标识-----读取 NT头 中的 Signature 信息.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature,sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("找不到NT标识"), _T("提示"), 0);
	}

	//文件头------读取 NT头 中的 _IMAGE_FILE_HEADER 结构体 信息
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader,sizeof(IMAGE_FILE_HEADER));

	//---可选头这里放在了另一个Dialog中显示
	//可选头------读取 NT头 中的 _IMAGE_OPTIONAL_HEADER 结构体 信息
	//SizeOfOptionalHeader:可选头的大小
	//if (m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader != 0) {
	    
		//m_Stream.read((char*)&m_ImageNtHeaders.OptionalHeader, m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader);
	//}

	
	//-----显示 文件头 相关信息到界面上.

	CString machine_temp;
	machine_temp.Format(_T("%04x"), m_ImageNtHeaders.FileHeader.Machine);
	GetDlgItem(IDC_EDIT_Machine)->SetWindowText(machine_temp);

	CString numberOfSections_temp;
	numberOfSections_temp.Format(_T("%04x"), m_ImageNtHeaders.FileHeader.NumberOfSections);
	GetDlgItem(IDC_EDIT_NumberOfSections)->SetWindowText(numberOfSections_temp);

	CString timeDateStamp_temp;
	timeDateStamp_temp.Format(_T("%08x"), m_ImageNtHeaders.FileHeader.TimeDateStamp);
	GetDlgItem(IDC_EDIT_TimeDateStamp)->SetWindowText(timeDateStamp_temp);

	CString pointerToSymbolTable_temp;
	pointerToSymbolTable_temp.Format(_T("%08x"), m_ImageNtHeaders.FileHeader.PointerToSymbolTable);
	GetDlgItem(IDC_EDIT_PointerOfSymbolTable)->SetWindowText(pointerToSymbolTable_temp);

	CString numberOfSymbols_temp;
	numberOfSymbols_temp.Format(_T("%08x"), m_ImageNtHeaders.FileHeader.NumberOfSymbols);
	GetDlgItem(IDC_EDIT_NumberOfSymbols)->SetWindowText(numberOfSymbols_temp);

	CString sizeOfOptionalHeader_temp;
	sizeOfOptionalHeader_temp.Format(_T("%04x"), m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader);
	GetDlgItem(IDC_EDIT_SizeOfOptionHeader)->SetWindowText(sizeOfOptionalHeader_temp);

	CString characteristics_temp;
	characteristics_temp.Format(_T("%04x"), m_ImageNtHeaders.FileHeader.Characteristics);
	GetDlgItem(IDC_EDIT_Characteristics)->SetWindowText(characteristics_temp);
}


// CImageFileHeaderDlg 消息处理程序


void CImageFileHeaderDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnOK();
}


void CImageFileHeaderDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CImageFileHeaderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnClose();
}
