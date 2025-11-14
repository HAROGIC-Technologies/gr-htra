/**
* @brief	本文件定义了API相关的变量与函数。注意使用UTF-8 with BOM编辑。
*/

#ifndef HTRA_API_H
#define HTRA_API_H

#define Major_Version 0 
#define Minor_Version 55
#define Increamental_Version 63

#define API_Version			 (Major_Version<<16)|(Minor_Version<<8)|(Increamental_Version)

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

/* 关键 用户数据类型 与 用户函数 的说明

用户数据类型是用户配置设备与从设备返回信息的核心内容，用户通过 用户数据类型 与 用户函数 实现API的使用，用户程序所使用到的数据类型如下

设备相关的 数据类型
BootProfile_TypeDef：设备启动所需要的配置信息
BootInfo_TypeDef：设备启动过程的反馈信息

设备相关的 主要函数
Device_Open(..)：打开设备，设备必须先打开（分配软硬件资源，比如内存与USB端口），再进行配置与调用
Device_Close(..)：关闭设备，设备结束使用后，需要关闭以让操作系统回收软硬件资源

标准频谱模式（SWP）相关的 数据类型 
SWP_Profile_TypeDef：SWP模式的配置信息
SWP_TraceInfo_TypeDef：SWP模式的配置反馈信息（迹线信息）
Freq_Hz[], PowerSpec_dBm[], HopIndex, FrameIndex, MeasAuxInfo：SWP获取的测量数据及附加信息，非封装形式
SWPSpectrum_TypeDef：SWP获取的测量数据及附加信息，封装形式

标准频谱模式（SWP）相关的 主要函数
SWP_ProfileDeInit(..,SWP_Profile)：将SWP_Profile配置信息初始化为默认值
SWP_Configuration(..,SWP_Profile, SWP_TraceInfo)：将设备配置为SWP模式，配置信息为SWP_Profile，并得到配置反馈信息SWP_TraceInfo
SWP_GetPartialSweep(.., Freq_Hz[], PowerSpec_dBm[], HopIndex, FrameIndex, MeasAuxInfo)：获取SWP模式的测量数据，部分迹线，使用标准数据数据结构，包含主要信息。
SWP_GetPartialSweep_PM1(.., SWPTrace_TypeDef)：获取SWP模式的测量数据，部分迹线，使用封装的数据结构体，包含全部信息。

IQ流模式（IQS）相关的 数据类型
IQS_Profile_TypeDef：IQS模式的配置信息
IQS_StreamInfo_TypeDef：IQS模式的配置反馈信息
IQStream_TypeDef：IQS模式获取的测量数据

IQ流模式（IQS）相关的 主要函数
IQS_ProfileDeInit(..)
IQS_Configuration(..)
IQS_BusTriggerStart(..)
IQS_BusTriggerStop(..)
IQS_GetIQStream(..,AlternIQStream, ScaleToV, TriggerInfo, MeasAuxInfo)
IQS_GetIQStream_PM1(..,IQStream_TypeDef)

检波模式（DET）相关的 数据类型
DET_Profile_TypeDef：DET模式的配置信息
DET_StreamInfo_TypeDef：
DETStream_TypeDef

检波模式（DET）相关的 主要函数
DET_ProfileDeInit
DET_Configuration
DET_BusTriggerStart
DET_BusTriggerStop
DET_GetPowerStream(.., PowerStream[], ScaleToV, TriggerInfo, MeasAuxInfo)

实时频谱模式（RTA）相关的 数据类型
RTA_Profile_TypeDef：RTA模式的配置信息
RTA_FrameInfo_TypeDef：RTA模式的配置反馈信息

SpectrumFrames[], SpectrumBitMap[], TriggerInfo, PlotInfo, MeasAuxInfo RTA模式下非封装的测量数据，匹配RTA_GetRealTimeSpectrum函数
RTAStream_TypeDef：RTA模式下封装的测量数据，匹配RTA_GetRealTimeSpectrum_PM1函数

实时频谱模式（RTA）相关的 主要函数
RTA_ProfileDeInit
RTA_Configuration
RTA_BusTriggerStart
RTA_BusTriggerStop
RTA_GetRealTimeSpectrum(..,SpectrumFrames[], SpectrumBitMap[], TriggerInfo, PlotInfo, MeasAuxInfo)
RTA_GetRealTimeSpectrum_PM1(RTAStream_TypeDef)

*/

/* 返回值定义  Return Value for public API functions-------------------------------------------------------------*/

/* 关于返回值的说明
1）0 为默认的正常值，任何非0值都表示存在特殊情况
2) APIRETVAL_ 前缀表示此返回值为API返回值
3）第二修饰符 ERROR_ 表示返回情况为 错误，此时不建议直接进行后续操作，需要尝试特定的恢复步骤，比如重新执行DeviceOpen
4）第二修饰符 WARNING_表示返回情况为 警告，此时可以继续后续API的调用,但当前API获取的测试数据可能存在不准确，比如本振失锁可能引起功率读数失准
*/

#define APIRETVAL_NoError									0			// 无错误

/* 设备打开过程中的 错误及警告 */
#define APIRETVAL_ERROR_BusOpenFailed						-1			// 总线打开错误，仅生产于DeviceOpen过程中
#define	APIRETVAL_ERROR_RFACalFileIsMissing					-3			// 射频幅度校准文件丢失，仅生产于DeviceOpen过程中，文件丢失将导致幅度校准失准 或 无法运行
#define	APIRETVAL_ERROR_IFACalFileIsMissing					-4			// 中频幅度校准文件丢失，仅生产于DeviceOpen过程中，文件丢失将导致幅度校准失准 或 无法运行
#define	APIRETVAL_ERROR_DeviceConfigFileIsMissing			-5			// 设备配置文件丢失，仅生产于DeviceOpen过程中，配置不优化
#define APIRETVAL_ERROR_DeviceSpecFileIsMissing				-6			// 设备规格文件丢失、仅生产于DeviceOpen过程中，参数范围限值不准确
#define APIRETVAL_ERROR_UpdateStrategyFailed				-7			// 下发配置策略至设备的过程失败，仅生产于DeviceOpen过程中

/* 总线调用过程中的 错误及警告 */
#define	APIRETVAL_ERROR_BusError							-8			// 总线传输错误
#define	APIRETVAL_ERROR_BusDataError						-9			// 总线数据错误，表示数据包整体格式正确，但数据内容错误，可能是配置冲突或者是传输过程中因物理干扰导致数据内容错误
#define APIRETVAL_WARNING_BusTimeOut						-10			// 总线数据返回超时，可能是触发未到来，也可能是通信存在错误
#define	APIRETVAL_ERROR_BusDownLoad							-11			// 通过总线下发配置错误

/* 数据包中反馈的关于数据内容与设备状态的 错误及警告 */
#define	APIRETVAL_WARNING_IFOverflow						-12			// 中频饱和，建议上调参考电平（RefLevel_dBm）直至不再出现该提示
#define APIRETVAL_WARNING_ReconfigurationIsRecommended		-14			// 距离最近一次配置，温度产生较大变化，建议重新调用配置函数（如SWP_Configuration）以获得最佳性能
#define APIRETVAL_WARNING_ClockUnlocked_SYSCLK				-15			// 硬件状态警告，系统时钟失锁
#define	APIRETVAL_WARNING_ClockUnlocked_ADCCLK				-16			// 硬件状态警告，ADC时钟失锁
#define	APIRETVAL_WARNING_ClockUnlocked_RXIFLO				-17			// 硬件状态警告，接收中频本振失锁
#define APIRETVAL_WARNING_ClockUnlocked_RXRFLO				-18			// 硬件状态警告，接收射频频本振失锁
#define APIRETVAL_WARNING_ADCConfigError					-19         // 硬件状态警告，ADC配置错误  

/* 固件更新过程中错误 */
#define APIRETVAL_ERRPR_OpenfileFiled						-20			// 打开更新文件失败
#define APIRETVAL_ERROR_ExecuteUpdateError					-21			// 进入更新程序失败
#define APIRETVAL_ERROR_FirmwareEraseError					-22			// 固件flash擦除失败
#define APIRETVAL_ERROR_FirmwareDownloadError				-23			// 固件flash下载失败
#define APIRETVAL_ERROR_UpdateExitError						-24			// 退出固件更新程序失败
#define APIRETVAL_ERROR_DisallowUpgrade						-301		// 不允许升级
#define APIRETVAL_ERROR_PowerRestart						-302		// 电源重启失败


/* 设置待机模式时错误状态 */
#define APIRETVAL_ERROR_SysPowerMode_ParamOutRange			-25			// 退出固件更新程序失败
#define APIRETVAL_ERROR_SysPowerMode_UpdateFailed			-26			// 退出固件更新程序失败

#define APIRETVAL_WARNING_VNACalKitFileIsMissing			-27			// 加载VNA校准件数据文件失败
#define APIRETVAL_WARNING_VNACalResultFileIsMissing			-28			// 加载VNA校准结果文件失败

#define APIRETVAL_WARNING_TxLevelCalFileIsMissing			-29			// 加载发射功率校准文件失败
#define APIRETVAL_WARNING_TxLevelExceededRefLevel			-30			// 加载发射功率校准文件失败

/* 校准文件加载警告 */
#define APIRETVAL_WARNING_DefaultRFACalFileIsLoaded			-33			// 设备个体的射频幅度校准文件丢失，系统加载默认校准文件
#define APIRETVAL_WARNING_DefaultIFACalFileIsLoaded			-34			// 设备个体的中频幅度校准文件丢失，系统加载默认校准文件

/* 设备状态警告*/
#define APIRETVAL_WARNING_ClockRelocked_SYSCLK				-36			// 硬件状态警告，系统时钟发生重锁现象
#define APIRETVAL_WARNING_ClockRelocked_ADCCLK				-37			// 硬件状态警告，ADC时钟发生重锁现象
#define APIRETVAL_WARNING_ClockRelocked_RXIFLO				-38			// 硬件状态警告，接收中频本振发生重锁现象
#define APIRETVAL_WARNING_ClockRelocked_RXRFLO				-39			// 硬件状态警告，接收射频本振发生重锁现象


#define APIRETVAL_WARNING_DefaultVNACalKitFileIsLoaded		-40			// 设备个体的矢量网络分析仪校准件文件丢失，系统加载了默认了校准件文件
#define APIRETVAL_WARNING_DefaultVNACalResultFileIsLoaded	-41			// 设备个体的矢量网络分析仪校准结果文件丢失，系统加载了默认了校准结果文件
#define APIRETVAL_WARNING_DefaultTxLevelCalFileIsLoaded		-42			// 设备个体的发射功率校准文件丢失，系统加载了默认了发射功率校准文件

#define APIRETVAL_ERROR_IQCalFileIsMissing					-43			// IQ校准文件丢失，仅生产于DeviceOpen过程中，文件丢失将导IQ校准失准 或 无法运行
#define APIRETVAL_WARNING_DefaultIQCalFileIsLoaded			-44			// 设备个体的IQ校准文件丢失，系统加载默认校准文件

#define APIRETVAL_ERROR_QueryResultsIncorrect				-45			// 查询结果与实际下发的参数不匹配
#define APIRETVAL_ERROR_ADCState							-46			// ADC状态错误
#define APIRETVAL_ERROR_Options_Missing						-47			// 硬件状态错误，该配置必需的选件不存在，请检查MUXIO连接是否正常

#define APIRETVAL_ERROR_CableLossFileIsMissing				-48			// 线损文件丢失，仅生产于DeviceOpen过程中

/* 非正常调用及调试过程中出现的 错误与警告 */
#define APIRETVAL_ERROR_ParamOutRange						-98			// 参数超界，非正常调用
#define APIRETVAL_ERROR_FunctionInternalError				-99			// 函数内部错误，非正常调用
#define APIRETVAL_WARNING_No_VSG_Function_Option			-100		// 设备个体的IQ校准文件丢失，系统加载默认校准文件

/* 非正常调用重采样函数 错误 */
#define APIRETVAL_ERROR_DecimateFactorOutRange				-201		// 抽取倍数设置超界，非正常调用（抽取倍数设置范围:1 ~ 2^16）
#define APIRETVAL_ERROR_IQSDataFormatError				    -202		// IQS数据格式设置错误，非正常调用（IQ数据格式目前只有int8、int16、int32和float）
#define APIRETVAL_WARNING_CarrierLoss				        -203		// 相位噪声测量过程中，未发现功率超过载波判决门限的信号
#define APIRETVAL_WARNING_MeasUpdate				        -204		// 相位噪声测量过程中，DUT状态发生改变，导致测量状态自动更新，需重新获取PartialUpdateCounts次数据

#define APIRETVAL_ERROR_ETHTimeOut                          10060       // 由于连接方在一段时间后没有正确的答复或连接的主机没有反应，连接尝试失败。 
#define APIRETVAL_ERROR_ETHDisconnected                     10054       // 设备断开网络连接,远程主机强迫关闭了一个现有的连接
#define APIRETVAL_ERROR_ETHDataError                        10062       // 设备未正常获取到数据

#define APIRETVAL_ERROR_NoDeviceExist                       10068       // 无设备存在
#define APIRETVAL_ERROR_DeviceNotFound                      10069       // 未找到需要修改IP地址的设备
#define APIRETVAL_ERROR_DeviceIPRepeatConfig                10070       // 需要修改的IP地址已经是此设备的IP地址
#define APIRETVAL_ERROR_DeviceIPExceed                      10071       // 不符合ip设置范围
#define APIRETVAL_ERROR_DeviceIPSetError                    10072       // 设备连接在路由器下时，设置的IP地址需要符合路由的要求
#define APIRETVAL_ERROR_DeviceIPExist                       10073       // 该网段下已存在需要修改的IP地址
#define APIRETVAL_ERROR_ConfigDeviceIPFailed                10074       // 配置IP地址失败
#define APIRETVAL_ERROR_DeviceInfoError                     10079       // 设备信息获取错误

/* MPS模式下获取到的数据类型 */
#define MPS_SWP			0x00			// Analysis返回0时为SWP模式		 
#define	MPS_IQS			0x01			// Analysis返回1时为IQS模式	
#define MPS_DET			0x02			// Analysis返回2时为DET模式	
#define MPS_RTA			0x03			// Analysis返回3时为RTA模式

/*---------------------------------------------------
设备启停结构体所需的枚举
-----------------------------------------------------*/

/*设备供电方式 BootProfile.DevicePowerSupply*/
typedef enum
{
	USBPortAndPowerPort = 0x00,	// 使用USB数据端口及电源端口供电
	USBPortOnly = 0x01,			// 仅使用USB数据端口
	Others = 0x02				// 其他方式, 当使用ETH总线时，使用此选项
}DevicePowerSupply_TypeDef;

/*物理总线类型 BootProfile.PhysicalInterface*/
typedef enum
{
	USB = 0x00,    // 使用USB作为物理接口，适用于SAE、SAM、SAN等USB接口产品
	QSFP = 0x02,   // 使用40Gbps QSFP+ 作为物理接口
	ETH = 0x03,	   // 使用100M/1000M 以太网 作为物理接口，适用于NXE、NXM、NXN等以太网接口产品
	HLVDS = 0x04,  // 调试专用，请勿使用
	VIRTUAL = 0x07 // 使用虚拟总线，即无物理总线，用于仿真与调试
} PhysicalInterface_TypeDef;

/*IP地址版本 BootProfile.IPVersion*/
typedef enum
{
	IPv4 = 0x00, // 使用IPv4地址
	IPv6 = 0x01  // 使用IPv6地址
}IPVersion_TypeDef;

/*功耗控制*/
typedef enum
{
	PowerON = 0x00,    // 系统所有工作区均上电
	RFPowerOFF = 0x01, // 射频处于下电状态，不可快速唤醒
	RFStandby = 0x02   // 射频处于待机状态，可快速唤醒
}SysPowerState_TypeDef;

/*固件类型*/
typedef enum
{
	MCU = 0x00,  // 单片机
	FPGA = 0x01, // FPGA
	GNSS = 0x02  // GNSS
}Firmware_TypeDef;

/*---------------------------------------------------
用户结构体所需的枚举，()括号内为适配的工作模式
包括全模式 all
扫描模式 SWP
时域流模式 IQS
实时频谱分析模式 RTA
检波模式 DET
数字信号处理函数 DSP
矢量网络分析模式 VNA
辅助信号源模式 SIG
-----------------------------------------------------*/

/*射频输入端口(all)*/
typedef enum
{
	ExternalPort = 0x00, // 外部端口
	InternalPort = 0x01, // 内部端口
	ANT_Port = 0x02,	 // only for TRx series
	TR_Port = 0x03,		 // only for TRx series
	SWR_Port = 0x04,	 // only for TRx series
	INT_Port = 0x05		 // only for TRx series
}RxPort_TypeDef;

/*增益方式(all)*/
typedef enum
{
	LowNoisePreferred = 0x00,	  // 侧重低噪声
	HighLinearityPreferred = 0x01 // 侧重高线性度
}GainStrategy_TypeDef;

/*触发输入边沿(all)*/
typedef enum
{
	RisingEdge = 0x00,	// 上升沿触发
	FallingEdge = 0x01,	// 下降沿触发
	DoubleEdge = 0x02   // 双边沿触发
}TriggerEdge_TypeDef;

/*触发输出类型(all)*/
typedef enum
{
	None = 0x00,	  // 无触发输出
	PerHop = 0x01,	  // 随每次跳频完成时输出
	PerSweep = 0x02,  // 随每次扫描完成时输出
	PerProfile = 0x03 // 随每次配置切换输出
}TriggerOutMode_TypeDef;

/*触发输出信号极性(all)*/
typedef enum
{
	Positive = 0x00, // 正极型脉冲
	Negative = 0x01  // 负极型脉冲
}TriggerOutPulsePolarity_TypeDef;

/*预选放大器(all)*/
typedef enum
{
	AutoOn = 0x00,	 // 自动使能前置放大器
	ForcedOff = 0x01 // 强制保持前置放大器关闭
}PreamplifierState_TypeDef;

/*系统参考时钟(all)*/
typedef enum
{
	ReferenceClockSource_Internal  = 0x00,        // 内部时钟源(默认10MHz)
	ReferenceClockSource_External = 0x01,         // 外部时钟源(默认10MHz)，当系统检测到外部时钟无法锁定时，将自动切换至内部参考
	ReferenceClockSource_Internal_Premium = 0x02, // 内部时钟源-高品质，选择DOCXO或OCXO
	ReferenceClockSource_External_Forced = 0x03   // 外部时钟源，并且无视锁定情况，即使失锁也不会切换至内部参考
}ReferenceClockSource_TypeDef;

/*系统时钟(all)*/
typedef enum
{
	SystemClockSource_Internal = 0x00, // 内部系统时钟源
	SystemClockSource_External = 0x01  // 外部系统时钟源
}SystemClockSource_TypeDef;

/*频率分配方式(SWP)*/
typedef enum
{
	StartStop = 0x00, // 以起始频率和终止频率来设置频率范围
	CenterSpan = 0x01 // 以中心频率和频率扫宽来设置频率范围
}SWP_FreqAssignment_TypeDef;

/*迹线更新方式(SWP)*/
typedef enum
{
	ClearWrite = 0x00,    	// 输出正常迹线
	MaxHold = 0x01,	      	// 输出经过最大值保持的迹线
	MinHold = 0x02,	      	// 输出经过最小值保持的迹线
	ClearWriteWithIQ = 0x03 // 同时输出当前频点的时域数据与频域数据
}SWP_TraceType_TypeDef;

/*RBW更新方式(SWP)*/
typedef enum
{
	RBW_Manual = 0x00,			  // 手动输入RBW
	RBW_Auto = 0x01,			  // 自动随SPAN更新RBW，RBW = SPAN / 2000
	RBW_OneThousandthSpan = 0x02, // 强制 RBW = 0.001 * SPAN
	RBW_OnePercentSpan = 0x03	  // 强制 RBW = 0.01 * SPAN
}RBWMode_TypeDef;

/*VBW更新方式(SWP)*/
typedef enum
{
	VBW_Manual = 0x00,	      // 手动输入VBW
	VBW_EqualToRBW = 0x01,    // 强制 VBW = RBW
	VBW_TenPercentRBW = 0x02, // 强制 VBW = 0.1 * RBW
	VBW_OnePercentRBW = 0x03, // 强制 VBW = 0.01 * RBW
	VBW_TenTimesRBW = 0x04    // 强制 VBW = 10 * RBW，完全旁路VBW滤波器
}VBWMode_TypeDef;

/*扫描时间配置模式(SWP)*/
typedef enum
{
	SWTMode_minSWT = 0x00,	  // 以最短扫描时间进行扫描
	SWTMode_minSWTx2 = 0x01,  // 以近似2倍最短扫描时间进行扫描
	SWTMode_minSWTx4 = 0x02,  // 以近似4倍最短扫描时间进行扫描
	SWTMode_minSWTx10 = 0x03, // 以近似10倍最短扫描时间进行扫描
	SWTMode_minSWTx20 = 0x04, // 以近似20倍最短扫描时间进行扫描
	SWTMode_minSWTx50 = 0x05, // 以近似50倍最短扫描时间进行扫描
	SWTMode_minSWTxN = 0x06,  // 以近似N倍最短扫描时间进行扫描，N等于SweepTimeMultiple
	SWTMode_Manual = 0x07,    // 以近似指定的扫描时间进行扫描，扫描时间等于SweepTime
	SWTMode_minSMPxN = 0x08   // 以近似N倍最短采样时间进行单个频点的采样，N等于SampleTimeMultiple
}SweepTimeMode_TypeDef;

