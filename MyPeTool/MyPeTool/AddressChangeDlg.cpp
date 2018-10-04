// AddressChangeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "AddressChangeDlg.h"
#include "afxdialogex.h"


// CAddressChangeDlg �Ի���

IMPLEMENT_DYNAMIC(CAddressChangeDlg, CDialogEx)

BOOL CAddressChangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_WhichTextEditGetFocus = 0;

	CButton *button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
	button1->SetState(TRUE);
	button1->SetCheck(TRUE);

	CString tipStr = _T("�����ʽ��͡���\n������ַ(ImageBase).\n�������ַ(Virtual Address, VA).\n����������ַ(Relative Virtual Address, RVA).\n���ļ�ƫ�Ƶ�ַ(File Offset) �ֳ� �����ַ(RAW Offset).\n\
\n�����㹫ʽ����\n����������ַ(RVA) = �����ַ(VA) - ����ַ(ImageBase).\n�������ַ(VA) = ��������ַ(RVA) + ����ַ(ImageBase).\n���ļ�ƫ��(File Offset) = ��������ַ(RVA) - �õ�ַ���ڽڵ�����ƫ��(Virtual Offset) + �õ�ַ���ڽڵ�����ƫ��(Raw Offset).\n����������ַ(RVA) = �ļ�ƫ��(File Offset) - �õ�ַ���ڽڵ�����ƫ��(Raw Offset) + �õ�ַ���ڽڵ�����ƫ��(Virtual Offset).");

	SetDlgItemText(IDC_STATIC_Tip, tipStr);

	//��PE�ļ�
	m_Stream.open(AnalysisExeName, std::fstream::binary | std::fstream::in);

	//����Dosͷ��Ϣ
	loadDosHead();

	//����NTͷ��Ϣ
	loadNtHead();

	//���� section �������Ϣ(�������������ܻ��ж��)
	loadSectionHeaders();
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

CAddressChangeDlg::CAddressChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_AddressChange, pParent)
{

}

//---����Dosͷ��Ϣ
//��Ȼ������NTͷ��Ϣģ��,��������ļ���NTͷ��ϢҪ�õ�Dosͷ��Ϣ�е� e_lfanew ����,
//����Ҫ�ټ���һ��,�Ա��ȡ�� e_lfanew ��ֵ.
void CAddressChangeDlg::loadDosHead() {

	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//����NTͷ��Ϣ
void CAddressChangeDlg::loadNtHead() {

	//DOSͷ��Ϣ�е�e_lfanewֵΪNTͷ�Ŀ�ʼλ��
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	//NT��ʶ-----��ȡ NTͷ �е� Signature ��Ϣ.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature, sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("�Ҳ���NT��ʶ"), _T("��ʾ"), 0);
	}

	//�ļ�ͷ------��ȡ NTͷ �е� _IMAGE_FILE_HEADER �ṹ�� ��Ϣ
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader, sizeof(IMAGE_FILE_HEADER));


	//��ѡͷ------��ȡ NTͷ �е� _IMAGE_OPTIONAL_HEADER �ṹ�� ��Ϣ
	//SizeOfOptionalHeader:��ѡͷ�Ĵ�С
	if (m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader != 0) {

		m_Stream.read((char*)&m_ImageNtHeaders.OptionalHeader, m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader);
	}
}

#define SIZE_OF_NT_SIGNATURE 4

//���� section �������Ϣ(�������������ܻ��ж��)
void CAddressChangeDlg::loadSectionHeaders() {

	//����PE�ʼλ�õ� section header �����λ�õ�ƫ��
	int sectionsOffset = m_ImageDosHeader.e_lfanew + SIZE_OF_NT_SIGNATURE + sizeof(IMAGE_FILE_HEADER)
		+ m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader;
	//section �ĸ���
	int sectionNum = m_ImageNtHeaders.FileHeader.NumberOfSections;

	//����section ���ܴ�С
	int sectionSize = sectionNum * sizeof(IMAGE_SECTION_HEADER);

	//���� ����� vector ��С
	m_sectionHeaders_vt.resize(sectionNum);

	//���ö�ȡ�ļ���ƫ��
	m_Stream.seekg(sectionsOffset);

	//��ȡ�ļ��е� ����� ����
	m_Stream.read((char*)m_sectionHeaders_vt.data(), sectionSize);
}

