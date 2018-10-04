// AddressChangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "AddressChangeDlg.h"
#include "afxdialogex.h"


// CAddressChangeDlg 对话框

IMPLEMENT_DYNAMIC(CAddressChangeDlg, CDialogEx)

BOOL CAddressChangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_WhichTextEditGetFocus = 0;

	CButton *button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
	button1->SetState(TRUE);
	button1->SetCheck(TRUE);

	CString tipStr = _T("【名词解释】：\n·基地址(ImageBase).\n·虚拟地址(Virtual Address, VA).\n·相对虚拟地址(Relative Virtual Address, RVA).\n·文件偏移地址(File Offset) 又称 物理地址(RAW Offset).\n\
\n【计算公式】：\n·相对虚拟地址(RVA) = 虚拟地址(VA) - 基地址(ImageBase).\n·虚拟地址(VA) = 相对虚拟地址(RVA) + 基地址(ImageBase).\n·文件偏移(File Offset) = 相对虚拟地址(RVA) - 该地址所在节的虚拟偏移(Virtual Offset) + 该地址所在节的物理偏移(Raw Offset).\n·相对虚拟地址(RVA) = 文件偏移(File Offset) - 该地址所在节的物理偏移(Raw Offset) + 该地址所在节的虚拟偏移(Virtual Offset).");

	SetDlgItemText(IDC_STATIC_Tip, tipStr);

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

CAddressChangeDlg::CAddressChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_AddressChange, pParent)
{

}

//---加载Dos头信息
//虽然这里是NT头信息模块,但是下面的加载NT头信息要用到Dos头信息中的 e_lfanew 属性,
//所以要再加载一次,以便获取到 e_lfanew 的值.
void CAddressChangeDlg::loadDosHead() {

	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//加载NT头信息
void CAddressChangeDlg::loadNtHead() {

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
}

#define SIZE_OF_NT_SIGNATURE 4

//加载 section 区块表信息(区块表的数量可能会有多个)
void CAddressChangeDlg::loadSectionHeaders() {

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
}

//VA 转 RVA
void CAddressChangeDlg::VAToRVA(DWORD address) {

	DWORD offset = address - m_ImageNtHeaders.OptionalHeader.ImageBase;

	//如果 VA值 比 exe进程内存大小还要大,说明输入的数据不合理.
	if (offset > m_ImageNtHeaders.OptionalHeader.SizeOfImage)  
	{
		CString resultStr("数据错误");
		SetDlgItemText(IDC_EDIT_RVA, resultStr);
		return;
	}
	
	CString resultStr;
	resultStr.Format(_T("%d"),offset);
	SetDlgItemText(IDC_EDIT_RVA, resultStr);
}

CAddressChangeDlg::~CAddressChangeDlg()
{
}

void CAddressChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddressChangeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Calculation, &CAddressChangeDlg::OnBnClickedButtonCalculation)
	ON_BN_CLICKED(IDOK, &CAddressChangeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddressChangeDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CAddressChangeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CAddressChangeDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CAddressChangeDlg::OnBnClickedButton5)
	ON_EN_SETFOCUS(IDC_EDIT_VA, &CAddressChangeDlg::OnEnSetfocusEditVa)
	ON_EN_SETFOCUS(IDC_EDIT_RVA, &CAddressChangeDlg::OnEnSetfocusEditRva)
	ON_EN_SETFOCUS(IDC_EDIT_RAW, &CAddressChangeDlg::OnEnSetfocusEditRaw)
END_MESSAGE_MAP()


// CAddressChangeDlg 消息处理程序