/*扫描模式触发源与触发模式(SWP)*/
typedef enum
{
	InternalFreeRun = 0x00,	  // 内部触发自由运行
	ExternalPerHop = 0x01,	  // 外部触发，每一次触发都跳一个频点
	ExternalPerSweep = 0x02,  // 外部触发，每一次触发都刷新一条迹线
	ExternalPerProfile = 0x03 // 外部触发，每一次触发都应用一个新配置
}SWP_TriggerSource_TypeDef;

/*杂散抑制类型(SWP)*/
typedef enum
{
	Bypass = 0x00,	 // 不进行杂散抑制
	Standard = 0x01, // 中级杂散抑制
	Enhanced = 0x02	 // 高级杂散抑制
}SpurRejection_TypeDef;

/*迹线点数逼近方式(SWP)*/
typedef enum
{
	SweepSpeedPreferred = 0x00,	    // 优先保证扫描速度最快，然后尽量靠近设置的目标迹线点数
	PointsAccuracyPreferred = 0x01, // 优先保证实际迹线点数接近设置的目标迹线点数
	BinSizeAssined = 0x02           // 优先保证按照设定的频率间隔来生成迹线
}TracePointsStrategy_TypeDef;

/*迹线对齐方式(SWP)*/
typedef enum
{	
	NativeAlign = 0x00,  //自然对齐
	AlignToStart = 0x01, //对齐至起始频率
	AlignToCenter = 0x02 //对齐至中心频率
}TraceAlign_TypeDef;

/*FFT平台(SWP)*/
typedef enum
{
	Auto = 0x00,				 // 根据设置自动选择使用CPU还是FPGA进行FFT计算
	Auto_CPUPreferred = 0x01,	 // 根据设置自动选择使用CPU还是FPGA进行FFT计算，CPU优先
	Auto_FPGAPreferred = 0x02,	 // 根据设置自动选择使用CPU还是FPGA进行FFT计算，FPGA优先
	CPUOnly_LowResOcc = 0x03,	 // 强制使用CPU计算，低资源占用，最大FFT点数256K
	CPUOnly_MediumResOcc = 0x04, // 强制使用CPU计算，中资源占用，最大FFT点数1M
	CPUOnly_HighResOcc = 0x05,	 // 强制使用CPU计算，高资源占用，最大FFT点数4M
	FPGAOnly = 0x06 			 // 强制使用FPGA计算
}FFTExecutionStrategy_TypeDef;

/*DSP运算平台(SWP)*/
typedef enum
{
	CPU_DSP  = 0x00, // 在CPU进行计算
	FPGA_DSP = 0x01	 // 在FPGA进行计算
}DSPPlatform_Typedef;

/*频点内多帧检波方式(SWP\RTA)*/
typedef enum
{
	Detector_Sample  = 0x00,   // 每个频点的功率谱间不进行帧间检波
	Detector_PosPeak = 0x01,   // 每个频点的功率谱间进行帧检波，最终输出一帧；帧与帧取MaxHold
	Detector_Average = 0x02,   // 每个频点的功率谱间进行帧检波，最终输出一帧；帧与帧取平均
	Detector_NegPeak = 0x03,   // 每个频点的功率谱间进行帧检波，最终输出一帧，帧与帧取MinHold
	Detector_MaxPower = 0x04,  // 在FFT前，每个频点都进行长时间的采样，从中选取功率最大的帧数据进行FFT，用于捕获脉冲等瞬时信号（仅SWP模式可用）
	Detector_RawFrames = 0x05, // 每个频点均进行多次采样，多次FFT分析，并逐帧输出功率谱（仅SWP模式可用）
	Detector_RMS = 0x06,       // 每个频点的功率谱间进行帧检波，最终输出一帧；帧与帧取RMS
	Detector_AutoPeak = 0x07   // 每个频点的功率谱间进行帧检波，最终输出一帧；帧与帧取AutoPeak
}Detector_TypeDef;

/*窗类型(SWP\RTA\DSP)*/
typedef enum
{
	FlatTop = 0x00,	 		  // 平顶窗
	Blackman_Nuttall = 0x01,  // Nuttall窗
	LowSideLobe = 0x02		  // 低副瓣窗
}Window_TypeDef;

/*迹线检波方式(SWP\RTA\DSP)*/
typedef enum
{
	TraceDetectMode_Auto = 0x00,  // 自动选择迹线检波模式
	TraceDetectMode_Manual = 0x01 // 指定迹线检波模式
}TraceDetectMode_TypeDef;

/*迹线检波方式(SWP\RTA\DSP)*/
typedef enum
{
	TraceDetector_AutoSample = 0x00, // 自动取样检波
	TraceDetector_Sample = 0x01,	 // 随机检波
	TraceDetector_PosPeak = 0x02,	 // 正峰值检波
	TraceDetector_NegPeak = 0x03,	 // 负峰值检波
	TraceDetector_RMS = 0x04,		 // 均方根检波
	TraceDetector_Bypass = 0x05,	 // 不执行检波
	TraceDetector_AutoPeak = 0x06 	 // 自动峰值检波
}TraceDetector_TypeDef;

/*定频点模式（IQS\RTA\DET）触发源*/
typedef enum
{
	FreeRun = 0x00,	 			   // 自由运行
	External = 0x01, 			   // 外部触发。由连接至设备外触发输入端口的物理信号进行触发
	Bus = 0x02,		 			   // 总线触发。由函数（指令）的方式进行触发
	Level = 0x03,	 			   // 电平触发。设备根据设定的电平门限对输入信号进行检测，当输入超过门限值后自动发起触发
	Timer = 0x04,	 			   // 定时器触发。使用设备内部定时器以设定的时间周期进行自动触发
	TxSweep = 0x05,	 			   // 信号源扫描的输出触发；当选择此触发源时，采集过程将由信号源扫描的输出触发信号进行触发
	MultiDevSyncByExt = 0x06, 	   // 在外部触发信号到来时，多机做同步触发行为
	MultiDevSyncByGNSS1PPS = 0x07, // 在GNSS-1PPS到来时，多机做同步触发行为
	SpectrumMask = 0x08,		   // 频谱模板触发，仅RTA模式下可用。暂未开放
	GNSS1PPS = 0x09				   // 使用系统内GNSS提供的1PPS进行触发
}IQS_TriggerSource_TypeDef;

/*定时器触发与外触发边沿同步*/
typedef enum
{
	NoneSync = 0x00,                        // 定时器触发不与外触发同步
	SyncToExt_RisingEdge = 0x01,            // 定时器触发与外触发上升沿同步 
	SyncToExt_FallingEdge = 0x02,           // 定时器触发与外触发下降沿同步
	SyncToExt_SingleRisingEdge = 0x03,      // 定时器触发与外触发上升沿单次同步（需要调用指令函数，执行单次同步）
	SyncToExt_SingleFallingEdge = 0x04,     // 定时器触发与外触发下降沿单次同步（需要调用指令函数，执行单次同步）
	SyncToGNSS1PPS_RisingEdge = 0x05,       // 定时器触发与GNSS-1PPS上升沿同步 
	SyncToGNSS1PPS_FallingEdge = 0x06,      // 定时器触发与GNSS-1PPS下降沿同步
	SyncToGNSS1PPS_SingleRisingEdge	= 0x07, // 定时器触发与GNSS-1PPS上升沿单次同步（需要调用指令函数，执行单次同步）
	SyncToGNSS1PPS_SingleFallingEdge = 0x08 // 定时器触发与GNSS-1PPS下降沿单次同步（需要调用指令函数，执行单次同步）
}
TriggerTimerSync_TypeDef;

typedef IQS_TriggerSource_TypeDef DET_TriggerSource_TypeDef;
typedef IQS_TriggerSource_TypeDef RTA_TriggerSource_TypeDef;

/*直流抑制方式(IQS\DET\RTA)*/
typedef enum
{
	DCCOff = 0x00,				  // 关闭直流抑制功能。
	DCCHighPassFilterMode = 0x01, // 开启，高通滤波器方式。该模式下有较好的直流抑制效果，但会损伤直流至低频（约100kHz）的信号分量
	DCCManualOffsetMode = 0x02,   // 开启，手动偏置方式。该模式下需要手动指定偏置值，且抑制效果弱于高通滤波器模式，但不会损伤直流上的信号分量
	DCCAutoOffsetMode = 0x03      // 开启，自动偏置方式。
}DCCancelerMode_TypeDef;

/*正交解调校正(IQS\DET\RTA)*/
typedef enum
{
	QDCOff = 0x00,		  // 关闭QDC
	QDCManualMode = 0x01, // 手动QDC，根据给定参数执行QDC
	QDCAutoMode = 0x02    // 自动QDC，系统在每次IQS_Configuration调用时自动执行校准并使用校准所获得的参数
}QDCMode_TypeDef;

/*触发模式(IQS\RTA\DET)*/
typedef enum
{
	FixedPoints = 0x00,	// 触发后获取定点长度的数据
	Adaptive = 0x01		// 触发后持续获取数据
}TriggerMode_TypeDef;

/*流模式数据格式类型(IQS/DSP)*/
typedef enum
{
	Complex16bit = 0x00, // IQ，单路数据16位
	Complex32bit = 0x01, // IQ，单路数据32位
	Complex8bit = 0x02,	 // IQ，单路数据8位
	Complexfloat = 0x06  // IQ，单路数据32位浮点（IQS模式不可用，仅由DDC函数输出数据时回写该枚举变量）
}DataFormat_TypeDef;

/*检波模式检波器(DET)，弃用 */
//typedef enum
//{
//	DET_Average = 0x00,	// 平均检波
//	DET_Max = 0x01,		// 最大值检波
//	DET_RMS = 0x02,		// 均方根值检波
//	DET_Sample = 0x03	// 取样检波
//}DETDetector_TypeDef;

/*信号源工作模式(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	SIG_PowerOff = 0x00,			 // 关闭
	SIG_Fixed = 0x01,				 // 固定频率和功率模式
	SIG_FreqSweep_Single = 0x02,	 // 单次频率扫描（未开放）
	SIG_FreqSweep_Continous = 0x03,	 // 连续频率扫描（未开放）
	SIG_PowerSweep_Single = 0x04,	 // 单次功率扫描（未开放）
	SIG_PowerSweep_Continous = 0x05, // 连续功率扫描（未开放）
	SIG_ListSweep_Single =0x06,		 // 单次列表扫描（未开放）
	SIG_ListSweep_Continous = 0x07,	 // 连续列表扫描（未开放）
	SIG_ManualGainCtrl = 0x08		 // 手动增益控制
}SIG_OperationMode_TypeDef;

/*信号源扫描源(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	SIG_SWEEPSOURCE_RF = 0x00,	 // 射频扫描
	SIG_SWEEPSOURCE_LF = 0x01,	 // 低频扫描
	SIG_SWEEPSOURCE_LF_RF = 0x02 // 低频和射频协同扫描
}SIG_SweepSource_TypeDef;

/*发射模式下发射端口状态(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	RF_ExternalPort = 0x00, // 外部端口
	RF_InternalPort = 0x01, // 内部端口
	RF_ANT_Port = 0x02,		// ANT端口（仅适用于TRx 系列）
	RF_TR_Port = 0x03,		// TR端口（仅适用于TRx 系列）
	RF_SWR_Port = 0x04,		// SWR端口（仅适用于TRx 系列）
	RF_INT_Port = 0x05		// INT端口（仅适用于TRx 系列）
}TxPort_TypeDef,RFPort_typedef;

/*发射基带传输复位状态(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	Tx_TRNASFERRESET_ON = 0x00, // 配置时发射传输通道复位开启
	Tx_TRNASFERRESET_OFF = 0x01 // 配置时发射传输通道复位关闭
}TxTransferReset_TypeDef;

/*发射传输链路封包状态*/
typedef enum
{
	TxPacking_Off = 0x00, // 发射链路封包关闭，直接传输有效数据
	TxPacking_On = 0x01   // 发射机中封包开启，为有效数据添加包头包尾
}TxPackingCmd_TypeDef;

/*发射链路的封包位宽*/
typedef enum
{
	TxPackingDataWidth_32Bits = 0x01,  // 封包开启时，封包的数据位宽采用32bits，并保证32bits对齐
	TxPackingDataWidth_64Bits = 0x02,  // 封包开启时，封包的数据位宽采用64bits，并保证64bits对齐
	TxPackingDataWidth_128Bits = 0x04, // 封包开启时，封包的数据位宽采用128bits，并保证128bits对齐
	TxPackingDataWidth_256Bits = 0x08  // 封包开启时，封包的数据位宽采用256bits，并保证256bits对齐
}TxPackingDataWidth_TypeDef;

/*发射模式下触发输入源(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	Tx_TRIGGERIN_SOURCE_INTERNAL = 0x00, // 内部触发
	Tx_TRIGGERIN_SOURCE_EXTERNAL = 0x01, // 外部触发
	Tx_TRIGGERIN_SOURCE_TIMER = 0x02,    // 定时器触发
	Tx_TRIGGERIN_SOURCE_RF = 0x03,       // 射频板触发
	Tx_TRIGGERIN_SOURCE_BUS = 0x04       // 总线触发
}TxTriggerInSource_TypeDef;

/*发射模式下触发输入模式(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	Tx_TRIGGER_MODE_FREERUN = 0x00,     // 自由运行
	Tx_TRIGGER_MODE_SINGLEPOINT = 0x01, // 单点触发（触发一次进行单次的频率或功率的配置）
	Tx_TRIGGER_MODE_SINGLESWEEP = 0x02, // 单次扫描触发（触发一次进行一个周期的扫描）
	Tx_TRIGGER_MODE_CONTINOUS = 0x03    // 连续扫描触发（触发一次连续工作）
}TxTriggerInMode_TypeDef;

/*发射模式下触发输出模式(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	Tx_TRIGGER_OUTMODE_NONE = 0x00,      // 不输出触发
	Tx_TRIGGER_OUTMODE_PERPOINT = 0x01,  // 单点输出（单次跳频、跳功率时输出单个触发）
	Tx_TRIGGER_OUTMODE_PERSWEEP = 0x02,  // 单次扫描输出（单次扫描周期完成后输出一个触发）
	Tx_TRIGGER_OUTMODE_PERPROFILE = 0x03 // 单次Profile输出（单个Profile扫描完成后输出一个触发）
}TxTriggerOutMode_TypeDef;

/*发射模式下的模拟IQ输入源(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	Tx_ANALOGIQSOURCE_INTERNAL = 0x00, // 内部模拟IQ输入
	Tx_ANALOGIQSOURCE_EXTERNAL = 0x01  // 外部模拟IQ输入
}TxAnalogIQSource_TypeDef;

/*发射模式下的数字IQ输入源(仅适用于TRx 系列)(SIG)*/
typedef enum
{
	Tx_DIGITALIQSOURCE_INTERNAL	 = 0x00, // 内部数字IQ输入
	Tx_DIGITALIQSOURCE_EXTERNAL = 0x01,  // 外部数字IQ输入
	Tx_DIGITALIQSOURCE_SIMULATION = 0x02 // 使用内部的正弦仿真信号
}TxDigitalIQSource_TypeDef;

/*收发模式下触发输入源(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	TRx_TRIGGERIN_SOURCE_INTERNAL = 0x00, // 内部触发
	TRx_TRIGGERIN_SOURCE_EXTERNAL = 0x01, // 外部触发
	TRx_TRIGGERIN_SOURCE_TIMER = 0x02,    // 定时器触发
	TRx_TRIGGERIN_SOURCE_RF = 0x03,       // 射频板触发
	TRx_TRIGGERIN_SOURCE_BUS = 0x04       // 总线触发
}TRxTriggerInSource_typedef;

/*收发模式下触发输入模式(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	TRx_TRIGGER_MODE_FREERUN = 0x00,     // 自由运行
	TRx_TRIGGER_MODE_SINGLEPOINT = 0x01, // 单点触发（触发一次进行单次的频率或功率的配置）
	TRx_TRIGGER_MODE_SINGLESWEEP = 0x02, // 单次扫描触发（触发一次进行一个周期的扫描）
	TRx_TRIGGER_MODE_CONTINOUS = 0x03    // 连续扫描触发（触发一次连续工作）
}TRxTriggerInMode_typedef;

/*收发射模式下触发输出模式(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	TRx_TRIGGER_OUTMODE_NONE = 0x00,      // 不输出触发
	TRx_TRIGGER_OUTMODE_PERPOINT = 0x01,  // 单点输出（单次跳频、跳功率时输出单个触发）
	TRx_TRIGGER_OUTMODE_PERSWEEP = 0x02,  // 单次扫描输出（单次扫描周期完成后输出一个触发）
	TRx_TRIGGER_OUTMODE_PERPROFILE = 0x03 // 单次Profile输出（单个Profile扫描完成后输出一个触发）
}TRxTriggerOutMode_typedef;

/*收发模式下收发端口状态:前者为发，后者为收(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	ANT_TR	= 0x00,  // 发射为ANT,接收为TR  	
	TR_ANT  = 0x01,  // 发射为TR,接收为ANT
	SWR_SWR = 0x02,  // 发射为SWR,接收为ANT
	INT_INT = 0x03,  // 发射为INT,接收为INT(内部至内部)
	ANT_ANT = 0x04,  // 发射为ANT,接收为ANT
	SWR_ANT  = 0x05, // 发射为SWR,接收为ANT
	SWR_TR  = 0x06,  // 发射为SWR,接收为TR
	TR_SWR  = 0x07,  // 发射为TR,接收为SWR
	SWR = 0x08,      // 专用来指驻波(S11)测试
	TRANSMIT = 0x09  // 专用来指传输（S21）测试,对于TRX60 =SWR->ANT;对于SAM-60 = Tx->Rx
}TRxPort_typedef;

/*收发模式下接收参考电平模式(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	RxRefLevel_Fixed = 0x00, // 固定参考电平，参考电平值为CurrentLevel_dBm
	RxRefLevel_Sync = 0x01   // 和发射功率同步变化，且偏移值为CurrentLevel_dBm
}RxLevelMode_typedef;

/*矢量网络分析仪工作模式(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	Sweep_S11 = 0x00,   // S11模式，测量反射特性
	Sweep_S21 = 0x01,   // S21模式，测量传输特性
	Sweep_Power = 0x02, // 功率扫描模式，测量功率压缩点等
	Fixed = 0x03        // 固定频率和功率模式
}VNA_OperationMode_TypeDef;

/*矢量网络分析仪校准模式(仅适用于TRx 系列)(VNA)*/
typedef enum
{
	Calibration_Open = 0x00,     // S11模式下，开路校准	
	Calibration_Short = 0x01,    // S11模式下，短路校准		
	Calibration_Match = 0x02,    // S11模式下，匹配校准	
	Calibration_Through = 0x03,  // S21模式下，直通校准
	Calibration_Isolation = 0x04 // S21模式下，隔离校准
}VNA_Calibration_TypeDef;

/*信号类型,用于生成模拟调制的待调制信号(Analog)*/
typedef enum 
{
	SIGNAL_SINE = 0x00,     // 正弦信号
	SIGNAL_COSINE = 0x01,   // 余弦信号
	SIGNAL_LINEAR = 0x02,   // 线性（锯齿）
	SIGNAL_TRIANGLE = 0x03, // 三角
	SIGNAL_CW = 0x04,       // 直流（调制后输出连续波）
	SIGNAL_COMPLEX = 0x05   // 复数信号
}AnalogSignalType_TypeDef;

/*AM调制类型(Analog)*/
typedef enum 
{
	AM_MOD_DSB = 0x00, // 双边带调制
	AM_MOD_LSB = 0x01, // 下边带调制
	AM_MOD_USB = 0x02, // 上边带调制
	AM_MOD_CSB = 0x03, // 残余边带调制
	AM_MOD_CW = 0x04   // 产生连续波
}AMModType_TypeDef;

/*AM调制载波抑制(Analog)*/
typedef enum
{
	AM_CARRIERSUPPRESSION_ON = 0x00, // 载波抑制开启
	AM_CARRIERSUPPRESSION_OFF = 0x01 // 载波抑制关闭
}AM_CarrierSuppression_Typedef;

/*频率检波方式(DSP)*/
typedef enum
{
	TraceFormat_Standard = 0x00, // 频率等间隔
	TraceFormat_PrecisFrq = 0x01 // 频率准确
}TraceFormat_TypeDef;

/*风扇模式(DEV)*/
typedef enum
{
	FanState_On = 0x00,  // 强制常开
	FanState_Off = 0x01, // 强制常关
	FanState_Auto = 0x02 // 自动模式
}FanState_TypeDef;

/*GNSS天线状态*/
typedef enum
{
	GNSS_AntennaExternal = 0x00,  // 外部天线
	GNSS_AntennaInternal = 0x01 // 内部天线
}GNSSAntennaState_TypeDef;

/*DOCXO状态*/
typedef enum
{
	DOCXO_LockMode = 0x00,  // 驯服模式
	DOCXO_HoldMode = 0x01 	// 跟踪模式
}DOCXOWorkMode_TypeDef;

//GNSS 信噪比
typedef struct
{
	uint8_t Max_SatxC_No;	//最大信噪比
	uint8_t Min_SatxC_No;	//最小信噪比
	uint8_t Avg_SatxC_No;	//平均信噪比
}GNSS_SNR_TypeDef;

typedef struct
{
	uint8_t SatsNum_All;	//当前范围可视卫星
	uint8_t SatsNum_Use;	//用于定位的卫星数量
	GNSS_SNR_TypeDef GNSS_SNR_UsePos;		//用于定位的卫星信噪比信息
	GNSS_SNR_TypeDef GNSS_SNR_NotUsePos;	//在视野内，但未用于定位的卫星信噪比信息

}GNSS_SatDate_TypeDef;


