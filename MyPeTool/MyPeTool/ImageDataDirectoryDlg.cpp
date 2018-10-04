// ImageDataDirectoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "ImageDataDirectoryDlg.h"
#include "afxdialogex.h"


// CImageDataDirectoryDlg 对话框

IMPLEMENT_DYNAMIC(CImageDataDirectoryDlg, CDialogEx)

BOOL CImageDataDirectoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetDlgItemText(IDC_EDIT_RVA, _T("RVA"));
	SetDlgItemText(IDC_EDIT_Size, _T("大小"));

	//打开PE文件
	m_Stream.open(AnalysisExeName, std::fstream::binary | std::fstream::in);

	//加载Dos头信息
	loadDosHead();

	//加载NT头信息
	loadNtHead();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

CImageDataDirectoryDlg::CImageDataDirectoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ImageDataDirectory, pParent)
{

}

//---加载Dos头信息
//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
//所以要再加载一次,以便获取到 e_lfanew 的值.
void CImageDataDirectoryDlg::loadDosHead() {

	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//加载NT头信息
void CImageDataDirectoryDlg::loadNtHead() {

	//DOS头信息中的e_lfanew值为NT头的开始位置
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	//NT标识-----读取 NT头 中的 Signature 信息.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature, sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("找不到NT标识"), _T("提示"), 0);
	}

	//文件头------读取 NT头 中的 _IMAGE_FILE_HEADER 结构体 信息
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader, sizeof(IMAGE_FILE_HEADER));


	//可选头------读取 NT头 中的 _IMAGE_OPTIONAL_HEADER 结构体 信息
	//SizeOfOptionalHeader:可选头的大小
	if (m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader != 0) {

		m_Stream.read((char*)&m_ImageNtHeaders.OptionalHeader, m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader);
	}

	//----在界面上显示数据

	CString exportAddress_temp;
	exportAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[0].VirtualAddress);
	GetDlgItem(IDC_EDIT_ExportAddress)->SetWindowText(exportAddress_temp);

	CString exportSize_temp;
	exportSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[0].Size);
	GetDlgItem(IDC_EDIT_ExportSize)->SetWindowText(exportSize_temp);

	CString importAddress_temp;
	importAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[1].VirtualAddress);
	GetDlgItem(IDC_EDIT_ImportAddress)->SetWindowText(importAddress_temp);

	CString importSize_temp;
	importSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[1].Size);
	GetDlgItem(IDC_EDIT_ImportSize)->SetWindowText(importSize_temp);

	CString resourceAddress_temp;
	resourceAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[2].VirtualAddress);
	GetDlgItem(IDC_EDIT_ResourceAddress)->SetWindowText(resourceAddress_temp);

	CString resourceSize_temp;
	resourceSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[2].Size);
	GetDlgItem(IDC_EDIT_ResourceSize)->SetWindowText(resourceSize_temp);

	CString exceptionAddress_temp;
	exceptionAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[3].VirtualAddress);
	GetDlgItem(IDC_EDIT_ExceptionAddress)->SetWindowText(exceptionAddress_temp);

	CString exceptionSize_temp;
	exceptionSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[3].Size);
	GetDlgItem(IDC_EDIT_ExceptionSize)->SetWindowText(exceptionSize_temp);

	CString securityCertificateAddress_temp;
	securityCertificateAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[4].VirtualAddress);
	GetDlgItem(IDC_EDIT1_SecurityCertificateAddress)->SetWindowText(securityCertificateAddress_temp);

	CString securityCertificateSize_temp;
	securityCertificateSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[4].Size);
	GetDlgItem(IDC_EDIT_SecurityCertificateSize)->SetWindowText(securityCertificateSize_temp);

	CString relocationAddress_temp;
	relocationAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[5].VirtualAddress);
	GetDlgItem(IDC_EDIT_RelocationAddress)->SetWindowText(relocationAddress_temp);

	CString relocationSize_temp;
	relocationSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[5].Size);
	GetDlgItem(IDC_EDIT_RelocationSize)->SetWindowText(relocationSize_temp);

	CString debugInfoAddress_temp;
	debugInfoAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[6].VirtualAddress);
	GetDlgItem(IDC_EDIT_DebugInfoAddress)->SetWindowText(debugInfoAddress_temp);

	CString debugInfoSize_temp;
	debugInfoSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[6].Size);
	GetDlgItem(IDC_EDIT_DebugInfoSize)->SetWindowText(debugInfoSize_temp);

	CString copyRightAddress_temp;
	copyRightAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[7].VirtualAddress);
	GetDlgItem(IDC_EDIT_CopyRightAddress)->SetWindowText(copyRightAddress_temp);

	CString copyRightSize_temp;
	copyRightSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[7].Size);
	GetDlgItem(IDC_EDIT_CopyRightSize)->SetWindowText(copyRightSize_temp);

	CString globalPointAddress_temp;
	globalPointAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[8].VirtualAddress);
	GetDlgItem(IDC_EDIT_GlobalPointAddress)->SetWindowText(globalPointAddress_temp);

	CString globalPointSize_temp;
	globalPointSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[8].Size);
	GetDlgItem(IDC_EDIT_GlobalPointSize)->SetWindowText(globalPointSize_temp);

	CString tlsAddress_temp;
	tlsAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[9].VirtualAddress);
	GetDlgItem(IDC_EDIT_TLSAddress)->SetWindowText(tlsAddress_temp);

	CString tlsSize_temp;
	tlsSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[9].Size);
	GetDlgItem(IDC_EDIT_TLSSize)->SetWindowText(tlsSize_temp);

	CString loadConfigAddress_temp;
	loadConfigAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[10].VirtualAddress);
	GetDlgItem(IDC_EDIT_LoadConfigAddress)->SetWindowText(loadConfigAddress_temp);

	CString loadConfigSize_temp;
	loadConfigSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[10].Size);
	GetDlgItem(IDC_EDIT_LoadConfigSize)->SetWindowText(loadConfigSize_temp);

	CString boundImportAddress_temp;
	boundImportAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[11].VirtualAddress);
	GetDlgItem(IDC_EDIT_BoundImportAddress)->SetWindowText(boundImportAddress_temp);

	CString boundImportSize_temp;
	boundImportSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[11].Size);
	GetDlgItem(IDC_EDIT_BoundImportSize)->SetWindowText(boundImportSize_temp);

	CString iatAddress_temp;
	iatAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[12].VirtualAddress);
	GetDlgItem(IDC_EDIT_IATAddress)->SetWindowText(iatAddress_temp);

	CString iatSize_temp;
	iatSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[12].Size);
	GetDlgItem(IDC_EDIT_IATSize)->SetWindowText(iatSize_temp);

	CString delayImportAddress_temp;
	delayImportAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[13].VirtualAddress);
	GetDlgItem(IDC_EDIT_DelayImportAddress)->SetWindowText(delayImportAddress_temp);

	CString delayImportSize_temp;
	delayImportSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[13].Size);
	GetDlgItem(IDC_EDIT_DelayImportSize)->SetWindowText(delayImportSize_temp);

	CString comAddress_temp;
	comAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[14].VirtualAddress);
	GetDlgItem(IDC_EDIT_ComAddress)->SetWindowText(comAddress_temp);

	CString comSize_temp;
	comSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[14].Size);
	GetDlgItem(IDC_EDIT_ComSize)->SetWindowText(comSize_temp);

	CString reservedAddress_temp;
	reservedAddress_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[15].VirtualAddress);
	GetDlgItem(IDC_EDIT_ReservedAddress)->SetWindowText(reservedAddress_temp);

	CString reservedSize_temp;
	reservedSize_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.DataDirectory[15].Size);
	GetDlgItem(IDC_EDIT_ReservedSize)->SetWindowText(reservedSize_temp);
	
}

CImageDataDirectoryDlg::~CImageDataDirectoryDlg()
{
	
}

void CImageDataDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageDataDirectoryDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImageDataDirectoryDlg::OnBnClickedOk)
	ON_BN_CLICKED(2, &CImageDataDirectoryDlg::OnBnClicked2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImageDataDirectoryDlg 消息处理程序





void CImageDataDirectoryDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnOK();
}


void CImageDataDirectoryDlg::OnBnClicked2()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

}


void CImageDataDirectoryDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnClose();
}
