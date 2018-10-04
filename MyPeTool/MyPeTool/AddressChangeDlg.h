#pragma once
#include <fstream>
#include <vector>

// CAddressChangeDlg �Ի���

class CAddressChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddressChangeDlg)

public:
	CAddressChangeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddressChangeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AddressChange };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCalculation();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnSetfocusEditVa();
	afx_msg void OnEnSetfocusEditRva();
	afx_msg void OnEnSetfocusEditRaw();

	//---����Dosͷ��Ϣ
	//��Ȼ������NTͷ��Ϣģ��,��������ļ���NTͷ��ϢҪ�õ�Dosͷ��Ϣ�е� e_lfanew ����,
	//����Ҫ�ټ���һ��,�Ա��ȡ�� e_lfanew ��ֵ.
	void loadDosHead();

	//����NTͷ��Ϣ
	void loadNtHead();

	//���� section �������Ϣ(�������������ܻ��ж��)
	void loadSectionHeaders();

	//--------��ַת�����

	//VA ת RVA
	void VAToRVA(DWORD address);

public:

	std::fstream m_Stream;

	//Dosͷ��Ϣ����
	IMAGE_DOS_HEADER m_ImageDosHeader;
	//NTͷ��Ϣ����
	IMAGE_NT_HEADERS m_ImageNtHeaders;

	//����� vector
	std::vector<IMAGE_SECTION_HEADER> m_sectionHeaders_vt;
	
	//�ĸ��ı�������ý���(�����1��ʼ)
	int m_WhichTextEditGetFocus;
};