/*GNSS获取设备信息*/
typedef struct
{
	float	latitude;	        //经度
    float	longitude;          //纬度
    int16_t	altitude;           //海拔
    uint8_t SatsNum;             //当前使用卫星数量

    uint8_t GNSS_LockState;     //GNSS锁定状态 
    uint8_t DOCXO_LockState;    //DOCXO锁定状态
    DOCXOWorkMode_TypeDef DOCXO_WorkMode;     //DOCXO工作状态
    GNSSAntennaState_TypeDef GNSSAntennaState; //天线状态

    int16_t hour; 
    int16_t minute; 
    int16_t second; 
    int16_t Year; 
    int16_t month; 
    int16_t day;

}GNSSInfo_TypeDef;

//GNSS外设类型
typedef enum
{
	GNSS_None = 0,		//无外设
	GNSS_For_EIO = 1,	//EIO
	GNSS_For_NX = 2,	//NX
	GNSS_For_PX = 3,	//PX
	GNSS_For_PXZ = 4	//PXZ

}GNSSPeriphType_TypeDef;

//GNSS类型
typedef enum
{
	None_GPS = 0, 		//无GPS接收机
	GNSS_GPS = 1,		//标准GPS
	GNSS_GPS_Pro = 2	//高性能GPS
	
}GNSSType_TypeDef;

//OCXO类型
typedef enum
{
	None_OCXO = 0,		//无OCXO
	GNSS_OCXO = 1,		//GNSS上的普通OCXO
	GNSS_DOCXO = 2		//GNSS上的可驯服OCXO
	
}OCXOType_TypeDef;

typedef struct
{
	GNSSPeriphType_TypeDef GNSSPeriphType;	//GNSS外设类型
	GNSSType_TypeDef GNSSType;				//GNSS接收机类型
	OCXOType_TypeDef OCXOType;				//GNSS上的OCXO类型

	uint8_t InternalOCXO;		//设备内部参考时钟是否是恒温晶振
	uint8_t SignalSourceEn;		//设备是否支持 信号源 功能
	uint8_t ADC_VariableRateEn;	//设备是否支持 ADC 可变采样率
	uint8_t IM3_filter;			//补充中频滤波器（IM3增强）

}HardWareState_TypeDef;

/*本振优化(all)*/
typedef enum
{
	LOOpt_Auto = 0x00,		// 本振优化，自动
	LOOpt_Speed = 0x01,		// 本振优化，高扫速
	LOOpt_Spur = 0x02,		// 本振优化，低杂散
	LOOpt_PhaseNoise = 0x03	// 本振优化，低相噪
}LOOptimization_TypeDef;

/*---------------------------------------------------
设备结构体
-----------------------------------------------------*/

/*启动配置(配置)*/
typedef struct
{
	PhysicalInterface_TypeDef PhysicalInterface; // 物理接口选择
	DevicePowerSupply_TypeDef DevicePowerSupply; // 供电方式选择
	IPVersion_TypeDef ETH_IPVersion;             // ETH网际协议版本
	uint8_t ETH_IPAddress[16];                   // ETH接口的IP地址
	uint16_t ETH_RemotePort;                     // ETH接口的侦听端口
	int32_t	ETH_ErrorCode;	                     // ETH接口的返回代码
	int32_t ETH_ReadTimeOut;                     // ETH接口的读取超时(ms)
}BootProfile_TypeDef;

/*设备信息(返回)*/
typedef struct
{
	uint64_t DeviceUID;		  // 设备序列号
	uint16_t Model;			  // 设备类型
	uint16_t HardwareVersion; // 硬件版本
	uint32_t MFWVersion;	  // MCU固件版本
	uint32_t FFWVersion;	  // FPGA固件版本
}DeviceInfo_TypeDef;

/*NX设备信息(返回)*/
typedef struct
{
	uint64_t DeviceUID;		  // 设备序列号
	uint16_t Model;			  // 设备类型
	uint16_t HardwareVersion; // 硬件版本
	uint32_t MFWVersion;	  // MCU固件版本
	uint32_t FFWVersion;	  // FPGA固件版本
	uint8_t IPAddress[4];     // IP地址
	uint8_t SubnetMask[4];    // 子网掩码
}NetworkDeviceInfo_TypeDef;

/*启动信息(返回)*/
typedef struct 
{
	DeviceInfo_TypeDef DeviceInfo; // 设备信息

	uint32_t BusSpeed;	           // 总线速度信息
	uint32_t BusVersion;           // 总线固件版本
	uint32_t APIVersion;           // API版本

	int ErrorCodes[7];	           // 启动过程中的错误代码清单
	int Errors;			           // 启动过程中的错误总数
	int WarningCodes[7];           // 启动过程中的警告代码清单
	int Warnings;		           // 启动过程中的警告总数
}BootInfo_TypeDef;

/*设备状态(高级变量，不建议直接使用)(返回)*/
typedef struct
{
	int16_t	Temperature;	  // 设备温度，摄氏度 = 0.01 * Temperature
	uint16_t RFState;		  // 射频状态
	uint16_t BBState;		  // 基带状态

	double AbsoluteTimeStamp; // 当前数据包对应的绝对时间戳
	float Latitude;           // 当前数据包对应的纬度坐标,北纬为正数，南纬为负数，以此区分南北纬
	float Longitude;          // 当前数据包对应的经度坐标,东经为正数，西经为负数，以此区分东西经

	uint16_t GainPattern;	  // 当前数据包对应频点所使用的增益控制字
	int64_t	RFCFreq;		  // 当前数据包对应频点所使用的射频中心频率

	uint32_t ConvertPattern;  // 当前数据包频点所使用的频率变换方式
	uint32_t NCOFTW;	      // 当前数据包频点所使用的NCO频率字

	uint32_t SampleRate;	  // 当前数据包频点所使用的等效采样率，等效采样率 = ADC采样率/抽取倍数
	uint16_t CPU_BCFlag;      // CPU-FFT做拼帧时，所需的BC标志位
	uint16_t IFOverflow;      // 设备是否中频过载，考虑并BBState或RFState
	uint16_t DecimateFactor;  // 当前数据包频点所使用的抽取倍数
	uint16_t OptionState;     // 选件状态

	//int16_t		LicenseCode;   // 许可证代码
}DeviceState_TypeDef;

/*测量数据的辅助信息(返回)*/
typedef struct
{
	uint32_t MaxIndex;			// 功率最大值在当前数据包中的索引
	float	 MaxPower_dBm;		// 当前数据包中的功率最大值

	int16_t	 Temperature;		// 设备温度，摄氏度 = 0.01 * Temperature
	uint16_t RFState;			// 射频状态
	uint16_t BBState;			// 基带状态
	uint16_t GainPattern;	    // 当前数据包对应频点所使用的增益控制字

	uint32_t ConvertPattern;	// 当前数据包频点所使用的频率变换方式

	double	 SysTimeStamp;		// 当前数据包所对应的系统时间戳，单位s

	double   AbsoluteTimeStamp;	// 当前数据包对应的绝对时间戳
	float    Latitude;			// 当前数据包对应的纬度坐标,北纬为正数，南纬为负数，以此区分南北纬
	float    Longitude;			// 当前数据包对应的经度坐标,东经为正数，西经为负数，以此区分东西经

}MeasAuxInfo_TypeDef;

/*---------------------------------------------------
扫描模式结构体(sweep)
-----------------------------------------------------*/
/*SWP的配置结构体(简洁配置)*/
typedef struct
{
	double StartFreq_Hz; 							   // 起始频率
	double StopFreq_Hz;	 							   // 终止频率
	double CenterFreq_Hz;                              // 中心频率
	double Span_Hz;                                    // 频率扫宽
	double RefLevel_dBm;							   // 参考电平
	double RBW_Hz;		 							   // 分辨率带宽
	double VBW_Hz;									   // 视频带宽
	double SweepTime;								   // 当扫描时间模式指定为Manual时，该参数为绝对时间；当指定为*N时，该参数为扫描时间倍率	

	SWP_FreqAssignment_TypeDef FreqAssignment;         // 频率指定方式，选择以StartStop或CenterSpan方式设定频率

	Window_TypeDef Window; 							   // FFT分析所使用的窗型

	RBWMode_TypeDef RBWMode; 						   // RBW更新方式。手动输入、根据Span自动设置

	VBWMode_TypeDef VBWMode;						   // VBW更新方式。手动输入、VBW = RBW、VBW = 0.1*RBW、 VBW = 0.01*RBW

	SweepTimeMode_TypeDef SweepTimeMode;			   // 扫描时间模式

	Detector_TypeDef Detector;						   // 检波器

	TraceDetectMode_TypeDef TraceDetectMode;		   // 迹线检波模式（频率轴）
	TraceDetector_TypeDef TraceDetector; 			   // 迹线检波器

	uint32_t TracePoints;  							   // 迹线点数

	RxPort_TypeDef RxPort; 							   // 射频输入端口

	SpurRejection_TypeDef SpurRejection; 			   // 杂散抑制

	ReferenceClockSource_TypeDef ReferenceClockSource; // 参考时钟源
	double ReferenceClockFrequency; 				   // 参考时钟频率，Hz

	SWP_TriggerSource_TypeDef TriggerSource; 		   // 输入触发源
	TriggerEdge_TypeDef	TriggerEdge; 		 		   // 输入触发边沿

	PreamplifierState_TypeDef Preamplifier;			   // 前置放大器动作设置

	SWP_TraceType_TypeDef TraceType; 				   // 输出迹线类型

}SWP_EZProfile_TypeDef;

/*扫描模式的配置结构体(配置)*/
typedef struct
{
	double StartFreq_Hz; 									 // 起始频率
	double StopFreq_Hz;	 									 // 终止频率
	double CenterFreq_Hz;                                    // 中心频率
	double Span_Hz;                                          // 频率扫宽
	double RefLevel_dBm;									 // 参考电平
	double RBW_Hz;		 									 // 分辨率带宽
	double VBW_Hz;											 // 视频带宽
	double SweepTime;										 // 当扫描时间模式指定为Manual时，该参数为绝对时间；当指定为*N时，该参数为扫描时间倍率	
	double TraceBinSize_Hz;                                  // 迹线相邻频点之间的频率间隔

	SWP_FreqAssignment_TypeDef FreqAssignment;               // 频率指定方式，选择以StartStop或CenterSpan方式设定频率
	
	Window_TypeDef Window; 									 // FFT分析所使用的窗型

	RBWMode_TypeDef RBWMode; 							 	 // RBW更新方式。手动输入、根据Span自动设置

	VBWMode_TypeDef VBWMode;							 	 // VBW更新方式。手动输入、VBW = RBW、VBW = 0.1*RBW、 VBW = 0.01*RBW

	SweepTimeMode_TypeDef SweepTimeMode;					 // 扫描时间模式

	Detector_TypeDef Detector;								 // 检波器

	TraceFormat_TypeDef TraceFormat;						 // 迹线格式
	TraceDetectMode_TypeDef TraceDetectMode;				 // 迹线检波模式（频率轴）
	TraceDetector_TypeDef TraceDetector; 					 // 迹线检波器

	uint32_t TracePoints;  									 // 迹线点数
	TracePointsStrategy_TypeDef	TracePointsStrategy; 		 // 迹线点数映射策略
	TraceAlign_TypeDef TraceAlign;                           // 迹线对齐方式指定

	FFTExecutionStrategy_TypeDef FFTExecutionStrategy; 		 // FFT执行策略

	RxPort_TypeDef RxPort; 									 // 射频输入端口

	SpurRejection_TypeDef SpurRejection; 					 // 杂散抑制

	ReferenceClockSource_TypeDef ReferenceClockSource; 		 // 参考时钟源
	double ReferenceClockFrequency; 				   		 // 参考时钟频率，Hz
	uint8_t EnableReferenceClockOut;						 // 使能参考时钟输出

	SystemClockSource_TypeDef SystemClockSource; 		     // 系统时钟源，默认使用内部系统时钟，请在厂商指导下使用
	double ExternalSystemClockFrequency; 				   	 // 外部系统时钟频率，Hz

	SWP_TriggerSource_TypeDef TriggerSource; 				 // 输入触发源
	TriggerEdge_TypeDef	TriggerEdge; 		 				 // 输入触发边沿
	TriggerOutMode_TypeDef TriggerOutMode;   				 // 触发输出模式
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // 触发输出脉冲极性

	uint32_t PowerBalance; 									 // 功耗与扫描速度平衡
	GainStrategy_TypeDef GainStrategy; 						 // 增益策略
	PreamplifierState_TypeDef Preamplifier;					 // 前置放大器动作设置
	
	uint8_t	AnalogIFBWGrade; 								 //	中频带宽档位
	uint8_t IFGainGrade; 	 								 //	中频增益档位

	uint8_t	EnableDebugMode; 	 							 //	调试模式，高级应用不推荐用户自行使用，默认值为 0
	uint8_t	CalibrationSettings; 							 // 校准选择，高级应用不推荐用户自行使用，默认值为 0

	int8_t Atten;                                            // 衰减dB,设定频谱仪通道衰减量，默认-1（自动）

	SWP_TraceType_TypeDef TraceType; 						 // 输出迹线类型

	LOOptimization_TypeDef LOOptimization;					 // 本振优化

}SWP_Profile_TypeDef;

/*扫描模式的迹线信息结构体(返回)*/
typedef struct
{
	int FullsweepTracePoints;	     // 完整迹线的点数
	int PartialsweepTracePoints;     // 每个频点的迹线点数，即每次GetPart的点数
	int TotalHops;				     // 完整迹线的频点数，即一条完整迹线需要GetPart的次数
	uint32_t UserStartIndex;	     // 迹线数组中与用户指定StartFreq_Hz对应的数组索引，即HopIndex = 0时，Freq[UserStartIndex]是与SWPProfile.StartFreq_Hz最为近的频率点
	uint32_t UserStopIndex;	         // 迹线数组中与用户指定SttopFreq_Hz对应的数组索引，即HopIndex = TotalHops - 1时，Freq[UserStopIndex]是与SWPProfile.StopFreq_Hz最为近的频率点
	
	double TraceBinBW_Hz;		     // 迹线两点间的频率间隔
	double StartFreq_Hz;		     // 迹线第一个频点的频率
	double AnalysisBW_Hz;		     // 每个频点对应的分析带宽

	int TraceDetectRatio;		     // 视频检波的检波比
	int DecimateFactor;			     // 时域数据的抽取倍数
	float FrameTimeMultiple;	     // 帧分析时间倍率，设备在单一频点上的分析时间 = 默认分析时间（系统自行设定） * 帧时间倍率。提高帧时间倍率将增加设备的最小扫描时间，但不严格线性。
	double FrameTime;		 	     // 帧扫描时间：用于进行单帧FFT分析的信号持续时间（单位为秒）
	double EstimateMinSweepTime;     // 当前配置下，所能设定的最小扫描时间（单位为秒，结果主要受Span、RBW、VBW、帧扫描时间等因素影响） 
	DataFormat_TypeDef DataFormat;   // 时域数据格式
	uint64_t SamplePoints;		     // 时域数据采样长度
	uint32_t GainParameter;		     // 增益相关参数，包括Space(31~24Bit)、PreAmplifierState(23~16Bit)、StartRFBand(15~8Bit)、StopRFBand(7~0Bit)
	DSPPlatform_Typedef DSPPlatform; // 当前配置所使用的DSP运算平台
}SWP_TraceInfo_TypeDef;

/*扫描模式的调试信息结构体(返回)*/
typedef struct
{
	double	RFCFreq;				//
	double	RFLOFreq;				//
	double	IFLOFreq;				//
	double	IF1STFreq;				//
	double	IF2NDFreq;				//
	double	NCOFreq;				//

	int		HopIndex;				//

	uint8_t	RFBand;					//
	uint8_t	HighSideRFLO;			//
	uint8_t HighSideIFLO;			//
	uint8_t IQInvert;				//

	uint8_t RFGainSpace;			//
	uint8_t	RFGainGrade;			//
	uint8_t AnalogIFBWGrade;		//
	uint8_t IFGainGrade;			//

	uint16_t RFState;				//
	uint16_t BBState;				//

	uint32_t LowCTIndex;			//
	uint32_t HighCTIndex;			//
	uint32_t RTIndex;				//

	int16_t CalibratedTemperature;  //
	int16_t LowCharactTemperature;  //
	int16_t HighCharactTemperature; //
	int16_t ReferenceTemperature;	//

	float	 RFACalVal;				//
}SWP_DebugInfo_TypeDef;

/*SWP包含配置、返回信息和暂存数据的顶层结构体(配置、返回、暂存)*/
typedef struct
{
	double* Freq_Hz;                     // 暂存在Device指针中的Frequency数据的地址
	float* PowerSpec_dBm;                // 暂存在Device指针中的PowerSpec_dBm数据的地址
	int HopIndex;					     // 当前数据对应的频点索引，用于拼接频谱
	int FrameIndex;					     // 当一个频点存在多帧（多次FFT）时，指示当前数据的帧索引

	void* AlternIQStream;			 	 // 当选择频谱迹线与IQ数据一同上传时，交织IQ数据的地址
	float ScaletoV;						 // 当选择频谱迹线与IQ数据一同上传时，IQ至V的比例因子

	MeasAuxInfo_TypeDef MeasAuxInfo;     // 当前数据对应信息，包括当前包内最大数据，及当前时刻的温度坐标设备状态等

	SWP_Profile_TypeDef SWP_Profile;	 // 当前SWP数据对应的SWP配置信息，一般通过SWP_Configuration函数更新（SWP_ProfileOut）
	SWP_TraceInfo_TypeDef SWP_TraceInfo; // 当前SWP数据对应的迹线信息，一般通过SWP_Configuration函数更新（SWP_TraceInfo）

	DeviceInfo_TypeDef DeviceInfo;       // 当前SWP数据对应的设备信息
	DeviceState_TypeDef DeviceState;     // 当前SWP数据对应的设备状态
}SWPTrace_TypeDef;

/*---------------------------------------------------
IQ流模式结构体(IQStream)
-----------------------------------------------------*/
/*IQS的配置结构体(简洁配置)*/
typedef struct
{
	double CenterFreq_Hz; 							   // 中心频率
	double RefLevel_dBm;							   // 参考电平
	uint32_t DecimateFactor;						   // 时域数据的抽取倍数

	RxPort_TypeDef RxPort;							   // 射频输入端口

	uint32_t BusTimeout_ms;							   // 传输超时时间

	IQS_TriggerSource_TypeDef TriggerSource;		   // 输入触发源
	TriggerEdge_TypeDef	TriggerEdge;				   // 输入触发边沿

	TriggerMode_TypeDef	TriggerMode;				   // 输入触发模式	
	uint64_t TriggerLength;							   // 输入触发后的采样点数，仅在FixedPoints模式下生效

	double TriggerLevel_dBm;						   // 电平触发：门限
	double TriggerTimer_Period;						   // 定时触发：触发周期，单位为s		

	DataFormat_TypeDef DataFormat;					   // 数据格式

	PreamplifierState_TypeDef Preamplifier;			   // 前置放大器动作

	uint8_t	AnalogIFBWGrade;						   // 中频带宽档位

	ReferenceClockSource_TypeDef ReferenceClockSource; // 参考时钟源
	double ReferenceClockFrequency;					   // 参考时钟频率

	double  NativeIQSampleRate_SPS;					   // 特定设备适用。原生的IQ采样速率，对于可变采样率设备，可通过调整该参数对采样率进行调整；非可变采样率设备配总是配置为系统默认的固定值

	uint8_t EnableIFAGC;							   // 特定设备适用。中频AGC控制，0：AGC关闭，使用MGC方式；1：AGC开启。

}IQS_EZProfile_TypeDef;

/*IQS的配置结构体(配置)*/
typedef struct
{
	double CenterFreq_Hz; 								 	 // 中心频率
	double RefLevel_dBm;								 	 // 参考电平
	uint32_t DecimateFactor;								 // 时域数据的抽取倍数

	RxPort_TypeDef RxPort;									 // 射频输入端口

	uint32_t BusTimeout_ms;							 		 // 传输超时时间

	IQS_TriggerSource_TypeDef TriggerSource;				 // 输入触发源
	TriggerEdge_TypeDef	TriggerEdge;						 // 输入触发边沿
	TriggerMode_TypeDef	TriggerMode;						 // 输入触发模式	
	uint64_t TriggerLength;									 // 输入触发后的采样点数，仅在FixedPoints模式下生效

	TriggerOutMode_TypeDef TriggerOutMode;					 // 触发输出模式
	TriggerOutPulsePolarity_TypeDef TriggerOutPulsePolarity; // 触发输出脉冲极性

	double TriggerLevel_dBm;								 // 电平触发：门限
	double TriggerLevel_SafeTime;							 // 电平触发：防抖安全时间，单位为s
	double TriggerDelay;									 // 电平触发：触发延迟，单位为s
	double PreTriggerTime;									 // 电平触发：预触发时间，单位为s

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // 定时触发：与外触发边沿的同步选项
	double TriggerTimer_Period;								 // 定时触发：触发周期，单位为s		
	
	uint8_t EnableReTrigger;							     // 自动重触发：使能设备在捕获到一次触发后，进行多次响应，仅可用于FixedPoint模式下
	double ReTrigger_Period;								 // 自动重触发：多次响应的时间间隔，也作为Timer触发模式下的触发周期，单位为s
	uint16_t ReTrigger_Count;				 	    		 // 自动重触发：每次原触发动作之后，自动重触发的执行次数

	DataFormat_TypeDef DataFormat;						 	 // 数据格式

	GainStrategy_TypeDef GainStrategy;				 		 // 增益策略
	PreamplifierState_TypeDef Preamplifier;					 // 前置放大器动作

	uint8_t	AnalogIFBWGrade;								 //	中频带宽档位
	uint8_t	IFGainGrade;									 //	中频增益档位

	uint8_t	EnableDebugMode;								 //	调试模式，高级应用不推荐用户自行使用，默认值为 0

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	参考时钟源
	double ReferenceClockFrequency;							 //	参考时钟频率
	uint8_t EnableReferenceClockOut;						 // 使能参考时钟输出

	SystemClockSource_TypeDef SystemClockSource; 		     // 系统时钟源
	double ExternalSystemClockFrequency; 				   	 // 外部系统时钟频率，Hz

	double  NativeIQSampleRate_SPS;							 // 特定设备适用。原生的IQ采样速率，对于可变采样率设备，可通过调整该参数对采样率进行调整；非可变采样率设备配总是配置为系统默认的固定值
	
	uint8_t EnableIFAGC;									 // 特定设备适用。中频AGC控制，0：AGC关闭，使用MGC方式；1：AGC开启。

	int8_t Atten;                                            // 衰减

	DCCancelerMode_TypeDef DCCancelerMode;					 // 特定设备适用。直流抑制。0: 关闭DCC；1：开启，高通滤波器模式（更好的抑制效果，但会损伤DC至100kHz范围内的信号）；2：开启，手动偏置模式（需要人工校准，但不低频损伤信号）

	QDCMode_TypeDef	QDCMode;								 // 特定设备适用。IQ幅相修正器。QDCOff: 关闭QDC功能；QDCManualMode：开启并使用手动模式；QDCAutoMode：开启并使用自动QDC模式

	float QDCIGain;											 // 特定设备适用。归一化线性增益 I路，1.0 表示无增益，设置范围 0.8~1.2
	float QDCQGain;											 // 特定设备适用。归一化线性增益 Q路，1.0 表示无增益 ，设置范围 0.8~1.2
	float QDCPhaseComp;										 // 特定设备适用。归一化相位补偿系数，设置范围 -0.2~+0.2

	int8_t DCCIOffset;										 // 特定设备适用。I通道直流偏置， LSB
	int8_t DCCQOffset;										 // 特定设备适用。Q通道直流偏置， LSB

	LOOptimization_TypeDef LOOptimization;					 // 本振优化

}IQS_Profile_TypeDef;

