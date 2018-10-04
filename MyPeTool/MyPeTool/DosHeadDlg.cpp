// DosHeadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyPeTool.h"
#include "DosHeadDlg.h"
#include "afxdialogex.h"

// CDosHeadDlg �Ի���

IMPLEMENT_DYNAMIC(CDosHeadDlg, CDialogEx)

void CDosHeadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

CDosHeadDlg::CDosHeadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DosHead, pParent)
{
	
}


BOOL CDosHeadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��PE�ļ�
	m_Stream.open(AnalysisExeName, std::fstream::binary | std::fstream::in);

	//����PE��Dosͷ��Ϣ
	loadDosHead();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//����Dosͷ��Ϣ
void CDosHeadDlg::loadDosHead() {
	IMAGE_DOS_HEADER imageDosHeader;
	memset(&imageDosHeader, 0, sizeof(IMAGE_DOS_HEADER));

	m_Stream.seekg(0);

	m_Stream.read((char*)&imageDosHeader, sizeof(IMAGE_DOS_HEADER));

	if (imageDosHeader.e_magic == IMAGE_DOS_SIGNATURE) {    // MZ
		CString e_magic_temp;
		e_magic_temp.Format(_T("%04x"), imageDosHeader.e_magic);
		GetDlgItem(IDC_EDIT_eMagic)->SetWindowText(e_magic_temp);
	}
	else {
	
		MessageBox(_T("�Ҳ���MZ��ʶ"), _T("��ʾ"), 0);
	}

	CString e_cblp_temp;
	e_cblp_temp.Format(_T("%04x"), imageDosHeader.e_cblp);
	GetDlgItem(IDC_EDIT_eCblp)->SetWindowText(e_cblp_temp);

	CString e_cp_temp;
	e_cp_temp.Format(_T("%04x"), imageDosHeader.e_cp);
	GetDlgItem(IDC_EDIT_eCp)->SetWindowText(e_cp_temp);

	CString e_crlc_temp;
	e_crlc_temp.Format(_T("%04x"), imageDosHeader.e_crlc);
	GetDlgItem(IDC_EDIT_eCrlc)->SetWindowText(e_crlc_temp);

	CString e_cparhdr_temp;
	e_cparhdr_temp.Format(_T("%04x"), imageDosHeader.e_cparhdr);
	GetDlgItem(IDC_EDIT_eCparhdr)->SetWindowText(e_cparhdr_temp);

	CString e_minalloc_temp;
	e_minalloc_temp.Format(_T("%04x"), imageDosHeader.e_minalloc);
	GetDlgItem(IDC_EDIT_eMinalloc)->SetWindowText(e_minalloc_temp);

	CString e_maxalloc_temp;
	e_maxalloc_temp.Format(_T("%04x"), imageDosHeader.e_maxalloc);
	GetDlgItem(IDC_EDIT_eMaxalloc)->SetWindowText(e_maxalloc_temp);

	CString e_ss_temp;
	e_ss_temp.Format(_T("%04x"), imageDosHeader.e_ss);
	GetDlgItem(IDC_EDIT_eSs)->SetWindowText(e_ss_temp);

	CString e_sp_temp;
	e_sp_temp.Format(_T("%04x"), imageDosHeader.e_sp);
	GetDlgItem(IDC_EDIT_eSp)->SetWindowText(e_sp_temp);

	CString e_csum_temp;
	e_csum_temp.Format(_T("%04x"), imageDosHeader.e_csum);
	GetDlgItem(IDC_EDIT_eCsum)->SetWindowText(e_csum_temp);

	CString e_ip_temp;
	e_ip_temp.Format(_T("%04x"), imageDosHeader.e_ip);
	GetDlgItem(IDC_EDIT_eIp)->SetWindowText(e_ip_temp);

	CString e_cs_temp;
	e_cs_temp.Format(_T("%04x"), imageDosHeader.e_cs);
	GetDlgItem(IDC_EDIT_eCs)->SetWindowText(e_cs_temp);

	CString e_lfarlc_temp;
	e_lfarlc_temp.Format(_T("%04x"), imageDosHeader.e_lfarlc);
	GetDlgItem(IDC_EDIT_eLfarlc)->SetWindowText(e_lfarlc_temp);

	CString e_ovno_temp;
	e_ovno_temp.Format(_T("%04x"), imageDosHeader.e_ovno);
	GetDlgItem(IDC_EDIT_eOvno)->SetWindowText(e_ovno_temp);

	CString e_res4_temp;
	for (int i = 0; i < 4;i++) {
		CString e_res4_temp2;
		e_res4_temp2.Format(_T("%04x"), imageDosHeader.e_res[i]);
		e_res4_temp.AppendFormat(e_res4_temp2);
	}
	GetDlgItem(IDC_EDIT_eResArry)->SetWindowText(e_res4_temp);

	CString e_oemid_temp;
	e_oemid_temp.Format(_T("%04x"), imageDosHeader.e_oemid);
	GetDlgItem(IDC_EDIT_eOemid)->SetWindowText(e_oemid_temp);

	CString e_oeminfo_temp;
	e_oeminfo_temp.Format(_T("%04x"), imageDosHeader.e_oeminfo);
	GetDlgItem(IDC_EDIT_eOeminfo)->SetWindowText(e_oeminfo_temp);

	CString e_resArry10_temp;
	for (int i = 0; i < 10;i++) {
		CString e_resArry10_temp2;
		e_resArry10_temp2.Format(_T("%04x"), imageDosHeader.e_res2[i]);
		e_resArry10_temp.AppendFormat(e_resArry10_temp2);
	}
	GetDlgItem(IDC_EDIT_eResArry20)->SetWindowText(e_resArry10_temp);

	CString e_lfanew_temp;
	e_lfanew_temp.Format(_T("%08x"), imageDosHeader.e_lfanew);
	GetDlgItem(IDC_EDIT_eLfanew)->SetWindowText(e_lfanew_temp);
}

CDosHeadDlg::~CDosHeadDlg()
{
	
}


BEGIN_MESSAGE_MAP(CDosHeadDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDosHeadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDosHeadDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDosHeadDlg ��Ϣ�������



void CDosHeadDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnOK();
}


void CDosHeadDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnCancel();
}


void CDosHeadDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�ر�PE�ļ�
	m_Stream.close();

	CDialogEx::OnClose();
}
