
// RTSPServer manager class [7/18/2016 SwordTwelve]
#pragma once

//DShow����Ƶ�ɼ���
#include "./DShowCapture/DShowCaptureAudioAndVideo_Interface.h"
#include "./EasyEncoder/FAACEncoder.h"
#include "./EasyEncoder/H264Encoder.h"
#include "./EasyEncoder/H264EncoderManager.h"
//RTSPServer support
#include "../../Include/EasyIPCameraAPI.h"
#pragma comment(lib, "../../Lib/libEasyIPCamera.lib")
#include "YUVTransform.h"
#include "CaptureScreen.h"

#define MSG_LOG 1001
#define MAX_CHANNELS 6

typedef enum tagSOURCE_TYPE
{
	SOURCE_LOCAL_CAMERA = 0,//��������Ƶ
	SOURCE_SCREEN_CAPTURE =1,//��Ļ����
	SOURCE_FILE_STREAM = 2,       //�ļ�������(mp4,ts,flv???)
	SOURCE_RTSP_STREAM=3,//RTSP��
	// 	//SOURCE_ONVIF_STREAM=4,//Onvif��

}SOURCE_TYPE;

class CEasyIpCamera_WinDlg;

class CServerManager
{
public:
	CServerManager(void);
	~CServerManager(void);

public:
	//��ʼ����(�ɼ�)
	int StartCapture(SOURCE_TYPE eSourceType, int nCamId, int nAudioId,  HWND hCapWnd, 
		int nVideoWidth=640, int nVideoHeight=480, int nFps=25, int nBitRate=2048, char* szDataType = "YUY2",  //VIDEO PARAM
		int nSampleRate=44100, int nChannel=2 );//AUDIO PARAM
	//ֹͣ�ɼ�
	void StopCapture();
	//��ʼRTSP����
	int StartServer(int listenport, char *username, char *password,  LIVE_CHANNEL_INFO_T *channelInfo, Easy_U32 channelNum);
	void StopServer();

	int StartDSCapture(int nCamId, int nAudioId,HWND hShowWnd,int nVideoWidth, int nVideoHeight, int nFps, int nBitRate, char* szDataype, int nSampleRate, int nChannel);
	static int CALLBACK RealDataCallbackFunc(int nDevId, unsigned char *pBuffer, int nBufSize, 
		RealDataStreamType realDataType, /*RealDataStreamInfo*/void* realDataInfo, void* pMaster);
	void DSRealDataManager(int nDevId, unsigned char *pBuffer, int nBufSize, 
		RealDataStreamType realDataType, /*RealDataStreamInfo*/void* realDataInfo);
	
	//��Ļ�ɼ����
	int StartScreenCapture(HWND hShowWnd, int nCapMode);
	void StopScreenCapture();
	void RealseScreenCapture();
	int GetScreenCapSize(int& nWidth, int& nHeight);
	//�ص�����
	static int CALLBACK CaptureScreenCallBack(int nId, unsigned char *pBuffer, int nBufSize,  RealDataStreamType realDataType, /*RealDataStreamInfo*/void* realDataInfo, void* pMaster);
	void CaptureScreenManager(int nId, unsigned char *pBuffer, int nBufSize,  RealDataStreamType realDataType, /*RealDataStreamInfo*/void* realDataInfo);


	BOOL IsInCapture()
	{
		return m_bDSCapture;
	}
	void SetCanOutput(BOOL bOutput)
	{
		m_bOutput = bOutput;
	}

	EASY_MEDIA_INFO_T GetMediaInfo()
	{
		return m_mediainfo;
	}

	DEVICE_LIST_T* GetAudioInputDevList()
	{
		if(!m_pAudioManager)
		{
			m_pAudioManager = Create_AudioCapturer();
		}

		if (m_pAudioManager)
		{
			return m_pAudioManager->GetAudioInputDevList();
		}
		return NULL;

	}

	DEVICE_LIST_T* GetCameraList()
	{
		if(!m_pVideoManager)
		{
			m_pVideoManager = Create_VideoCapturer();
		}
		if (m_pVideoManager)
		{
			return m_pVideoManager->GetCameraList();
		}
		return NULL;
	}

	void LogErr(CString strLog);
	void SetMainDlg(CEasyIpCamera_WinDlg* pMainDlg);

private:
	//��Ļ�������
	CCaptureScreen* m_pScreenCaptrue;
	HWND m_hCaptureWnd;
	int m_nScreenCaptureId;

	CEasyIpCamera_WinDlg* m_pMainDlg;
	//��Ƶ�豸����ʵ��
	LPVideoCapturer m_pVideoManager;
	//��Ƶ�豸����ʵ��
	LPAudioCapturer m_pAudioManager;
	//����Dshow�����������
	DEVICE_CONFIG_INFO m_sDevConfigInfo;
	EASY_MEDIA_INFO_T   m_mediainfo;

	// x264+faac Encoder
	//AAC������
	FAACEncoder m_AACEncoderManager;
	//H264������
	CH264EncoderManager m_H264EncoderManager;
	//������Ϣ����
	Encoder_Config_Info*	m_pEncConfigInfo;
	byte m_sps[100];
	byte  m_pps[100];
	long m_spslen;
	long m_ppslen;
	byte* m_pFrameBuf; 

	//�Ƿ�ʼ��ͻ�����������
	BOOL m_bOutput;
	BOOL m_bDSCapture;

};