/*IQS配置后返回的流信息结构体(返回)*/
typedef struct
{
	double Bandwidth;        // 当前配置对应的接收机物理通道或数字信号处理的带宽
	double IQSampleRate;     // 当前配置对应的IQ单路采样率，单位S/s（Sample/second）
	uint64_t PacketCount;	 // 当前配置对应的总数据包数，仅在FixedPoints模式下生效
	
	uint64_t StreamSamples;	 // Fixedpoints模式下表示当前配置对应采样的总点数；Adaptive模式下没有物理意义，值为0
	uint64_t StreamDataSize; // Fixedpoints模式下表示当前配置对应采样的总字节数；Adaptive模式下没有物理意义，值为0

	uint32_t PacketSamples;  // 每次调用IQS_GetIQStream 获取到的数据包中采样点数 每个数据包中包含的样点数
	uint32_t PacketDataSize; // 每次调用IQS_GetIQStream 所得到的有效数据字节数
	uint32_t GainParameter;	 // 增益相关参数，包括Space(31~24Bit)、PreAmplifierState(23~16Bit)、StartRFBand(15~8Bit)、StopRFBand(7~0Bit)
}IQS_StreamInfo_TypeDef;

/*IQS数据包中包含的触发信息结构体，DET和RTA的触发信息返回结构体与其相同(返回)*/
typedef struct
{
	uint64_t	SysTimerCountOfFirstDataPoint;	  // 当前包的首个数据点对应的系统时间戳
	uint16_t	InPacketTriggeredDataSize;		  // 当前包中有效触发数据的字节数
	uint16_t	InPacketTriggerEdges;			  // 当前包中所包含的触发边沿个数
	uint32_t	StartDataIndexOfTriggerEdges[25]; // 当前包中触发边沿对应的包中数据位置
	uint64_t	SysTimerCountOfEdges[25];		  // 当前包中触发边沿的系统时间戳
	int8_t		EdgeType[25];					  // 当前包中各触发边沿的极性
}IQS_TriggerInfo_TypeDef;

typedef IQS_TriggerInfo_TypeDef DET_TriggerInfo_TypeDef;
typedef IQS_TriggerInfo_TypeDef RTA_TriggerInfo_TypeDef;

/*IQS包含配置、返回信息和暂存数据的顶层结构体(配置、返回、暂存)*/
typedef struct
{
	void* AlternIQStream;				     // 交织分布的IQ时域数据，单路可能为i8 i16 i32 格式

	float IQS_ScaleToV;						 // int类型至电压绝对值（V）的系数

	float	 MaxPower_dBm;					 // 当前数据包中的功率最大值
	uint32_t MaxIndex;						 // 功率最大值在当前数据包中的索引

	IQS_Profile_TypeDef IQS_Profile;		 // 当前IQ流对应的IQS配置信息，一般通过IQS_Configuration函数更新（IQS_ProfileOut）
	IQS_StreamInfo_TypeDef IQS_StreamInfo;	 // 当前IQ流对应的IQS流格式信息，一般通过IQS_Configuration函数更新

	IQS_TriggerInfo_TypeDef IQS_TriggerInfo; // 当前IQ流对应的IQS触发信息，一般通过IQS_GetIQStream函数更新
	DeviceInfo_TypeDef DeviceInfo;           // 当前IQ流对应的设备信息，一般通过IQS_GetIQStream函数更新
	DeviceState_TypeDef DeviceState;         // 当前IQ流对应的设备状态，一般通过IQS_GetIQStream函数更新

}IQStream_TypeDef; 


/*---------------------------------------------------
检波模式结构体(Detector)
-----------------------------------------------------*/
/*DET的配置结构体(简洁配置)*/
typedef struct
{
	double CenterFreq_Hz; 							   // 中心频率
	double RefLevel_dBm;							   // 参考电平
	uint32_t DecimateFactor;						   // 时域数据的抽取倍数

	RxPort_TypeDef RxPort;							   // 射频输入端口

	uint32_t BusTimeout_ms;							   // 传输超时时间

	DET_TriggerSource_TypeDef TriggerSource;		   // 输入触发源
	TriggerEdge_TypeDef TriggerEdge;				   // 输入触发边沿

	TriggerMode_TypeDef	TriggerMode;				   // 输入触发模式	
	uint64_t TriggerLength;							   // 输入触发后的采样点数，仅在FixedPoints模式下生效

	double TriggerLevel_dBm;						   // 电平触发：门限
	double TriggerTimer_Period;						   // 定时触发：周期				

	Detector_TypeDef Detector;  					   // 检波器
	uint16_t DetectRatio;  							   // 检波比，检波器对功率迹线进行检波，每DetectRatio个原始数据点检出为1个输出迹线点

	PreamplifierState_TypeDef Preamplifier;			   // 前置放大器动作

	uint8_t	AnalogIFBWGrade;						   // 中频带宽档位

	ReferenceClockSource_TypeDef ReferenceClockSource; // 参考时钟源
	double ReferenceClockFrequency;					   // 参考时钟频率

}DET_EZProfile_TypeDef;

/*DET的配置结构体(配置)*/
typedef struct
{
	double CenterFreq_Hz; 									 // 中心频率
	double RefLevel_dBm;									 // 参考电平
	uint32_t DecimateFactor;								 // 时域数据的抽取倍数

	RxPort_TypeDef RxPort;									 // 射频输入端口

	uint32_t BusTimeout_ms;									 // 传输超时时间

	DET_TriggerSource_TypeDef TriggerSource;				 // 输入触发源
	TriggerEdge_TypeDef TriggerEdge;						 // 输入触发边沿

	TriggerMode_TypeDef	TriggerMode;						 // 输入触发模式	
	uint64_t TriggerLength;									 // 输入触发后的采样点数，仅在FixedPoints模式下生效

	TriggerOutMode_TypeDef TriggerOutMode;					 // 触发输出模式
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // 触发输出脉冲极性

	double TriggerLevel_dBm;								 // 电平触发：门限
	double TriggerLevel_SafeTime;							 // 电平触发：防抖安全时间，单位为s
	double TriggerDelay;									 // 电平触发：触发延迟，单位为s
	double PreTriggerTime;									 // 电平触发：预触发时间，单位为s

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // 定时触发：与外触发边沿同步选项
	double TriggerTimer_Period;								 // 定时触发：周期				
	
	uint8_t EnableReTrigger;							     // 自动重触发：使能设备在捕获到一次触发后，进行多次响应，仅可用于FixedPoint模式下
	double ReTrigger_Period;								 // 自动重触发：多次响应的时间间隔，也作为Timer触发模式下的触发周期，单位为s
	uint16_t ReTrigger_Count;				 	    		 // 自动重触发：每次原触发动作之后，自动重触发的执行次数

	Detector_TypeDef Detector;  							 // 检波器
	uint16_t DetectRatio;  									 // 检波比，检波器对功率迹线进行检波，每DetectRatio个原始数据点检出为1个输出迹线点

	GainStrategy_TypeDef GainStrategy;						 // 增益策略
	PreamplifierState_TypeDef Preamplifier;					 // 前置放大器动作

	uint8_t	AnalogIFBWGrade;								 //	中频带宽档位
	uint8_t	IFGainGrade;									 //	中频增益档位

	uint8_t	EnableDebugMode;								 //	调试模式，高级应用不推荐用户自行使用，默认值为0

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	参考时钟源
	double ReferenceClockFrequency;							 //	参考时钟频率
	uint8_t EnableReferenceClockOut;						 // 使能参考时钟输出

	SystemClockSource_TypeDef SystemClockSource; 		     // 系统时钟源
	double ExternalSystemClockFrequency; 				   	 // 外部系统时钟频率，Hz

	int8_t Atten;                                            // 衰减

	DCCancelerMode_TypeDef DCCancelerMode;					 // 特定设备适用。直流抑制。0: 关闭DCC；1：开启，高通滤波器模式（更好的抑制效果，但会损伤DC至100kHz范围内的信号）；2：开启，手动偏置模式（需要人工校准，但不低频损伤信号）

	QDCMode_TypeDef	QDCMode;								 // 特定设备适用。IQ幅相修正器。QDCOff: 关闭QDC功能；QDCManualMode：开启并使用手动模式；QDCAutoMode：开启并使用自动QDC模式

	float QDCIGain;											 // 特定设备适用。归一化线性增益 I路，1.0 表示无增益，设置范围 0.8~1.2
	float QDCQGain;											 // 特定设备适用。归一化线性增益 Q路，1.0 表示无增益 ，设置范围 0.8~1.2
	float QDCPhaseComp;										 // 特定设备适用。归一化相位补偿系数，设置范围 -0.2~+0.2

	int8_t DCCIOffset;										 // 特定设备适用。I通道直流偏置， LSB
	int8_t DCCQOffset;										 // 特定设备适用。Q通道直流偏置， LSB

	LOOptimization_TypeDef LOOptimization;					 // 本振优化

}DET_Profile_TypeDef;


/*RBW滤波器类型*/
typedef enum
{
	RBWFilter_80PercentABW = 0x00,
	RBWFilter_Gaussian_3dB = 0x01,
	RBWFilter_Gaussian_6dB = 0x02,
	RBWFilter_Gaussian_Impulse = 0x03,
	RBWFilter_Gaussian_Noise = 0x04,
	RBWFilter_Flattop = 0x05
}RBWFilterType_TypeDef;

/*DET的配置结构体(配置)*/
typedef struct
{
	double CenterFreq_Hz; 									 // 中心频率
	double RefLevel_dBm;									 // 参考电平
	uint32_t DecimateFactor;								 // 时域数据的抽取倍数

	RxPort_TypeDef RxPort;									 // 射频输入端口

	uint32_t BusTimeout_ms;									 // 传输超时时间

	DET_TriggerSource_TypeDef TriggerSource;				 // 输入触发源
	TriggerEdge_TypeDef TriggerEdge;						 // 输入触发边沿

	TriggerMode_TypeDef	TriggerMode;						 // 输入触发模式	
	uint64_t TriggerLength;									 // 输入触发后的采样点数，仅在FixedPoints模式下生效

	TriggerOutMode_TypeDef TriggerOutMode;					 // 触发输出模式
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // 触发输出脉冲极性

	double TriggerLevel_dBm;								 // 电平触发：门限
	double TriggerLevel_SafeTime;							 // 电平触发：防抖安全时间，单位为s
	double TriggerDelay;									 // 电平触发：触发延迟，单位为s
	double PreTriggerTime;									 // 电平触发：预触发时间，单位为s

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // 定时触发：与外触发边沿同步选项
	double TriggerTimer_Period;								 // 定时触发：周期				
	
	uint8_t EnableReTrigger;							     // 自动重触发：使能设备在捕获到一次触发后，进行多次响应，仅可用于FixedPoint模式下
	double ReTrigger_Period;								 // 自动重触发：多次响应的时间间隔，也作为Timer触发模式下的触发周期，单位为s
	uint16_t ReTrigger_Count;				 	    		 // 自动重触发：每次原触发动作之后，自动重触发的执行次数

	Detector_TypeDef Detector;  							 // 检波器
	uint16_t DetectRatio;  									 // 检波比，检波器对功率迹线进行检波，每DetectRatio个原始数据点检出为1个输出迹线点

	GainStrategy_TypeDef GainStrategy;						 // 增益策略
	PreamplifierState_TypeDef Preamplifier;					 // 前置放大器动作

	uint8_t	AnalogIFBWGrade;								 //	中频带宽档位
	uint8_t	IFGainGrade;									 //	中频增益档位

	uint8_t	EnableDebugMode;								 //	调试模式，高级应用不推荐用户自行使用，默认值为0

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	参考时钟源
	double ReferenceClockFrequency;							 //	参考时钟频率
	uint8_t EnableReferenceClockOut;						 // 使能参考时钟输出

	SystemClockSource_TypeDef SystemClockSource; 		     // 系统时钟源
	double ExternalSystemClockFrequency; 				   	 // 外部系统时钟频率，Hz

	int8_t Atten;                                            // 衰减

	DCCancelerMode_TypeDef DCCancelerMode;					 // 特定设备适用。直流抑制。0: 关闭DCC；1：开启，高通滤波器模式（更好的抑制效果，但会损伤DC至100kHz范围内的信号）；2：开启，手动偏置模式（需要人工校准，但不低频损伤信号）

	QDCMode_TypeDef	QDCMode;								 // 特定设备适用。IQ幅相修正器。QDCOff: 关闭QDC功能；QDCManualMode：开启并使用手动模式；QDCAutoMode：开启并使用自动QDC模式

	float QDCIGain;											 // 特定设备适用。归一化线性增益 I路，1.0 表示无增益，设置范围 0.8~1.2
	float QDCQGain;											 // 特定设备适用。归一化线性增益 Q路，1.0 表示无增益 ，设置范围 0.8~1.2
	float QDCPhaseComp;										 // 特定设备适用。归一化相位补偿系数，设置范围 -0.2~+0.2

	int8_t DCCIOffset;										 // 特定设备适用。I通道直流偏置， LSB
	int8_t DCCQOffset;										 // 特定设备适用。Q通道直流偏置， LSB

	LOOptimization_TypeDef LOOptimization;					 // 本振优化

	double RBW_Hz;											 // 分辨率带宽
	double VBW_Hz;											 // 视频带宽
	VBWMode_TypeDef VBWMode;								 // VBW更新方式
	RBWFilterType_TypeDef RBWFilterType;					 // RBW滤波器类型

}ZSP_Profile_TypeDef;


/*DET配置后返回的流信息结构体(返回)*/
typedef struct
{
	uint64_t PacketCount;	 // 当前配置对应的总数据包数，仅在FixedPoints模式下生效
	
	uint64_t StreamSamples;  // Fixedpoints模式下表示当前配置对应采样的总点数；Adaptive模式下没有物理意义，值为0
	uint64_t StreamDataSize; // Fixedpoints模式下表示当前配置对应采样的总字节数；Adaptive模式下没有物理意义，值为0

	uint32_t PacketSamples;  // 每次调用DET_GetTrace 获取到的数据包中采样点数 每个数据包中包含的样点数
	uint32_t PacketDataSize; // 每次调用DET_GetTrace 所得到的有效数据字节数
	double TimeResolution;   // 时域点分辨率
	uint32_t GainParameter;	 // 增益相关参数，包括Space(31~24Bit)、PreAmplifierState(23~16Bit)、StartRFBand(15~8Bit)、StopRFBand(7~0Bit)
}DET_StreamInfo_TypeDef;

/*DET包含配置、返回信息和暂存数据的顶层结构体(配置、返回、暂存)*/
typedef struct
{
	float* NormalizedPowerStream;            // 暂存在Device指针中的归一化的功率流
	float DET_ScaleToV;                      // int类型至电压绝对值（V）的系数

	uint32_t MaxIndex;						 // 功率最大值在当前数据包中的索引
	float	 MaxPower_dBm;					 // 当前数据包中的功率最大值


	DET_Profile_TypeDef DET_Profile;         // 当前DET流对应的DET配置信息，一般通过DET_Configuration函数更新（DET_ProfileOut）
	DET_StreamInfo_TypeDef DET_StreamInfo;   // 当前DET流对应的DET格式信息，一般通过DET_Configuration函数更新

	DET_TriggerInfo_TypeDef DET_TriggerInfo; // 当前DET流对应的DET触发信息
	DeviceInfo_TypeDef DeviceInfo;           // 当前DET流对应的设备信息
	DeviceState_TypeDef DeviceState;         // 当前DET流对应的设备状态

}DETStream_TypeDef;

/*---------------------------------------------------
实时频谱分析模式结构体(RealTime Analysis)
-----------------------------------------------------*/
/*RTA的配置结构体(简洁配置)*/
typedef struct
{
	double CenterFreq_Hz; 							  // 中心频率
	double RefLevel_dBm;							  // 参考电平
	double RBW_Hz;									  // 分辨率带宽
	double VBW_Hz;									  // 视频带宽

	RBWMode_TypeDef RBWMode; 						  // RBW更新方式。手动输入、根据Span自动设置
	VBWMode_TypeDef VBWMode;						  // VBW更新方式。手动输入、VBW = RBW、VBW = 0.1*RBW、 VBW = 0.01*RBW

	uint32_t DecimateFactor;						  // 时域数据的抽取倍数

	Window_TypeDef	Window;							  // 窗型

	SweepTimeMode_TypeDef SweepTimeMode;			  // 扫描时间模式
	double SweepTime;								  // 当扫描时间模式指定为Manual时，该参数为绝对时间；当指定为*N时，该参数为扫描时间倍率
	Detector_TypeDef Detector;						  // 检波器

	TraceDetectMode_TypeDef TraceDetectMode;		  // 迹线检波模式
	TraceDetector_TypeDef TraceDetector;  			  // 迹线检波器
	uint32_t TraceDetectRatio;  					  // 迹线检波检波比。迹线检波器将原始频谱迹线以每TraceDetectRatio个原始频谱数据点检出1个输出频谱数据点

	RxPort_TypeDef	RxPort;							  // 接收端口设置

	uint32_t BusTimeout_ms;							  // 传输超时时间

	RTA_TriggerSource_TypeDef TriggerSource;		   // 输入触发源
	TriggerEdge_TypeDef TriggerEdge;				   // 输入触发边沿

	TriggerMode_TypeDef	TriggerMode;				   // 输入触发模式
	double TriggerAcqTime;							   // 输入触发后的采样时间，仅在FixedPoints模式下生效，单位s

	double TriggerLevel_dBm;						   // 电平触发：门限
	double TriggerTimer_Period;						   // 定时触发：周期					

	PreamplifierState_TypeDef Preamplifier;			   // 前置放大器动作

	ReferenceClockSource_TypeDef ReferenceClockSource; // 参考时钟源
	double ReferenceClockFrequency;					   // 参考时钟频率

}RTA_EZProfile_TypeDef;

