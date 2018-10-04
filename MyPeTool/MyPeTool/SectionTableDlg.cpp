// SectionTableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "SectionTableDlg.h"
#include "afxdialogex.h"


// CSectionTableDlg 对话框

IMPLEMENT_DYNAMIC(CSectionTableDlg, CDialogEx)


BOOL CSectionTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//显示表格标题
	showListControlHead();

	//打开PE文件
	m_Stream.open(AnalysisExeName, std::fstream::binary | std::fstream::in);

	//加载Dos头信息
	loadDosHead();

	//加载NT头信息
	loadNtHead();

	//加载 section 区块表信息(区块表的数量可能会有多个)
	loadSectionHeaders();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

CSectionTableDlg::CSectionTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SectionTables, pParent)
{

}

void CSectionTableDlg::showListControlHead() {

	m_sectionTablesList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_sectionTablesList.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 50);
	m_sectionTablesList.InsertColumn(1, _T("V.内存中偏移"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(2, _T("V.内存中大小"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(3, _T("R.文件中偏移"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(4, _T("R.文件中大小"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(5, _T("标志"), LVCFMT_LEFT, 110);
}

//---加载Dos头信息
//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
//所以要再加载一次,以便获取到 e_lfanew 的值.
void CSectionTableDlg::loadDosHead() {

	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//加载NT头信息
void CSectionTableDlg::loadNtHead() {

	//DOS头信息中的e_lfanew值为NT头的开始位置
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	//NT标识-----读取 NT头 中的 Signature 信息.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature, sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("找不到NT标识"), _T("提示"), 0);
	}

	//文件头------读取 NT头 中的 _IMAGE_FILE_HEADER 结构体 信息
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader, sizeof(IMAGE_FILE_HEADER));
}

#define SIZE_OF_NT_SIGNATURE 4

//加载 section 区块表信息(区块表的数量可能会有多个)
void CSectionTableDlg::loadSectionHeaders() {

	//计算PE最开始位置到 section header 区块表位置的偏移
	int sectionsOffset = m_ImageDosHeader.e_lfanew + SIZE_OF_NT_SIGNATURE + sizeof(IMAGE_FILE_HEADER)
		+ m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader;
	//section 的个数
	int sectionNum = m_ImageNtHeaders.FileHeader.NumberOfSections;

	//所有section 的总大小
	int sectionSize = sectionNum * sizeof(IMAGE_SECTION_HEADER);

	//重置 区块表 vector 大小
	m_sectionHeaders_vt.resize(sectionNum);

	//设置读取文件的偏移
	m_Stream.seekg(sectionsOffset);

	//读取文件中的 区块表 数据
	m_Stream.read((char*)m_sectionHeaders_vt.data(), sectionSize);


	//-----------------将数据显示到表格上
	for (int i = 0; i < m_sectionHeaders_vt.size();i++) {

		CStringA name_A_temp;
		name_A_temp.Format("%s", m_sectionHeaders_vt[i].Name);
		CString name_W_temp;
		USES_CONVERSION;
		name_W_temp.Format(_T("%s"), A2W(name_A_temp.GetBuffer()));

		CString virtualAddress_temp;
		virtualAddress_temp.Format(_T("%08x"), m_sectionHeaders_vt[i].VirtualAddress);

		CString virtualSize_temp;
		virtualSize_temp.Format(_T("%08x"), m_sectionHeaders_vt[i].Misc.VirtualSize);

		CString pointerToRawData_temp;
		pointerToRawData_temp.Format(_T("%08x"), m_sectionHeaders_vt[i].PointerToRawData);

		CString sizeOfRawData_temp;
		sizeOfRawData_temp.Format(_T("%08x"), m_sectionHeaders_vt[i].SizeOfRawData);

		CString characteristics_temp;
		characteristics_temp.Format(_T("%08x"), m_sectionHeaders_vt[i].Characteristics);
	
		int nIndex = m_sectionTablesList.GetItemCount();
		m_sectionTablesList.InsertItem(nIndex, NULL);
		m_sectionTablesList.SetItemText(nIndex, 0, name_W_temp);
		m_sectionTablesList.SetItemText(nIndex, 1, virtualAddress_temp);
		m_sectionTablesList.SetItemText(nIndex, 2, virtualSize_temp);
		m_sectionTablesList.SetItemText(nIndex, 3, pointerToRawData_temp);
		m_sectionTablesList.SetItemText(nIndex, 4, sizeOfRawData_temp);
		m_sectionTablesList.SetItemText(nIndex, 5, characteristics_temp);
	}
}

CSectionTableDlg::~CSectionTableDlg()
{
}

void CSectionTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SectionTables, m_sectionTablesList);
}


BEGIN_MESSAGE_MAP(CSectionTableDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSectionTableDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSectionTableDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSectionTableDlg 消息处理程序




void CSectionTableDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnOK();
}


void CSectionTableDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CSectionTableDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnClose();
}
