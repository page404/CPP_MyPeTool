// ImageOptionalHeaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "ImageOptionalHeaderDlg.h"
#include "afxdialogex.h"


// CImageOptionalHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CImageOptionalHeaderDlg, CDialogEx)


BOOL CImageOptionalHeaderDlg::OnInitDialog()
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

CImageOptionalHeaderDlg::CImageOptionalHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ImageOptionalHeader, pParent)
{

}

//---加载Dos头信息
//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
//所以要再加载一次,以便获取到 e_lfanew 的值.
void CImageOptionalHeaderDlg::loadDosHead() {

	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//加载NT头信息
void CImageOptionalHeaderDlg::loadNtHead() {

	//DOS头信息中的e_lfanew值为NT头的开始位置
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	//NT标识-----读取 NT头 中的 Signature 信息.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature, sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("找不到NT标识"), _T("提示"), 0);
	}

	//---文件头这里放在了另一个Dialog中显示
	//文件头------读取 NT头 中的 _IMAGE_FILE_HEADER 结构体 信息
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader, sizeof(IMAGE_FILE_HEADER));

	
	//可选头------读取 NT头 中的 _IMAGE_OPTIONAL_HEADER 结构体 信息
	//SizeOfOptionalHeader:可选头的大小
	if (m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader != 0) {

	    m_Stream.read((char*)&m_ImageNtHeaders.OptionalHeader, m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader);
	}


	//-----显示 可选头 相关信息到界面上.

	CString magic_temp;
	magic_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.Magic);
	GetDlgItem(IDC_EDIT_Magic)->SetWindowText(magic_temp);

	CString majorLinkerVersion_temp;
	majorLinkerVersion_temp.Format(_T("%02x"), m_ImageNtHeaders.OptionalHeader.MajorLinkerVersion);
	GetDlgItem(IDC_EDIT_MajorLinkerVersion)->SetWindowText(majorLinkerVersion_temp);

	CString minorLinkerVersion_temp;
	minorLinkerVersion_temp.Format(_T("%02x"), m_ImageNtHeaders.OptionalHeader.MinorLinkerVersion);
	GetDlgItem(IDC_EDIT_MinorLinkerVersion)->SetWindowText(minorLinkerVersion_temp);

	CString sizeOfCode_temp;
	sizeOfCode_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfCode);
	GetDlgItem(IDC_EDIT_SizeOfCode)->SetWindowText(sizeOfCode_temp);

	CString sizeOfInitializedData_temp;
	sizeOfInitializedData_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfInitializedData);
	GetDlgItem(IDC_EDIT_SizeOfInitData)->SetWindowText(sizeOfInitializedData_temp);

	CString sizeOfUninitializedData_temp;
	sizeOfUninitializedData_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfUninitializedData);
	GetDlgItem(IDC_EDIT_SizeOfUninitData)->SetWindowText(sizeOfUninitializedData_temp);

	CString addressOfEntryPoint_temp;
	addressOfEntryPoint_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.AddressOfEntryPoint);
	GetDlgItem(IDC_EDIT_EntryPoint)->SetWindowText(addressOfEntryPoint_temp);

	CString baseOfCode_temp;
	baseOfCode_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.BaseOfCode);
	GetDlgItem(IDC_EDIT_BaseOfCode)->SetWindowText(baseOfCode_temp);

	CString baseOfData_temp;
	baseOfData_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.BaseOfData);
	GetDlgItem(IDC_EDIT_BaseOfData)->SetWindowText(baseOfData_temp);

	CString imageBase_temp;
	imageBase_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.ImageBase);
	GetDlgItem(IDC_EDIT_ImageBase)->SetWindowText(imageBase_temp);

	CString sectionAlignment_temp;
	sectionAlignment_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SectionAlignment);
	GetDlgItem(IDC_EDIT_SectionAlignment)->SetWindowText(sectionAlignment_temp);

	CString fileAlignment_temp;
	fileAlignment_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.FileAlignment);
	GetDlgItem(IDC_EDIT_FileAlignment)->SetWindowText(fileAlignment_temp);

	CString majorOperatingSystemVersion_temp;
	majorOperatingSystemVersion_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.MajorOperatingSystemVersion);
	GetDlgItem(IDC_EDIT_MajorOSVersion)->SetWindowText(majorOperatingSystemVersion_temp);

	CString minorOperatingSystemVersion_temp;
	minorOperatingSystemVersion_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.MinorOperatingSystemVersion);
	GetDlgItem(IDC_EDIT_MinorOSVersion)->SetWindowText(minorOperatingSystemVersion_temp);

	CString majorImageVersion_temp;
	majorImageVersion_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.MajorImageVersion);
	GetDlgItem(IDC_EDIT_MajorImageVersion)->SetWindowText(majorImageVersion_temp);

	CString minorImageVersion_temp;
	minorImageVersion_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.MinorImageVersion);
	GetDlgItem(IDC_EDIT_MinorImageVersion)->SetWindowText(minorImageVersion_temp);

	CString majorSubsystemVersion_temp;
	majorSubsystemVersion_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.MajorSubsystemVersion);
	GetDlgItem(IDC_EDIT_MajorSubsystemVersion)->SetWindowText(majorSubsystemVersion_temp);

	CString minorSubsystemVersion_temp;
	minorSubsystemVersion_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.MinorSubsystemVersion);
	GetDlgItem(IDC_EDIT_MinorSubsystemVersion)->SetWindowText(minorSubsystemVersion_temp);

	CString win32VersionValue_temp;
	win32VersionValue_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.Win32VersionValue);
	GetDlgItem(IDC_EDIT_Win32VersionValue)->SetWindowText(win32VersionValue_temp);

	CString sizeOfImage_temp;
	sizeOfImage_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfImage);
	GetDlgItem(IDC_EDIT_SizeOfImage)->SetWindowText(sizeOfImage_temp);

	CString sizeOfHeaders_temp;
	sizeOfHeaders_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfHeaders);
	GetDlgItem(IDC_EDIT_SizeOfHeaders)->SetWindowText(sizeOfHeaders_temp);

	CString checkSum_temp;
	checkSum_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.CheckSum);
	GetDlgItem(IDC_EDIT_CheckSum)->SetWindowText(checkSum_temp);

	CString subsystem_temp;
	subsystem_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.Subsystem);
	GetDlgItem(IDC_EDIT_Subsystem)->SetWindowText(subsystem_temp);

	CString dllCharacteristics_temp;
	dllCharacteristics_temp.Format(_T("%04x"), m_ImageNtHeaders.OptionalHeader.DllCharacteristics);
	GetDlgItem(IDC_EDIT_DllCharacteristics)->SetWindowText(dllCharacteristics_temp);

	CString sizeOfStackReserve_temp;
	sizeOfStackReserve_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfStackReserve);
	GetDlgItem(IDC_EDIT_SizeOfStackReserve)->SetWindowText(sizeOfStackReserve_temp);

	CString sizeOfStackCommit_temp;
	sizeOfStackCommit_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfStackCommit);
	GetDlgItem(IDC_EDIT_SizeOfStackCommit)->SetWindowText(sizeOfStackCommit_temp);

	CString sizeOfHeapReserve_temp;
	sizeOfHeapReserve_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfHeapReserve);
	GetDlgItem(IDC_EDIT_SizeOfHeapReserve)->SetWindowText(sizeOfHeapReserve_temp);

	CString sizeOfHeapCommit_temp;
	sizeOfHeapCommit_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.SizeOfHeapCommit);
	GetDlgItem(IDC_EDIT_SizeOfHeapCommit)->SetWindowText(sizeOfHeapCommit_temp);

	CString loaderFlags_temp;
	loaderFlags_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.LoaderFlags);
	GetDlgItem(IDC_EDIT_LoaderFlags)->SetWindowText(loaderFlags_temp);

	CString numberOfRvaAndSizes_temp;
	numberOfRvaAndSizes_temp.Format(_T("%08x"), m_ImageNtHeaders.OptionalHeader.NumberOfRvaAndSizes);
	GetDlgItem(IDC_EDIT_NumberOfRvaAndSizes)->SetWindowText(numberOfRvaAndSizes_temp);
}

CImageOptionalHeaderDlg::~CImageOptionalHeaderDlg()
{
}

void CImageOptionalHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageOptionalHeaderDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImageOptionalHeaderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImageOptionalHeaderDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImageOptionalHeaderDlg 消息处理程序




void CImageOptionalHeaderDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnOK();
}


void CImageOptionalHeaderDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CImageOptionalHeaderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnClose();
}