/*RTA的配置结构体(配置)*/
typedef struct
{
	double CenterFreq_Hz; 									 // 中心频率
	double RefLevel_dBm;									 // 参考电平
	double RBW_Hz;											 // 分辨率带宽
	double VBW_Hz;											 // 视频带宽
	RBWMode_TypeDef RBWMode; 							 	 // RBW更新方式。手动输入、根据Span自动设置
	VBWMode_TypeDef VBWMode;							 	 // VBW更新方式。手动输入、VBW = RBW、VBW = 0.1*RBW、 VBW = 0.01*RBW

	uint32_t DecimateFactor;								 // 时域数据的抽取倍数

	Window_TypeDef	Window;									 // 窗型

	SweepTimeMode_TypeDef SweepTimeMode;					 // 扫描时间模式
	double SweepTime;										 // 当扫描时间模式指定为Manual时，该参数为绝对时间；当指定为*N时，该参数为扫描时间倍率
	Detector_TypeDef Detector;								 // 检波器

	TraceDetectMode_TypeDef TraceDetectMode;				 // 迹线检波模式
	uint32_t TraceDetectRatio;  						     // 迹线检波检波比
	TraceDetector_TypeDef TraceDetector;  				     // 迹线检波检波器

	RxPort_TypeDef	RxPort;									 // 接收端口设置

	uint32_t BusTimeout_ms;									 // 传输超时时间

	RTA_TriggerSource_TypeDef TriggerSource;				 // 输入触发源
	TriggerEdge_TypeDef TriggerEdge;						 // 输入触发边沿

	TriggerMode_TypeDef	TriggerMode;						 // 输入触发模式
	double TriggerAcqTime;							     	 // 输入触发后的采样时间，仅在FixedPoints模式下生效，单位s

	TriggerOutMode_TypeDef TriggerOutMode;					 // 触发输出模式
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // 触发输出脉冲极性

	double TriggerLevel_dBm;								 // 电平触发：门限
	double TriggerLevel_SafeTime;							 // 电平触发：防抖安全时间，单位为s
	double TriggerDelay;									 // 电平触发：触发延迟，单位为s
	double PreTriggerTime;									 // 电平触发：预触发时间，单位为s

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // 定时触发：与外触发边沿同步选项
	double TriggerTimer_Period;								 // 定时触发：周期					

	uint8_t EnableReTrigger;							     // 自动重触发：使能设备在捕获到一次触发后，进行多次响应，仅可用于FixedPoint模式下
	double ReTrigger_Period;								 // 自动重触发：多次响应的时间间隔，也作为Timer触发模式下的触发周期，单位为s
	uint16_t ReTrigger_Count;				 	    		 // 自动重触发：每次原触发动作之后，自动重触发的执行次数

	GainStrategy_TypeDef GainStrategy;						 // 增益策略
	PreamplifierState_TypeDef Preamplifier;					 // 前置放大器动作
	uint8_t AnalogIFBWGrade;								 //	中频带宽档位
	uint8_t	IFGainGrade;									 //	中频增益档位

	uint8_t	EnableDebugMode;								 //	调试模式，高级应用不推荐用户自行使用，默认值为 0

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	参考时钟源
	double ReferenceClockFrequency;							 //	参考时钟频率
	uint8_t EnableReferenceClockOut;						 // 使能参考时钟输出

	SystemClockSource_TypeDef SystemClockSource; 		     // 系统时钟源
	double ExternalSystemClockFrequency; 				   	 // 外部系统时钟频率，Hz

	int8_t Atten;                                            // 衰减

	DCCancelerMode_TypeDef DCCancelerMode;					 // 特定设备适用。直流抑制。0: 关闭DCC；1：开启，高通滤波器模式（更好的抑制效果，但会损伤DC至100kHz范围内的信号）；2：开启，手动偏置模式（需要人工校准，但不低频损伤信号）

	QDCMode_TypeDef	QDCMode;								 // 特定设备适用。IQ幅相修正器。QDCOff: 关闭QDC功能；QDCManualMode：开启并使用手动模式；QDCAutoMode：开启并使用自动QDC模式

	float QDCIGain;											 // 特定设备适用。归一化线性增益 I路，1.0 表示无增益，设置范围 0.8~1.2
	float QDCQGain;											 // 特定设备适用。归一化线性增益 Q路，1.0 表示无增益 ，设置范围 0.8~1.2
	float QDCPhaseComp;										 // 特定设备适用。归一化相位补偿系数，设置范围 -0.2~+0.2

	int8_t DCCIOffset;										 // 特定设备适用。I通道直流偏置， LSB
	int8_t DCCQOffset;										 // 特定设备适用。Q通道直流偏置， LSB

	LOOptimization_TypeDef LOOptimization;					 // 本振优化

}RTA_Profile_TypeDef;

/*RTA配置后返回的包信息结构体(返回)*/
typedef struct
{
	double StartFrequency_Hz;	 // 频谱的起始频率	
	double StopFrequency_Hz;     // 频谱的终止频率
	double POI;				     // 100%截获概率下的信号最短持续时间,以s为单位。

	double TraceTimestampStep;   // 每包数据内各条Trace的时间戳步进。(包整体时间戳为TriggerInfo中的SysTimerCountOfFirstDataPoint)
	double TimeResolution;	 	 // 每个时域数据的采样时间，也是时间戳的分辨率
	double PacketAcqTime;	 	 // 每包数据对应的采集时间

	uint32_t PacketCount;        // 当前配置对应的总数据包数，仅在FixedPoints模式下生效
	uint32_t PacketFrame;        // 每个数据包中的有效帧数
	uint32_t FFTSize;            // 每帧FFT的点数
	uint32_t FrameWidth;		 // FFT帧截取后的点数，也是数据包中每条Trace的点数，可作为概率密度图的X轴点数(宽度)
	uint32_t FrameHeight;		 // FFT帧对应的频谱幅度范围，可作为概率密度图的Y轴点数(高度)

	uint32_t PacketSamplePoints; // 每包数据对应的采集点数
	uint32_t PacketValidPoints;	 // 每包数据中所含的频域有效数据点数

	uint32_t MaxDensityValue;	 // 概率密度位图的单个位点元素值上限
	uint32_t GainParameter;		 // 包括Space(31~24Bit)、PreAmplifierState(23~16Bit)、StartRFBand(15~8Bit)、StopRFBand(7~0Bit)
}RTA_FrameInfo_TypeDef;

/*RTA获取后返回的绘图信息结构体 */
typedef struct
{
	float ScaleTodBm;	          // 由线性功率转对数功率导致的压缩。Trace的绝对功率等于 Trace[] * ScaleTodBm + OffsetTodBm(Bitmap的绝对功率轴同下)
	float OffsetTodBm;            // 相对功率转换成绝对功率的偏移。bitmap的绝对功率轴范围(Y轴)等于 FrameHeigh * ScaleTodBm + OffsetTodBm(Trace物理功率同上)
	uint64_t SpectrumBitmapIndex; // 概率密度图的获取次数，可在绘图时当做索引使用
}RTA_PlotInfo_TypeDef;

/*RTA包含配置、返回信息和暂存数据的顶层结构体(配置、返回、暂存)*/
typedef struct
{
	uint8_t* SpectrumStream;                 // 暂存在Device指针中的RTA频谱数据的地址
	uint16_t* SpectrumBitmap;                // 暂存在Device指针中的RTA频谱位图数据的地址

	uint32_t MaxIndex;						 // 功率最大值在当前数据包中的索引
	float	 MaxPower_dBm;					 // 当前数据包中的功率最大值

	RTA_Profile_TypeDef RTA_Profile;         // 当前RTA流对应的RTA配置信息，一般通过RTA_Configuration函数更新（RTA_ProfileOut）
	RTA_FrameInfo_TypeDef RTA_FrameInfo;     // 当前RTA流对应的RTA包信息，一般通过RTA_Configuration函数更新

	RTA_PlotInfo_TypeDef RTA_PlotInfo;       // 当前RTA流对应的RTA绘图信息
	RTA_TriggerInfo_TypeDef RTA_TriggerInfo; // 当前RTA流对应的RTA触发信息
	DeviceInfo_TypeDef DeviceInfo;           // 当前RTA流对应的设备信息
	DeviceState_TypeDef DeviceState;         // 当前RTA流对应的设备状态

}RTAStream_TypeDef;


/*---------------------------------------------------
多Profile模式联合体(MPS)
-----------------------------------------------------*/
typedef union
{
	/*一次获取只会得到一种模式的数据，通过返回的Analysis表示当前获取的模式，通过返回的ProfileNum表示当前获取的配置号*/
	SWPTrace_TypeDef SWPSpectrum;
	IQStream_TypeDef IQStream;
	DETStream_TypeDef DETStream;
	RTAStream_TypeDef RTAStream;

}MPSData_TypeDef;

/*---------------------------------------------------
多Profile模式数据信息(MPS)
-----------------------------------------------------*/
typedef struct
{
	uint8_t Analysis;
	uint16_t ProfileIndex;
	float IQS_ScaleToV;
	float DET_ScaleToV;
	RTA_PlotInfo_TypeDef RTA_PlotInfo;

}MPSDataInfo_TypeDef;


/*---------------------------------------------------
校准相关(CAL)
-----------------------------------------------------*/

/*CAL的配置结构体(配置)*/
typedef struct
{
	double	RFCFreq_Hz;			   // 设备射频中心频率，Hz
	double	BBCFreq_Hz;			   // 所选取的校准信号频点，Hz
	float	Temperature;		   // 设备温度

	float	RefLevel_dBm;		   // 设备参考电平，dBm
	float	CalSignalLevel_dBm;	   // 校准信号功率，dBm

	float	QDCIGain;			   // 求取的IGain优化值
	float	QDCQGain;			   // 求取的QGain优化值
	float	QDCPhaseComp;		   // 求取的PhaseComp优化值
	float	EstimatedOptRejection; // 优化值带入后所得到的预期边带抑制值

}CAL_QDCOptParam_TypeDef;

/*---------------------------------------------------
信号源功能结构体(Signal Generator)
-----------------------------------------------------*/

/*发射模式下手动增益控制结构体，在发射源工作在SIG_ManualGainCtrl模式下，次参数生效(配置)*/
typedef struct
{
	uint8_t TxPreDSA;      // 前置衰减器值；输入范围：0-127; 步进1
	uint8_t TxPostDSA;     // 后置衰减器值；输入范围：0-127; 步进1
	uint8_t TxAuxDSA;      // 辅助衰减器值；输入范围：0-127; 步进1
	uint8_t PreAmplifier;  // 前置放大器状态：0：关闭； 1：开启
	uint8_t PostAmplifier; // 后置放大器状态：0：关闭； 1：开启
}TxManualGain_TypeDef;

/*信号源IQStream的播放模式*/
typedef enum
{
	SIG_IQStreamPlayMode_Null = 0x00,		   // 实时传输播放模式
	SIG_IQStreamPlayMode_RealTime = 0x01,	   // 实时传输播放模式
	SIG_IQStreamPlayMode_SinglePlay = 0x02,    // 单次播放
	SIG_IQStreamPlayMode_ContinousPlay = 0x04, // 连续播放
	SIG_IQStreamPlayMode_MultiPlay = 0x08,	   // 多次播放
	SIG_IQStreamPlayMode_Simulation = 0x10 	   // 仿真信号

}SIG_IQStreamPlayMode_TypeDef;

/*信号源IQStream的播放的触发源*/
typedef enum
{
	SIG_IQStreamPlayTrigger_Null = 0x00,	 // 无触发；等待重配成其他触发类型
	SIG_IQStreamPlayTrigger_Internal = 0x01, // 内部触发
	SIG_IQStreamPlayTrigger_External = 0x02, // 外部触发
	SIG_IQStreamPlayTrigger_Timer = 0x04, 	 // 定时触发
	SIG_IQStreamPlayTrigger_RFBoard = 0x08,  // 定时触发
	SIG_IQStreamPlayTrigger_Bus = 0x10		 // 总线触发

}SIG_IQStreamPlayTrigger_TypeDef;

/*信号源模式用户接口定义(配置)*/
typedef struct
{
	double	CenterFreq_Hz;                                   // 当前中心频率，单位为Hz，在信号源工作在SIG_Fixed模式下生效；输入范围1M-1GHz;步进1Hz
	double	StartFreq_Hz;                                    // 频率扫描模式下的起始频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double	StopFreq_Hz;                                     // 频率扫描模式下的终止频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double	StepFreq_Hz;                                     // 频率扫描模式下的步进频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double	CurrentLevel_dBm;                                // 当前功率，单位为dBm，在信号源工作在SIG_Fixed模式下生效；输入范围-127--5dBm；步进0.25dB
	double	StartLevel_dBm;                                  // 功率扫描模式下的起始功率，单位为Hz，在信号源工作在SIG_PowerSweep_*模式下生效；输入范围-127--5dBm；步进0.25dB
	double	StopLevel_dBm;                                   // 功率扫描模式下的终止功率，单位为Hz，在信号源工作在SIG_PowerSweep_*模式下生效；输入范围-127--5dBm；步进0.25dB
	double	StepLevel_dBm;                                   // 功率扫描模式下的步进功率，单位为Hz，在信号源工作在SIG_PowerSweep_*模式下生效；输入范围-127--5dBm；步进0.25dB
	double	DwellTime_ms;                                    // 扫描驻留时间，在信号源工作在*Sweep*模式下生效；输入范围0-1000000；步进1
	double  DACSampleRate;						             // 指定DAC的采样率
	double	InterpolationFactor;                             // 信号源基带插值倍数，该参数决定了信号源的基带带宽，输入范围:1-1024；除1以外，输入为偶数
	double  ReferenceClockFrequency;                         // 信号源模式下参考输入频率；该参数目前仅支持10MHz的参考频率输入，微调该参数可以补偿内部或外部的参考频率的频偏
	ReferenceClockSource_TypeDef ReferenceClockSource;       // 参考频率输入源；0：内部 1：外部
	SIG_OperationMode_TypeDef OperationMode;                 // 信号源工作模式
	SIG_SweepSource_TypeDef SweepSource;                     // 信号源扫描源

	SIG_IQStreamPlayMode_TypeDef	SIG_IQStreamPlayMode;	 // 信号源IQStream的播放模式
	SIG_IQStreamPlayTrigger_TypeDef	SIG_IQStreamPlayTrigger; // 信号源IQStream的播放的触发源
	uint32_t	SIG_IQStreamPlayLength;						 // 信号源IQStream的播放长度
	uint32_t	SIG_IQStreamPlayCounts;						 // 信号源IQStream的播放次数
	uint32_t SIG_IQStreamPlayPrevDelay;						 // 信号源IQStream的播放的前延时，对每一次的播放均生效
	uint32_t SIG_IQStreamPlayPostDelay;						 // 信号源IQStream的播放的后延时，对每一次的播放均生效

	uint32_t SIG_IQStreamDownloadStartAddress;				 // 信号源IQStream下载至板载内存中的起始内存地址
	uint32_t SIG_IQStreamDownloadStopAddress;				 // 信号源IQStream下载至板载内存中的终止内存地址

	uint32_t SIG_IQStreamPlayStartAddress;					 // 信号源从板载内存中读取IQStream的起始内存地址
	uint32_t SIG_IQStreamPlayStopAddress;					 // 信号源从板载内存中读取IQStream的终止内存地址

	int16_t SIG_IQStreamIdle_DC_I;			 	             // I路信号非播放模式下的静默直流值
	int16_t SIG_IQStreamIdle_DC_Q;			 	             // Q路信号非播放模式下的的静默直流值

	int16_t SIG_IQStream_DC_Offset_I;			             // I路信号的直流偏移范围为-32768~+32767
	int16_t SIG_IQStream_DC_Offset_Q;			             // Q路信号的直流偏移范围为-32768~+32767

	double	SIG_IQStream_Gain_I;				             // I路信号的线性增益： 增益范围为-256~255
	double	SIG_IQStream_Gain_Q;				             // Q路信号的线性增益： 增益范围为-256~255

	double  SIG_IQStreamSimFrequency;			             // 当SIG_IQStreamPlayMode=Simualtion模式时，指定仿真的基带频率；

	TxPort_TypeDef TxPort;                                   // 信号源输出端口
	TxTriggerInSource_TypeDef TxTriggerInSource;             // 信号源触发输入源
	TxTriggerInMode_TypeDef TxTriggerInMode;                 // 信号源输出输入模式
	TxTriggerOutMode_TypeDef TxTriggerOutMode;               // 信号源触发输出模式
	TxAnalogIQSource_TypeDef TxAnalogIQSource;               // 信号源模拟IQ输入源
	TxDigitalIQSource_TypeDef TxDigitalIQSource;             // 信号源数字QI输入源
	TxTransferReset_TypeDef TransferResetCmd;                // 信号源基带传输复位状态
	TxPackingCmd_TypeDef	TxPackingCmd;		             // 信号源封包模式状态
	TxManualGain_TypeDef TxManualGain;                       // 信号源手动增益控制
}SIG_Profile_TypeDef;

/*信号源模式返回信息(待更新)(返回)*/
typedef struct
{
	uint32_t SweepPoints; // 扫描点数
}SIG_Info_TypeDef;

/*---------------------------------------------------
模拟信号源功能结构体(Analog Signal Generator)(为SIG的子功能)
-----------------------------------------------------*/

/*模拟信号源的工作模式*/
typedef enum
{
	ASG_Mute = 0x00,		   // 静音
	ASG_FixedPoint = 0x01, 	   // 定频定功率
	ASG_FrequencySweep = 0x02, // 频率扫描
	ASG_PowerSweep = 0x03 	   // 功率扫描

}ASG_Mode_TypeDef;

/*模拟信号源的触发输入源*/
typedef enum
{
	ASG_TriggerIn_FreeRun = 0x00,  // 自由运行
	ASG_TriggerIn_External = 0x01, // 外触发
	ASG_TriggerIn_Bus = 0x02	   // 定时器触发

}ASG_TriggerSource_TypeDef;

/*模拟信号源的触发输入模式*/
typedef enum
{
	ASG_TriggerInMode_Null = 0x00,        // 自由运行
	ASG_TriggerInMode_SinglePoint = 0x01, // 单点触发（触发一次进行单次的频率或功率的配置）
	ASG_TriggerInMode_SingleSweep = 0x02, // 单次扫描触发（触发一次进行一个周期的扫描）
	ASG_TriggerInMode_Continous = 0x03    // 连续扫描触发（触发一次连续工作）
}ASG_TriggerInMode_TypeDef;

/*模拟信号源的触发输出端口*/
typedef enum
{
	ASG_TriggerOut_Null = 0x00, 	// 外部端口
	ASG_TriggerOut_External = 0x01, // 内部端口
	ASG_TriggerOut_Bus = 0x02 		// 内部端口
}ASG_TriggerOutPort_TypeDef;

/*模拟信号源的触发输出模式*/
typedef enum
{
	ASG_TriggerOutMode_Null = 0x00,        // 自由运行
	ASG_TriggerOutMode_SinglePoint = 0x01, // 单点触发（一次跳频输出一个脉冲）
	ASG_TriggerOutMode_SingleSweep = 0x02  // 单次扫描触发（一次扫描输出一个脉冲）
}ASG_TriggerOutMode_TypeDef;

/*模拟信号源的射频输出端口*/
typedef enum
{
	ASG_Port_External = 0x00, // 外部端口
	ASG_Port_Internal = 0x01, // 内部端口
	ASG_Port_ANT = 0x02,	  // ANT端口（仅适用于TRx 系列）
	ASG_Port_TR = 0x03,		  // TR端口（仅适用于TRx 系列）
	ASG_Port_SWR = 0x04,	  // SWR端口（仅适用于TRx 系列）
	ASG_Port_INT = 0x05		  // INT端口（仅适用于TRx 系列）

}ASG_Port_TypeDef;

/*模拟信号源配置结构体(配置)*/
typedef struct
{
	double   CenterFreq_Hz; 						   // 当前中心频率，单位为Hz，在信号源工作在SIG_Fixed模式下生效；输入范围1M-1GHz;步进1Hz
	double   Level_dBm; 						  	   // 当前功率，单位为dBm，在信号源工作在SIG_Fixed模式下生效；输入范围-127--5dBm；步进0.25dB

	double   StartFreq_Hz; 						  	   // 频率扫描模式下的起始频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double   StopFreq_Hz; 							   // 频率扫描模式下的终止频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double   StepFreq_Hz; 							   // 频率扫描模式下的步进频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz

	double   StartLevel_dBm; 					  	   // 功率扫描模式下的起始功率，单位为Hz
	double   StopLevel_dBm; 						   // 功率扫描模式下的终止功率，单位为Hz
	double   StepLevel_dBm; 						   // 功率扫描模式下的步进功率，单位为Hz
	
	double   DwellTime_s; 						  	   // 频率扫描模式 或 功率扫描模式 下，单位为s，当触发模式为BUS时，扫描驻留时间，单位为s，在信号源工作在*Sweep*模式下生效；输入范围0-1000000；步进1
	
	double ReferenceClockFrequency;					   // 指定参考频率：对内参考和外参考均生效;
	ReferenceClockSource_TypeDef ReferenceClockSource; // 选择参考时钟的输入源：内参考或外参考;

	ASG_Port_TypeDef Port; 							   // 信号源输出端口

	ASG_Mode_TypeDef	Mode; 						   // 关闭、点频、频率扫描（外触发，同步至接收）、功率扫描（外触发，同步至接收） 													  

	ASG_TriggerSource_TypeDef	TriggerSource; 		   // 信号源触发输入模式：
	ASG_TriggerInMode_TypeDef	TriggerInMode;		   // 信号源的触发模式;
	ASG_TriggerOutMode_TypeDef	TriggerOutMode;		   // 信号源的触发模式;
	
}ASG_Profile_TypeDef;

/*模拟信号源模式返回信息(待更新)(返回)*/
typedef struct
{
	uint32_t SweepPoints; // 扫描点数
}ASG_Info_TypeDef;

/*---------------------------------------------------
矢量信号源功能结构体(Vector Signal Generator)(为SIG的子功能)
-----------------------------------------------------*/

/*矢量信号源的工作模式*/
typedef enum
{
	VSG_Mode_Null = 0x00,		 // 无输出，静音模式
	VSG_Mode_RealTime = 0x01,	 // 实时播放模式
	VSG_Mode_PlayFromRAM = 0x02, // 从内存中读取数据并播放
	VSG_Mode_SimulatedCW = 0x04, // 内部仿真生成的连续波模式
	VSG_Mode_Advanced = 0x08	 // 高级模式（暂未开放）

}VSG_Mode_TypeDef;

/*矢量信号源源的触发输入源*/
typedef enum
{
	VSG_TriggerSource_Null = 0x00,	   // 无触发；等待重配成其他触发类型
	VSG_TriggerSource_Internal = 0x01, // 内部触发
	VSG_TriggerSource_External = 0x02, // 外部触发
	VSG_TriggerSource_Timer = 0x04,    // 定时触发
	VSG_TriggerSource_RFBoard = 0x08,  // 定时触发
	VSG_TriggerSource_Bus = 0x10	   // 总线触发

}VSG_TriggerSource_TypeDef;

/*矢量信号源的射频输出端口*/
typedef enum
{
	VSG_Port_External = 0x00, // 外部端口
	VSG_Port_Internal = 0x01, // 内部端口
	VSG_Port_ANT = 0x02,	  // ANT端口（仅适用于TRx 系列）
	VSG_Port_TR = 0x03,		  // TR端口（仅适用于TRx 系列）
	VSG_Port_SWR = 0x04,	  // SWR端口（仅适用于TRx 系列）
	VSG_Port_INT = 0x05		  // INT端口（仅适用于TRx 系列）

}VSG_Port_TypeDef;

