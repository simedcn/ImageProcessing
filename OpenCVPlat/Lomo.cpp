// Lomo.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenCVPlat.h"
#include "Lomo.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "OpenCVPlatDoc.h"
#include "OpenCVPlatView.h"

// CLomo 对话框

IMPLEMENT_DYNAMIC(CLomo, CDialogEx)

CLomo::CLomo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLomo::IDD, pParent)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	COpenCVPlatDoc *pDoc = (COpenCVPlatDoc*)pMain->GetActiveDocument();
	old_img = pDoc->image.clone();
	lomo_img = pDoc->image.clone();
}

CLomo::~CLomo()
{
}

void CLomo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLomo, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CLomo 消息处理程序


void CLomo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_LOMO);
	double value = pSlidCtrl->GetPos() / 100.0;
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	COpenCVPlatDoc *pDoc = (COpenCVPlatDoc*)pMain->GetActiveDocument();
	COpenCVPlatView *pView = (COpenCVPlatView*)pMain->GetActiveView();
	int height = pDoc->image.size().height;
	int width = pDoc->image.size().width;
	for (int row = 0; row < height - 1; row++)
	{
		uchar *old_data = old_img.ptr<uchar>(row);
		uchar *data = pDoc->image.ptr<uchar>(row);
		uchar *lomo_data = lomo_img.ptr<uchar>(row);
		for (int col = 0; col < width - 1; col++)
		{
			if (pDoc->image.channels() == 3)
			{
				data[col * 3] = (1 - value) * old_data[col * 3] + value * lomo_data[col * 3];
				data[col * 3 + 1] = (1 - value) * old_data[col * 3 + 1] + value * lomo_data[col * 3 + 1];
				data[col * 3 + 2] = (1 - value) * old_data[col * 3 + 2] + value * lomo_data[col * 3 + 2];
			}
			else
			{
				data[col] = (1 - value) * old_data[col] + value * lomo_data[col];
			}
		}
	}
	pView->Invalidate();
}


BOOL CLomo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_LOMO);
	pSlidCtrl->SetRange(0, 100);
	pSlidCtrl->SetPos(0);

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	COpenCVPlatDoc *pDoc = (COpenCVPlatDoc*)pMain->GetActiveDocument();
	COpenCVPlatView *pView = (COpenCVPlatView*)pMain->GetActiveView();
	int height = pDoc->image.size().height;
	int width = pDoc->image.size().width;
	int cx = width / 2;
	int cy = height / 2;
	for (int row = 0; row < height; row++)
	{
		uchar *old_data = old_img.ptr<uchar>(row);
		uchar *data = lomo_img.ptr<uchar>(row);
		for (int col = 0; col < width; col++)
		{
			double dark = 1 - vignette(col, row, cx, cy);
			if (pDoc->image.channels() == 3)
			{
				double b = old_data[col * 3] * Logistic(old_data[col * 3] / 255.0) * dark + 10;
				double g = old_data[col * 3 + 1] * Logistic(old_data[col * 3 + 1] / 255.0) * dark + 20;
				double r = old_data[col * 3 + 2] * Logistic(old_data[col * 3 + 2] / 255.0) * dark;
				data[col * 3] = b > 255 ? 255 : b;
				data[col * 3 + 1] = g > 255 ? 255 : g;
				data[col * 3 + 2] = r > 255 ? 255 : r;
			}
			else
			{
				double b = old_data[col] * Logistic(old_data[col] / 255.0);
				b = b * dark;
				data[col] = b > 255 ? 255 : b;
			}
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

inline double Logistic(double x)
{
	double t = sqrt(1 + exp(6 - 12 * x));
	return 1.5 / t;
}

inline double vignette(int i, int j, int cx, int cy)
{
	double di = (i - cx)*(i - cx);
	double dj = (j - cy)*(j - cy);
	double rs = cx*cx + cy*cy;
	double x = (di + dj) / rs;
	return x;
}
