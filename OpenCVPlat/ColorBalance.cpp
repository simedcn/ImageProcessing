// ColorBalance.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenCVPlat.h"
#include "ColorBalance.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "OpenCVPlatDoc.h"
#include "OpenCVPlatView.h"


// CColorBalance 对话框

IMPLEMENT_DYNAMIC(CColorBalance, CDialogEx)

CColorBalance::CColorBalance(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColorBalance::IDD, pParent)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	COpenCVPlatDoc *pDoc = (COpenCVPlatDoc*)pMain->GetActiveDocument();
	old_img = pDoc->image.clone();
}

CColorBalance::~CColorBalance()
{
}

void CColorBalance::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColorBalance, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_AWB, &CColorBalance::OnBnClickedBtnAwb)
END_MESSAGE_MAP()


// CColorBalance 消息处理程序


BOOL CColorBalance::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_RED);
	pSlidCtrl->SetRange(1, 255);
	pSlidCtrl->SetPos(255);
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_GREEN);
	pSlidCtrl->SetRange(1, 255);
	pSlidCtrl->SetPos(255);
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_BLUE);
	pSlidCtrl->SetRange(1, 255);
	pSlidCtrl->SetPos(255);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CColorBalance::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_RED);
	int r = pSlidCtrl->GetPos();
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_GREEN);
	int g = pSlidCtrl->GetPos();
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_BLUE);
	int b = pSlidCtrl->GetPos();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	COpenCVPlatDoc *pDoc = (COpenCVPlatDoc*)pMain->GetActiveDocument();
	COpenCVPlatView *pView = (COpenCVPlatView*)pMain->GetActiveView();
	if (pDoc->image.channels() == 1 || BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_ALL))
	{
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_RED);
		pSlidCtrl->SetPos(pSlider->GetPos());
		pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_GREEN);
		pSlidCtrl->SetPos(pSlider->GetPos());
		pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_BLUE);
		pSlidCtrl->SetPos(pSlider->GetPos());
		b = pSlider->GetPos();
		g = pSlider->GetPos();
		r = pSlider->GetPos();
	}
	int height = pDoc->image.size().height;
	int width = pDoc->image.size().width;
	if (pDoc->image.channels() == 3)
	{
		for (int row = 0; row < height; row++)
		{
			uchar *data = pDoc->image.ptr<uchar>(row);
			uchar *old_data = old_img.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
			{
				int bn = old_data[col * 3] * 255.0 / b;
				int gn = old_data[col * 3 + 1] * 255.0 / g;
				int rn = old_data[col * 3 + 2] * 255.0 / r;
				data[col * 3] = bn > 255 ? 255 : bn;
				data[col * 3 + 1] = gn > 255 ? 255 : gn;
				data[col * 3 + 2] = rn > 255 ? 255 : rn;
			}
		}
	}
	else
	{
		for (int row = 0; row < height; row++)
		{
			uchar *data = pDoc->image.ptr<uchar>(row);
			uchar *old_data = old_img.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
			{
				int bn = old_data[col] * 255.0 / b;
				data[col] = bn > 255 ? 255 : bn;
			}
		}
	}
	pView->Invalidate();
}


void CColorBalance::OnBnClickedBtnAwb()
{
	// TODO:  在此添加控件通知处理程序代码
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	COpenCVPlatDoc *pDoc = (COpenCVPlatDoc*)pMain->GetActiveDocument();
	COpenCVPlatView *pView = (COpenCVPlatView*)pMain->GetActiveView();
	int height = pDoc->image.size().height;
	int width = pDoc->image.size().width;
	double total_r = 0, total_g = 0, total_b = 0;
	double avg_r = 0;
	double avg_g = 0;
	double avg_b = 0;
	if (pDoc->image.channels() == 3)
	{
		for (int row = 0; row < height; row++)
		{
			uchar *old_data = old_img.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
			{
				total_b += old_data[col * 3];
				total_g += old_data[col * 3 + 1];
				total_r += old_data[col * 3 + 2];
			}
		}
		avg_r = total_r / (width * height);
		avg_g = total_g / (width * height);
		avg_b = total_b / (width * height);
	}
	else
	{
		for (int row = 0; row < height; row++)
		{
			uchar *old_data = old_img.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
				total_b += old_data[col];
		}
		avg_b = total_b / (width * height);
		avg_r = avg_g = avg_b;
	}
	double gray = (avg_r + avg_g + avg_b) / 3;
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_RED);
	pSlidCtrl->SetPos(255 * avg_r / gray);
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_GREEN);
	pSlidCtrl->SetPos(255 * avg_g / gray);
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_COLOR_BALANCE_BLUE);
	pSlidCtrl->SetPos(255 * avg_b / gray);

	if (pDoc->image.channels() == 3)
	{
		for (int row = 0; row < height; row++)
		{
			uchar *data = pDoc->image.ptr<uchar>(row);
			uchar *old_data = old_img.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
			{
				int bn = old_data[col * 3] * gray / avg_b;
				int gn = old_data[col * 3 + 1] * gray / avg_g;
				int rn = old_data[col * 3 + 2] * gray / avg_r;
				data[col * 3] = bn > 255 ? 255 : bn;
				data[col * 3 + 1] = gn > 255 ? 255 : gn;
				data[col * 3 + 2] = rn > 255 ? 255 : rn;
			}
		}
	}
	else
	{
		for (int row = 0; row < height; row++)
		{
			uchar *data = pDoc->image.ptr<uchar>(row);
			uchar *old_data = old_img.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
			{
				int bn = old_data[col] * gray / avg_b;
				data[col] = bn > 255 ? 255 : bn;
			}
		}
	}
	pView->Invalidate();
}