/*矢量信号源的配置结构体（配置）*/
typedef struct
{
	double CenterFrequency;			                   // 矢量信号源的中心频率
	double Outlevel;				                   // 矢量信号源的输出功率
	double DACSampleRate;			                   // 矢量信号源的采样率
	double InterpolationFactor;		                   // 矢量信号源的插值倍数
	uint32_t Triggerlength;			                   // 矢量信号源的IQStream播放的长度
	int32_t TriggerCount;			                   // 矢量信号源的IQStream播放次数
	uint32_t TriggerDelay_Prev;		                   // 矢量信号源的IQStream播放前延时，对每次播放均生效
	uint32_t TriggerDelay_Post; 	                   // 矢量信号源的IQStream播放后延时，对每次播放均生效
	uint32_t PlayStartAddress;		                   // 矢量信号源的IQStream播放的板载内存起始地址
	uint32_t PlayStopAddress;		                   // 矢量信号源的IQStream播放的板载内存终止地址
	double ReferenceClockFrequency;                    // 信号源模式下参考输入频率；该参数目前仅支持10MHz的参考频率输入，微调该参数可以补偿内部或外部的参考频率的频偏
	ReferenceClockSource_TypeDef ReferenceClockSource; // 矢量信号源的参考时钟数据源
	VSG_Mode_TypeDef Mode;							   // 矢量信号源的工作模式
	VSG_Port_TypeDef Port;							   // 矢量信号源的射频输出端口	
	VSG_TriggerSource_TypeDef TriggerSource;		   // 矢量信号源的触发输入源
}VSG_Profile_TypeDef;

/*矢量信号源的模式下的高级参数（配置），仅适用于VSG_Mode_Advanced模式*/
typedef struct
{
	double	StartFreq_Hz;                        // 频率扫描模式下的起始频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double	StopFreq_Hz;                         // 频率扫描模式下的终止频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double	StepFreq_Hz;                         // 频率扫描模式下的步进频率，单位为Hz，在信号源工作在SIG_FreqSweep_*模式下生效；输入范围1M-1GHz;步进1Hz
	double	StartLevel_dBm;                      // 功率扫描模式下的起始功率，单位为Hz，在信号源工作在SIG_PowerSweep_*模式下生效；输入范围-127--5dBm；步进0.25dB
	double	StopLevel_dBm;                       // 功率扫描模式下的终止功率，单位为Hz，在信号源工作在SIG_PowerSweep_*模式下生效；输入范围-127--5dBm；步进0.25dB
	double	StepLevel_dBm;                       // 功率扫描模式下的步进功率，单位为Hz，在信号源工作在SIG_PowerSweep_*模式下生效；输入范围-127--5dBm；步进0.25dB
	double	DwellTime_ms;                        // 扫描驻留时间，在信号源工作在*Sweep*模式下生效；输入范围0-1000000；步进1

	SIG_OperationMode_TypeDef OperationMode;     // 信号源工作模式
	SIG_SweepSource_TypeDef SweepSource;         // 信号源扫描源

	int16_t SIG_IQStreamIdle_DC_I;			 	 // I路信号非播放模式下的静默直流值
	int16_t SIG_IQStreamIdle_DC_Q;			 	 // Q路信号非播放模式下的的静默直流值

	int16_t SIG_IQStream_DC_Offset_I;			 // I路信号的直流偏移范围为-32768~+32767
	int16_t SIG_IQStream_DC_Offset_Q;			 // Q路信号的直流偏移范围为-32768~+32767

	double	SIG_IQStream_Gain_I;				 // I路信号的线性增益： 增益范围为-256~255
	double	SIG_IQStream_Gain_Q;				 // Q路信号的线性增益： 增益范围为-256~255
	double  SIG_IQStreamSimFrequency;			 // 当SIG_IQStreamPlayMode=Simualtion模式时，指定仿真的基带频率；

	TxTriggerInSource_TypeDef TxTriggerInSource; // 信号源触发输入源
	TxTriggerInMode_TypeDef TxTriggerInMode;     // 信号源输出输入模式
	TxTriggerOutMode_TypeDef TxTriggerOutMode;   // 信号源触发输出模式
	TxAnalogIQSource_TypeDef TxAnalogIQSource;   // 信号源模拟IQ输入源
	TxDigitalIQSource_TypeDef TxDigitalIQSource; // 信号源数字QI输入源
	TxTransferReset_TypeDef TransferResetCmd;    // 信号源基带传输复位状态
	TxPackingCmd_TypeDef	TxPackingCmd;		 // 信号源封包模式状态
	TxManualGain_TypeDef TxManualGain;           // 信号源手动增益控制
}VSG_PrivateParam_TypeDef;

/*矢量信号源的参数返回信息（返回)*/
typedef struct
{
	uint32_t PlayAddressRange; // 扫描点数
}VSG_Info_TypeDef;

/*矢量信号源文件格式*/
typedef enum
{
	VSG_FileFormat_wav = 0x00, // wav格式
	VSG_FileFormat_dat = 0x01  // dat格式

}VSG_FileFormat_TypeDef;

/*矢量信号源播放文件返回的信息(返回)*/
typedef struct
{
	uint32_t FileSize_Byte; 	   // 文件大小（字节）
	uint32_t IQStreamLength;	   // IQStream长度（采样点）
	uint32_t PlayStartAddress;	   // 存入板载内存中的起始地址
	uint32_t PlayStopAddress;	   // 存入板载内存中的终止地址
	uint32_t NextFileStartAddress; // 指示下一个文件或者片段可下载播放的起始内存地址
	uint32_t PlayAddressRange;	   // 文件播放的地址范围	
	uint32_t AddressOverRangeFlag; // 当写入地址超过最大地址时，置位该标志位
	uint32_t File_Format;		   // 文件格式
	double	 FileSampleRate;	   // 文件中的IQStream的采样率

}VSG_FileInfo_TypeDef;

/*---------------------------------------------------
矢量网络分析仪模式结构体(Vector Network Analyzer)
-----------------------------------------------------*/

/*矢量网络分析仪模式用户接口定义(配置)*/
typedef struct
{
	double	CenterFreq_Hz;							   // 当设置为功率扫描或固定模式下，此参数为中心频率；输入范围1M-1GHz;步进1Hz
	double	StartFreq_Hz;							   // S11/S21扫描模式下的起始频率，单位为Hz；输入范围1M-1GHz;步进1Hz
	double	StopFreq_Hz;							   // S11/S21扫描模式下的终止频率，单位为Hz；输入范围1M-1GHz;步进1Hz
	double	StepFreq_Hz;							   // S11/S21扫描模式下的步进频率，单位为Hz；输入范围1M-1GHz;步进1Hz
	double	TxOutLevel_dBm;							   // 此参数指定发射机的当前发射功率；输入范围-127--5dBm；步进0.25dB
	double	TxStartOutLevel_dBm;					   // 此参数指定发射机的起始发射功率；输入范围-127--5dBm；步进0.25dB
	double	TxStopOutLevel_dBm;						   // 此参数指定发射机的终止发射功率；输入范围-127--5dBm；步进0.25dB
	double	TxStepOutLevel_dBm;						   // 此参数指定发射机的步进发射功率；输入范围-127--5dBm；步进0.25dB
	double	RxRefLevel_dBm;							   // 此参数指定接收机的参考电平或参考电平相对发射功率的偏移值，或当与发射功率同步时，指定相对于发射的功率偏移值；输入范围-127--5dBm；步进0.25dB
	double	RBW;									   // 指定接收机的分析带宽；输入范围1Hz-1MHz（待确定）；步进1Hz
	double	DwellTime_us;							   // 指定驻留时间，此参数适用于当被测设备的传输延时较大时；输入范围0-1000000；步进1
	double ReferenceClockFrequency;					   // 信号源模式下参考输入频率；该参数目前仅支持10MHz的参考频率输入，微调该参数可以补偿内部或外部的参考频率的频偏
	ReferenceClockSource_TypeDef ReferenceClockSource; // 参考频率输入源；0：内部 1：外部
	VNA_OperationMode_TypeDef OperationMode;		   // 网络分析仪工作模式
	RxLevelMode_typedef	RxLevelMode;				   // 当工作在功率扫描模式下时，此参数指定接收机参考电平的工作状态：固定、与发射机同步
	TRxTriggerInSource_typedef TriggerInSource;		   // 触发输入源
	TRxTriggerInMode_typedef TriggerInMode;			   // 触发输入模式
	TRxTriggerOutMode_typedef TriggerOutMode;		   // 触发输出模式
}VNA_Profile_TypeDef;

/*矢量网络分析模式的返回信息(待更新)(返回)*/
typedef struct
{
	double	StartFreq_Hz; // S11/S21扫描模式下的起始频率，单位为Hz；输入范围1M-1GHz;步进1Hz
	double	StopFreq_Hz;  // S11/S21扫描模式下的终止频率，单位为Hz；输入范围1M-1GHz;步进1Hz
	double	StepFreq_Hz;  // S11/S21扫描模式下的步进频率，单位为Hz；输入范围1M-1GHz;步进1Hz
	int SweepPoints;      // 扫描点数	
	int AnalysisPoints;   // 分析点数
}VNA_Info_TypeDef;


/*---------------------------------------------------
模拟调制解调结构体(Analog)
-----------------------------------------------------*/

/*信号源FM调制信号接口定义(配置)*/
typedef struct
{
	double SignalFrequency;                    // FM调制信号频率：输入范围：暂未确定
	double ModDeviation;                       // FM调制频偏：输入范围：暂未确定
	double SampleRate;                         // 采样率：该采样率= DAC原始采样率/插值倍数
	short Amplitude;                           // 信号幅度；输入范围：0-8191，步进1
	int resetflag;                             // 复位状态，重新配置后第一运行应设置为1，之后需要置0
	AnalogSignalType_TypeDef AnalogSignalType; // 模拟信号类型
}SIG_FM_Profile_TypeDef;

typedef struct
{
	float* DemodWaveform;       //解调波形 %
	float* AFSpectrum_ModDepth; //音频频谱的幅值，长度为DemodWaveformSize / 2，使用线性刻度，单位为%
	double* AFSpectrum_Freq;    //音频频谱的频率 Hz
	uint32_t DemodWaveformSize; //解调波形的点数

	float ModDepth;         // 调制深度 %
	float ModDepthPeakPos;  // 正峰值的调制深度, Peak+ %
	float ModDepthPeakNeg;  // 负峰值的调制深度平均值, Peak- %
	float ModDepthHalfPeak; // 半峰值的调制深度平均值, (Peak+ - Peak-)/2 %
	float ModDepthRMS;      // 均方根值的调制深度平均值 %

	float CarrierPower;     // 载波功率 dBm
	double ModRate;         // 调制率 Hz
	float SINAD;            // 信纳德 dB
	float RMSPower;         // 均方根功率 dBm
	double FreqError;       // 频率误差 Hz
	float SNR;              // 信噪比 dB
	float DistTotalVrms;    // 失真电压的占比(RMS) %
	float THD;              // 总谐波失真 %
	float PEP;              // 峰包络功率 dBm
}AMDemodParam_TypeDef;

typedef struct
{
	float* DemodWaveform;        //解调波形 Hz
	float* AFSpectrum_Deviation; //音频频谱的幅值，长度为DemodWaveformSize / 2，使用线性刻度，单位为Hz
	double* AFSpectrum_Freq;     //音频频谱的频率 Hz
	uint32_t DemodWaveformSize;  //解调波形的点数

	float Deviation;         // 调制频偏 Hz
	float DeviationPeakPos;  // 正峰值的调制频偏, Peak+ Hz
	float DeviationPeakNeg;  // 负峰值的调制频偏平均值, Peak- Hz
	float DeviationHalfPeak; // 半峰值的调制频偏平均值, (Peak+ - Peak-)/2 Hz
	float DeviationRMS;      // 均方根值的调制频偏平均值 Hz

	float CarrierPower;      // 载波功率 dBm
	double CarrierFreqErr;   // 载波频偏 Hz
	double ModRate;          // 调制率 Hz

	float SINAD;             // 信纳德 dB
	float SNR;               // 信噪比 dB
	float DistTotalVrms;     // 失真占总有效值比(RMS) %
	float THD;               // 总谐波失真 %
}FMDemodParam_TypeDef;

/*信号源AM调制信号接口定义(配置)*/
typedef struct
{
	double SignalFrequency;								// AM调制信号频率：输入范围：暂未确定
	double ModIndex;									// AM调制指数：输入范围：暂未确定
	double SampleRate;									// 采样率：该采样率= DAC原始采样率/插值倍数
	short Amplitude;									// 信号幅度；0-8191
	int resetflag;										// 复位状态，重新配置后第一运行应设置为1，之后需要置0
	AnalogSignalType_TypeDef AnalogSignalType;			// 模拟信号类型
	AMModType_TypeDef AMModType;						// AM调制类型
	AM_CarrierSuppression_Typedef AMCarrierSuppression; // AM调制载波抑制
}SIG_AM_Profile_TypeDef;

typedef struct
{
	double PulseRepetitionFrequency; // 脉冲重复频率（PRF）
	double SampleRate;				 // 采样率：该采样率= DAC原始采样率/插值倍数
	double PulseDepth;				 // 脉冲的调制深度，即通断下的功率比,单位为dB;且必须为正数
	double PulseDuty;				 // 脉冲的占空比：0-1;
	short Amplitude;				 // 信号幅度；0-8191
	int resetflag;					 // 复位状态，重新配置后第一运行应设置为1，之后需要置0
}SIG_Pulse_Profile_TypeDef;

/*---------------------------------------------------
DSP辅助函数结构体(DSP)
-----------------------------------------------------*/

/*内部时钟校准源的配置结构体*/
typedef enum
{
	CalibrateByExternal = 0x00, // 通过Ext触发进行Clock校准
	CalibrateByGNSS1PPS = 0x01  // 通过GNSS-1PPS进行Clock校准
}ClkCalibrationSource_TypeDef;

/*快速傅里叶变换模式的配置结构体(配置)*/
typedef struct
{
	uint32_t FFTSize;				     // FFT分析点数
	uint32_t SamplePts;				     // 有效采样点数
	Window_TypeDef WindowType;		     // 窗型

	TraceDetector_TypeDef TraceDetector; // 视频检波方式
	uint32_t DetectionRatio;		     // 迹线检波比
	float Intercept;				     // 输出频谱截取，例如 Intercept = 0.8 则表示输出 80%的频谱结果

	bool Calibration;				     // 是否进行校准
}DSP_FFT_TypeDef;

/*需要生成滤波器系数的各项参数(配置)*/
typedef struct
{
	int n;            // 滤波器抽头数（ n > 0 ）
	float fc;         // 截止频率 ( 截止频率/采样率  0 < fc < 0.5 )
	float As;         // 阻带衰减（ As > 0 , [dB]）
	float mu;         // 分数采样偏移量（ -0.5 < mu < 0.5 ）
	uint32_t Samples; // 采样点数（Samples > 0）

}Filter_TypeDef;

/*需要生成正弦波的各项参数(配置)*/
typedef struct
{
	double Frequency;  // 正弦波的频率
	float Amplitude;   // 正弦波的幅度
	float Phase;       // 正弦波的相位
	double SampleRate; // 正弦波的采样率
	uint32_t Samples;  // 正弦波的采样点数
}Sin_TypeDef;

/*数字下变频模式的配置结构体(配置)*/
typedef struct
{
	double DDCOffsetFrequency; // 复混频的频率偏移值
	double SampleRate;         // 复混频的采样率
	float DecimateFactor;      // 重采样抽取倍数,范围:1 ~ 2^16
	uint64_t SamplePoints;     // 采样点数
}DSP_DDC_TypeDef;

/*IP3测试结构体*/
typedef struct
{
	double	LowToneFreq;       // 低音信号频率，单位随数据源
	double	HighToneFreq;      // 高音信号频率，单位随数据源
	double	LowIM3PFreq;       // 低频交调频率，单位随数据源
	double	HighIM3PFreq;      // 高频交调频率，单位随数据源

	float	LowTonePower_dBm;  // 低音功率, dBm
	float	HighTonePower_dBm; // 高音功率, dBm
	float	TonePowerDiff_dB;  // 低音功率 - 高音功率
	float	LowIM3P_dBc;	   // LowIM3P_dBc = max(LowTonePower_dBm, HighTonePower_dBm) - LowTonePower_dBm, 低频交调产物相对于最强主信号的强度
	float	HighIM3P_dBc;	   // HighIM3P_dBc = max(LowTonePower_dBm, HighTonePower_dBm) - HighTonePower_dBm, 高频交调强产物相对于最强主信号的度

	float	IP3_dBm;		   // 第三阶互调点，dBm

}TraceAnalysisResult_IP3_TypeDef;

/*IP2测试结构体*/
typedef struct
{
	double	LowToneFreq;	   // 低音信号频率，单位随数据源
	double	HighToneFreq;	   // 高音信号频率，单位随数据源
	double	IM2PFreq;	       // 低频交调频率，单位随数据源

	float	LowTonePower_dBm;  // 低音功率, dBm
	float	HighTonePower_dBm; // 高音功率, dBm
	float	TonePowerDiff_dB;  // 低音功率 - 高音功率
	float	IM2P_dBc;		   // IM2P_dBc = max(LowTonePower_dBm, HighTonePower_dBm) - IM2P_dBm, 低频交调产物相对于最强主信号的强度

	float	IP2_dBm;		   // 第二阶互调点，dBm

}TraceAnalysisResult_IP2_TypeDef;

/*AudioAnalysis结构体*/
typedef struct
{
	double AudioVoltage;   // 音频电压(V)
	double AudioFrequency; // 音频频率(Hz)
	double SINDA;          // 信纳德(dB)
	double THD;            // 总谐波失真(%)

}DSP_AudioAnalysis_TypeDef;

/*XdB带宽信息结构体(返回)*/
typedef struct
{
	double XdBBandWidth_Hz; // XdB带宽(Hz)
	double CenterFreq_Hz;   // XdB带宽的中心频率(Hz)
	double StartFreq_Hz;    // XdB带宽的起始频率(Hz)
	double StopFreq_Hz;     // XdB带宽的终止频率(Hz)
	float StartPower_dBm;   // XdB带宽的起始频率对应的功率(dBm)
	float StopPower_dBm;    // XdB带宽的终止频率对应的功率(dBm)
	uint32_t PeakIndex;     // XdB带宽内的峰值索引
	double PeakFreq_Hz;     // XdB带宽内的峰值频率(Hz）
	float PeakPower_dBm;    // XdB带宽内的峰值功率(dBm)

}TraceAnalysisResult_XdB_TypeDef;

/*占用带宽信息结构体(返回)*/
typedef struct
{
	double OccupiedBandWidth; // 占用带宽(Hz)
	double CenterFreq_Hz;     // 占用带宽的中心频率(Hz)
	double StartFreq_Hz;      // 占用带宽的起始频率(Hz)
	double StopFreq_Hz;       // 占用带宽的终止频率(Hz)
	float StartPower_dBm;     // 占用带宽的起始频率对应的功率(dBm)
	float StopPower_dBm;      // 占用带宽的终止频率对应的功率(dBm)
	float StartRatio;         // 占用带宽的起始频率对应的功率占比
	float StopRatio;          // 占用带宽的终止频率对应的功率占比
	uint32_t PeakIndex;       // 占用带宽内的峰值索引
	double PeakFreq_Hz;       // 占用带宽内的峰值频率(Hz）
	float PeakPower_dBm;      // 占用带宽内的峰值功率(dBm)

}TraceAnalysisResult_OBW_TypeDef;

/*邻道功率比频率信息结构体(输入)*/
typedef struct
{
	double RBW;                 // 分辨率带宽(Hz)
	double MainChCenterFreq_Hz; // 主信道中心频率(Hz)
	double MainChBW_Hz;         // 主信道带宽(Hz)
	double AdjChSpace_Hz;       // 邻道间隔，主信道中心频率与邻道中心频率的差值(Hz)
	uint32_t AdjChPair;         // 邻道对。1代表左右2个邻道，2代表左右4个邻道。

}DSP_ACPRFreqInfo_TypeDef;

/*邻道功率比信息结构体(返回)*/
typedef struct
{
	float MainChPower_dBm;       // 主信道功率(dBm)
	uint32_t MainChPeakIndex;    // 主信道的峰值索引
	double MainChPeakFreq_Hz;    // 主信道的峰值频率(Hz）
	float MainChPeakPower_dBm;   // 主信道峰值功率(dBm)

	double L_AdjChCenterFreq_Hz; // 左邻道中心频率(Hz)
	double L_AdjChBW_Hz;         // 左邻道带宽(Hz)
	float L_AdjChPower_dBm;      // 左邻道功率(dBm)
	float L_AdjChPowerRatio;     // 左邻道功率比（左邻道功率/主信道功率）
	float L_AdjChPowerDiff_dBc;  // 左邻道功率差（左邻道功率-主信道功率 dBc）
	float L_AdjChPeakIndex;      // 左邻道的峰值索引
	double L_AdjChPeakFreq_Hz;   // 左信道的峰值频率(Hz）
	float L_AdjChPeakPower_dBm;  // 左邻道的值功率(dBm)

	double R_AdjChCenterFreq_Hz; // 右邻道中心频率(Hz)
	double R_AdjChBW_Hz;         // 右邻道带宽(Hz)
	float R_AdjChPower_dBm;      // 右邻道功率(dBm)
	float R_AdjChPowerRatio;     // 右邻道功率比（右邻道功率/主信道功率）
	float R_AdjChPowerDiff_dBc;  // 右邻道功率差（右邻道功率-主信道功率 dBc）
	float R_AdjChPeakIndex;      // 右邻道的峰值索引
	double R_AdjChPeakFreq_Hz;   // 右信道的峰值频率(Hz）
	float R_AdjChPeakPower_dBm;  // 右邻道的值功率(dBm)

}TraceAnalysisResult_ACPR_TypeDef;

