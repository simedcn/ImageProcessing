#pragma once
#include <opencv2\opencv.hpp>

// CColorBalance 对话框

class CColorBalance : public CDialogEx
{
	DECLARE_DYNAMIC(CColorBalance)

public:
	CColorBalance(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CColorBalance();

// 对话框数据
	enum { IDD = IDD_DIALOG_COLOR_BALANCE };
	cv::Mat old_img;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnAwb();
};