//点击 计算 按钮
void CAddressChangeDlg::OnBnClickedButtonCalculation()
{
	// TODO: 在此添加控件通知处理程序代码

	/*
	CString contentString;
	GetDlgItemText(IDC_EDIT_VA, contentString);
	if (contentString != "") {

		VAToRVA((DWORD)_wtol(contentString));
	}
	*/

	//-----------得到将要转换的数据
	CString contentString;
	if (m_WhichTextEditGetFocus == 0) {
		MessageBox(_T("未获得文本框焦点"), _T("标题"), 0);
		return;
	}
	else if (m_WhichTextEditGetFocus == 1) {
		GetDlgItemText(IDC_EDIT_VA, contentString);
		if (contentString == "") {
			MessageBox(_T("请填写要转换的数据"), _T("标题"), 0);
			return;
		}
	}
	else if (m_WhichTextEditGetFocus == 2) {
		GetDlgItemText(IDC_EDIT_RVA, contentString);
		if (contentString == "") {
			MessageBox(_T("请填写要转换的数据"), _T("标题"), 0);
			return;
		}
	}
	else if (m_WhichTextEditGetFocus == 3) {
		GetDlgItemText(IDC_EDIT_RAW, contentString);
		if (contentString == "") {
			MessageBox(_T("请填写要转换的数据"), _T("标题"), 0);
			return;
		}
	}
	
	//我们输入的十六进制地址->整型
	char* stopstring = NULL;
	USES_CONVERSION;
	char* szContent = T2A(contentString.GetBuffer(0));
	contentString.ReleaseBuffer();
	UINT unAddr = strtol(szContent, &stopstring, 16);

	//区块的数量
	int nNumberOfSection = 0;
	//内存镜像基址
	int nImageBase = 0;
	//区块表的首位置
	int nSectionTableAddr = 0;
	//内存对齐大小
	int nSectionAlignment = 0;
	//文件对齐大小
	int nFileAlignment = 0;

	//得到区块数量
	nNumberOfSection = m_ImageNtHeaders.FileHeader.NumberOfSections;

	//计算PE最开始位置到 section header 区块表位置的偏移
	nSectionTableAddr = m_ImageDosHeader.e_lfanew + SIZE_OF_NT_SIGNATURE + sizeof(IMAGE_FILE_HEADER)
		+ m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader;

	//得到内存镜像基址
	nImageBase = m_ImageNtHeaders.OptionalHeader.ImageBase;

	//得到内存对齐大小
	nSectionAlignment = m_ImageNtHeaders.OptionalHeader.SectionAlignment;

	//得到文件对齐大小
	nFileAlignment = m_ImageNtHeaders.OptionalHeader.FileAlignment;


	int nVA = 0;
	int nRVA = 0;
	int nFA = 0;

	if (m_WhichTextEditGetFocus == 1) {    //将要用来转换的是 虚拟地址(VA)
	
		//因为该判断中是输入地址到 虚拟地址(VA) 文本框,所以 nVA 的值就是输入的值.
		nVA = unAddr; 
		//相对虚拟地址 = 虚拟地址 - 内存镜像基址
		nRVA = unAddr - nImageBase;  

		for (int i = 0; i < m_sectionHeaders_vt.size(); i++) {  //遍历区块表
		
			//当前节区在内存中的偏移地址
			int nVirtualAddr = m_sectionHeaders_vt[i].VirtualAddress;
			//当前节区在对齐前,内存的大小
			int nVirtualAddrSize = m_sectionHeaders_vt[i].Misc.VirtualSize;

			//相对虚拟地址>=当前节区在内存中的偏移地址 
			//&& 相对虚拟地址<=当前节区在内存中的偏移地址+当前节区在对齐前,内存中的大小
			if (nRVA >= nVirtualAddr && nRVA <= nVirtualAddr + nVirtualAddrSize)
			{
				//文件偏移地址 计算公式 : 
				//文件偏移地址=相对虚拟地址 - 当前节区在内存中的偏移地址+当前节区距离exe的最头部偏移(文件对齐)
				nFA           =  nRVA       -      nVirtualAddr         + m_sectionHeaders_vt[i].PointerToRawData;

				//文件偏移地址>=当前节区距离exe的文件最头部偏移+当前节区对齐后的区块大小(文件对齐)
				if (nFA >= m_sectionHeaders_vt[i].PointerToRawData + m_sectionHeaders_vt[i].SizeOfRawData)
				{
					MessageBox(_T("错误的范围"),_T("标题"),0);
					return;
				}

				CString strTemp;
				strTemp.Format(_T("%08X"), nRVA);
				SetDlgItemText(IDC_EDIT_RVA, strTemp);
				strTemp.Format(_T("%08X"), nFA);
				SetDlgItemText(IDC_EDIT_RAW, strTemp);
				return;
			}
			//相对虚拟地址 >= 0 && 相对虚拟地址 <= 当前节区在内存中的相对虚拟地址
			else if (nRVA >= 0 && nRVA <= nVirtualAddr)  
			{
				//文件偏移地址 计算公式 : 
				//文件偏移地址=相对虚拟地址 - 当前节区在内存中的偏移地址+当前节区距离exe的最头部偏移(文件对齐)
				nFA           = nRVA        - nVirtualAddr              + m_sectionHeaders_vt[i].PointerToRawData;
				//文件偏移地址 >= 当前节区距离exe的最头部偏移(文件对齐) + 当前节区对齐后的区块大小(文件对齐)
				if (nFA >= m_sectionHeaders_vt[i].PointerToRawData + m_sectionHeaders_vt[i].SizeOfRawData)
				{
					MessageBox(_T("错误的范围"), _T("标题"), 0);
					return;
				}

				CString strTemp;
				strTemp.Format(_T("%08X"), nRVA);
				SetDlgItemText(IDC_EDIT_RVA, strTemp);
				strTemp.Format(_T("%08X"), nFA);
				SetDlgItemText(IDC_EDIT_RAW, strTemp);
				return;
			}
		}
	}
	else if (m_WhichTextEditGetFocus == 2)  //将要用来转换的是 相对虚拟地址(RVA)
	{
		nRVA = unAddr;  //当进入该判断时,说明直接填写的是 相对虚拟地址(RVA) 的文本框
		//虚拟地址 = 相对虚拟地址+内存镜像基址
		nVA = nRVA + nImageBase;

		for (int i = 0; i < m_sectionHeaders_vt.size(); i++) {  //遍历区块表
			//节区在内存中的偏移地址
			int nVirtualAddr = m_sectionHeaders_vt[i].VirtualAddress;
			//节区在对齐前,内存的大小
			int nVirtualAddrSize = m_sectionHeaders_vt[i].Misc.VirtualSize;

			//相对虚拟地址 >= 0 && 相对虚拟地址 <= 节区在内存中的偏移地址+节区在对齐前,内存的大小
			if (nRVA >= 0 && nRVA <= nVirtualAddr + nVirtualAddrSize)
			{
				//文件偏移地址 计算公式 : 
				//文件偏移地址=相对虚拟地址 - 当前节区在内存中的偏移地址+当前节区距离exe的最头部偏移(文件对齐)
				nFA           = nRVA        - nVirtualAddr              + m_sectionHeaders_vt[i].PointerToRawData;

				//文件偏移地址 >= 当前节区距离exe的最头部偏移(文件对齐) + 当前节区对齐后的区块大小(文件对齐)
				if (nFA >= m_sectionHeaders_vt[i].PointerToRawData + m_sectionHeaders_vt[i].SizeOfRawData)
				{
					nFA = -1;
				}

				CString strTemp;
				strTemp.Format(_T("%08X"), nVA);
				SetDlgItemText(IDC_EDIT_VA, strTemp);
				strTemp.Format(_T("%08X"), nFA);
				SetDlgItemText(IDC_EDIT_RAW, strTemp);

				return;
			}
		}

	}
	else if (m_WhichTextEditGetFocus == 3)  //将要用来转换的是 文件偏移地址(RAW)
	{
		nFA = unAddr;  //进入该判断,说明直接填写的是 文件偏移地址(RAW) 文本框

		for (int i = 0; i < m_sectionHeaders_vt.size(); i++) {  //遍历区块表

			//得到当前节区距离exe的最头部偏移(文件对齐)
			int nRawDataAddr = m_sectionHeaders_vt[i].PointerToRawData;
			//得到当前节区对齐后的区块大小(文件对齐)
			int nRawDataSize = m_sectionHeaders_vt[i].SizeOfRawData;

			//文件偏移地址 >= 得到当前节区距离exe的最头部偏移(文件对齐)
			//&& 文件偏移地址 <= 得到当前节区距离exe的最头部偏移(文件对齐) + 得到当前节区对齐后的区块大小(文件对齐)
			if (nFA >= nRawDataAddr && nFA <= nRawDataAddr + nRawDataSize)
			{
				//文件偏移地址 计算公式 : 
				//文件偏移地址=相对虚拟地址 - 当前节区在内存中的偏移地址+当前节区距离exe的最头部偏移(文件对齐)
				nRVA          = nFA         - nRawDataAddr              + m_sectionHeaders_vt[i].VirtualAddress;
				//虚拟地址 = 相对虚拟地址 + 内存镜像基址
				nVA = nRVA + nImageBase;

				CString strTemp;
				strTemp.Format(_T("%08X"), nVA);
				SetDlgItemText(IDC_EDIT_VA, strTemp);
				strTemp.Format(_T("%08X"), nRVA);
				SetDlgItemText(IDC_EDIT_RVA, strTemp);
				return;
			}
		}
	}

	MessageBox(_T("无法转换"), _T("标题"), 0);
}


void CAddressChangeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnOK();
}


void CAddressChangeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CAddressChangeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//关闭PE文件
	m_Stream.close();

	CDialogEx::OnClose();
}


//点击 虚拟地址 按钮
void CAddressChangeDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CButton *button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
	button1->SetState(TRUE);
	button1->SetCheck(TRUE);

	CButton *button4 = (CButton*)GetDlgItem(IDC_BUTTON4);
	button4->SetState(FALSE);
	button4->SetCheck(FALSE);

	CButton *button5 = (CButton*)GetDlgItem(IDC_BUTTON5);
	button5->SetState(FALSE);
	button5->SetCheck(FALSE);

	SetDlgItemText(IDC_EDIT_VA, _T(""));
	SetDlgItemText(IDC_EDIT_RVA, _T(""));
	SetDlgItemText(IDC_EDIT_RAW, _T(""));

	CEdit *editVA = (CEdit*)GetDlgItem(IDC_EDIT_VA);
	editVA->SetReadOnly(FALSE);

	CEdit *editRVA = (CEdit*)GetDlgItem(IDC_EDIT_RVA);
	editRVA->SetReadOnly(TRUE);

	CEdit *editRAW = (CEdit*)GetDlgItem(IDC_EDIT_RAW);
	editRAW->SetReadOnly(TRUE);
}

