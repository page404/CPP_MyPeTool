// SectionTableDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "SectionTableDlg.h"
#include "afxdialogex.h"


// CSectionTableDlg �Ի���

IMPLEMENT_DYNAMIC(CSectionTableDlg, CDialogEx)


BOOL CSectionTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʾ������
	showListControlHead();

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

CSectionTableDlg::CSectionTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SectionTables, pParent)
{

}

void CSectionTableDlg::showListControlHead() {

	m_sectionTablesList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_sectionTablesList.InsertColumn(0, _T("����"), LVCFMT_LEFT, 50);
	m_sectionTablesList.InsertColumn(1, _T("V.�ڴ���ƫ��"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(2, _T("V.�ڴ��д�С"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(3, _T("R.�ļ���ƫ��"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(4, _T("R.�ļ��д�С"), LVCFMT_LEFT, 110);
	m_sectionTablesList.InsertColumn(5, _T("��־"), LVCFMT_LEFT, 110);
}

//---����Dosͷ��Ϣ
//��Ȼ������NTͷ��Ϣģ��,��������ļ���NTͷ��ϢҪ�õ�Dosͷ��Ϣ�е� e_lfanew ����,
//����Ҫ�ټ���һ��,�Ա��ȡ�� e_lfanew ��ֵ.
void CSectionTableDlg::loadDosHead() {

	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//����NTͷ��Ϣ
void CSectionTableDlg::loadNtHead() {

	//DOSͷ��Ϣ�е�e_lfanewֵΪNTͷ�Ŀ�ʼλ��
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	//NT��ʶ-----��ȡ NTͷ �е� Signature ��Ϣ.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature, sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("�Ҳ���NT��ʶ"), _T("��ʾ"), 0);
	}

	//�ļ�ͷ------��ȡ NTͷ �е� _IMAGE_FILE_HEADER �ṹ�� ��Ϣ
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader, sizeof(IMAGE_FILE_HEADER));
}

#define SIZE_OF_NT_SIGNATURE 4

//���� section �������Ϣ(�������������ܻ��ж��)
void CSectionTableDlg::loadSectionHeaders() {

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


	//-----------------��������ʾ�������
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


// CSectionTableDlg ��Ϣ�������




void CSectionTableDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnOK();
}


void CSectionTableDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CSectionTableDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnClose();
}
