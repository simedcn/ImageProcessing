#pragma once
#include <opencv2\opencv.hpp>

// CLomo 对话框

class CLomo : public CDialogEx
{
	DECLARE_DYNAMIC(CLomo)

public:
	CLomo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLomo();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOMO };
	cv::Mat old_img;
	cv::Mat lomo_img;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
};

double Logistic(double x);
double vignette(int i, int j, int cx, int cy);
