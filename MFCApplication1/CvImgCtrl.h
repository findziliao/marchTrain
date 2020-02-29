/************************************************************************/
/*                              OpenCV MFC图像显示控件 v1.0
/*    该控件很容易集成到mfc中显示，具体用法见demo：
/*                                                                               Wish: 512690069
/*                                                                      2016年4月6日 15:28:58 
/************************************************************************/

#pragma once

#include "cv.h"
#include "highgui.h"
#include <afxwin.h>

//自定义消息基础值
#define MsgBase					(WM_USER + 0xA0)
#define MsgLBDown			(MsgBase + 0)					//鼠标左键被按下的消息
#define MsgLBUp					(MsgBase + 1)					//左键抬起来
#define MsgRBDown			(MsgBase + 2)					//右键按下
#define MsgMove				(MsgBase + 3)					//鼠标移动

#define MsgMBDown			(MsgBase + 4)					//中键按下
#define MsgMBUp				(MsgBase + 5)					//中键抬起

#define MsgKeyDown			(MsgBase + 6)					//按下某个键盘按键	
#define MsgDragFile				(MsgBase + 7)					//拖动文件到控件上去的时候

#define MsgVideoStart			(MsgBase + 8)					//视频开始的时候
#define MsgVideoOver			(MsgBase + 9)					//视频结束了


//得到一帧视频画面了，具体看demo
//1、在dlg的头文件中写：afx_msg LRESULT onFrame(WPARAM wp, LPARAM lp);声明
//2、在dlg的BEGIN_MESSAGE_MAP里面写：ON_MESSAGE(MsgVideoFrame, onFrame)
//3、在dlg的cpp中写：
/*
LRESULT CdemoDlg::onFrame(WPARAM wp, LPARAM lp){
	Mat frame((IplImage*)lp, false);
	flip(frame, frame, -1);
	//这里就是在处理每一帧的图像了，类似的MsgVideoPreDisplay消息也是这么来的
	return 0;
}
*/
#define MsgVideoFrame			(MsgBase + 0x0A)			
#define MsgVideoPreDisplay	(MsgBase + 0x0B)		//视频一帧画面准备显示了，他会在MsgVideoFrame之后

//控件被单击消息
#define MsgClick							(MsgBase + 0x0C)

//内部消息，内部使用，不需要理会
#define InnerMsgBase					(MsgBase + 0xF0)					//基本消息起点
#define InnerMsgVideoFrame		(InnerMsgBase + 0)				//接受到一帧画面

//图像显示控件
class CvImgCtrl : public CWnd
{
	DECLARE_DYNAMIC(CvImgCtrl)
	
public:
	//视图缩放类型
	enum ResizeType
	{
		ResizeType_Raw,					//原始大小，以左上角为准，图像多大就多大，不做缩放
		ResizeType_CenterResize,		//居中缩放，图像宽高等比例，画面居中
		ResizeType_Tensile				//拉伸，宽高拉伸到控件一样大
	};

	//视频播放的状态
	enum VideoState{
		VideoState_Stop,					//视频停止了
		VideoState_Playing,				//视频播放中
		VideoState_Pause					//视频暂停了
	};

public:
	CvImgCtrl();
	virtual ~CvImgCtrl();

	//链接到对话框上的控件，链接过后，就被这个类接管了，他会负责将画面绘制到控件上去
	//idd：控件的id
	//parent：父控件的指针，比如在dialog里面，给this就行了
	BOOL linkDlgItem(UINT idd, CWnd* parent);

