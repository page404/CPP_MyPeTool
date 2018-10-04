// ImageFileHeaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "ImageFileHeaderDlg.h"
#include "afxdialogex.h"


// CImageFileHeaderDlg �Ի���

IMPLEMENT_DYNAMIC(CImageFileHeaderDlg, CDialogEx)

BOOL CImageFileHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��PE�ļ�
	m_Stream.open(AnalysisExeName, std::fstream::binary | std::fstream::in);

	//����Dosͷ��Ϣ
	loadDosHead();

	//����NTͷ��Ϣ
	loadNtHead();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
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

//---����Dosͷ��Ϣ
//��Ȼ������NTͷ��Ϣģ��,��������ļ���NTͷ��ϢҪ�õ�Dosͷ��Ϣ�е� e_lfanew ����,
//����Ҫ�ټ���һ��,�Ա��ȡ�� e_lfanew ��ֵ.
void CImageFileHeaderDlg::loadDosHead() {
	
	memset(&m_ImageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&m_ImageDosHeader, sizeof(IMAGE_DOS_HEADER));
}

//����NTͷ��Ϣ
void CImageFileHeaderDlg::loadNtHead() {

	//DOSͷ��Ϣ�е�e_lfanewֵΪNTͷ�Ŀ�ʼλ��
	m_Stream.seekg(m_ImageDosHeader.e_lfanew);

	memset(&m_ImageNtHeaders, 0, sizeof(IMAGE_NT_HEADERS));

	//NT��ʶ-----��ȡ NTͷ �е� Signature ��Ϣ.
	m_Stream.read((char*)&m_ImageNtHeaders.Signature,sizeof(DWORD));
	if (m_ImageNtHeaders.Signature != IMAGE_NT_SIGNATURE) {

		MessageBox(_T("�Ҳ���NT��ʶ"), _T("��ʾ"), 0);
	}

	//�ļ�ͷ------��ȡ NTͷ �е� _IMAGE_FILE_HEADER �ṹ�� ��Ϣ
	m_Stream.read((char*)&m_ImageNtHeaders.FileHeader,sizeof(IMAGE_FILE_HEADER));

	//---��ѡͷ�����������һ��Dialog����ʾ
	//��ѡͷ------��ȡ NTͷ �е� _IMAGE_OPTIONAL_HEADER �ṹ�� ��Ϣ
	//SizeOfOptionalHeader:��ѡͷ�Ĵ�С
	//if (m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader != 0) {
	    
		//m_Stream.read((char*)&m_ImageNtHeaders.OptionalHeader, m_ImageNtHeaders.FileHeader.SizeOfOptionalHeader);
	//}

	
	//-----��ʾ �ļ�ͷ �����Ϣ��������.

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


// CImageFileHeaderDlg ��Ϣ�������


void CImageFileHeaderDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnOK();
}


void CImageFileHeaderDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CImageFileHeaderDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnClose();
}
