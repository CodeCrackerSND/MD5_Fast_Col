// MD5_Fast_ColDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MD5_Fast_Col.h"
#include "MD5_Fast_ColDlg.h"
#include "main.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMD5_Fast_ColDlg dialog




CMD5_Fast_ColDlg::CMD5_Fast_ColDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMD5_Fast_ColDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMD5_Fast_ColDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMD5_Fast_ColDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMD5_Fast_ColDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMD5_Fast_ColDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMD5_Fast_ColDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMD5_Fast_ColDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMD5_Fast_ColDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMD5_Fast_ColDlg message handlers

BOOL CMD5_Fast_ColDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMD5_Fast_ColDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMD5_Fast_ColDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

unsigned load_block(istream& i, uint32 block[]) {
	unsigned len = 0;
	char uc;
	for (unsigned k = 0; k < 16; ++k) {
		block[k] = 0;
		for (unsigned c = 0; c < 4; ++c) {
			i.get(uc);
			if (i) ++len;
			else uc = 0;
			block[k] += uint32((unsigned char)(uc))<<(c*8);
		}
	}
	return len;
}

// IV = 0123456789abcdeffedcba9876543210
const uint32 MD5IV[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

void find_collision(const uint32 IV[], uint32 msg1block0[], uint32 msg1block1[], uint32 msg2block0[], uint32 msg2block1[]) {
	// Generating first block:
	find_block0(msg1block0, IV);

	uint32 IHV[4] = { IV[0], IV[1], IV[2], IV[3] };
	md5_compress(IHV, msg1block0);

	// Generating second block:
	find_block1(msg1block1, IHV);

	for (int t = 0; t < 16; ++t) {
		msg2block0[t] = msg1block0[t];
		msg2block1[t] = msg1block1[t];
	}
	msg2block0[4] += 1 << 31; msg2block0[11] += 1 << 15; msg2block0[14] += 1 << 31;
	msg2block1[4] += 1 << 31; msg2block1[11] -= 1 << 15; msg2block1[14] += 1 << 31;

}

unsigned int Endian_Conv(unsigned int dword)
 {
   return ((dword>>24)&0x000000FF) | ((dword>>8)&0x0000FF00) | ((dword<<8)&0x00FF0000) | ((dword<<24)&0xFF000000);
}

void CMD5_Fast_ColDlg::OnBnClickedOk()
{
// TODO: Add your control notification handler code here
seed32_1 = uint32(time(NULL));
seed32_2 = 0x12345678;
uint32 IV[4] = { MD5IV[0], MD5IV[1], MD5IV[2], MD5IV[3] };

char initialValues[2000];
GetDlgItem(IDC_EDIT4)->GetWindowText(initialValues, 2000);

if (initialValues[0]!=0)
{
char str1[9];
strncpy(str1, initialValues, 8);
str1[8] = 0;

char str2[9];
strncpy(str2, initialValues+1*8, 8);
str2[8] = 0;

char str3[9];
strncpy(str3, initialValues+2*8, 8);
str3[8] = 0;

char str4[9];
strncpy(str4, initialValues+3*8, 8);
str4[8] = 0;

sscanf(str1, "%x", &IV[0]);
sscanf(str2, "%x", &IV[1]);
sscanf(str3, "%x", &IV[2]);
sscanf(str4, "%x", &IV[3]);

IV[0] = Endian_Conv(IV[0]);
IV[1] = Endian_Conv(IV[1]);
IV[2] = Endian_Conv(IV[2]);
IV[3] = Endian_Conv(IV[3]);

}

uint32 msg1block0[16];
uint32 msg1block1[16];
uint32 msg2block0[16];
uint32 msg2block1[16];
find_collision(IV, msg1block0, msg1block1, msg2block0, msg2block1);

char block1hexstr[16*8+16*8+2];
int i;
for (i=0; i<16; i++)
sprintf_s(block1hexstr+i*8, sizeof(block1hexstr),
		  "%08X", Endian_Conv(msg1block0[i]));

for (i=0; i<16; i++)
sprintf_s(block1hexstr+16*8+i*8, sizeof(block1hexstr)
		  , "%08X", Endian_Conv(msg1block1[i]));

GetDlgItem(IDC_EDIT1)->SetWindowText(block1hexstr);

char block2hexstr[16*8+16*8+2];
for (i=0; i<16; i++)
sprintf_s(block2hexstr+i*8, sizeof(block2hexstr)
		  , "%08X", Endian_Conv(msg2block0[i]));

for (i=0; i<16; i++)
sprintf_s(block2hexstr+16*8+i*8, sizeof(block2hexstr)
		  , "%08X", Endian_Conv(msg2block1[i]));

GetDlgItem(IDC_EDIT2)->SetWindowText(block2hexstr);

}


void CMD5_Fast_ColDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this);

	if(dlg.DoModal() == IDOK)
		SetDlgItemText(IDC_EDIT3, dlg.GetPathName());

}


void CMD5_Fast_ColDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	uint32 IV[4] = { MD5IV[0], MD5IV[1], MD5IV[2], MD5IV[3] };
	char* filename = new char[2000];
	GetDlgItem(IDC_EDIT3)->GetWindowText(filename, 2000);

	if (filename[0]!=0)
	{
	ifstream ifs(filename, ios::binary);
	uint32 block[16];
	while (true) {
		unsigned len = load_block(ifs, block);
		if (len) {
			//save_block(ofs1, block);
			//save_block(ofs2, block);
			md5_compress(IV, block);
		} else
			break;
	}
}  // if file name textbox not empty

char *s = new char[33];
sprintf_s(s, 33, "%08x%08x%08x%08x", Endian_Conv(IV[0]),
		  Endian_Conv(IV[1]), Endian_Conv(IV[2]), Endian_Conv(IV[3]));
GetDlgItem(IDC_EDIT4)->SetWindowText(s);

}

void CMD5_Fast_ColDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
char* block1Orig = new char[2000];
GetDlgItem(IDC_EDIT1)->GetWindowText(block1Orig, 2000);

if (block1Orig[0]==0) return;

unsigned char block1Conv[128];  // 128 bytes

for (int i=0;i<sizeof(block1Conv);i++)
sscanf(block1Orig+i*2, "%02hhx", &block1Conv[i]);

	CFileDialog dlg(false, NULL, NULL, NULL, NULL, this);

	if(dlg.DoModal() == IDOK)
	{
	ofstream out(dlg.GetPathName(), ios::binary);
	out.write((char *) &block1Conv, sizeof block1Conv);
	out.close();
	}


}

void CMD5_Fast_ColDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
char* block2Orig = new char[2000];
GetDlgItem(IDC_EDIT2)->GetWindowText(block2Orig, 2000);

if (block2Orig[0]==0) return;

unsigned char block2Conv[128];  // 128 bytes

for (int i=0;i<sizeof(block2Conv);i++)
sscanf(block2Orig+i*2, "%02hhx", &block2Conv[i]);

	CFileDialog dlg(false, NULL, NULL, NULL, NULL, this);

	if(dlg.DoModal() == IDOK)
	{
	ofstream out(dlg.GetPathName(), ios::binary);
	out.write((char *) &block2Conv, sizeof block2Conv);
	out.close();
	}

}