	//创建一个控件，如果需要动态创建，就调用这个创建函数，而不是CWnd的创建函数
	//pParent：父级的指针，一般给dialog的this指针就好了
	//rcPos：控件所在位置，可以直接给CRect()也行，然后在OnSize里边调整宽高，或者给指定值也行
	//autoDelete：如果CvImgCtrl* m_show，这种，指针，采用new的方式得到的控件，而不是实体变量，那么当这个参数给true的时候，控件销毁时
	//                    会自己回收内存（delete this），也就是你就不用手动的去delete他了
	//resizeType：缩放方式，当显示图像和窗口大小不一致时，该如何缩放，有3种方式，看枚举类型ResizeType的备注
	//defaultColor：默认颜色，即背景默认色，比如居中显示的时候，四周会留空，该是什么颜色，或者刚创建完毕时是什么颜色等
	bool create(CWnd* pParent, CRect rcPos, bool autoDelete = false, ResizeType resizeType = ResizeType_Raw, CvScalar defaultColor = cvScalarAll(255));

	//开始视频，这个控件内置了VideoCapture，所以这里直接可以调用openVideo来打开视频
	//file：视频文件路径，直接是VideoCapture.open，如果失败格式不支持，那就去看VideoCapture
	//waitTime：延迟时间，每一帧画面需要延迟的时间，单位毫秒，如果不延迟，画面会很快的，当然你可以改改成从VideoCapture中获取的
	bool openVideo(const char* file, unsigned int waitTime = 30);

	//开始视频摄像头，这个控件内置了VideoCapture，所以这里直接可以调用openVideo来打开视频
	//cameraIndex：摄像头索引，一样是VideoCapture的open函数，opencv默认-1会弹出选择视频对话框，0-n是你电脑n个摄像头的序号，他们有固定顺序的
	//waitTime：延迟时间，每一帧画面需要延迟的时间，单位毫秒，如果不延迟，画面会很快的，当然你可以改改成从VideoCapture中获取的
	bool openVideo(int cameraIndex = 0, unsigned int waitTime = 30);

	//暂停视频
	void pauseVideo(); 

	//继续视频
	void resumeVideo();

	//关闭视频
	void closeVideo();

	//设置图像，设置后控件会刷新显示，会执行onUpdateImage，允许你设置任意图像，内部会自动调整
	//如果设置的图像是相同大小、相同通道的图像，则只会进行copy，所以效率是杠杠的
	void setImage(const cv::Mat img){setImage(&IplImage(img));}

	//设置图像ipl方式，设置后控件会刷新显示，会执行onUpdateImage
	void setImage(const IplImage* img);

	//获取当前图像
	cv::Mat getImageMat(){return cv::Mat(m_img, false);}

	//获取当前图像
	IplImage* getImage(){return m_img;}

	//更新显示图像，如果你通过getImageMat得到的Mat或者IplImage，你修改了他，那么请使用onUpdateImage来刷新画面应用到控件上
	void onUpateImage();

	//获取显示图像，显示图像即跟控件一样大小的那个图像，随着控件大小改变而改变
	IplImage* getDisplayImage(){return m_imgResize;}

	//获取显示图像Mat
	cv::Mat getDisplayImageMat(){return cv::Mat(m_imgResize, false);}

	//更新显示图像，当你通过getDisplayImage获取到的图像，并修改了他，那么请使用这个函数来刷新应用到画面上
	void onUpdateDisplayImage();
	
	//设置缩放方式，共3种方式，见ResizeType的定义上写的备注，设置后立马生效并刷新
	void setResizeType(ResizeType rt){m_rt = rt; onUpateImage();}

	//设置数据，存一个int，以备特殊时候使用，比如要存什么id之类的，指针之类的
	void setData(int data){m_saveData = data;}
	int getData(){return m_saveData;}

	//获取视频捕获器
	cv::VideoCapture* getVideoCapture(){return &m_videoCapture;}

	//返回当前播放视频状态
	VideoState getVideoState(){return m_videoState;}
	// 设置视频播放帧率时间（毫秒）
	void setWaitTime(int waitTime)	{ m_videoWaitTime = waitTime; }

private:
	//设置图像，只是设置图像，不会显示到控件上去
	void _onlySetImage(const IplImage* img);

	//启动视频，device_or_file，如果给定-1或者0-1024以内的数字，则认为是camera
	//否则认为是文件路径，waittime则是帧延迟时间
	bool _startVideo(const void* device_or_file, unsigned int waitTime = 30);

