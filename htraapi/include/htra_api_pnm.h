/**
 * \file htra_api_pnm.h
 * 
 * \~chinese \brief 相位噪声测量API
 *
 * \~chinese 对信号进行相位噪声测量分析。
 *
 *
 */


#ifndef HTRA_API_PNM_H
#define HTRA_API_PNM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef HTRA_API_EXPORTS
#define HTRA_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#else
#define HTRA_API
#endif
#else
#ifdef HTRA_API_EXPORTS
#if __GNUC__ >= 4
#define HTRA_API __attribute__ ((visibility ("default")))
#else
#define HTRA_API
#endif
#else
#define HTRA_API
#endif
#endif


/** \~chinese 频率分段（十倍频程段）总数上限 */
#define PHASENOISE_MAXFREQSEGMENT 7

/**
 * \struct PNM_Profile_TypeDef
 * \~chinese \brief 相位噪声测量配置结构体
 */
typedef struct
{
    /** \~chinese 中心频率 */
    double CenterFreq;

    /** \~chinese 载波判决门限 */
    float Threshold;

    /** \~chinese RBW比例（各段RBW/各段起始频率） */
	double RBWRatio;

    /** \~chinese 起始频偏 */
    double StartOffsetFreq;

    /** \~chinese 终止频偏 */
    double StopOffsetFreq;

    /** \~chinese 迹线平滑次数 */
    uint32_t TraceAverage;  

}PNM_Profile_TypeDef;

/**
 * \struct PNM_MeasInfo_TypeDef
 * \~chinese \brief 相位噪声测量配置结构体
 */
typedef struct
{
    /** \~chinese 频率分段数（十倍频程划分） */
    uint32_t Segments;

    /** \~chinese 迹线点数 */
    uint32_t TracePoints;

    /** \~chinese 一次分析对应的迹线刷新次数（Get接口调用次数） */
    uint32_t PartialUpdateCounts;

    /** \~chinese 各分段的总帧数 */
    uint32_t FramesInSegment[PHASENOISE_MAXFREQSEGMENT];

    /** \~chinese 各分段的帧检波比 */
    uint32_t FrameDetRatioOfSegment[PHASENOISE_MAXFREQSEGMENT];

    /** \~chinese 各分段的起始频率 */
    double StartFreqOfSegment[PHASENOISE_MAXFREQSEGMENT];

    /** \~chinese 各分段的终止频率 */
    double StopFreqOfSegment[PHASENOISE_MAXFREQSEGMENT];

    /** \~chinese 各分段的RBW */
    double RBWOfSegment[PHASENOISE_MAXFREQSEGMENT];

}PNM_MeasInfo_TypeDef;

/**
 * \~chinese \brief 提供相位噪声测量的默认配置参数
 * \~chinese @param[in] Device 设备指针
 * \~chinese @param[out] PNM_Profile 相位噪声测量的配置结构体，元素请参考结构体定义
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_ProfileDeInit(void** Device, PNM_Profile_TypeDef* PNM_Profile);

/**
 * \~chinese \brief 配置相位噪声测量
 * \~chinese @param[in] Device 设备指针
 * \~chinese @param[in] PNM_Profile_in 相位噪声测量的配置结构体（输入），元素请参考结构体定义
 * \~chinese @param[out] PNM_Profile_out 相位噪声测量的配置结构体（返回），元素请参考结构体定义
 * \~chinese @param[out] PNM_MeasInfo 相位噪声测量的测量信息结构体，元素请参考结构体定义
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_Configuration(void** Device, const PNM_Profile_TypeDef* PNM_Profile_in, PNM_Profile_TypeDef* PNM_Profile_out, PNM_MeasInfo_TypeDef* PNM_MeasInfo);

/**
 * \~chinese \brief 开始一次相位噪声测量
 * \~chinese @param[in] Device 设备指针
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_StartMeasure(void** Device);

/**
 * \~chinese \brief 强制停止本次相位噪声测量
 * \~chinese @param[in] Device 设备指针
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_StopMeasure(void** Device);

/**
 * \~chinese \brief 获取相位噪声测量结果迹线
 * \~chinese @param[in] Device 设备指针
 * \~chinese @param[out] CarrierFreq 载波频率
 * \~chinese @param[out] CarrierPower 载波功率
 * \~chinese @param[out] Freq 迹线频率轴（以Hz为单位）
 * \~chinese @param[out] PhaseNoise 迹线功率轴（以dBc/Hz为单位）
 * \~chinese @param[out] FrameUpdateCounts 刷新计数器（索引0为最远端的分段，N为最近端的分段；元素为段的已刷新帧数）
 * \~chinese @param[out] MeasAuxInfo 辅助测量信息结构体，元素请参考结构体定义
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_GetPartialUpdatedFullTrace(void** Device, double* CarrierFreq, float* CarrierPower, double Freq[], float PhaseNoise[], uint32_t FrameUpdateCounts[], MeasAuxInfo_TypeDef *MeasAuxInfo, float *RefLevel);

/**
 * \~chinese \brief 高级功能：通过全景扫描寻找功率超过载波门限的信号
 * \~chinese @param[in] Device 设备指针
 * \~chinese @param[out] CarrierFreq 载波频率
 * \~chinese @param[out] Found 载波存在标志
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_AutoSearch(void** Device, double* CarrierFreq, uint8_t* Found);

/**
 * \~chinese \brief 高级功能：复位各频率分段的帧检波比
 * \~chinese @param[in] Device 设备指针
 * \~chinese @param[out] MeasInfo 复位帧检波比后，更新相位噪声测量的测量信息结构体
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_Preset_FrameDetRatio(void** Device, PNM_MeasInfo_TypeDef* MeasInfo);

/**
 * \~chinese \brief 高级功能：手动配置各频率分段的帧检波比
 * \~chinese @param[in] Device 设备指针
 * \~chinese @param[in] FrameDetRatioOfSegment 帧检波比的设定数组（数组长度为Segments；索引0为最远端的分段，N为最近端的分段）
 * \~chinese @param[out] MeasInfo 调整帧检波比后，更新相位噪声测量的测量信息结构体
 * \~chinese @return 函数调用状态，0为正常，非0请参考相关宏定义
 */
HTRA_API int PNM_Set_FrameDetRatio(void **Device, uint32_t FrameDetRatioOfSegment[], PNM_MeasInfo_TypeDef* MeasInfo);

#ifdef __cplusplus
}
#endif

#endif