/*信道功率结构体(返回)*/
typedef struct
{
	float ChannelPower_dBm;     // 信道功率(dBm)
	float PowerDensity;         // 信道功率密度(dBm/Hz)
	float ChannelPeakIndex;     // 信道内的峰值索引
	double ChannelPeakFreq_Hz;  // 信道内的峰值频率(Hz）
	float ChannelPeakPower_dBm; // 信道内的峰值功率(dBm）

}DSP_ChannelPowerInfo_TypeDef;

/*迹线平滑模式结构体*/
typedef enum
{
	MovingAvrage = 0x00, // 滑动平均
	MedianFilter = 0x01, // 中值滤波
	PolynomialFit = 0x02 // 多项式拟合
}SmoothMethod_TypeDef;

typedef enum
{
	SWPPhaseNoiseMeas = 0x00,   //相位噪声测量
	SWPNoiseMeas = 0x01,		//显示平均噪声电平测量
	SWPChannelPowerMeas = 0x02, //信道功率测量
	SWPOBWMeas = 0x03,          //占用带宽测量
	SWPACPRMeas = 0x04,         //邻道功率比测量
	SWPIM3Meas = 0x05           //IP3/IM3测量
}SWPApplication_TypeDef;

//typedef struct
//{
//	/*频率：FREQ*/
//	SWP_FreqAssignment_TypeDef FreqAssignment; // 频率指定方式
//	double StartFreq_Hz; 					   // 起始频率
//	double StopFreq_Hz;	 					   // 终止频率
//	double CenterFreq_Hz;                      // 中心频率
//	double Span_Hz;                            // 频率扫宽
//
//	/*幅度：AMP*/
//	double RefLevel_dBm;					   // 参考电平
//
//	/*带宽：BW*/
//	double RBW_Hz;		 					   // 分辨率带宽
//	VBWMode_TypeDef VBWMode;				   // VBW更新方式
//
//	/*扫描：SWEEP*/
//	Window_TypeDef Window; 					   // FFT分析所使用的窗型
//	SpurRejection_TypeDef SpurRejection; 	   // 杂散抑制
//	TraceDetector_TypeDef TraceDetector; 	   // 迹线检波器
//	unsigned int TracePoints;  					   // 迹线点数
//}PTest_SWPProfile_TypeDef;

// SEM模版中的一段线
typedef struct
{
	bool mState;        // 关闭=0； 使能 =1；
	double mStartFreq;  // 开始频率
	double mLimitStart; // 开始门限
	double mStopFreq;   // 截止频率
	double mLimitStop;  // 截止门限
	int mMode;          // 绝对 = 0; 相对 = 1
	int mPriority;      // 要求 = 0； 建议 = 1;
}DSP_SEMSegmentProfile_TypeDef;

// SEM模版中最多有16段线
typedef struct
{
	int mRefSetType;                          // 参考设置方式：0 峰值 1 手动
	float mManualRefLevel;                    // 手动设置参考
	DSP_SEMSegmentProfile_TypeDef mSegments[16]; // 每一段的配置
}DSP_SEMProfile_TypeDef;

// SEM一段线的测量结果
typedef struct
{
	double mLowerFreq;     // 低端频率
	float mLowerLevel;     // 低端电平
	float mLowerMargin;    // 低端余量
	bool mLowerPassOrFail; // 低端通过、失败。 0：通过；1：失败
	double mUpperFreq;     // 高端频率
	float mUpperLevel;     // 高端电平
	float mUpperMargin;    // 高端余量
	bool mUpperPassOrFail; // 高端通过、失败
}DSP_SEMSegmentResult_TypeDef;

// SEM所有线段的测量结果
typedef struct
{
	DSP_SEMSegmentResult_TypeDef mSegmentResults[16]; // 测量的结果
	DSP_SEMProfile_TypeDef mProfile;                  // 测量时的配置
	float mRefLevel;                               // 测量时采用的参考
}DSP_SEMResult_TypeDef;

/* -------------------------------------------以下为设备控制(dev相关接口)------------------------------------------------------ */

//HTRA_API int PTest_SWP_Configuration(void** Device, const PTest_SWPProfile_TypeDef* ProfileIn, PTest_SWPProfile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);

HTRA_API int Get_APIVersion(void);

/* dev接口，打开设备，在对设备操作前必须调用此函数以获取设备资源 */
HTRA_API int Device_Open(void** Device, int DeviceNum, const BootProfile_TypeDef* BootProfile, BootInfo_TypeDef* BootInfo);

/* dev接口，关闭设备，在对设备操作完成后必须调用此函数以释放设备资源 */
HTRA_API int Device_Close(void** Device);

/* dev接口，使数据接口（如USB等）重新进行枚举过程。本函数成功后，需要调用 Device_Close() 函数，然后再调用 Device_Open（），以恢复和设备的连接。*/
HTRA_API int Device_ReEnumerate(void **Device);

/* dev接口，设置系统电源状态*/
HTRA_API int Device_SetSysPowerState(void **Device, SysPowerState_TypeDef SysPowerState);

/* dev接口，获取设备信息，包括设备序列号及软硬件版本等相关信息，非实时方式，不打断数据获取，但信息只在获取数据包后更新 */
HTRA_API int Device_QueryDeviceInfo(void** Device, DeviceInfo_TypeDef* DeviceInfo);

/* dev接口，获取设备信息，包括设备序列号及软硬件版本号等相关信息，实时获取，短时间内会占用数据通道 */
HTRA_API int Device_QueryDeviceInfo_Realtime(void** Device, DeviceInfo_TypeDef* DeviceInfo);

/* dev接口，获取设备状态，包括设备温度、硬件工作状态、地理时间信息（需要选件支持）等，非实时方式，不打断数据获取，但信息只在获取数据包后更新 */
HTRA_API int Device_QueryDeviceState(void** Device, DeviceState_TypeDef* DeviceState);

/* dev接口，获取设备状态，包括设备温度、硬件工作状态、地理时间信息（需要选件支持）等，实时获取，短时间内会占用数据通道 */
HTRA_API int Device_QueryDeviceState_Realtime(void** Device, DeviceState_TypeDef* DeviceState);

/* dev接口，通过已知时钟周期的外触发信号校准参考时钟频率(高级应用) ExtTriggerPeriod_s：表示触发信号周期，ExtTriggerCount：表示触发次数，RewriteRFCal：表示是否将校准的参考时钟频率重写入校准文件以作为后续启动的默认值（不可逆），RefCLKFreq_Hz：表示校准后的参考时钟频率*/
HTRA_API int Device_CalibrateRefClock(void** Device, ClkCalibrationSource_TypeDef ClkCalibrationSource, const double TriggerPeriod_s, const uint64_t TriggerCount, const bool RewriteRFCal, double* RefCLKFreq_Hz);

/* dev接口，更新设备固件(高级应用)*/
HTRA_API int Device_UpdateFirmware(int DeviceNum, const Firmware_TypeDef Firmware, const char* path);

/* dev接口，控制设备风扇工作模式*/
HTRA_API int Device_SetFanState(void** Device, const FanState_TypeDef FanState, const float ThreshouldTemperature);

/* dev接口，设置频率响应补偿*/
HTRA_API int Devcie_SetFreqResponseCompensation(void** Device, uint8_t State, const double *Frequency_Hz, const float *CorrectVal_dB, uint8_t Points);

/* dev接口，设置GNSS天线状态（需要选件支持）*/
HTRA_API int Device_SetGNSSAntennaState(void** Device, const GNSSAntennaState_TypeDef GNSSAntennaState);

/* dev接口，获取GNSS天线状态（需要选件支持），非实时方式，不打断数据获取，但信息只在获取数据包后更新*/
HTRA_API int Device_GetGNSSAntennaState(void** Device, GNSSAntennaState_TypeDef* GNSSAntennaState);

/* dev接口，获取GNSS天线状态（需要选件支持），实时获取，短时间内会占用数据通道*/
HTRA_API int Device_GetGNSSAntennaState_Realtime(void** Device, GNSSAntennaState_TypeDef* GNSSAntennaState);

/* dev接口，解析GNSS时间日期信息（需要选件支持）*/
HTRA_API int Device_AnysisGNSSTime(double ABSTimestamp, int16_t* hour, int16_t* minute, int16_t* second, int16_t* Year, int16_t* month, int16_t* day);

/* dev接口，获取GNSS海拔（需要选件支持），非实时方式，不打断数据获取，但信息只在获取数据包后更新*/
HTRA_API int Device_GetGNSSAltitude(void** Device, int16_t* Altitude);

/* dev接口，获取GNSS海拔（需要选件支持），实时获取，短时间内会占用数据通道*/
HTRA_API int Device_GetGNSSAltitude_Realtime(void** Device, int16_t* Altitude);

/*dev接口，设置GNSS中DOCXO工作状态（需要选件支持） */
HTRA_API int Device_SetDOCXOWorkMode(void** Device, const DOCXOWorkMode_TypeDef DOCXOWorkMode);

/* dev接口，获取GNSS中DOCXO工作状态（需要选件支持），实时获取，短时间内会占用数据通道*/
HTRA_API int Device_GetDOCXOWorkMode_Realtime(void** Device, DOCXOWorkMode_TypeDef* DOCXOWorkMode);

/* dev接口，获取GNSS中DOCXO工作状态（需要选件支持），非实时方式，不打断数据获取，但信息只在获取数据包后更新*/
HTRA_API int Device_GetDOCXOWorkMode(void** Device, DOCXOWorkMode_TypeDef* DOCXOWorkMode);

/* dev接口，获取GNSS设备状态（需要选件支持），非实时方式，不打断数据获取，但信息只在获取数据包后更新*/
HTRA_API int Device_GetGNSSInfo(void** Device, GNSSInfo_TypeDef* GNSSInfo);

/* dev接口，获取GNSS设备状态（需要选件支持），实时获取，短时间内会占用数据通道*/
HTRA_API int Device_GetGNSSInfo_Realtime(void** Device, GNSSInfo_TypeDef* GNSSInfo);

/* dev接口，获取网络中所有网络设备的IP地址和子网掩码等信息*/
HTRA_API int Device_GetNetworkDeviceList(uint8_t* DeviceCount, NetworkDeviceInfo_TypeDef DeviceInfo[64], uint8_t LocalIP[4], uint8_t LocalMask[4]);

/* dev接口，通过设备UID，配置网络设备的IP地址和子网掩码*/
HTRA_API int Device_SetNetworkDeviceIP(const uint64_t DeviceUID, const uint8_t IPAddress[4], const uint8_t SubnetMask[4]);

/* dev接口，通过设备IP地址，配置网络设备的IP地址和子网掩码*/
HTRA_API int Device_SetNetworkDeviceIP_PM1(const uint8_t DeviceIP[4], const uint8_t IPAddress[4], const uint8_t SubnetMask[4]);

/* dev接口，获取完整UID信息，非实时方式，不打断数据获取，但信息只在获取数据包后更新*/
HTRA_API int Device_GetFullUID(void** Device, uint64_t* UID_L64, uint32_t* UID_H32);

/* dev接口，获取GNSS信噪比（需要选件支持），非实时方式，不打断数据获取，但信息只在获取数据包后更新*/
HTRA_API int Device_GetGNSS_SatDate(void** Device, GNSS_SatDate_TypeDef * GNSS_SatDate);

/* dev接口，获取GNSS信噪比（需要选件支持），实时获取，短时间内会占用数据通道*/
HTRA_API int Device_GetGNSS_SatDate_Realtime(void** Device, GNSS_SatDate_TypeDef * GNSS_SatDate);

/* dev接口，更新GNSS固件(高级应用)*/
HTRA_API int Device_GNSSFirmwareUpdate(int DeviceNum, const char* path);

/* dev接口，获取硬件状态信息*/
HTRA_API int Device_GetHardwareState(void** Device, HardWareState_TypeDef* HardWareState);

/* dev接口，通过*/
HTRA_API int Device_QueryDeviceInfoWithBus(int DeviceNum, const BootProfile_TypeDef* BootProfile, BootInfo_TypeDef* BootInfo);

/* dev接口，配置频率扫描参数*/
HTRA_API int Device_SetFreqScan(void** Device, double StartFreq_Hz, double StopFreq_Hz, uint16_t SweepPts);

/* -------------------------------------------以下为扫描模式(SWEEP相关接口)------------------------------------------------------ */

/* SWP模式，配置SWP_Profile 为默认值 */
HTRA_API int SWP_ProfileDeInit(void** Device, SWP_Profile_TypeDef* UserProfile_O);
HTRA_API int SWP_EZProfileDeInit(void** Device, SWP_EZProfile_TypeDef* UserProfile_O);

/* SWP模式，配置SWP模式相关参数 */
HTRA_API int SWP_Configuration(void** Device, const SWP_Profile_TypeDef* ProfileIn, SWP_Profile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);
HTRA_API int SWP_EZConfiguration(void** Device, const SWP_EZProfile_TypeDef* ProfileIn, SWP_EZProfile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);

/* SWP模式，获取SWP模式下部分扫描的频谱数据 */
HTRA_API int SWP_GetPartialSweep(void** Device, double Freq_Hz[], float PowerSpec_dBm[], int* HopIndex, int* FrameIndex, MeasAuxInfo_TypeDef* MeasAuxInfo);
HTRA_API int SWP_GetPartialSweep_PM1(void** Device, SWPTrace_TypeDef* PartialTrace);