	//重画image到displayimage（即resizeimage）
	void redrawResize();

	//清空显示的那个图像
	void voidResize();

	//通知父亲，默认wp是this
	LRESULT notifyParent(UINT msg, LPARAM lp = 0);

private:
	cv::VideoCapture m_videoCapture;
	VideoState m_videoState;
	int m_videoWaitTime;  
	unsigned int m_lastdownTime;
	int m_saveData;
	IplImage* m_img;
	IplImage* m_imgResize;
	BITMAPINFO m_bmpHeader;
	bool m_autoDelete;
	CvScalar m_defaultColor;
	ResizeType m_rt;
	CRITICAL_SECTION m_resizeCS;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg LRESULT OnVideoFrame(WPARAM wp, LPARAM lp);

	
};


#define BEGIN_MESSAGE_MAP_EX(class_name)				\
	PTM_WARNING_DISABLE													\
	inline const AFX_MSGMAP* class_name::GetMessageMap() const	\
{return GetThisMessageMap();}			 \
	inline const AFX_MSGMAP* PASCAL class_name::GetThisMessageMap()		\
{typedef class_name ThisClass;							\
	typedef CWnd TheBaseClass;							\
	static const AFX_MSGMAP_ENTRY _messageEntries[] = {

#define IMPLEMENT_DYNAMIC_EX(class_name, super_class)			\
AFX_COMDAT const CRuntimeClass class_name::class##class_name = {	\
#class_name, sizeof(class class_name), 0xFFFF, NULL,						\
((CRuntimeClass*)(&super_class::class##super_class)), NULL, NULL };		\
inline CRuntimeClass* class_name::GetRuntimeClass() const \
{ return RUNTIME_CLASS(class_name); }

IMPLEMENT_DYNAMIC_EX(CvImgCtrl, CWnd)

inline static IplImage imgOfROI(const IplImage* img, CvRect roi, bool invalidROI=false)
{
	IplImage ret = *img;
	int imgWidth = img->roi != 0 && !invalidROI ? img->roi->width : img->width;
	int imgHeight = img->roi != 0 && !invalidROI ? img->roi->height : img->height;
	ret.roi = 0;

	if(roi.x >= imgWidth || roi.y >= imgHeight){
		ret.width = 0;
		ret.height = 0;
		ret.imageData = 0;
		return ret;
	}

	//区域裁剪
	if(roi.x < 0){
		roi.width += roi.x;
		roi.x = 0;
	}

	if(roi.y < 0) {
		roi.height += roi.y;
		roi.y = 0;
	}

	if(roi.x + roi.width > imgWidth)		roi.width = imgWidth - roi.x;
	if(roi.y + roi.height > imgHeight)		roi.height = imgHeight - roi.y;

	CV_Assert(roi.width > 0 && roi.height > 0);
	ret.width = roi.width;
	ret.height = roi.height;
	ret.imageSize = ret.height * ret.widthStep;
	ret.imageDataOrigin = 0;

	if (!invalidROI && img->roi != 0)
		ret.imageData = img->imageData + (img->roi->xOffset + roi.x) * img->nChannels * (img->depth & ~IPL_DEPTH_SIGN) / 8 + (img->roi->yOffset + roi.y) * img->widthStep;
	else
		ret.imageData = img->imageData + roi.x * img->nChannels * (img->depth & ~IPL_DEPTH_SIGN) / 8 + roi.y * img->widthStep;

	return ret;
}

inline static void doEvent(unsigned int waitMS = -1){
	MSG msg;
	unsigned int time = GetTickCount();

	while(GetTickCount() - time < waitMS){
		while(GetTickCount() - time < waitMS && PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

#define min(a,b) (((a) < (b)) ? (a) : (b))

inline CvImgCtrl::CvImgCtrl(){
	memset(&m_bmpHeader, 0, sizeof(m_bmpHeader));
	m_bmpHeader.bmiHeader.biSize = sizeof(m_bmpHeader);
	m_bmpHeader.bmiHeader.biXPelsPerMeter = 72;
	m_bmpHeader.bmiHeader.biYPelsPerMeter = 72;
	m_bmpHeader.bmiHeader.biPlanes = 1;
	m_imgResize = 0;
	m_img = 0;
	m_rt = ResizeType_Tensile;
	m_autoDelete = false;
	m_videoState = VideoState_Stop;
	m_defaultColor = cvScalarAll(255);
	InitializeCriticalSection(&m_resizeCS);
}

inline CvImgCtrl::~CvImgCtrl(){
	DeleteCriticalSection(&m_resizeCS);
}

BEGIN_MESSAGE_MAP_EX(CvImgCtrl)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCDESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_DROPFILES()
	ON_MESSAGE(InnerMsgVideoFrame, OnVideoFrame)
END_MESSAGE_MAP()

inline bool CvImgCtrl::openVideo(const char* file, unsigned int waitTime){
	return _startVideo(file, waitTime);
}

inline void CvImgCtrl::pauseVideo(){
	if(m_videoState == VideoState_Playing)
		m_videoState = VideoState_Pause;
}

inline bool CvImgCtrl::openVideo(int cameraIndex, unsigned int waitTime){
	return _startVideo((const void*)cameraIndex, waitTime);
}

inline void CvImgCtrl::resumeVideo(){
	if(m_videoState == VideoState_Pause){
		m_videoState = VideoState_Playing;
		PostMessage(InnerMsgVideoFrame, 0, 0);
	}
}

inline void CvImgCtrl::closeVideo(){
	m_videoState = VideoState_Stop;
	if(m_videoCapture.isOpened())
		m_videoCapture.release();
}

inline bool CvImgCtrl::_startVideo(const void* device_or_file, unsigned int waitTime){
	if(!::IsWindow(m_hWnd)) return false;

	int dof = (int)device_or_file;
	if(m_videoCapture.isOpened())
		m_videoCapture.release();

	bool success = false;
	const int max_count_of_device_index = 1024;
	const char* file = (const char*)device_or_file;
	const int device_index = (int)device_or_file;
	m_videoWaitTime = waitTime;

	if(dof == -1 || dof >= 0 && dof <= max_count_of_device_index)
		success = m_videoCapture.open(device_index);
	else
		success = m_videoCapture.open(file);

	m_videoState = success ? VideoState_Playing : VideoState_Stop;
	if(success) {
		cv::Mat frame;
		m_videoCapture >> frame;
		if(!frame.empty()){
			_onlySetImage(&IplImage(frame));
			notifyParent(MsgVideoStart, (LPARAM)device_or_file);
			notifyParent(MsgVideoFrame, (LPARAM)m_img);

			if (m_imgResize != 0){
				redrawResize();
				notifyParent(MsgVideoPreDisplay, (LPARAM)m_imgResize);

				this->onUpdateDisplayImage();
			}

			if(m_videoState == VideoState_Playing)
				PostMessage(InnerMsgVideoFrame, 0, 0);
		}else{
			success = false;
		}
	}
	m_videoState = success ? VideoState_Playing : VideoState_Stop;
	return success;
}

inline LRESULT CvImgCtrl::notifyParent(UINT msg, LPARAM lp){
	return ::SendMessage(GetParent()->m_hWnd, msg, (WPARAM)this, lp);
}

inline LRESULT CvImgCtrl::OnVideoFrame(WPARAM wp, LPARAM lp){
	if(m_videoState == VideoState_Playing){
		bool grab = m_videoCapture.grab();
		bool retrieve = grab && m_videoCapture.retrieve(cv::Mat(m_img, false));

		if(retrieve){
			notifyParent(MsgVideoFrame, (LPARAM)m_img);

			if (m_imgResize != 0){
				redrawResize();
				notifyParent(MsgVideoPreDisplay, (LPARAM)m_imgResize);

				this->onUpdateDisplayImage();
			}

			if(m_videoState == VideoState_Playing){
				doEvent(m_videoWaitTime);
				PostMessage(InnerMsgVideoFrame, wp, lp);
			}
		}else{
			notifyParent(MsgVideoOver);
			m_videoState = VideoState_Stop;
		}
	}
	return 0;
}

inline bool CvImgCtrl::create(
	CWnd* pParent, 
	CRect rcPos, 
	bool autoDelete /*= false*/,
	ResizeType rt /*= _ResizeType_Non*/, 
	CvScalar defaultColor /*= cvScalarAll(255)*/){

		m_defaultColor = defaultColor;
		m_autoDelete = autoDelete;
		m_rt = rt;
	return CWnd::Create(_T("Static"), 0, WS_OVERLAPPED | WS_CHILD | WS_VISIBLE, rcPos, pParent, 0, 0) != 0;
}

inline void CvImgCtrl::redrawResize(){
	if(m_imgResize == 0 || m_img == 0 || m_img->width * m_img->height == 0)
		return;

	EnterCriticalSection(&m_resizeCS);
	switch(m_rt)	{
	case ResizeType_Tensile:	{
			if(m_img->nChannels != 3){
				IplImage* re = cvCreateImage(cvGetSize(m_img), 8, 3);
				cvCvtColor(m_img, re, CV_GRAY2BGR);
				cvResize(re, m_imgResize);
				cvReleaseImage(&re);
			}
			else
				cvResize(m_img, m_imgResize);
		}
		break;

	case ResizeType_Raw: {
			voidResize();
			CvRect roi = {0, 0, min(m_img->width, m_imgResize->width), min(m_img->height, m_imgResize->height)};
			if(m_img->nChannels != 3)
				cvCvtColor(&imgOfROI(m_img, roi), &imgOfROI(m_imgResize, roi), CV_GRAY2BGR);
			else
				cvCopy(&imgOfROI(m_img, roi), &imgOfROI(m_imgResize, roi));
		}
		break;

	case ResizeType_CenterResize: {
			voidResize();
			int nw = m_imgResize->width;
			int nh = int(m_imgResize->width / (double)m_img->width * m_img->height);

			if (nh > m_imgResize->height)	{
				nh = m_imgResize->height;
				nw = int(m_imgResize->height / (double)m_img->height * m_img->width);
			}

			cvSetImageROI(m_imgResize, cvRect((m_imgResize->width - nw) / 2, (m_imgResize->height - nh) / 2, nw, nh));
			if(m_img->nChannels != 3){
				IplImage* re = cvCreateImage(cvGetSize(m_img), 8, 3);
				cvCvtColor(m_img, re, CV_GRAY2BGR);
				cvResize(re, m_imgResize);
				cvReleaseImage(&re);
			}
			else
				cvResize(m_img, m_imgResize);
			cvResetImageROI(m_imgResize);
		}
		break;
	}
	LeaveCriticalSection(&m_resizeCS);
}

inline void CvImgCtrl::onUpateImage(){
	if (m_imgResize != 0 && ::IsWindow(m_hWnd)){
		redrawResize();
		this->onUpdateDisplayImage();
	}
}

inline void CvImgCtrl::voidResize(){
	cvSet(m_imgResize, m_defaultColor);
}

inline BOOL CvImgCtrl::linkDlgItem(UINT idd, CWnd* parent){
	BOOL r = CWnd::SubclassDlgItem(idd, parent);
	if(r){
		CRect rc;
		GetWindowRect(&rc);

		OnSize(0, rc.Width(), rc.Height());
	}
	return r;
}

inline void CvImgCtrl::OnSize(UINT nType, int cx, int cy){
	if (cx < 1 || cy < 1){
		CWnd::OnSize(nType, cx, cy);
		return;
	}

	EnterCriticalSection(&m_resizeCS);
	cvReleaseImage(&m_imgResize);
	m_imgResize = cvCreateImage(cvSize(cx, cy), 8, 3);
	LeaveCriticalSection(&m_resizeCS);

	if(m_img != 0)
		redrawResize();
	else
		voidResize();

	m_bmpHeader.bmiHeader.biBitCount = m_imgResize->nChannels * m_imgResize->depth;
	m_bmpHeader.bmiHeader.biHeight = -m_imgResize->height;
	m_bmpHeader.bmiHeader.biWidth = m_imgResize->width;
	m_bmpHeader.bmiHeader.biSizeImage = m_imgResize->imageSize;
	this->onUpdateDisplayImage();
}

inline void CvImgCtrl::_onlySetImage(const IplImage* img){
	if (img == 0)	{
		if (m_img != 0)
			cvReleaseImage(&m_img);

		if (m_imgResize != 0)
			voidResize();

		return;
	}

	if (m_img == 0){
		m_img = (IplImage*)cvClone(img);
	}
	else{
		if (
			m_img->width != img->width || 
			m_img->height != img->height || 
			m_img->nChannels != img->nChannels || 
			m_img->depth != img->depth){

				cvReleaseImage(&m_img);
				m_img = (IplImage*)cvClone(img);
		}
		else
			memcpy(m_img->imageData, img->imageData, min(img->imageSize, m_img->imageSize));
		//cvCopy(img, m_img);
	}
}

inline void CvImgCtrl::setImage(const IplImage* img){
	_onlySetImage(img);
	onUpateImage();
}

inline void CvImgCtrl::OnDestroy(){
	CWnd::OnDestroy();
	if (m_img != 0)
		cvReleaseImage(&m_img);

	if (m_imgResize != 0)
		cvReleaseImage(&m_imgResize);
}

inline BOOL CvImgCtrl::OnEraseBkgnd(CDC* pDC){
	return TRUE;
}

inline void CvImgCtrl::onUpdateDisplayImage(){
	Invalidate();
	UpdateWindow();
}

inline void CvImgCtrl::OnPaint(){
	if(m_imgResize != 0){
		PAINTSTRUCT ps;
		HDC hDC = ::BeginPaint(m_hWnd, &ps);
		CRect rcPaint = ps.rcPaint;
		StretchDIBits(hDC, 0, 0, m_imgResize->width, m_imgResize->height, 0, 0, m_imgResize->width, m_imgResize->height, m_imgResize->imageData, &m_bmpHeader, DIB_RGB_COLORS, SRCCOPY);
		::EndPaint(m_hWnd, &ps);
	}
	else
		CWnd::OnPaint();
}

inline void CvImgCtrl::OnNcDestroy(){
	CWnd::OnNcDestroy();

	if (m_autoDelete)
		delete this;
}

inline void CvImgCtrl::OnMouseMove(UINT nFlags, CPoint point){
	notifyParent(MsgMove, (LPARAM)&point);
}

inline void CvImgCtrl::OnLButtonDown(UINT nFlags, CPoint point){
	m_lastdownTime = ::GetTickCount();
	notifyParent(MsgLBDown, (LPARAM)&point);
}

inline void CvImgCtrl::OnLButtonUp(UINT nFlags, CPoint point){
	notifyParent(MsgLBUp, (LPARAM)&point);

	if(GetTickCount() - m_lastdownTime <= 300)
		notifyParent(MsgClick, (LPARAM)&point);
}

inline void CvImgCtrl::OnRButtonDown(UINT nFlags, CPoint point){
	notifyParent(MsgRBDown, (LPARAM)&point);
}

inline LRESULT CvImgCtrl::OnNcHitTest(CPoint point){
	return HTCLIENT;
}

inline void CvImgCtrl::OnMButtonDown(UINT nFlags, CPoint point){
	notifyParent(MsgMBDown, (LPARAM)&point);
}

inline void CvImgCtrl::OnMButtonUp(UINT nFlags, CPoint point){
	notifyParent(MsgMBUp, (LPARAM)&point);
}

inline void CvImgCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	notifyParent(MsgKeyDown, nChar);
}

inline void CvImgCtrl::OnDropFiles(HDROP hDropInfo){
	CWnd::OnDropFiles(hDropInfo);
	notifyParent(MsgDragFile, (LPARAM)hDropInfo);
}