//VA ת RVA
void CAddressChangeDlg::VAToRVA(DWORD address) {

	DWORD offset = address - m_ImageNtHeaders.OptionalHeader.ImageBase;

	//��� VAֵ �� exe�����ڴ��С��Ҫ��,˵����������ݲ�����.
	if (offset > m_ImageNtHeaders.OptionalHeader.SizeOfImage)  
	{
		CString resultStr("���ݴ���");
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


// CAddressChangeDlg ��Ϣ�������

//��� ���� ��ť
void CAddressChangeDlg::OnBnClickedButtonCalculation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	/*
	CString contentString;
	GetDlgItemText(IDC_EDIT_VA, contentString);
	if (contentString != "") {

		VAToRVA((DWORD)_wtol(contentString));
	}
	*/

	//-----------�õ���Ҫת��������
	CString contentString;
	if (m_WhichTextEditGetFocus == 0) {
		MessageBox(_T("δ����ı��򽹵�"), _T("����"), 0);
		return;
	}
	else if (m_WhichTextEditGetFocus == 1) {
		GetDlgItemText(IDC_EDIT_VA, contentString);
		if (contentString == "") {
			MessageBox(_T("����дҪת��������"), _T("����"), 0);
			return;
		}
	}
	else if (m_WhichTextEditGetFocus == 2) {
		GetDlgItemText(IDC_EDIT_RVA, contentString);
		if (contentString == "") {
			MessageBox(_T("����дҪת��������"), _T("����"), 0);
			return;
		}
	}
	else if (m_WhichTextEditGetFocus == 3) {
		GetDlgItemText(IDC_EDIT_RAW, contentString);
		if (contentString == "") {
			MessageBox(_T("����дҪת��������"), _T("����"), 0);
			return;
		}
	}
	
	//���������ʮ�����Ƶ�ַ->����
	char* stopstring = NULL;
	USES_CONVERSION;
	char* szContent = T2A(contentString.GetBuffer(0));
	contentString.ReleaseBuffer();
	UINT unAddr = strtol(szContent, &stopstring, 16);

	//���������
	int nNumberOfSection = 0;
	//�ڴ澵���ַ
	int nImageBase = 0;
	//��������λ��
	int nSectionTableAddr = 0;
	//�ڴ�����С
	int nSectionAlignment = 0;
	//�ļ������С
	int nFileAlignment = 0;

	//�õ���������
	nNumberOfSection = m_ImageNtHeaders.FileHeader.NumberOfSections;

	//����PE�ʼλ�õ� section header �����λ�õ�ƫ��
	nSectionTableAddr = m_ImageDosHeader.e_lfanew + SIZE_OF_NT_SIGNATURE + sizeof(IMAGE_FILE_HEADER)
		+ m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader;

	//�õ��ڴ澵���ַ
	nImageBase = m_ImageNtHeaders.OptionalHeader.ImageBase;

	//�õ��ڴ�����С
	nSectionAlignment = m_ImageNtHeaders.OptionalHeader.SectionAlignment;

	//�õ��ļ������С
	nFileAlignment = m_ImageNtHeaders.OptionalHeader.FileAlignment;


	int nVA = 0;
	int nRVA = 0;
	int nFA = 0;

	if (m_WhichTextEditGetFocus == 1) {    //��Ҫ����ת������ �����ַ(VA)
	
		//��Ϊ���ж����������ַ�� �����ַ(VA) �ı���,���� nVA ��ֵ���������ֵ.
		nVA = unAddr; 
		//��������ַ = �����ַ - �ڴ澵���ַ
		nRVA = unAddr - nImageBase;  

		for (int i = 0; i < m_sectionHeaders_vt.size(); i++) {  //���������
		
			//��ǰ�������ڴ��е�ƫ�Ƶ�ַ
			int nVirtualAddr = m_sectionHeaders_vt[i].VirtualAddress;
			//��ǰ�����ڶ���ǰ,�ڴ�Ĵ�С
			int nVirtualAddrSize = m_sectionHeaders_vt[i].Misc.VirtualSize;

			//��������ַ>=��ǰ�������ڴ��е�ƫ�Ƶ�ַ 
			//&& ��������ַ<=��ǰ�������ڴ��е�ƫ�Ƶ�ַ+��ǰ�����ڶ���ǰ,�ڴ��еĴ�С
			if (nRVA >= nVirtualAddr && nRVA <= nVirtualAddr + nVirtualAddrSize)
			{
				//�ļ�ƫ�Ƶ�ַ ���㹫ʽ : 
				//�ļ�ƫ�Ƶ�ַ=��������ַ - ��ǰ�������ڴ��е�ƫ�Ƶ�ַ+��ǰ��������exe����ͷ��ƫ��(�ļ�����)
				nFA           =  nRVA       -      nVirtualAddr         + m_sectionHeaders_vt[i].PointerToRawData;

				//�ļ�ƫ�Ƶ�ַ>=��ǰ��������exe���ļ���ͷ��ƫ��+��ǰ���������������С(�ļ�����)
				if (nFA >= m_sectionHeaders_vt[i].PointerToRawData + m_sectionHeaders_vt[i].SizeOfRawData)
				{
					MessageBox(_T("����ķ�Χ"),_T("����"),0);
					return;
				}

				CString strTemp;
				strTemp.Format(_T("%08X"), nRVA);
				SetDlgItemText(IDC_EDIT_RVA, strTemp);
				strTemp.Format(_T("%08X"), nFA);
				SetDlgItemText(IDC_EDIT_RAW, strTemp);
				return;
			}
			//��������ַ >= 0 && ��������ַ <= ��ǰ�������ڴ��е���������ַ
			else if (nRVA >= 0 && nRVA <= nVirtualAddr)  
			{
				//�ļ�ƫ�Ƶ�ַ ���㹫ʽ : 
				//�ļ�ƫ�Ƶ�ַ=��������ַ - ��ǰ�������ڴ��е�ƫ�Ƶ�ַ+��ǰ��������exe����ͷ��ƫ��(�ļ�����)
				nFA           = nRVA        - nVirtualAddr              + m_sectionHeaders_vt[i].PointerToRawData;
				//�ļ�ƫ�Ƶ�ַ >= ��ǰ��������exe����ͷ��ƫ��(�ļ�����) + ��ǰ���������������С(�ļ�����)
				if (nFA >= m_sectionHeaders_vt[i].PointerToRawData + m_sectionHeaders_vt[i].SizeOfRawData)
				{
					MessageBox(_T("����ķ�Χ"), _T("����"), 0);
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
	else if (m_WhichTextEditGetFocus == 2)  //��Ҫ����ת������ ��������ַ(RVA)
	{
		nRVA = unAddr;  //��������ж�ʱ,˵��ֱ����д���� ��������ַ(RVA) ���ı���
		//�����ַ = ��������ַ+�ڴ澵���ַ
		nVA = nRVA + nImageBase;

		for (int i = 0; i < m_sectionHeaders_vt.size(); i++) {  //���������
			//�������ڴ��е�ƫ�Ƶ�ַ
			int nVirtualAddr = m_sectionHeaders_vt[i].VirtualAddress;
			//�����ڶ���ǰ,�ڴ�Ĵ�С
			int nVirtualAddrSize = m_sectionHeaders_vt[i].Misc.VirtualSize;

			//��������ַ >= 0 && ��������ַ <= �������ڴ��е�ƫ�Ƶ�ַ+�����ڶ���ǰ,�ڴ�Ĵ�С
			if (nRVA >= 0 && nRVA <= nVirtualAddr + nVirtualAddrSize)
			{
				//�ļ�ƫ�Ƶ�ַ ���㹫ʽ : 
				//�ļ�ƫ�Ƶ�ַ=��������ַ - ��ǰ�������ڴ��е�ƫ�Ƶ�ַ+��ǰ��������exe����ͷ��ƫ��(�ļ�����)
				nFA           = nRVA        - nVirtualAddr              + m_sectionHeaders_vt[i].PointerToRawData;

				//�ļ�ƫ�Ƶ�ַ >= ��ǰ��������exe����ͷ��ƫ��(�ļ�����) + ��ǰ���������������С(�ļ�����)
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
	else if (m_WhichTextEditGetFocus == 3)  //��Ҫ����ת������ �ļ�ƫ�Ƶ�ַ(RAW)
	{
		nFA = unAddr;  //������ж�,˵��ֱ����д���� �ļ�ƫ�Ƶ�ַ(RAW) �ı���

		for (int i = 0; i < m_sectionHeaders_vt.size(); i++) {  //���������

			//�õ���ǰ��������exe����ͷ��ƫ��(�ļ�����)
			int nRawDataAddr = m_sectionHeaders_vt[i].PointerToRawData;
			//�õ���ǰ���������������С(�ļ�����)
			int nRawDataSize = m_sectionHeaders_vt[i].SizeOfRawData;

			//�ļ�ƫ�Ƶ�ַ >= �õ���ǰ��������exe����ͷ��ƫ��(�ļ�����)
			//&& �ļ�ƫ�Ƶ�ַ <= �õ���ǰ��������exe����ͷ��ƫ��(�ļ�����) + �õ���ǰ���������������С(�ļ�����)
			if (nFA >= nRawDataAddr && nFA <= nRawDataAddr + nRawDataSize)
			{
				//�ļ�ƫ�Ƶ�ַ ���㹫ʽ : 
				//�ļ�ƫ�Ƶ�ַ=��������ַ - ��ǰ�������ڴ��е�ƫ�Ƶ�ַ+��ǰ��������exe����ͷ��ƫ��(�ļ�����)
				nRVA          = nFA         - nRawDataAddr              + m_sectionHeaders_vt[i].VirtualAddress;
				//�����ַ = ��������ַ + �ڴ澵���ַ
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

	MessageBox(_T("�޷�ת��"), _T("����"), 0);
}


void CAddressChangeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnOK();
}


void CAddressChangeDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CAddressChangeDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnClose();
}


//��� �����ַ ��ť
void CAddressChangeDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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

//��� ��������ַ ��ť
void CAddressChangeDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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

//��� �ļ�ƫ�Ƶ�ַ ��ť
void CAddressChangeDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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

//��һ���ı����ѻ�ý���
void CAddressChangeDlg::OnEnSetfocusEditVa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_WhichTextEditGetFocus = 1;
}

//�ڶ����ı����ѻ�ý���
void CAddressChangeDlg::OnEnSetfocusEditRva()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_WhichTextEditGetFocus = 2;
}

//�������ı����ѻ�ý���
void CAddressChangeDlg::OnEnSetfocusEditRaw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_WhichTextEditGetFocus = 3;
}
