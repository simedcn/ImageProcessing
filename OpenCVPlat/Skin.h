#pragma once
#include <opencv2\opencv.hpp>

// CSkin 对话框

class CSkin : public CDialogEx
{
	DECLARE_DYNAMIC(CSkin)

public:
	CSkin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSkin();

// 对话框数据
	enum { IDD = IDD_DIALOG_SKIN };
	cv::Mat old_img;
	cv::Mat rub_img;
	std::vector<cv::Rect> faces;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void face_detect(const cv::Mat &frame);
	// 提示是否检测到人脸
	CString face_info;
};

double wij(int i, int j, int x, int y, uchar gij, uchar gxy);
inline double mask(int x, int y, int cx, int cy, int w, int h);