/* SWP模式，获取SWP模式下完整刷新的频谱数据 */
HTRA_API int SWP_GetFullSweep(void** Device, double Freq_Hz[], float PowerSpec_dBm[], MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int SWP_GetFullSweep_PM1(void** Device, SWPTrace_TypeDef* PartialTrace); （暂不提供）

/* SWP模式，获取SWP模式下部分刷新但包含非刷新部分数据的频谱数据(高级应用) */
HTRA_API int SWP_GetPartialUpdatedFullSweep(void** Device, double Freq_Hz[], float PowerSpec_dBm[], int* HopIndex, int* FrameIndex, MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int SWP_GetPartialUpdatedFullSweep_RM1(void** Device, SWPTrace_TypeDef* PartialTrace);（暂不提供）

/* SWP模式，获取SWP模式下校准数据曲线(高级应用) */
HTRA_API int SWP_GetAmplitudeCalibrationTrace(void** Device, double Frequency_Hz[], float Compensate_dB[], float* CalTemperature_C);

/* SWP模式，重置SWP模式下MaxHold和MinHold的频谱数据(高级应用) */
HTRA_API void SWP_ResetTraceHold(void** Device);

/* SWP模式，根据应用目标给出推荐设备配置 */
HTRA_API int SWP_AutoSet(void** Device, SWPApplication_TypeDef Application, const SWP_Profile_TypeDef* ProfileIn, SWP_Profile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo, uint8_t ifDoConfig);

/* -------------------------------------------以下为时域流模式(IQS相关接口)------------------------------------------------------ */

/* IQS模式，配置IQS_Profile 为默认值 */
HTRA_API int IQS_ProfileDeInit(void** Device, IQS_Profile_TypeDef* UserProfile_O);
HTRA_API int IQS_EZProfileDeInit(void** Device, IQS_EZProfile_TypeDef* UserProfile_O);

/* IQS模式，配置IQS模式相关参数 */
HTRA_API int IQS_Configuration(void** Device, const IQS_Profile_TypeDef* ProfileIn, IQS_Profile_TypeDef* ProfileOut, IQS_StreamInfo_TypeDef* StreamInfo);
HTRA_API int IQS_EZConfiguration(void** Device, const IQS_EZProfile_TypeDef* ProfileIn, IQS_EZProfile_TypeDef* ProfileOut, IQS_StreamInfo_TypeDef* StreamInfo);

/* IQS模式，发起总线触发 */
HTRA_API int IQS_BusTriggerStart(void** Device);

/* IQS模式，终止总线触发 */
HTRA_API int IQS_BusTriggerStop(void** Device);

/* IQS模式，等待多机同步触发信号*/
HTRA_API int IQS_MultiDevice_WaitExternalSync(void** Device, const IQS_Profile_TypeDef* ProfileIn);

/* IQS模式，使能多机同步运行*/
HTRA_API int IQS_MultiDevice_Run(void **Device);

/* IQS模式，获取IQS模式下交织的IQ数据流，并得到整型至绝对幅度（V单位）的比例因子 及 触发相关的信息 */
HTRA_API int IQS_GetIQStream(void** Device, void** AlternIQStream, float* ScaleToV, IQS_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);	// 函数多态-形态0：非封装数据输出
HTRA_API int IQS_GetIQStream_PM1(void **Device, IQStream_TypeDef* IQStream); // 函数多态-形态1：封装数据输出
HTRA_API int IQS_GetIQStream_PM2(void **Device, IQStream_TypeDef* IQStream, MeasAuxInfo_TypeDef* MeasAuxInfo); // 函数多态-形态2：封装数据输出
HTRA_API int IQS_GetIQStream_Data(void** Device, int16_t IQ_data[]);

/* IQS模式，发起定时器-外触发单次同步*/
HTRA_API int IQS_SyncTimer(void** Device);

/* -------------------------------------------以下为检波模式(DET相关接口)------------------------------------------------------ */

/* DET模式，配置DET_Profile 为默认值 */
HTRA_API int DET_ProfileDeInit(void** Device, DET_Profile_TypeDef* UserProfile_O);
HTRA_API int DET_EZProfileDeInit(void** Device, DET_EZProfile_TypeDef* UserProfile_O);

/* DET模式，配置DET模式相关参数 */
HTRA_API int DET_Configuration(void** Device, const DET_Profile_TypeDef* ProfileIn, DET_Profile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);
HTRA_API int DET_EZConfiguration(void** Device, const DET_EZProfile_TypeDef* ProfileIn, DET_EZProfile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);

/* ZeroSpan模式，配置ZeroSpan模式相关参数 */
HTRA_API int ZSP_ProfileDeInit(void** Device, ZSP_Profile_TypeDef* UserProfile_O);
HTRA_API int ZSP_Configuration(void** Device, const ZSP_Profile_TypeDef* ProfileIn, ZSP_Profile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);

/* DET模式，发起总线触发 */
HTRA_API int DET_BusTriggerStart(void** Device);

/* DET模式，终止总线触发 */
HTRA_API int DET_BusTriggerStop(void** Device);

/* DET模式，获取DET模式下的检波数据，并得到整型至绝对幅度（V单位）的比例因子 及 触发相关的信息，NormalizedPowerStream为 I方+Q方开根号 */
HTRA_API int DET_GetPowerStream(void** Device, float NormalizedPowerStream[], float* ScaleToV, DET_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int DET_GetPowerStream_PM1(void** Device, DETStream_TypeDef* DETStream);（暂不实现）

/* DET模式，发起定时器-外触发单次同步*/
HTRA_API int DET_SyncTimer(void** Device);

/* -------------------------------------------以下为实时频谱分析模式(RTA相关接口)------------------------------------------------------ */

/* RTA模式，配置RTA_Profile 为默认值 */
HTRA_API int RTA_ProfileDeInit(void** Device, RTA_Profile_TypeDef* UserProfile_O);
HTRA_API int RTA_EZProfileDeInit(void** Device, RTA_EZProfile_TypeDef* UserProfile_O);

/* RTA模式，配置RTA模式相关参数 */
HTRA_API int RTA_Configuration(void** Device, const RTA_Profile_TypeDef* ProfileIn, RTA_Profile_TypeDef* ProfileOut, RTA_FrameInfo_TypeDef* FrameInfo);
HTRA_API int RTA_EZConfiguration(void** Device, const RTA_EZProfile_TypeDef* ProfileIn, RTA_EZProfile_TypeDef* ProfileOut, RTA_FrameInfo_TypeDef* FrameInfo);

/* RTA模式，发起总线触发 */
HTRA_API int RTA_BusTriggerStart(void **Device);

/* RTA模式，终止总线触发 */
HTRA_API int RTA_BusTriggerStop(void **Device);

/* RTA模式，获取RTA模式下的实时频谱数据 及 触发相关的信息 */
HTRA_API int RTA_GetRealTimeSpectrum(void** Device, uint8_t SpectrumStream[], uint16_t SpectrumBitmap[], RTA_PlotInfo_TypeDef* PlotInfo, RTA_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);

/*RTA模式，获取RTA模式下的实时频谱（无概率密度图）及触发相关信息*/
HTRA_API int RTA_GetRealTimeSpectrum_Raw(void **Device, uint8_t SpectrumStream[], RTA_PlotInfo_TypeDef* PlotInfo, RTA_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int RTA_GetRealTimeSpectrum_PM1(void** Device, RTAStream_TypeDef* RTAStream);（暂不实现）

/*RTA模式，采用频谱模板触发时下发触发门限。输入关注点个数，Freq数组和Power数组一一对应，输入关注点即可，点与点间的功率将自动补全*/
HTRA_API int RTA_SpectrumMaskTrigger_SetMask(void** Device, uint32_t NodeNum, double FreqHz_In[], float PowerdBm_In[], double FreqHz_Out[], float PowerdBm_Out[]);

/*RTA模式，发起频谱模板触发*/
HTRA_API int RTA_SpectrumMaskTrigger_Run(void **Device);

/*RTA模式，停止频谱模板触发*/
HTRA_API int RTA_SpectrumMaskTrigger_Stop(void **Device);

/* RTA模式，发起定时器-外触发单次同步*/
HTRA_API int RTA_SyncTimer(void** Device);

/* -------------------------------------------以下为辅助信号源控制(ASG相关接口)------------------------------------------------------ */

/* 模拟信号源，配置Profile 为默认值 */
HTRA_API int ASG_ProfileDeInit(void** Device, ASG_Profile_TypeDef* Profile);

/* 模拟信号源，工作状态配置 */
HTRA_API int ASG_Configuration(void **Device, ASG_Profile_TypeDef *ProfileIn, ASG_Profile_TypeDef *ProfileOut,ASG_Info_TypeDef *ASG_Info);

/* -------------------------------------------以下为模拟解调(Analog相关接口)------------------------------------------------------ */

/* 模拟解调Analog Demod，创建 */
HTRA_API void ADM_Open(void** ADM);

/* 模拟解调Analog Demod，释放 */
HTRA_API void ADM_Close(void** ADM);

/* 模拟解调Analog Demod，AM解调，仅返回解调后的波形 */
HTRA_API int ADM_AMDemod(void** ADM, const void* AlternIQStream, DataFormat_TypeDef DataFormat, uint64_t SamplePoints, double IQSampleRate, float DemodWaveform[]);

/* 模拟解调Analog Demod，FM解调，仅返回解调后的波形 */
HTRA_API int ADM_FMDemod(void** ADM, const void* AlternIQStream, DataFormat_TypeDef DataFormat, uint64_t SamplePoints, double IQSampleRate, bool reset, float DemodWaveform[]);

/* 模拟解调Analog Demod，AM解调，返回解调后的波形和调制参数 */
HTRA_API int ADM_AMDemod_PM1(void** ADM, const void* AlternIQStream, DataFormat_TypeDef DataFormat, uint64_t SamplePoints, double IQSampleRate, float IQS_ScaleToV, AMDemodParam_TypeDef* AMDemodParam);

/* 模拟解调Analog Demod，FM解调，返回解调后的波形和调制参数 */
HTRA_API int ADM_FMDemod_PM1(void** ADM, const void* AlternIQStream, DataFormat_TypeDef DataFormat, uint64_t SamplePoints, double IQSampleRate, float IQS_ScaleToV, bool reset, FMDemodParam_TypeDef* FMDemodParam);


/* -------------------------------------------以下为数字信号处理辅助函数(DSP相关辅助函数)------------------------------------------------------ */

/* DSP辅助函数，启动DSP功能 */
HTRA_API int DSP_Open(void** DSP);

/* DSP辅助函数，初始化FFT参数 */
HTRA_API int DSP_FFT_DeInit(DSP_FFT_TypeDef* IQToSpectrum);

/* DSP辅助函数，配置FFT参数 */
HTRA_API int DSP_FFT_Configuration(void** DSP, const DSP_FFT_TypeDef* ProfileIn, DSP_FFT_TypeDef* ProfileOut, uint32_t* TracePoints, double* RBWRatio);

/* DSP辅助函数，将IQ数据转换成频谱数据 */
HTRA_API int DSP_FFT_IQSToSpectrum(void** DSP, const IQStream_TypeDef* IQStream, double Freq_Hz[], float PowerSpec_dBm[]);

/* DSP辅助函数，初始化LPF参数 */
HTRA_API void DSP_LPF_DeInit(Filter_TypeDef* LPF_ProfileIn);

/* DSP辅助函数，配置低通滤波器 */
HTRA_API void DSP_LPF_Configuration(void** DSP, const Filter_TypeDef* LPF_ProfileIn, Filter_TypeDef* LPF_ProfileOut);

/* DSP辅助函数，重置LPF中的缓存 */
HTRA_API void DSP_LPF_Reset(void** DSP);

/* DSP辅助函数，低通滤波_实信号 */
HTRA_API void DSP_LPF_Execute_Real(void** DSP, float Signal[], float LPF_Signal[]);

/* DSP辅助函数，低通滤波_复信号 */
HTRA_API void DSP_LPF_Execute_Complex(void** DSP, const IQStream_TypeDef* IQStreamIn, IQStream_TypeDef* IQStreamOut);

/* DSP辅助函数，初始化DDC配置参数 */
HTRA_API int DSP_DDC_DeInit(DSP_DDC_TypeDef* DDC_Profile);

/* DSP辅助函数，配置DDC */
HTRA_API int DSP_DDC_Configuration(void** DSP, const DSP_DDC_TypeDef* DDC_ProfileIn, DSP_DDC_TypeDef* DDC_ProfileOut);

/* DSP辅助函数，重置DDC中的缓存 */
HTRA_API void DSP_DDC_Reset(void** DSP);

/* DSP辅助函数，获取DDC的延时 */
HTRA_API void DSP_DDC_GetDelay(void** DSP, uint32_t* delay);

/* DSP辅助函数，执行DDC */
HTRA_API int DSP_DDC_Execute(void** DSP, const IQStream_TypeDef* IQStreamIn, IQStream_TypeDef* IQStreamOut);

/* DSP辅助函数，分析迹线的 IM3 参数 */
HTRA_API int DSP_TraceAnalysis_IM3(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, TraceAnalysisResult_IP3_TypeDef* IM3Result);

/* DSP辅助函数，分析迹线的 IM2 参数 */
HTRA_API int DSP_TraceAnalysis_IM2(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, TraceAnalysisResult_IP2_TypeDef* IM2Result);

/* DSP辅助函数，分析迹线的 相位噪声 参数 */
HTRA_API int DSP_TraceAnalysis_PhaseNoise(const double Freq_Hz[], const float PowerSpec_dBm[], const double OffsetFreqs[], const uint32_t TracePoints, const uint32_t OffsetFreqsToAnalysis, double CarrierFreqOut[], float PhaseNoiseOut_dBc[]);

/* DSP辅助函数，分析迹线的 信道功率*/
HTRA_API int DSP_TraceAnalysis_ChannelPower(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const double CenterFrequency, const double AnalysisSpan, const double RBW, DSP_ChannelPowerInfo_TypeDef* ChannelPowerResult);

/* DSP辅助函数，分析迹线的 XdB带宽*/
HTRA_API int DSP_TraceAnalysis_XdBBW(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const float XdB, TraceAnalysisResult_XdB_TypeDef* XdBResult);

/* DSP辅助函数，分析迹线的 占用带宽*/
HTRA_API int DSP_TraceAnalysis_OBW(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const float OccupiedRatio, TraceAnalysisResult_OBW_TypeDef* OBWResult);

/* DSP辅助函数，分析迹线的 邻道功率比*/
HTRA_API int DSP_TraceAnalysis_ACPR(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const DSP_ACPRFreqInfo_TypeDef ACPRFreqInfo, TraceAnalysisResult_ACPR_TypeDef* ACPRResult);

/* 音频分析函数，分析音频的 音频电压(V)、音频频率(Hz)、信纳德(dB)和总谐波失真(%) 参数 */
HTRA_API void DSP_AudioAnalysis(const double Audio[], const uint64_t SamplePoints, const double SampleRate, DSP_AudioAnalysis_TypeDef* AudioAnalysis);

/* DSP辅助函数，频谱截取 */
HTRA_API void DSP_InterceptSpectrum(const double StartFreq_Hz, const double StopFreq_Hz, const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t FullsweepTracePoints, double FrequencyOut[], float PowerSpecOut_dBm[], uint32_t* InterceptPoints);

/* DSP辅助函数，迹线平滑 */
HTRA_API int DSP_TraceSmooth(float* Trace, unsigned int TracePoints, SmoothMethod_TypeDef SmoothMethod, unsigned int IndexOffset, unsigned int WindowLength, unsigned int PolynomialOrder);

/* DSP辅助函数，关闭DSP功能 */
HTRA_API int  DSP_Close(void** DSP);

/* DSP辅助函数，频谱辐射模板分析 */
HTRA_API int DSP_SEMAnalysis(const DSP_SEMProfile_TypeDef* semProfile, const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const double CenterFreq, DSP_SEMResult_TypeDef* SEMResult);


/* -------------------------------------------以下函数为弃用、暂未开放和非必要调用函数------------------------------------------------------ */


/* -------------------------------------------以下为矢量网络分析仪控制(VNA相关接口，暂未开放)------------------------------------------------------ */

/* VNA，配置VNA_Profile 为默认值 */
HTRA_API int VNA_ProfileDeInit(void** Device, VNA_Profile_TypeDef* Profile);

/* 矢量网络分析模式，工作状态配置 */
HTRA_API int VNA_Configuration(void** Device, VNA_Profile_TypeDef* ProfileIn, VNA_Profile_TypeDef* ProfileOut, VNA_Info_TypeDef* VNA_Info);

/* 矢量网络分析模式，获取完整扫描结果 */
HTRA_API int VNA_GetFullSweep(void** Device, double Frequency[], double Result_dB[], double Result_Deg[]);

/* 矢量网络分析模式，获取部分扫描结果 */
HTRA_API int VNA_GetPartialSweep(void** Device, double* Frequency, double* Result_dB, double* Result_Deg, int* Index);

/* 矢量网络分析模式，获取单次的IQStream */
HTRA_API int VNA_GetIQStream(void** Device, double* I_Ref, double* Q_Ref, double* I_Meas, double* Q_Meas, int DataLength, int* PacketIndex);

/* 矢量网络分析，校准 */
HTRA_API int VNA_Calibration(void** Device, VNA_Calibration_TypeDef VNA_Calibration);

/* 矢量网络分析，校准完成 */
HTRA_API int VNA_Calibration_Done(void** Device);

/* 矢量网络分析仪，保存校准结果*/
HTRA_API int VNA_SaveCalibrationData(void** Device, const char* Calibrationfile);
HTRA_API int VNA_LoadCalibrationData(void** Device, const char* Calibratiofile);

/* -------------------------------------------以下为多Profile模式(MPS相关接口，暂未开放)------------------------------------------------------ */

/* MPS模式，配置SWP模式相关参数 */
HTRA_API int MPS_SWP_Configuration(void** Device, uint16_t ProfileNum, const SWP_Profile_TypeDef* ProfileIn, SWP_Profile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);

/* MPS模式，配置RTA模式相关参数 */
HTRA_API int MPS_RTA_Configuration(void** Device, uint16_t ProfileNum, const RTA_Profile_TypeDef* ProfileIn, RTA_Profile_TypeDef* ProfileOut, RTA_FrameInfo_TypeDef* FrameInfo);

/* MPS模式，配置IQS模式相关参数 */
HTRA_API int MPS_IQS_Configuration(void** Device, uint16_t ProfileNum, const IQS_Profile_TypeDef* ProfileIn, IQS_Profile_TypeDef* ProfileOut, IQS_StreamInfo_TypeDef* StreamInfo);

/* MPS模式，配置DET模式相关参数 */
HTRA_API int MPS_DET_Configuration(void** Device, uint16_t ProfileNum, const DET_Profile_TypeDef* ProfileIn, DET_Profile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);

/* MPS模式，开始MPS模式 */
HTRA_API int MPS_Start(void** Device, uint16_t AllProfileNum);

/* MPS模式，获取数据，并且标识是哪种模式和第几个Profile */
HTRA_API int MPS_GetData(void** Device, MPSData_TypeDef* MPSData, uint8_t* Analysis, uint16_t* ProfileNum);

/* MPS模式，简易版获取数据 */
HTRA_API int MPS_GetData_Simple(void** Device, MPSDataInfo_TypeDef* MPSDataInfo, double* Frequency, float* PowerSpec_dBm, int16_t* AlternIQStream, float* DETTrace, uint8_t* SpectrumTrace, uint16_t* SpectrumBitmap);

/* MPS模式，开辟内存 */
HTRA_API int MPS_DataBuffer_New(void** Device);

/* MPS模式，释放内存 */
HTRA_API int MPS_DataBuffer_Free(void** Device);


/* -------------------------------------------以下为弃用函数，不建议继续使用------------------------------------------------------ */

/* dev接口，控制NX系列设备修改IP地址(高级应用)*/
HTRA_API int Device_ChangeIPAddr(void** Device, const IPVersion_TypeDef ETH_IPVersion, const uint8_t ETH_IPAddress[], const uint8_t SubnetMask[]);

HTRA_API int Device_GetIPAddr(uint8_t ETH_IPAddress[10][4], uint8_t SubnetMask[10][4], uint8_t* IPAddressNum);

/* dev接口，控制NX系列设备修改IP地址(高级应用)*/
HTRA_API int Device_SetIPAddr(void** Device, const IPVersion_TypeDef ETH_IPVersion, const uint8_t ETH_IPAddress[], const uint8_t SubnetMask[]);

/* dev接口，控制NX系列设备重启网络，使修改的IP地址生效(高级应用)*/
HTRA_API int Device_RestartNetwork(void** Device);

/* dev接口，控制NX系列设备重启(高级应用)*/
HTRA_API int Device_Reboot(void** Device);

/* dev接口，更新设备MCU固件(高级应用)*/
HTRA_API int Device_MCUFirmwareUpdate(int DeviceNum, const char* path);

/* dev接口，更新设备FPGA固件(高级应用)*/
HTRA_API int Device_FPGAFirmwareUpdate(int DeviceNum, const char* path);

/* SWP模式，重置SWP模式下MaxHold频谱数据(高级应用) */
HTRA_API void SWP_MaxHoldReset(void** Device);

/* IQS模式，发起定时器-外触发单次同步*/
HTRA_API int IQS_SyncTimerByExtTrigger_Single(void** Device);

/* DET模式，发起定时器-外触发单次同步*/
HTRA_API int DET_SyncTimerByExtTrigger_Single(void** Device);

/* RTA模式，发起定时器-外触发单次同步*/
HTRA_API int RTA_SyncTimerByExtTrigger_Single(void** Device);

/* 模拟调制，创建 */
HTRA_API void ASD_Open(void** AnalogMod);

/* 模拟调制，释放 */
HTRA_API void ASD_Close(void** AnalogMod);

/*模拟解调模式，FM解调，仅返回解调后的音频波形和在波偏移*/
HTRA_API int ASD_Demodulate_FM(void** AnalogMod, const IQStream_TypeDef* IQStreamIn, bool reset, float result[], double* carrierOffsetHz);

/*模拟解调模式，AM解调，仅返回解调后的音频波形*/
HTRA_API int ASD_Demodulate_AM(const IQStream_TypeDef* IQStreamIn, float result[]);

typedef struct
{
	double ModRate;                      	// AM调制速率
	double ModDepth;                       	// AM调制深度 或调制指数
	double ModRate_Avg;                   	// 多次平均后的AM调制速率
	double ModDepth_Avg;                   	// 多次平均后的AM调制深度 或调制指数
}AM_DemodParam_TypeDef;

typedef struct
{
	double ModRate;                       // FM调制信号频率
	double ModDeviation;                  // FM调制频偏
	double CarrierFreqOffset;			  // 载波偏移
	double ModRate_Avg;                   // 多次平均后的FM调制信号频率
	double ModDeviation_Avg;              // 多次平均后的FM调制频偏
	double CarrierFreqOffset_Avg;		  // 多次平均后的载波偏移	
}FM_DemodParam_TypeDef;

/* 调制解调模式，FM解调，同时返回解调后的音频模型和调制参数*/
HTRA_API int ASD_FMDemodulation(void** AnalogMod, const IQStream_TypeDef* IQStreamIn, bool reset, float result[], FM_DemodParam_TypeDef* FM_DemodParam);

/* 调制解调模式，AM解调，同时返回解调后的音频模型和调制参数*/
HTRA_API int ASD_AMDemodulation(void** AnalogMod, const IQStream_TypeDef* IQStreamIn, bool reset, float result[], AM_DemodParam_TypeDef* AM_DemodParam);


/* -------------------------------------------以下为非必要调用函数------------------------------------------------------ */

/* dev接口，打印启动信息（BootInfo），包括UID、Model、HardwareVersion等 */
HTRA_API int Device_PrintBootInformation(BootInfo_TypeDef BootInfo);

/* dev接口，获取当前时间-h-m-s，并可选择是否打印 */
HTRA_API void Device_GetLocalTime(int* hour, int* min, int* sec, int ifprint);

/* SWP模式，获取SWP模式下指定频率处的调试信息(高级应用) */
HTRA_API int SWP_GetDebugInfo(void** Device, double Freq, SWP_DebugInfo_TypeDef* DebugInfo);

/* Cal模式，校准功能(CAL相关接口) */
HTRA_API int CAL_QDC_GetOptParam(const IQStream_TypeDef* IQStream, CAL_QDCOptParam_TypeDef* OptParam_O);

/* dev接口，查询设备启动信息 */
HTRA_API int Device_QueryBootInfo(int DeviceNum, const BootProfile_TypeDef* BootProfile, BootInfo_TypeDef* BootInfo, uint32_t* UID_H32);

/* dev接口， 检查设备可用性 */
HTRA_API int Device_CheckDeviceAvailability(int DeviceNum, const BootProfile_TypeDef* BootProfile);

/* -------------------------------------------以下为信号源控制(SIG和VSG相关接口)------------------------------------------------------ */

/* 信号源参数初始化 */
HTRA_API int SIG_ProfileDeInit(void** Device, SIG_Profile_TypeDef* Profile);
/* 信号源工作状态配置 */
HTRA_API int SIG_Configuration(void** Device, SIG_Profile_TypeDef* ProfileIn, SIG_Profile_TypeDef* ProfileOut, SIG_Info_TypeDef* SIG_Info);
/* 信号源更新包头包尾大小 */
HTRA_API int SIG_UpdateHeaderTailSize(void** Device, uint32_t* HeaderSize, uint32_t* TailSize);
/* 信号源发送数据 */
HTRA_API int SIG_SendIQStream(void** Device, unsigned char* DataBuffer, int DataSize);
/* 信号源总线触发开始 */
HTRA_API int SIG_BusTriggerStart(void** Device);
/* 信号源总线触发停止 */
HTRA_API int SIG_BusTriggerStop(void** Device);

/* 矢量信号源参数初始化 */
HTRA_API int VSG_ProfileDeInit(void** Device, VSG_Profile_TypeDef* Profile);
/* 矢量信号源工作状态配置 */
HTRA_API int VSG_Configuration(void** Device, VSG_Profile_TypeDef* ProfileIn, VSG_Profile_TypeDef* ProfileOut, VSG_Info_TypeDef* VSG_Info);
/* 矢量信号源配置下载用的起始地址 */
HTRA_API int VSG_Download_SetStartAddress(void** Device, uint32_t StartAddress);
/* 矢量信号源下在IQStream文件（单个）*/
HTRA_API int VSG_Download_IQFile(void** Device, const char* File, VSG_FileInfo_TypeDef* VSG_FileInfo);
/* 矢量信号源下在IQStream文件（多个）*/
HTRA_API int VSG_Download_IQFiles(void** Device, char File[8][128], VSG_FileInfo_TypeDef* VSG_FileInfo, uint32_t VSG_FileCount);
/* 矢量信号源更新包头包尾大小 */
HTRA_API int VSG_UpdateHeaderTailSize(void** Device, uint32_t* HeaderSize, uint32_t* TailSize);
/* 矢量信号源下载IQStream数据 */
HTRA_API int VSG_Download_IQData(void** Device, int16_t* I, int16_t* Q, uint32_t length, uint32_t* StartAddress, uint32_t* StopAddress);
/* 矢量信号源下载IQStream数据（交织）*/
HTRA_API int VSG_Download_IQData_Interleave(void** Device, int16_t* IQ, uint32_t length, uint32_t* StartAddress, uint32_t* StopAddress);
/* 矢量信号源总线触发开始 */
HTRA_API int VSG_BusTriggerStart(void** Device);
/* 矢量信号源总线触发停止 */
HTRA_API int VSG_PrivateParamDeInit(void** Device, VSG_PrivateParam_TypeDef* VSG_PrivateParam);
/* 矢量信号源更新私有参数 */
HTRA_API int VSG_UpdatePrivateParam(void** Device, VSG_PrivateParam_TypeDef* VSG_PrivateParam);

/* 模拟调制，产生待调制信号波形 */
HTRA_API int ASD_GenerateSignalWaveform(void** AnalogMod, AnalogSignalType_TypeDef AnalogSignalType, double SignalFrequency, double  SampleRate, int resetflag, float* m, int* SamplePoints);

/* 模拟调制，产生最终的FM调制波形 */
HTRA_API int ASD_GenerateFMWaveform(void** AnalogMod, SIG_FM_Profile_TypeDef* SIG_FM_Profile, float* m, short* I, short* Q, int* SamplePoints);
HTRA_API int ASD_GeneratePulseWaveform(void** AnalogMod, SIG_Pulse_Profile_TypeDef* SIG_Pulse_Profile, short* I, short* Q, int* SamplePoints);

/* 模拟调制，产生最终的AM调制波形 */
HTRA_API int ASD_GenerateAMWaveform(void** AnalogMod, SIG_AM_Profile_TypeDef* SIG_AM_Profile, float* m, short* I, short* Q, int* SamplePoints);

/* DSP辅助函数，执行中频校准 */
HTRA_API int DSP_IFCalibration(void** Device, double* Frequency, float* Spectrum, unsigned int SpectrumPoints);

/* DSP辅助函数，获取指定窗型的窗型因子 */
HTRA_API void DSP_GetWindowCoefficient(Window_TypeDef Window, int n, double Coefficient[]);

/* DSP辅助函数，执行加窗*/
HTRA_API void DSP_Windowing(DataFormat_TypeDef DataFormat, void* Data_I, const double* Window, uint32_t Samples, double* Data_O);

/* DSP辅助函数，获取白噪声 */
HTRA_API void DSP_GetWhiteGaussianNoise(uint32_t n, float* noise);

/* DSP辅助函数，卷积 */
HTRA_API void DSP_Convolution(float x[], float y[], int x_size, int y_size, float Output[]);

/* DSP辅助函数，抽取 */
HTRA_API void DSP_Decimate(float xn[], float yn[], uint64_t size, uint32_t DecimateFactor);

/* DSP辅助函数，生成正弦波 */
HTRA_API void DSP_GenerateSineWaveform(float yn[], const Sin_TypeDef* Sin_Profile);

/* DSP辅助函数，执行NCO生成正弦、余弦信号 */
HTRA_API void DSP_NCO_Execute(const Sin_TypeDef* NCO_Profile, float* sin, float* cos);

#ifdef __cplusplus
}
#endif

#endif //HTRA_API_H