//点击 相对虚拟地址 按钮
void CAddressChangeDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	CButton *button4 = (CButton*)GetDlgItem(IDC_BUTTON4);
	button4->SetState(TRUE);
	button4->SetCheck(TRUE);

	CButton *button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
	button1->SetState(FALSE);
	button1->SetCheck(FALSE);

	CButton *button5 = (CButton*)GetDlgItem(IDC_BUTTON5);
	button5->SetState(FALSE);
	button5->SetCheck(FALSE);


	CEdit *editVA = (CEdit*)GetDlgItem(IDC_EDIT_VA);
	editVA->SetReadOnly(TRUE);

	CEdit *editRVA = (CEdit*)GetDlgItem(IDC_EDIT_RVA);
	editRVA->SetReadOnly(FALSE);

	CEdit *editRAW = (CEdit*)GetDlgItem(IDC_EDIT_RAW);
	editRAW->SetReadOnly(TRUE);
}

//点击 文件偏移地址 按钮
void CAddressChangeDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	CButton *button5 = (CButton*)GetDlgItem(IDC_BUTTON5);
	button5->SetState(TRUE);
	button5->SetCheck(TRUE);

	CButton *button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
	button1->SetState(FALSE);
	button1->SetCheck(FALSE);

	CButton *button4 = (CButton*)GetDlgItem(IDC_BUTTON4);
	button4->SetState(FALSE);
	button4->SetCheck(FALSE);


	CEdit *editVA = (CEdit*)GetDlgItem(IDC_EDIT_VA);
	editVA->SetReadOnly(TRUE);

	CEdit *editRVA = (CEdit*)GetDlgItem(IDC_EDIT_RVA);
	editRVA->SetReadOnly(TRUE);

	CEdit *editRAW = (CEdit*)GetDlgItem(IDC_EDIT_RAW);
	editRAW->SetReadOnly(FALSE);
}

//第一个文本框已获得焦点
void CAddressChangeDlg::OnEnSetfocusEditVa()
{
	// TODO: 在此添加控件通知处理程序代码

	m_WhichTextEditGetFocus = 1;
}

//第二个文本框已获得焦点
void CAddressChangeDlg::OnEnSetfocusEditRva()
{
	// TODO: 在此添加控件通知处理程序代码

	m_WhichTextEditGetFocus = 2;
}

//第三个文本框已获得焦点
void CAddressChangeDlg::OnEnSetfocusEditRaw()
{
	// TODO: 在此添加控件通知处理程序代码

	m_WhichTextEditGetFocus = 3;
}
