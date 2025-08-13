/**
* @brief	This file defines API related variables and functions. Note edit using UTF-8 with BOM.
*/

#ifndef HTRA_API_H
#define HTRA_API_H

#define Major_Version 0 
#define Minor_Version 55
#define Increamental_Version 61

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

/* Description of critical user data types and critical user function

Users use apis through user data types and functions. The data types used in this APIs are as follows:

Device related data type
BootProfile_TypeDef: Configuration information required for device startup.
BootInfo_TypeDef: Feedback information about the device startup process.

The main function of the device
Device_Open(..) : Enable the device. The device must be enabled before being configured and invoked.
Device_Close(..) : Close the device so that the operating system can reclaim hardware and software resources.

Standard spectrum mode (SWP) related data types
SWP_Profile_TypeDef: indicates SWP mode configuration information.
SWP_TraceInfo_TypeDef:Configuration feedback information (trace information) for SWP mode.
Freq_Hz[], PowerSpec_dBm[], HopIndex, FrameIndex, MeasAuxInfo:data and additional information obtained by SWP in non-encapsulated form.
SWPSpectrum_TypeDef:data and additional information obtained by SWP in encapsulated form.

The main function of SWP
SWP_ProfileDeInit(..,SWP_Profile):initializes the SWP_Profile configuration information to the default value
SWP_Configuration(..,SWP_Profile, SWP_TraceInfo):Configure the device as SWP mode, set the configuration information with SWP_Profile and give feedback to SWP_TraceInfo
SWP_GetPartialSweep(.., Freq_Hz[], PowerSpec_dBm[], HopIndex, FrameIndex, MeasAuxInfo):Get SWP mode measurement data, partial traces, using standard-data data structures, containing the main information.
SWP_GetPartialSweep_PM1(.., SWPTrace_TypeDef):Get SWP mode measurement data, partial traces, using encapsulated data structures, containing all information

IQ stream mode (IQS) - related data type
IQS_Profile_TypeDef: indicates IQS mode configuration information.
IQS_StreamInfo_TypeDef: Configuration feedback information of IQS mode.
IQStream_TypeDef: Measurement data obtained by IQS mode.

The main function of IQ stream mode (IQS)
IQS_ProfileDeInit(..)
IQS_Configuration(..)
IQS_BusTriggerStart(..)
IQS_BusTriggerStop(..)
IQS_GetIQStream(..,AlternIQStream, ScaleToV, TriggerInfo, MeasAuxInfo)
IQS_GetIQStream_PM1(..,IQStream_TypeDef)

The main function of Detection mode (DET)
DET_Profile_TypeDef:indicates DET mode configuration information
DET_StreamInfo_TypeDef:
DETStream_TypeDef

The main function of detection mode (DET)
DET_ProfileDeInit
DET_Configuration
DET_BusTriggerStart
DET_BusTriggerStop
DET_GetPowerStream(.., PowerStream[], ScaleToV, TriggerInfo, MeasAuxInfo)

Real-time spectrum mode (RTA) - related data type
RTA_Profile_TypeDef:indicates RTA mode configuration information.
RTA_FrameInfo_TypeDef:Configuration feedback information of RTA mode.

SpectrumFrames[], SpectrumBitMap[], TriggerInfo, PlotInfo, MeasAuxInfo unencapsulated measurement data in RTA mode, matching the RTA_GetRealTimeSpectrum function.
RTAStream_TypeDef:unencapsulated measurement data in RTA mode, matching the RTA_GetRealTimeSpectrum_PM1 functiom.

The main function of real-time spectral mode (RTA)
RTA_ProfileDeInit
RTA_Configuration
RTA_BusTriggerStart
RTA_BusTriggerStop
RTA_GetRealTimeSpectrum(..,SpectrumFrames[], SpectrumBitMap[], TriggerInfo, PlotInfo, MeasAuxInfo)
RTA_GetRealTimeSpectrum_PM1(RTAStream_TypeDef)

*/

/* Return value definition  Return Value for public API functions-------------------------------------------------------------*/

/* Description of the return value
1) 0 is the default normal value. Any value other than 0 indicates a special condition.
2) The prefix APIRETVAL_ indicates that this return value is an API return value.
3) The second modifier ERROR_ indicates that the returned condition is an error. In this case, it is not recommended to directly perform subsequent operations. You need to try specific recovery steps, such as re-running DeviceOpen.
4) The second modifier WARNING_ indicates that the return condition is warning, and subsequent API calls can be continued. However, the test data obtained by the current API may be inaccurate, for example, local vibration loss lock may cause power reading miscalibration.
*/

#define APIRETVAL_NoError									0			// No Error

/* Errors and warnings during device open process */
#define APIRETVAL_ERROR_BusOpenFailed						-1			// Bus opening error, only in DeviceOpen procedure.
#define	APIRETVAL_ERROR_RFACalFileIsMissing					-3			// RF amplitude calibration file is lost. This file is produced only in DeviceOpen. If the file is lost, the amplitude calibration will be out of alignment or cannot run.#define	APIRETVAL_ERROR_RFACalFileIsMissing
#define	APIRETVAL_ERROR_IFACalFileIsMissing					-4			// If the amplitude calibration file is lost, produced only in the DeviceOpen process.If the file is lost, the amplitude calibration will be out of alignment or cannot run.
#define	APIRETVAL_ERROR_DeviceConfigFileIsMissing			-5			// The device configuration file is lost and is produced only in the DeviceOpen process. The configuration is not optimized.
#define APIRETVAL_ERROR_DeviceSpecFileIsMissing				-6			// The device specification file is lost and is produced only in the DeviceOpen process. The parameter range is incorrect.
#define APIRETVAL_ERROR_UpdateStrategyFailed				-7			// The process of delivering the configuration policy to the device fails. It is produced only in the process of DeviceOpen.

/* Errors and warnings during bus invocation */
#define	APIRETVAL_ERROR_BusError							-8			// Bus transmission error.
#define	APIRETVAL_ERROR_BusDataError						-9			// Bus data error means that the overall format of the data packet is correct, but the data content is wrong, which may be due to configuration conflict or physical interference during transmission
#define APIRETVAL_WARNING_BusTimeOut						-10			// The bus data return time out, either because the trigger did not come, or because there was a communication error.
#define	APIRETVAL_ERROR_BusDownLoad							-11			// The configuration is incorrectly delivered through the bus.

/* Errors and warnings in data packets about data content and device status */
#define	APIRETVAL_WARNING_IFOverflow						-12			// If the intermediate frequency is saturated, it is recommended to lower the reference level(RefLevel_dBm) until this prompt no longer appears.
#define APIRETVAL_WARNING_ReconfigurationIsRecommended		-14			// Since the temperature has changed significantly since the last configuration, it is recommended to re-invoke configuration functions (such as SWP_Configuration) for best performance.
#define APIRETVAL_WARNING_ClockUnlocked_SYSCLK				-15			// Hardware status warning:the system clock is out of lock.
#define	APIRETVAL_WARNING_ClockUnlocked_ADCCLK				-16			// Hardware status warning:ADC clock is out of lock.
#define	APIRETVAL_WARNING_ClockUnlocked_RXIFLO				-17			// Hardware status warning:the received IF local oscillator loss lock.
#define APIRETVAL_WARNING_ClockUnlocked_RXRFLO				-18			// Hardware status warning:the received RF local oscillator lost lock.
#define APIRETVAL_WARNING_ADCConfigError					-19         // Hardware status warning:ADC configuration error.  

/* An error occurred during firmware update */
#define APIRETVAL_ERRPR_OpenfileFiled						-20			// Failed to open the update file.
#define APIRETVAL_ERROR_ExecuteUpdateError					-21			// Failed to enter the update program.
#define APIRETVAL_ERROR_FirmwareEraseError					-22			// Failed to erase firmware flash.
#define APIRETVAL_ERROR_FirmwareDownloadError				-23			// Failed to download the firmware flash.
#define APIRETVAL_ERROR_UpdateExitError						-24			// Failed to exit the firmware update program.


/* Error state when setting standby mode */
#define APIRETVAL_ERROR_SysPowerMode_ParamOutRange			-25			// Failed to exit the firmware update program.
#define APIRETVAL_ERROR_SysPowerMode_UpdateFailed			-26			// Failed to exit the firmware update program.

#define APIRETVAL_WARNING_VNACalKitFileIsMissing			-27			// Failed to load VNA calibration component data file.
#define APIRETVAL_WARNING_VNACalResultFileIsMissing			-28			// Failed to load the VNA calibration file.

#define APIRETVAL_WARNING_TxLevelCalFileIsMissing			-29			// Failed to load the transmit power calibration file.
#define APIRETVAL_WARNING_TxLevelExceededRefLevel			-30			// Failed to load the transmit power calibration file.

/* Calibration file loading warning */
#define APIRETVAL_WARNING_DefaultRFACalFileIsLoaded			-33			// The RF amplitude calibration file of the device is lost. The system loads the default calibration file.
#define APIRETVAL_WARNING_DefaultIFACalFileIsLoaded			-34			// The IF amplitude calibration file of the device is lost. The system loads the default calibration file.

/* Device state warning*/
#define APIRETVAL_WARNING_ClockRelocked_SYSCLK				-36			// Hardware state warning，system clock re-lock
#define APIRETVAL_WARNING_ClockRelocked_ADCCLK				-37			// Hardware state warning，ADC clock re-lock
#define APIRETVAL_WARNING_ClockRelocked_RXIFLO				-38			// Hardware state warning，IF LO re-lock
#define APIRETVAL_WARNING_ClockRelocked_RXRFLO				-39			// Hardware state warning，RF LO re-lock


#define APIRETVAL_WARNING_DefaultVNACalKitFileIsLoaded		-40			// The vector network analyzer calibration file of the device is lost, and the system loads the calibration file by default.
#define APIRETVAL_WARNING_DefaultVNACalResultFileIsLoaded	-41			// The calibration result file of the vector network analyzer of the device is lost. The system loads the calibration result file by default.
#define APIRETVAL_WARNING_DefaultTxLevelCalFileIsLoaded		-42			// The transmitted power calibration file of the individual device is lost. The system loads the transmitted power calibration file by default.

#define APIRETVAL_ERROR_IQCalFileIsMissing					-43			// IQ calibration file loss, only produced in the DeviceOpen process, the file loss will cause IQ calibration misalignment or unable to run.
#define APIRETVAL_WARNING_DefaultIQCalFileIsLoaded			-44			// The IQ calibration file of the individual device is lost, and the system loads the default calibration.

#define APIRETVAL_ERROR_QueryResultsIncorrect				-45			// The query result does not match the actual delivered parameters.
#define APIRETVAL_ERROR_ADCState							-46			// The ADC status is incorrect.
#define APIRETVAL_ERROR_Options_Missing						-47			// The hardware status is incorrect. The required options do not exist. Please check whether the MUXIO connection is normal.
#define APIRETVAL_ERROR_CableLossFileIsMissing				-48			// Line loss files are lost and are produced only during the DeviceOpen process

/* Abnormal call and debugging errors and warnings */
#define APIRETVAL_ERROR_ParamOutRange						-98			// Parameter out of bounds, not called properly.
#define APIRETVAL_ERROR_FunctionInternalError				-99			// Function internal error, abnormal call.
#define APIRETVAL_WARNING_No_VSG_Function_Option			-100		// The IQ calibration file of the individual device is lost, and the system loads the default calibration file.

/* Abnormal call resampling function errors */
#define APIRETVAL_ERROR_DecimateFactorOutRange				-201		// Decimate factor setting exceeds the limit,abnormal call(The decimate factor are set in the range 1 ~ 2^16)
#define APIRETVAL_ERROR_IQSDataFormatError				    -202		// IQS Data tpye error,abnormal call(The IQ date type is currently only int8、int16、int32 and float)
#define APIRETVAL_WARNING_CarrierLoss   				    -203		// During the phase noise measurement process, no signals with power exceeding the carrier decision threshold were detected
#define APIRETVAL_WARNING_MeasUpdate				        -204		// During the phase noise measurement process, a change in the DUT's status triggered an automatic update of the measurement state, requiring reacquisition of data PartialUpdateCounts times

#define APIRETVAL_ERROR_ETHTimeOut                          10060       // The connection attempt fails due to no correct reply from the connecting party after some time or no reaction from the connected host. 
#define APIRETVAL_ERROR_ETHDisconnected                     10054       // The device disconnects from the network, and the remote host forces the closure of an existing connection.
#define APIRETVAL_ERROR_ETHDataError                        10062       // The device is not retrieving data properly.

#define APIRETVAL_ERROR_NoDeviceExist                       10068       // No device exists.
#define APIRETVAL_ERROR_DeviceNotFound                      10069       // The device whose IP address you want to change cannot be found.
#define APIRETVAL_ERROR_DeviceIPRepeatConfig                10070       // The IP address to be changed is the IP address of the device
#define APIRETVAL_ERROR_DeviceIPExceed                      10071       // The ip address range does not match
#define APIRETVAL_ERROR_DeviceIPSetError                    10072       // When the device is connected to a router, the IP address set must meet routing requirements
#define APIRETVAL_ERROR_DeviceIPExist                       10073       // The IP address to be changed exists in this network segment
#define APIRETVAL_ERROR_ConfigDeviceIPFailed                10074       // Failed to configure the IP address
#define APIRETVAL_ERROR_DeviceInfoError                     10079       // Incorrect device information is obtained

/* Data type obtained in MPS mode */
#define MPS_SWP			0x00			// When Analysis returns 0, it is SWP mode.		 
#define	MPS_IQS			0x01			// When Analysis returns 1, it is IQS mode.	
#define MPS_DET			0x02			// When Analysis returns 2, it is DET mode.	
#define MPS_RTA			0x03			// When Analysis returns 3, it is RTA mode.

/*---------------------------------------------------
Enumeration required by the device start-stop structure
-----------------------------------------------------*/

/*Equipment power supply mode BootProfile.DevicePowerSupply */
typedef enum
{
	USBPortAndPowerPort = 0x00,	// Use USB data port and power port for power supply.
	USBPortOnly = 0x01,			// Only USB data ports are used.
	Others = 0x02				// Other modes: If the ETH bus is used, select this option.
}DevicePowerSupply_TypeDef;

/*Physical bus type BootProfile.PhysicalInterface */
typedef enum
{
	USB = 0x00,    // Uses USB as the physical interface, applicable to products with USB interfaces, such as SAE and SAM.
	QSFP = 0x02,   // Use 40Gbps QSFP+ as a physical interface.
	ETH = 0x03,	   // Uses 100M/1000M Ethernet as the physical interface and applies to Ethernet interface products such as NXEs and NXMS.
	HLVDS = 0x04,  // Only for debugging, do not use.
	VIRTUAL = 0x07 // Use a virtual bus, that is, no physical bus, for simulation and debugging.
} PhysicalInterface_TypeDef;

/*IP address version BootProfile.IPVersion*/
typedef enum
{
	IPv4 = 0x00, // Use an IPv4 address.
	IPv6 = 0x01  // Use an IPv6 address.
}IPVersion_TypeDef;

/*Power consumption control*/
typedef enum
{
	PowerON = 0x00,    // All workspaces in the system are powered on.
	RFPowerOFF = 0x01, // The RF is powered off and cannot be woken up quickly.
	RFStandby = 0x02   // The RF is in standby state and can wake up quickly.
}SysPowerState_TypeDef;

/*Firmware type*/
typedef enum
{
	MCU = 0x00, // SCM
	FPGA = 0x01 // FPGA
}Firmware_TypeDef;

/*---------------------------------------------------
Enumeration required by the user structure, with the adaptation working mode in parentheses ().
all:Including the full mode 
SWP:Scan mode 
IQS:Time domain flow mode
RTA:Real-time spectrum analysis mode
DET:Detection mode 
DSP:Digital signal processing function 
VNA:Vector network analysis mode .
SIG:Auxiliary signal generator mode .
-----------------------------------------------------*/

/*Rf input port(all)*/
typedef enum
{
	ExternalPort = 0x00, // External port.
	InternalPort = 0x01, // Internal port.
	ANT_Port = 0x02,	 // only for TRx series
	TR_Port = 0x03,		 // only for TRx series
	SWR_Port = 0x04,	 // only for TRx series
	INT_Port = 0x05		 // only for TRx series
}RxPort_TypeDef;

/*Gain mode(all)*/
typedef enum
{
	LowNoisePreferred = 0x00,	   // Low noise.
	HighLinearityPreferred = 0x01 // High linearity.
}GainStrategy_TypeDef;

/*Trigger input edge(all)*/
typedef enum
{
	RisingEdge = 0x00,	// Rising edge triggers.
	FallingEdge = 0x01,	// Falling edge triggers.
	DoubleEdge = 0x02   // Two-sided edge triggers.
}TriggerEdge_TypeDef;

/*Trigger output type(all)*/
typedef enum
{
	None = 0x00,	  // No trigger output.
	PerHop = 0x01,	  // Output with each completion of frequency hopping.
	PerSweep = 0x02,  // Output with the completion of each scan.
	PerProfile = 0x03 // Output with each configuration switch.
}TriggerOutMode_TypeDef;

/*Trigger the output signal polarity(all)*/
typedef enum
{
	Positive = 0x00, // positive type pulse.
	Negative = 0x01 // negative type pulse.
}TriggerOutPulsePolarity_TypeDef;

/*Preselected amplifier(all)*/
typedef enum
{
	AutoOn = 0x00,	 // Automatically enables the preamplifier.
	ForcedOff = 0x01 // Force to keep the preamplifier off.
}PreamplifierState_TypeDef;

/*System reference clock(all)*/
typedef enum
{
	ReferenceClockSource_Internal = 0x00,         // Internal clock source (Default: 10MHz).
	ReferenceClockSource_External = 0x01,         // External clock source (Default: 10MHz). When the system detects that the external clock source cannot be locked, the system automatically switches to the internal reference.
	ReferenceClockSource_Internal_Premium = 0x02, // Internal Clock Source - High quality, choose DOCXO or OCXO.
	ReferenceClockSource_External_Forced = 0x03   // External clock source, and regardless of locking, will not switch to internal reference even if the lock is lost.
}ReferenceClockSource_TypeDef;

/*System clock(all)*/
typedef enum
{
	SystemClockSource_Internal = 0x00, // Internal system clock source.
	SystemClockSource_External = 0x01  // External system clock source.
}SystemClockSource_TypeDef;

/*Frequency allocation mode(SWP)*/
typedef enum
{
	StartStop = 0x00, // Set the frequency range by the start and the stop frequency.
	CenterSpan = 0x01 // Set the frequency range by center frequency and span.
}SWP_FreqAssignment_TypeDef;

/*Trace updating mode(SWP)*/
typedef enum
{
	ClearWrite = 0x00,    	// Output the normal trace.
	MaxHold = 0x01,	      	// Output the trace maintained by the maximum value.
	MinHold = 0x02,	      	// Output the trace maintained by the minimum value.
	ClearWriteWithIQ = 0x03 // Output time domain data and frequency domain data of current frequency point at the same time.
}SWP_TraceType_TypeDef;

/*RBW update mode(SWP)*/
typedef enum
{
	RBW_Manual = 0x00,              // Enter RBW manually.
	RBW_Auto = 0x01,			    // Update RBW automatically with SPAN,RBW = SPAN / 2000
	RBW_OneThousandthSpan = 0x02,	// Mandatory  RBW = 0.001 * SPAN
	RBW_OnePercentSpan = 0x03		// Mandatory  RBW = 0.01 * SPAN
}RBWMode_TypeDef;

/*VBW update mode(SWP)*/
typedef enum
{
	VBW_Manual = 0x00,		  // Enter VBW manually.
	VBW_EqualToRBW = 0x01,    // Mandatory VBW = RBW.
	VBW_TenPercentRBW = 0x02, // Mandatory VBW = 0.1 * RBW.
	VBW_OnePercentRBW = 0x03,  // Mandatory VBW = 0.01 * RBW.
	VBW_TenTimesRBW = 0x04		// Mandatory  VBW = 10 * RBW,Fully bypass VBW filter
}VBWMode_TypeDef;

/*SwpTime_Mode*/
typedef enum
{
	SWTMode_minSWT = 0x00,		// Sweep with minimum sweep time.
	SWTMode_minSWTx2 = 0x01,	// Sweep with approximately 2 times of the minimum sweep time.
	SWTMode_minSWTx4 = 0x02,	// Sweep with approximately 4 times of the minimum sweep time.
	SWTMode_minSWTx10 = 0x03,	// Sweep with approximately 10 times of the minimum sweep time.
	SWTMode_minSWTx20 = 0x04,	// Sweep with approximately 20 times of the minimum sweep time.
	SWTMode_minSWTx50 = 0x05,	// Sweep with approximately 50 times of the minimum sweep time.
	SWTMode_minSWTxN = 0x06,	// Sweep with approximately N times of the minimum sweep time，N=SweepTimeMultiple.
	SWTMode_Manual = 0x07,		// Sweep at approximately the specified sweep time, which is equal to SweepTime.
	SWTMode_minSMPxN = 0x08   // A single frequency point is sampled with approximately N times the shortest sampling time, N=SampleTimeMultiple
}SweepTimeMode_TypeDef;

/*Scan mode trigger source and trigger mode(SWP)*/
typedef enum
{
	InternalFreeRun = 0x00,	  // Internal trigger free run.
	ExternalPerHop = 0x01,	  // External trigger, each trigger jumps a frequency point.
	ExternalPerSweep = 0x02,  // External triggers, with each trigger refreshing a trace.
	ExternalPerProfile = 0x03 // External triggers, with each trigger applying a new configuration.
}SWP_TriggerSource_TypeDef;

/*Spurious rejection type(SWP)*/
typedef enum
{
	Bypass = 0x00,	 // No spurious rejection.
	Standard = 0x01, // Standard spurious rejection.
	Enhanced = 0x02	 // Enhanced spurious rejection.
}SpurRejection_TypeDef;

/*Trace point approximation method(SWP)*/
typedef enum
{
	SweepSpeedPreferred = 0x00,	    // Give priority to the fastest scanning speed, and then try to get as close as possible to the set target trace points.
	PointsAccuracyPreferred = 0x01, // The priority is to ensure that the actual trace points are close to the set target trace points.
	BinSizeAssined = 0x02           // The priority is to ensure that the trace is generated at the set frequency interval.
}TracePointsStrategy_TypeDef;

/*Trace aligment(SWP)*/
typedef enum
{	
	NativeAlign = 0x00,  //Natural alignment.
	AlignToStart = 0x01, //Align to start frequency.
	AlignToCenter = 0x02 //Align to center frequency.
}TraceAlign_TypeDef;

/*FFT Execution Strategy(SWP)*/
typedef enum
{
	Auto = 0x00,				// the CPU or FPGA is automatically selected for FFT calculation
	Auto_CPUPreferred = 0x01,	// the CPU or FPGA is automatically selected for FFT calculation, CPU is preferred.
	Auto_FPGAPreferred = 0x02,	// the CPU or FPGA is automatically selected for FFT calculation, FPGA is preferred.
	CPUOnly_LowResOcc = 0x03,	// Forced use of CPU calculation, low resource occupation, maximum FFT points 256K.
	CPUOnly_MediumResOcc = 0x04,// Forced use of CPU calculation, medium resource occupation, maximum FFT points 1M.
	CPUOnly_HighResOcc = 0x05,	// Forced use of CPU calculation, high resource occupation, maximum FFT points 4M.
	FPGAOnly = 0x06			// Forced use of FPGA calculation.
}FFTExecutionStrategy_TypeDef;

/*DSP calculating platform (SWP)*/
typedef enum
{
	CPU_DSP = 0x00, // CPU
	FPGA_DSP = 0x01	// FPGA
}DSPPlatform_Typedef;

/*Multi - frame detection in frequency point(SWP\RTA)*/
typedef enum
{
	Detector_Sample  = 0x00,   // No inter-frame detection
	Detector_PosPeak = 0x01,   // MaxHold Frame detection is carried between power spectrum of each frequency point
	Detector_Average = 0x02,   // Average Frame detection is carried between power spectrum of each frequency point
	Detector_NegPeak = 0x03,   // MinHold Frame detection is carried between power spectrum of each frequency point
	Detector_MaxPower = 0x04,  // Each frequency point is sampled for a long time before FFT and frame spectrum with highest power is selected for FFT to capture instantaneous signals such as pulse. (SWP mode only)
	Detector_RawFrames = 0x05, // Multiple sampling, multiple FFT analyses for each frequency, and frame-by-frame output power spectrum (SWP mode only)
	Detector_RMS = 0x06,       // RMS Frame detection is carried between power spectrum of each frequency point
	Detector_AutoPeak = 0x07   // Aoto Peak Frame detection is carried between power spectrum of each frequency point
}Detector_TypeDef;

/*Window type(SWP\RTA\DSP)*/
typedef enum
{
	FlatTop = 0x00,	 		 // Flat top window.
	Blackman_Nuttall = 0x01, // Nuttall window.
	LowSideLobe = 0x02		  // Low sidelobe window
}Window_TypeDef;

/* Trace detector mode (SWP\RTA\DSP)*/
typedef enum
{
	TraceDetectMode_Auto = 0x00,		// automatically selection of a detection.
	TraceDetectMode_Manual = 0x01		// specification of a detection.
}TraceDetectMode_TypeDef;

/*Detection mode(SWP\RTA\DSP)*/
typedef enum
{
	TraceDetector_AutoSample = 0x00,	// auto sample detection.
	TraceDetector_Sample = 0x01,	    // sample detection.
	TraceDetector_PosPeak = 0x02,	    // positive peak detection.
	TraceDetector_NegPeak = 0x03,	    // negative peak detection.
	TraceDetector_RMS = 0x04,		    // RMS detection.
	TraceDetector_Bypass = 0x05,		// no detection.
	TraceDetector_AutoPeak = 0x06 	    // auto peak detection.
}TraceDetector_TypeDef;

/*Fixed frequency point mode (IQS\RTA\DET) trigger source*/
typedef enum
{
	FreeRun = 0x00,	 			   // Free running
	External = 0x01, 			   // External trigger. Triggered by a physical signal connected to a trigger input port outside the device.
	Bus = 0x02,		 			   // The bus is triggered. Triggered by a function (instruction).
	Level = 0x03,	 			   // Level trigger. The device detects the input signal according to the set level threshold, and triggers automatically when the input exceeds the threshold.
	Timer = 0x04,	 			   // The timer is triggered. Use the device internal timer to automatically trigger the set time period.
	TxSweep = 0x05,	 			   // Output trigger of signal generator scan; When this trigger source is selected, the acquisition process will be triggered by the output trigger signal scanned by the signal source.
	MultiDevSyncByExt = 0x06, 	   // When the external trigger signal arrives, multiple machines perform synchronous trigger behavior.
	MultiDevSyncByGNSS1PPS = 0x07, // When GNSS-1PPS arrives, multiple machines trigger synchronously.
	SpectrumMask = 0x08,		   // Spectrum template trigger, available only in RTA mode. Not available in this stage.
	GNSS1PPS = 0x09				   // Use 1PPS provided by GNSS in the system to trigger.
}IQS_TriggerSource_TypeDef;

/*The timer trigger is synchronized with the outer trigger edge*/
typedef enum
{
	NoneSync = 0x00,                        // Timer trigger is not synchronized with the external trigger.
	SyncToExt_RisingEdge = 0x01,            // Timer trigger is synchronized with the external trigger rising edge. 
	SyncToExt_FallingEdge = 0x02,           // Timer trigger is synchronized with the external trigger falling edge.
	SyncToExt_SingleRisingEdge = 0x03,      // Timer trigger and external trigger rise edge single synchronization (need to call instruction function, single synchronization).
	SyncToExt_SingleFallingEdge = 0x04,     // Timer trigger and external trigger fall edge single synchronization (need to call instruction function, single synchronization).
	SyncToGNSS1PPS_RisingEdge = 0x05,       // Timer trigger synchronizes with the rising edge of GNSS-1PPS. 
	SyncToGNSS1PPS_FallingEdge = 0x06,      // Timer trigger synchronizes with the falling edge of GNSS-1PPS.
	SyncToGNSS1PPS_SingleRisingEdge = 0x07, // Timer triggers single synchronization with GNSS-1PPS rising edge (need to call instruction function, perform single synchronization).
	SyncToGNSS1PPS_SingleFallingEdge = 0x08 // Timer triggers single synchronization with GNSS-1PPS falling edge (need to call instruction function, perform single synchronization).
}
TriggerTimerSync_TypeDef;

typedef IQS_TriggerSource_TypeDef DET_TriggerSource_TypeDef;
typedef IQS_TriggerSource_TypeDef RTA_TriggerSource_TypeDef;

/*DC suppression(IQS\DET\RTA)*/
typedef enum
{
	DCCOff = 0x00,				  // Disable the DC suppression function.
	DCCHighPassFilterMode = 0x01, // Open, high-pass filter mode. This mode has a good DC suppression effect, but will damage the signal component from DC to low frequency (about 100kHz).
	DCCManualOffsetMode = 0x02,	  // Open, manual bias mode. In this mode, the bias value needs to be specified manually, and the suppression effect is weaker than that of the high-pass filter mode, but the signal component on the DC will not be damaged.
	DCCAutoOffsetMode = 0x03	  // Open, automatic bias mode.
}DCCancelerMode_TypeDef;

/*Quadrature demodulation correction(IQS\DET\RTA)*/
typedef enum
{
	QDCOff = 0x00,		  // Close the QDC.
	QDCManualMode = 0x01, // Manually perform QDC based on the given parameters.
	QDCAutoMode = 0x02	  // Automatic QDC: The system automatically performs the calibration and uses the parameters obtained during each IQS_Configuration call.
}QDCMode_TypeDef;

/*Fixed frequency point mode (IQS\RTA\DET) trigger mode*/
typedef enum
{
	FixedPoints = 0x00,	// Get the fixed point length data.
	Adaptive = 0x01		// Get data continuously.
}TriggerMode_TypeDef;

/*Stream mode data format type(IQS/DSP)*/
typedef enum
{
	Complex16bit = 0x00, // IQ, single channel data 16 bits.
	Complex32bit = 0x01, // IQ, single channel data 32 bits.
	Complex8bit = 0x02,	 // IQ, single channel data 8 bits.
	Complexfloat = 0x06  // IQ, single channel data 32-bit float format tpye (IQS mode is not available, only by DDC function output data to write back the enumeration variable).
}DataFormat_TypeDef;

/*Detector mode detector(DET)*,not available anymore/
//typedef enum
//{
//	DET_Average = 0x00,	// Average detection.
//	DET_Max = 0x01,		// Maximum detection.
//	DET_RMS = 0x02,		// root mean square value detection.
//	DET_Sample = 0x03	// Sample detection.
//}DETDetector_TypeDef;

/*Source operating mode (TRx series only)(SIG)*/
typedef enum
{
	SIG_PowerOff = 0x00,			 // Close.
	SIG_Fixed = 0x01,				 // Fixed frequency and power mode.
	SIG_FreqSweep_Single = 0x02,	 // Single frequency scan (not available).
	SIG_FreqSweep_Continous = 0x03,	 // Continuous frequency scan (not available).
	SIG_PowerSweep_Single = 0x04,	 // Single power scan (not available).
	SIG_PowerSweep_Continous = 0x05, // Continuous power scan (not available).
	SIG_ListSweep_Single = 0x06,	 // Single list scan (not available).
	SIG_ListSweep_Continous = 0x07,	 // Continuous list scan (not available).
	SIG_ManualGainCtrl = 0x08		 // Manual gain control.
}SIG_OperationMode_TypeDef;

/*signal generator Scan Source (TRx Series only)(SIG)*/
typedef enum
{
	SIG_SWEEPSOURCE_RF = 0x00,	 // Radio frequency scan.
	SIG_SWEEPSOURCE_LF = 0x01,	 // Low frequency scan.
	SIG_SWEEPSOURCE_LF_RF = 0x02 // Low frequency and RF co-scan.
}SIG_SweepSource_TypeDef;

/*Launch port Status in Launch Mode (TRx Series only)(SIG)*/
typedef enum
{
	RF_ExternalPort = 0x00, // External port.
	RF_InternalPort = 0x01, // Internal port.
	RF_ANT_Port = 0x02,		// ANT port (TRx series only).
	RF_TR_Port = 0x03,		// TR port (TRx series only).
	RF_SWR_Port = 0x04,		// SWR port (TRx series only).
	RF_INT_Port = 0x05		// INT port (TRx series only).
}TxPort_TypeDef,RFPort_typedef;

/*Transmit baseband Transmission Reset State (TRx series only)(SIG)*/
typedef enum
{
	Tx_TRNASFERRESET_ON = 0x00,  // The transmit transmission channel reset is enabled during configuration.
	Tx_TRNASFERRESET_OFF = 0x01 // The transmit transmission channel reset is disabled during configuration.
}TxTransferReset_TypeDef;

//Transmit transmission link packet state
typedef enum
{
	TxPacking_Off = 0x00, // The transmit link packets are closed and valid data are transmitted directly
	TxPacking_On = 0x01   // The middle packet of the transmitter is opened and the packet header and packet tail are added for the valid data
}TxPackingCmd_TypeDef;

/*The bit width of the transmit transmission link packet*/
typedef enum
{
	TxPackingDataWidth_32Bits = 0x01,  // When the packet is opened, the data bit width of the packet is 32bits, and the 32bits are aligned
	TxPackingDataWidth_64Bits = 0x02,  // When the packet is opened, the data bit width of the packet is 64bits, and the 64bits are aligned
	TxPackingDataWidth_128Bits = 0x04, // When the packet is opened, the data bit width of the packet is 128bits, and the 128bits are aligned
	TxPackingDataWidth_256Bits = 0x08  // When the packet is opened, the data bit width of the packet is 256bits, and the 256bits are aligned
}TxPackingDataWidth_TypeDef;

/*Trigger input source in emission mode (TRx series only)(SIG)*/
typedef enum
{
	Tx_TRIGGERIN_SOURCE_INTERNAL = 0x00, // Internal trigger.
	Tx_TRIGGERIN_SOURCE_EXTERNAL = 0x01, // External trigger.
	Tx_TRIGGERIN_SOURCE_TIMER = 0x02,    // The timer trigger.
	Tx_TRIGGERIN_SOURCE_RF = 0x03,       // RF board trigger.
	Tx_TRIGGERIN_SOURCE_BUS = 0x04       // The bus trigger.
}TxTriggerInSource_TypeDef;

/*Trigger input Mode in Emission Mode (TRx series only)(SIG)*/
typedef enum
{
	Tx_TRIGGER_MODE_FREERUN = 0x00,     // Free running.
	Tx_TRIGGER_MODE_SINGLEPOINT = 0x01, // Single point trigger (trigger a single frequency or power configuration).
	Tx_TRIGGER_MODE_SINGLESWEEP = 0x02, // Single scan trigger (trigger one cycle scan at a time).
	Tx_TRIGGER_MODE_CONTINOUS = 0x03    // Continuous scan trigger (trigger a continuous work).
}TxTriggerInMode_TypeDef;

/*Trigger Output Mode in Emission Mode (TRx Series only)(SIG)*/
typedef enum
{
	Tx_TRIGGER_OUTMODE_NONE = 0x00,      // No output trigger.
	Tx_TRIGGER_OUTMODE_PERPOINT = 0x01,  // Single point output (single trigger output for single frequency hopping and power hopping).
	Tx_TRIGGER_OUTMODE_PERSWEEP = 0x02,  // Output a single scan (output a trigger after the completion of a single scan cycle).
	Tx_TRIGGER_OUTMODE_PERPROFILE = 0x03 // Single Profile output (output a trigger after a single Profile scan is completed).
}TxTriggerOutMode_TypeDef;

/*Analog IQ Input Source in Emission Mode (TRx Series only)(SIG)*/
typedef enum
{
	Tx_ANALOGIQSOURCE_INTERNAL = 0x00, // Internal simulated IQ input.
	Tx_ANALOGIQSOURCE_EXTERNAL = 0x01  // External simulated IQ input.
}TxAnalogIQSource_TypeDef;

/*Digital IQ input source in Emission Mode (TRx Series only)(SIG)*/
typedef enum
{
	Tx_DIGITALIQSOURCE_INTERNAL = 0x00,  // Internal digital IQ input.
	Tx_DIGITALIQSOURCE_EXTERNAL = 0x01,  // External digital IQ input.
	Tx_DIGITALIQSOURCE_SIMULATION = 0x02  //Use internal sinusoidal simulation signals.
}TxDigitalIQSource_TypeDef;

/*Trigger input source in Send/Receive mode (TRx Series only)(VNA)*/
typedef enum
{
	TRx_TRIGGERIN_SOURCE_INTERNAL = 0x00, // Internal trigger.
	TRx_TRIGGERIN_SOURCE_EXTERNAL = 0x01, // External trigger.
	TRx_TRIGGERIN_SOURCE_TIMER = 0x02,    // The timer trigger.
	TRx_TRIGGERIN_SOURCE_RF = 0x03,       // RF board trigger.
	TRx_TRIGGERIN_SOURCE_BUS = 0x04       // Bus trigger.
}TRxTriggerInSource_typedef;

/*Trigger input mode in Send/Receive Mode (Only for TRx Series)(VNA)*/
typedef enum
{
	TRx_TRIGGER_MODE_FREERUN = 0x00,     // Free running.
	TRx_TRIGGER_MODE_SINGLEPOINT = 0x01, // Single point trigger (trigger a single frequency or power configuration).
	TRx_TRIGGER_MODE_SINGLESWEEP = 0x02, // Single scan trigger (trigger one cycle scan at a time).
	TRx_TRIGGER_MODE_CONTINOUS = 0x03    // Continuous scan trigger (trigger a continuous work).
}TRxTriggerInMode_typedef;

/*Trigger output mode in Receive/Transmit Mode (Only for TRx series)(VNA)*/
typedef enum
{
	TRx_TRIGGER_OUTMODE_NONE = 0x00,      // No output trigger.
	TRx_TRIGGER_OUTMODE_PERPOINT = 0x01,  // Single point output (single trigger output for single frequency hopping and power hopping).
	TRx_TRIGGER_OUTMODE_PERSWEEP = 0x02,  // Output a single scan (output a trigger after the completion of a single scan cycle).
	TRx_TRIGGER_OUTMODE_PERPROFILE = 0x03 // Single Profile output (output a trigger after a single Profile scan is completed).
}TRxTriggerOutMode_typedef;

/*Status of the sending and receiving ports in the sending and receiving mode: The former is sending and the latter is receiving (Only applicable to TRx series)(VNA)*/
typedef enum
{
	ANT_TR = 0x00,   // The transmit is ANT and the receive is TR.  	
	TR_ANT = 0x01,   // The transmit is TR and the receive is ANT.
	SWR_SWR = 0x02,  // The transmit is SWR and the receive is SWR.
	INT_INT = 0x03,  // The transmit is INT and the receive is INT.(internal to internal))
	ANT_ANT = 0x04,  // The transmit is ANT and the receive is ANT.
	SWR_ANT = 0x05,  // The transmit is SWR and the receive is ANT.
	SWR_TR = 0x06,   // The transmit is SWR and the receive is TR.
	TR_SWR = 0x07,   // The transmit is TR and the receive is SWR.
	SWR = 0x08,      // Used specifically to refer to standing wave (S11) test.
	TRANSMIT = 0x09  // Dedicated to transport (S21) test, for TRX60 =SWR->ANT; For SAM-60 = Tx->Rx.
}TRxPort_typedef;

/*Receive reference Level Mode in Send/Receive Mode (TRx Series only)(VNA)*/
typedef enum
{
	RxRefLevel_Fixed = 0x00, // Set the reference level to CurrentLevel_dBm.
	RxRefLevel_Sync = 0x01  // Changes synchronously with the transmit power, and the offset is CurrentLevel_dBm.
}RxLevelMode_typedef;

/*Vector Network Analyzer working mode (only for TRx series)(VNA)*/
typedef enum
{
	Sweep_S11 = 0x00,   // S11 mode to measure reflection characteristics.
	Sweep_S21 = 0x01,   // S21 mode to measure transmission characteristics.
	Sweep_Power = 0x02, // Power scanning mode, measuring power compression point, etc.
	Fixed = 0x03        // Fixed frequency and power mode.
}VNA_OperationMode_TypeDef;

/*Vector Network Analyzer Calibration Mode (TRx Series only)(VNA)*/
typedef enum
{
	Calibration_Open = 0x00,     // In S11 mode, open circuit calibration.	
	Calibration_Short = 0x01,    // In S11 mode, short circuit calibration.		
	Calibration_Match = 0x02,    // In S11 mode, match calibration.	
	Calibration_Through = 0x03,  // S21 mode, through calibration.
	Calibration_Isolation = 0x04 // Isolated calibration in S21 mode.
}VNA_Calibration_TypeDef;

/*Signal type, used to generate the signal to be modulated (Analog) for analog modulation*/
typedef enum 
{
	SIGNAL_SINE = 0x00,     // Sinusoidal signal.
	SIGNAL_COSINE = 0x01,   // Cosine signal.
	SIGNAL_LINEAR = 0x02,   // linear (sawtooth).
	SIGNAL_TRIANGLE = 0x03, // Triangle.
	SIGNAL_CW = 0x04,       // DC (output continuous wave after modulation).
	SIGNAL_COMPLEX = 0x05   // complex signal
}AnalogSignalType_TypeDef;

/*AM modulation type (Analog)*/
typedef enum 
{
	AM_MOD_DSB = 0x00, // double sideband modulation.
	AM_MOD_LSB = 0x01, // lower band modulation.
	AM_MOD_USB = 0x02, // upper band modulation.
	AM_MOD_CSB = 0x03, // Residual sideband modulation.
	AM_MOD_CW = 0x04   // Generate continuous waves.
}AMModType_TypeDef;

/*AM modulated carrier suppression (Analog)*/
typedef enum
{
	AM_CARRIERSUPPRESSION_ON = 0x00, // Carrier suppression is on.
	AM_CARRIERSUPPRESSION_OFF = 0x01 // Carrier suppression is off.
}AM_CarrierSuppression_Typedef;

/*Frequency detection mode (DSP)*/
typedef enum
{
	TraceFormat_Standard = 0x00, // equal frequency interval.
	TraceFormat_PrecisFrq = 0x01 // precise frequency.
}TraceFormat_TypeDef;

/*Fan mode(DEV)*/
typedef enum
{
	FanState_On = 0x00,		// forced on.
	FanState_Off = 0x01,	// forced off.
	FanState_Auto = 0x02	// automatically.
}FanState_TypeDef;

/*GNSS Antenna status*/
typedef enum
{
	GNSS_AntennaExternal = 0x00,  // external antenna
	GNSS_AntennaInternal = 0x01 // inter antenna
}GNSSAntennaState_TypeDef;

/*DOCXO Antenna static_assert*/
typedef enum
{
	DOCXO_LockMode = 0x00,  // disciplining mode
	DOCXO_HoldMode = 0x01 	// tracking mode
}DOCXOWorkMode_TypeDef;

//GNSS signal-to-noise ratio (SNR)
typedef struct
{
	uint8_t Max_SatxC_No;	//maximum signal-to-noise ratio (SNR)
	uint8_t Min_SatxC_No;	//minimum signal-to-noise ratio (SNR)
	uint8_t Avg_SatxC_No;	//average signal-to-noise ratio (SNR)
}GNSS_SNR_TypeDef;

typedef struct
{
	uint8_t SatsNum_All;	//current range visual satellite
	uint8_t SatsNum_Use;	//number of satellites used for positioning
	GNSS_SNR_TypeDef GNSS_SNR_UsePos;		//satellite signal-to-noise ratio information for positioning
	GNSS_SNR_TypeDef GNSS_SNR_NotUsePos;	//satellite signal-to-noise ratio information in view, but not used for positioning

}GNSS_SatDate_TypeDef;


/*GNSS obtain device information*/
typedef struct
{
	float	latitude;	        //longitude
    float	longitude;          //lattitude
    int16_t	altitude;           //altitude
    uint8_t SatsNum;             //number of satellites currently in use

    uint8_t GNSS_LockState;     //GNSS lock state 
    uint8_t DOCXO_LockState;    //DOCXO lock state
    DOCXOWorkMode_TypeDef DOCXO_WorkMode;     //DOCXO work state
    GNSSAntennaState_TypeDef GNSSAntennaState; //Antenna sta

    int16_t hour; 
    int16_t minute; 
    int16_t second; 
    int16_t Year; 
    int16_t month; 
    int16_t day;

}GNSSInfo_TypeDef;


//Types of GNSS Peripheral Devices
typedef enum
{
	GNSS_None = 0,		//No peripherals
	GNSS_For_EIO = 1,	//EIO
	GNSS_For_NX = 2,	//NX
	GNSS_For_PX = 3		//PX

}GNSSPeriphType_TypeDef;

//Types of GNSS
typedef enum
{
	None_GPS = 0, 		//Without a GPS receiver
	GNSS_GPS = 1,		//Standard GPS
	GNSS_GPS_Pro = 2	//High-performance GPS
	
}GNSSType_TypeDef;

//Types of OCXO
typedef enum
{
	None_OCXO = 0,		//Without OCXO
	GNSS_OCXO = 1,		//Standard OCXO for GNSS
	GNSS_DOCXO = 2		//Disciplined OCXO for GNSS
	
}OCXOType_TypeDef;

typedef struct
{
	GNSSPeriphType_TypeDef GNSSPeriphType;	//Types of GNSS Peripheral Devices
	GNSSType_TypeDef GNSSType;				//Types of GNSS Receivers
	OCXOType_TypeDef OCXOType;				//Types of OCXO for GNSS

	uint8_t InternalOCXO;		//Is the internal reference clock of the device an Oven-Controlled Crystal Oscillator (OCXO)
	uint8_t SignalSourceEn;		//Does the device support the signal source function
	uint8_t ADC_VariableRateEn;	//Does the device support variable sampling rates for ADC (Analog-to-Digital Converter)
	uint8_t IM3_filter;			//Supplementary Intermediate Frequency (IF) Filter (for IM3 Enhancement)

}HardWareState_TypeDef;

/*LO optimization(all)*/
typedef enum
{
	LOOpt_Auto = 0x00,		// LO optimization,auto
	LOOpt_Speed = 0x01,		// LO optimization,high sweep speed
	LOOpt_Spur = 0x02,		// LO optimization,low spurious
	LOOpt_PhaseNoise = 0x03	// LO optimization,low phase noise
}LOOptimization_TypeDef;

/*---------------------------------------------------
Device structure
-----------------------------------------------------*/

/*Startup configuration (Configuration)*/
typedef struct
{
	PhysicalInterface_TypeDef PhysicalInterface; // Select the physical interface.
	DevicePowerSupply_TypeDef DevicePowerSupply; // Select the power supply mode.
	IPVersion_TypeDef ETH_IPVersion;             // ETH Protocol version.
	uint8_t ETH_IPAddress[16];                   // IP address of the ETH interface.
	uint16_t ETH_RemotePort;                     // Listening port of the ETH interface.
	int32_t	ETH_ErrorCode;	                     // Return code of the ETH interface.
	int32_t ETH_ReadTimeOut;                     // Read timeout of the ETH interface(ms).
}BootProfile_TypeDef;

/*Device information (returned)*/
typedef struct
{
	uint64_t DeviceUID;		  // Device serial number.
	uint16_t Model;			  // Device type.
	uint16_t HardwareVersion; // Hardware version.
	uint32_t MFWVersion;	  // MCU firmware version.
	uint32_t FFWVersion;	  // FPGA firmware version.
}DeviceInfo_TypeDef;

/*NX Device information (returned)*/
typedef struct
{
	uint64_t DeviceUID;		  // Device serial number.
	uint16_t Model;			  // Device type.
	uint16_t HardwareVersion; // Hardware version.
	uint32_t MFWVersion;	  // MCU firmware version.
	uint32_t FFWVersion;	  // FPGA firmware version.
	uint8_t IPAddress[4];     // IP Address.
	uint8_t SubnetMask[4];    // Subnet Mask.
}NetworkDeviceInfo_TypeDef;

/*Startup information (return)*/
typedef struct 
{
	DeviceInfo_TypeDef DeviceInfo; // Device information.

	uint32_t BusSpeed;	 // Bus speed information.
	uint32_t BusVersion; // Bus firmware version.
	uint32_t APIVersion; // API version

	int ErrorCodes[7];	           // List of error codes during startup.
	int Errors;			           // Total number of errors during startup.
	int WarningCodes[7];           // List of warning codes during startup.
	int Warnings;		           // Total number of warnings during startup.
}BootInfo_TypeDef;

/*Device status (advanced variable, not recommended)(Return)*/
typedef struct
{
	int16_t	Temperature;	  // Equipment Temperature, Celsius = 0.01 * Temperature.
	uint16_t RFState;		  // Radio status.
	uint16_t BBState;		  // Baseband status.

	double AbsoluteTimeStamp;   // The absolute timestamp of the current packet.
	float Latitude;           // Latitude coordinates corresponding to the current packet. North latitude is positive and south latitude is negative, so as to distinguish north and south latitudes.
	float Longitude;          // The longitude coordinate corresponding to the current packet is positive in east longitude and negative in west longitude, so as to distinguish east longitude from west longitude.

	uint16_t GainPattern;	  // Gain control word used by the frequency point of the current packet.
	int64_t	RFCFreq;		  // RF center frequency used by the frequency point of the current packet.

	uint32_t ConvertPattern;  // Frequency conversion mode used by the frequency point of the current packet.
	uint32_t NCOFTW;		  // NCO frequency word used by the current packet frequency point.

	uint32_t SampleRate;	  // Equivalent sampling rate used by the current packet frequency point, equivalent sampling rate = ADC sampling rate/extraction multiple.
	uint16_t CPU_BCFlag;	  // CPU-FFT Specifies the BC flag bit required for the frame.
	uint16_t IFOverflow;	  // If the equipment is overloaded, consider and BBState or RFState.
	uint16_t DecimateFactor;  // The extraction multiple used by the current packet frequency point.
	uint16_t OptionState;	  // Optional status.

	//int16_t		LicenseCode;   // License code.
}DeviceState_TypeDef;

/* Supplementary information for measurement data */
typedef struct
{
	uint32_t MaxIndex;			// Indicates the index of the maximum power value in the current packet.
	float	 MaxPower_dBm;		// The maximum power in the current packet.

	int16_t	 Temperature;		// Equipment Temperature, Celsius = 0.01 * Temperature.
	uint16_t RFState;			// Radio status.
	uint16_t BBState;			// Baseband status.
	uint16_t GainPattern;	    // Gain control word used by the frequency point of the current packet.

	uint32_t ConvertPattern;	// Frequency conversion mode used by the frequency point of the current packet.

	double	 SysTimeStamp;		// System time stamp corresponding to the current packet, in s.

	double   AbsoluteTimeStamp;	// Absolute timestamp of the current packet.
	float    Latitude;			// Latitude coordinates corresponding to the current packet. North latitude is positive and south latitude is negative, so as to distinguish north and south latitudes.
	float    Longitude;			// The longitude coordinate corresponding to the current packet is positive in east longitude and negative in west longitude, so as to distinguish east longitude from west longitude.

}MeasAuxInfo_TypeDef;

/*---------------------------------------------------
Standard spectrum mode(SWP)
-----------------------------------------------------*/
/*SWP configuration structure (basic configuration)*/
typedef struct
{
	double StartFreq_Hz; 									 // start frequency
	double StopFreq_Hz;	 									 // Stop frequency
	double CenterFreq_Hz;                                    // center frequency
	double Span_Hz;                                          // span
	double RefLevel_dBm;									 // R.L.
	double RBW_Hz;		 									 // RBW
	double VBW_Hz;											 // VBW
	double SweepTime;										 // When the sweep time mode is Manual, the parameter is absolute time. When specified as *N, this parameter is the scan time multiplier

	SWP_FreqAssignment_TypeDef FreqAssignment;               // Select StartStop or CenterSpan to set the frequency.

	Window_TypeDef Window; 									 // Window type used for FFT analysis.

	RBWMode_TypeDef RBWMode; 							 	 // RBW update mode. Input manually, set automatically according to Span.

	VBWMode_TypeDef VBWMode;							 	 // VBW update mode. Manual input, VBW = RBW, VBW = 0.1*RBW, VBW = 0.01*RBW.

	SweepTimeMode_TypeDef SweepTimeMode;					 // sweep time mode

	Detector_TypeDef Detector;								 // Detector

	TraceDetectMode_TypeDef TraceDetectMode;				 // trace detection mode(frequency axis)
	TraceDetector_TypeDef TraceDetector; 					 // trace detector

	uint32_t TracePoints;  									 // trace points

	RxPort_TypeDef RxPort; 									 // RF in port

	SpurRejection_TypeDef SpurRejection; 					 // spurious rejection

	ReferenceClockSource_TypeDef ReferenceClockSource; 		 // reference clock source
	double ReferenceClockFrequency; 				   		 // reference clock frequency,Hz

	SWP_TriggerSource_TypeDef TriggerSource; 				 // input trigger source.
	TriggerEdge_TypeDef	TriggerEdge; 		 				 // input trigger edge.

	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action Settings

	SWP_TraceType_TypeDef TraceType; 						 // output trace type

}SWP_EZProfile_TypeDef;

/*Standard spectrum configuration structure (configuration)*/
typedef struct
{
	double StartFreq_Hz; 									 // Start frequency.
	double StopFreq_Hz;	 									 // Stop frequency
	double CenterFreq_Hz;                                    // Center frequency
	double Span_Hz;                                          // Span
	double RefLevel_dBm;									 // Reference level
	double RBW_Hz;		 									 // RBW
	double VBW_Hz;											 // VBW
	double SweepTime;										 // When the sweep time mode is Manual, the parameter is absolute time. When specified as *N, this parameter is the scan time multiplier
	double TraceBinSize_Hz;                                  // The frequency interval between adjacent frequency points of the trace.

	SWP_FreqAssignment_TypeDef FreqAssignment;               // Select StartStop or CenterSpan to set the frequency.
	
	Window_TypeDef Window; 									 //  Window type used for FFT analysis.

	RBWMode_TypeDef RBWMode; 							 	 // RBW update mode. Input manually, set automatically according to Span.

	VBWMode_TypeDef VBWMode;							 	 // VBW update mode. Manual input, VBW = RBW, VBW = 0.1*RBW, VBW = 0.01*RBW.

	SweepTimeMode_TypeDef SweepTimeMode;					 // sweep time mode.

	Detector_TypeDef Detector;								 // detector

	TraceFormat_TypeDef TraceFormat;						 // Trace format
	TraceDetectMode_TypeDef TraceDetectMode;				 // trace detection mode (frequency axis)
	TraceDetector_TypeDef TraceDetector; 					 // trace detector
	uint32_t TracePoints;  									 // Trace point.
	TracePointsStrategy_TypeDef	TracePointsStrategy; 		 // Trace point mapping strategy.
	TraceAlign_TypeDef TraceAlign;                           // Trace alignment.
	FFTExecutionStrategy_TypeDef FFTExecutionStrategy; 		 // FFT execution strategy.

	RxPort_TypeDef RxPort; 									 // RF input port.

	SpurRejection_TypeDef SpurRejection; 					 // Spurious Rejection

	ReferenceClockSource_TypeDef ReferenceClockSource; 		 // Reference clock source.
	double ReferenceClockFrequency; 				   		 // Reference clock frequency, Hz.
	uint8_t EnableReferenceClockOut;						 // enable reference clock output.

	SystemClockSource_TypeDef SystemClockSource; 		     // The default system clock source is the internal system clock.Use it under the guidance of the vendor.
	double ExternalSystemClockFrequency; 				   	 // External system clock frequency (Hz).

	SWP_TriggerSource_TypeDef TriggerSource; 				 // Input trigger source.
	TriggerEdge_TypeDef	TriggerEdge; 		 				 // Input trigger edge.
	TriggerOutMode_TypeDef TriggerOutMode;   				 // Trigger output mode.
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // Trigger output pulse polarity.

	uint32_t PowerBalance; 									 // Balance between power consumption and scanning speed.
	GainStrategy_TypeDef GainStrategy; 						 // Gain strategy.
	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action set.
	
	uint8_t	AnalogIFBWGrade; 								 //	Trigger output mode.
	uint8_t IFGainGrade; 	 								 //	Trigger output pulse polarity.

	uint8_t	EnableDebugMode; 	 							 //	Debug mode. Advanced applications are not recommended. The default value is 0.
	uint8_t	CalibrationSettings; 							 // Calibration selection. Advanced applications are not recommended. The default value is 0.

	int8_t Atten;                                            // Attenuation dB, set the frequency analyzer channel attenuation, default -1 (automatic).

	SWP_TraceType_TypeDef TraceType; 						 // Output trace type.

	LOOptimization_TypeDef LOOptimization;					 // LO optimization

}SWP_Profile_TypeDef;

/*Trace information structure of SWP mode (return)*/
typedef struct
{
	int FullsweepTracePoints;	   // The points of the complete trace.
	int PartialsweepTracePoints;   // Trace points of each frequency point, that is, the points of GetPart each time.
	int TotalHops;				   // The number of frequency points of complete traces, that is, the number of times a complete trace needs GetPart
	uint32_t UserStartIndex;	   // Array index corresponding to the user-specified StartFreq_Hz in the trace array, that is, when HopIndex = 0, Freq[UserStartIndex] is the closest frequency point to SWPProfile.StartFreq_Hz.
	uint32_t UserStopIndex;	       // Array index corresponding to the user-specified StopFreq_Hz in the trace array, that is, when HopIndex = TotalHops-1, Freq[UserStopIndex] is the frequency point closest to SWPProfile.StopFreq_Hz.
	
	double TraceBinBW_Hz;		   // The frequency interval between two points of the trace.
	double StartFreq_Hz;		   // The frequency of the first frequency point of the trace.
	double AnalysisBW_Hz;		   // Analysis bandwidth corresponding to each frequency point.

	int TraceDetectRatio;		     // Detection ratio of video detection.
	int DecimateFactor;			     // Multiple of time domain data extraction.
	float FrameTimeMultiple;	     // Frame analysis time multiple: The analysis time of the device at a single frequency = default analysis time (set by the system) * frame time multiple. Increasing the frame time multiple will increase the device's minimum scan time, but is not strictly linear.
	double FrameTime;		 	     // Frame sweep time: duration (in seconds) of the signal used for single frame FFT analysis.
	double EstimateMinSweepTime;     // The minimum scanning time that can be set under the current configuration (unit: second, the result is mainly affected by Span, RBW, VBW, frame scanning time and other factors). 
	DataFormat_TypeDef DataFormat;   // Time domain data format.
	uint64_t SamplePoints;		     // Time domain data sampling length.
	uint32_t GainParameter;		     // Gain related parameters, including Space(31 to 24Bit), PreAmplifierState(23 to 16Bit), StartRFBand(15 to 8Bit), StopRFBand(7 to 0Bit).
	DSPPlatform_Typedef DSPPlatform; // DSP calculating platform under current configuration
}SWP_TraceInfo_TypeDef;

/*Debug information structure for scan mode (Return)*/
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

/*SWP A top-level structure containing configuration, return information, and staging data (configuration, return, staging)*/
typedef struct
{
	double* Freq_Hz;                     // The address of the Frequency data stored in the Device pointer.
	float* PowerSpec_dBm;                // The address of the PowerSpec_dBm data stored in the Device pointer.
	int HopIndex;					     // Frequency point index corresponding to the current data, used to splice the spectrum.
	int FrameIndex;					     // Indicates the frame index of the current data when multiple frames (multiple FFTS) exist at a frequency point.

	void* AlternIQStream;			 	 // Interweave the address of the IQ data when the selected spectrum trace is uploaded with the IQ data.
	float ScaletoV;						 // The scale factor of IQ to V when the selected spectrum trace is uploaded with IQ data.

	MeasAuxInfo_TypeDef MeasAuxInfo;     // Information corresponding to the current data, including the maximum data in the current packet, and the current temperature coordinate device status, etc.

	SWP_Profile_TypeDef SWP_Profile;	 // SWP configuration information corresponding to the current SWP data can be updated using SWP_Configuration function (SWP_ProfileOut).
	SWP_TraceInfo_TypeDef SWP_TraceInfo; // The trace information corresponding to the current SWP data is updated by SWP_Configuration function (SWP_TraceInfo).

	DeviceInfo_TypeDef DeviceInfo;       // Device information corresponding to the current SWP data.
	DeviceState_TypeDef DeviceState;     // Device status corresponding to the current SWP data.
}SWPTrace_TypeDef;

/*---------------------------------------------------
IQS mode structure(IQStream)
-----------------------------------------------------*/
/*IQS configuration structure (basic configuration)*/
typedef struct
{
	double CenterFreq_Hz; 								 	 // Center frequency.
	double RefLevel_dBm;								 	 // R.L.
	uint32_t DecimateFactor;								 // Decimate Factor of time domain.

	RxPort_TypeDef RxPort;									 // RF input port.

	uint32_t BusTimeout_ms;							 		 // Transmission timeout

	IQS_TriggerSource_TypeDef TriggerSource;				 // Input trigger source.
	TriggerEdge_TypeDef	TriggerEdge;						 // Input trigger edge.

	TriggerMode_TypeDef	TriggerMode;						 // Input trigger mode.	
	uint64_t TriggerLength;									 // Enter the number of sampling points after triggering. take effect only in FixedPoints mode.

	double TriggerLevel_dBm;								 // Level trigger threshold.
	double TriggerTimer_Period;								 // timing trigger,trigger period in s.		

	DataFormat_TypeDef DataFormat;						 	 // data format.

	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action.

	uint8_t	AnalogIFBWGrade;								 //	IF gain grade.

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	Reference clock source.
	double ReferenceClockFrequency;							 //	Reference clock frequency.

	double  NativeIQSampleRate_SPS;							 // for specific equipment. Native IQ sampling rate. For devices with variable sampling rate, the sampling rate can be adjusted by this parameter; device with fixed sample rate are configured to default fixed value.

	uint8_t EnableIFAGC;									 // for specific equipment. IF AGC control, 0: AGC off, using MGC mode; 1: The AGC is enabled.

}IQS_EZProfile_TypeDef;

/*Configuration structure of IQS (Configuration)*/
typedef struct
{
	double CenterFreq_Hz; 								 	 // Center frequency.
	double RefLevel_dBm;								 	 // Reference level.
	uint32_t DecimateFactor;								 // Decimate Factor of time domain.

	RxPort_TypeDef RxPort;									 // RF input port.

	uint32_t BusTimeout_ms;							 		 // Transmission timeout

	IQS_TriggerSource_TypeDef TriggerSource;				 // Input trigger source.
	TriggerEdge_TypeDef	TriggerEdge;						 // Input trigger edge.
	TriggerMode_TypeDef	TriggerMode;						 // Input trigger mode.	
	uint64_t TriggerLength;									 // Enter the number of sampling points after triggering. This takes effect only in FixedPoints mode.
	TriggerOutMode_TypeDef TriggerOutMode;					 // Trigger output mode.
	TriggerOutPulsePolarity_TypeDef TriggerOutPulsePolarity; // Trigger output pulse polarity.

	double TriggerLevel_dBm;								 // Level trigger threshold.
	double TriggerLevel_SafeTime;							 // Level trigger anti-shaking safety time, in seconds.
	double TriggerDelay;									 // Trigger delay,in seconds
	double PreTriggerTime;									 // Pre-trigger time,in seconds.

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // Synchronization options for timed and out-triggered edges. The trigger mode is effective when the trigger is timed.
	double TriggerTimer_Period;								 // The trigger period of the timed trigger, in s. The trigger mode is effective when the trigger is timed.
	
	uint8_t EnableReTrigger;							     // Enable the device to respond multiple times after capturing a trigger. This function is available only in FixedPoint mode.
	double ReTrigger_Period;								 // Time interval for multiple responses of a trigger device. It is also used as the trigger period in the Timer trigger mode (unit: s).
	uint16_t ReTrigger_Count;				 	    		 // After a trigger, several responses are required in addition to the triggered response.

	DataFormat_TypeDef DataFormat;						 	 // Data format.

	GainStrategy_TypeDef GainStrategy;				 		 // Gain strategy.
	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action.

	uint8_t	AnalogIFBWGrade;								 //	Intermediate frequency bandwidth grade.
	uint8_t	IFGainGrade;									 //	intermediate frequency gain grade.

	uint8_t	EnableDebugMode;								 //	Debug mode. Advanced applications are not recommended. The default value is 0.

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	Reference clock source.
	double ReferenceClockFrequency;							 // Reference clock frequency.
	uint8_t EnableReferenceClockOut;						 // enable reference clock output.

	SystemClockSource_TypeDef SystemClockSource; 		     // System clock source.
	double ExternalSystemClockFrequency; 				   	 // External system clock frequency (Hz).

	double  NativeIQSampleRate_SPS;							 // Suitable for specific equipment. Native IQ sampling rate. For devices with variable sampling rate, the sampling rate can be adjusted by adjusting this parameter; Nonvariable sampling rate device configurations are always configured to the system default fixed value.
	
	uint8_t EnableIFAGC;									 // Suitable for specific equipment. Medium frequency AGC control, 0: AGC off, using MGC mode; 1: The AGC is enabled.

	int8_t Atten;                                            // attenuation.

	DCCancelerMode_TypeDef DCCancelerMode;					 // Suitable for specific equipment. Dc suppression. 0: disables the DCC. 1: Open, high-pass filter mode (better suppression effect, but will damage the signal in the range of DC to 100 KHZ); 2: Open, manual bias mode (need manual calibration, but not low frequency damage signal).

	QDCMode_TypeDef	QDCMode;								 // Suitable for specific equipment. IQ amplitude and phase corrector. QDCOff: disables the QDC function. QDCManualMode: Enable and use manual mode; QDCAutoMode: Enables and uses the automatic QDC mode.

	float QDCIGain;											 // Suitable for specific equipment. Normalized linear gain I, 1.0 indicates no gain, set range 0.8 to 1.2.
	float QDCQGain;											 // Suitable for specific equipment. Normalized linear gain Q, 1.0 indicates no gain, set range 0.8~1.2.
	float QDCPhaseComp;										 // Suitable for specific equipment. Normalized phase compensation coefficient, set range -0.2~+0.2.

	int8_t DCCIOffset;										 // Suitable for specific equipment. I channel DC offset, LSB.
	int8_t DCCQOffset;										 // Suitable for specific equipment. Q channel DC offset, LSB.

	LOOptimization_TypeDef LOOptimization;					 // LO optimization

}IQS_Profile_TypeDef;

/* Flow information structure returned after IQS configuration (returned) */
typedef struct
{
	double Bandwidth;        // The current configuration corresponds to the receiver's physical channel or digital signal processing bandwidth.
	double IQSampleRate;     // Single-channel sampling rate of IQ corresponding to the current configuration (unit: Sample/second).
	uint64_t PacketCount;	 // The total number of data packets corresponding to the current configuration takes effect only in FixedPoints mode.
	
	uint64_t StreamSamples;	 // In Fixedpoints mode, it represents the total number of sampling points corresponding to the current configuration. In Adaptive mode, the value has no physical significance and is 0.
	uint64_t StreamDataSize; // In Fixedpoints mode, it indicates the total number of bytes of samples corresponding to the current configuration. In Adaptive mode, the value has no physical significance and is 0.

	uint32_t PacketSamples;  // Sampling points in packets obtained by each IQS_GetIQStream invocation Sample points contained in each packet.
	uint32_t PacketDataSize; // The number of valid data bytes to be obtained per call to IQS_GetIQStream.
	uint32_t GainParameter;	 // Gain dependent parameters, including Space(31~24Bit),PreAmplifierState(23~16Bit),StartRFBand(15~8Bit)、StopRFBand(7~0Bit).
}IQS_StreamInfo_TypeDef;

/* Trigger information structure contained in IQS packet, trigger information return structure of DET and RTA is the same (return) */
typedef struct
{
	uint64_t	SysTimerCountOfFirstDataPoint;	  // The system timestamp corresponding to the first data point of the current packet.
	uint16_t	InPacketTriggeredDataSize;		  // The number of bytes of valid trigger data in the current packet.
	uint16_t	InPacketTriggerEdges;			  // The number of trigger edges contained in the current package.
	uint32_t	StartDataIndexOfTriggerEdges[25]; // The data location in the current package corresponding to the trigger edge.
	uint64_t	SysTimerCountOfEdges[25];		  // The system timestamp of the trigger edge in the current package.
	int8_t		EdgeType[25];					  // The polarity of each trigger edge in the current packet.
}IQS_TriggerInfo_TypeDef;

typedef IQS_TriggerInfo_TypeDef DET_TriggerInfo_TypeDef;
typedef IQS_TriggerInfo_TypeDef RTA_TriggerInfo_TypeDef;

/* IQS Top-level structures containing configuration, return information, and staging data (configuration, return, staging) */
typedef struct
{
	void* AlternIQStream;				     // Interleaved distribution of IQ time domain data, single path may be in int-8, int-16 and int-32 format.

	float IQS_ScaleToV;						 // Coefficient from type int to absolute value of voltage (V).

	float	 MaxPower_dBm;					 // The maximum power of the current packet.
	uint32_t MaxIndex;					 	 // Index of the maximum power value in the current packet.

	IQS_Profile_TypeDef IQS_Profile;		 // The IQS_ProfileOut function is used to update the IQS configuration information corresponding to the current IQ stream.
	IQS_StreamInfo_TypeDef IQS_StreamInfo;	 // The IQS_Configuration function is used to update the IQS stream format corresponding to the IQ stream.

	IQS_TriggerInfo_TypeDef IQS_TriggerInfo; // The IQS trigger information corresponding to the current IQ stream is updated through the IQS_GetIQStream function.
	DeviceInfo_TypeDef DeviceInfo;           // The IQS_GetIQStream function is used to update device information corresponding to the current IQ stream.
	DeviceState_TypeDef DeviceState;         // The IQS_GetIQStream function is used to update the device status corresponding to the IQ stream.

}IQStream_TypeDef; 


/*---------------------------------------------------
Detector mode structure (Detector)
-----------------------------------------------------*/
/*DET configuration structure (basic configuration)*/
typedef struct
{
	double CenterFreq_Hz; 									 // Center frequency.
	double RefLevel_dBm;									 // R.L.
	uint32_t DecimateFactor;								 // Decimate factor of time domain data.

	RxPort_TypeDef RxPort;									 // RF input port.

	uint32_t BusTimeout_ms;									 // Transmission timeout.

	DET_TriggerSource_TypeDef TriggerSource;				 // Input trigger source.
	TriggerEdge_TypeDef TriggerEdge;						 // Input trigger edge.

	TriggerMode_TypeDef	TriggerMode;						 // Input trigger mode.	
	uint64_t TriggerLength;									 // number of sampling points after the input trigger, only available in FixedPoints mode.

	double TriggerLevel_dBm;								 // Level trigger threshold.
	double TriggerTimer_Period;								 // Period of timing trigger				

	Detector_TypeDef Detector;  							 // Detection.
	uint16_t DetectRatio;  									 // Detection ratio, the detector detects the power trace, and each original data point is detected as 1 output trace point

	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action.

	uint8_t	AnalogIFBWGrade;								 //	IF bandwidth grade.

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	Reference clock source.
	double ReferenceClockFrequency;							 //	Reference clock frequency.

}DET_EZProfile_TypeDef;

/* Configuration structure of DET (configuration) */
typedef struct
{
	double CenterFreq_Hz; 									 // Center frequency.
	double RefLevel_dBm;									 // Reference level.
	uint32_t DecimateFactor;								 // Decimate factor of time domain data.

	RxPort_TypeDef RxPort;									 // RF input port.

	uint32_t BusTimeout_ms;									 // Transmission timeout.

	DET_TriggerSource_TypeDef TriggerSource;				 // Input trigger source.
	TriggerEdge_TypeDef TriggerEdge;						 // Input trigger edge.

	TriggerMode_TypeDef	TriggerMode;						 // Input trigger mode.	
	uint64_t TriggerLength;									 // The number of sampling points after the input trigger, only available in FixedPoints mode.

	TriggerOutMode_TypeDef TriggerOutMode;					 // Trigger output mode.
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // Trigger the output pulse polarity.

	double TriggerLevel_dBm;								 // Level trigger threshold.
	double TriggerLevel_SafeTime;							 // Safety time of level trigger anti-shaking, unit: s.
	double TriggerDelay;									 // Trigger delay, unit: s.
	double PreTriggerTime;									 // Pre-trigger time, unit: s.

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // Synchronization options of timer trigger and outer trigger edge.
	double TriggerTimer_Period;								 // Period of timed trigger				
	
	uint8_t EnableReTrigger;							     // Enable the device to respond multiple times after capturing a trigger. This function is available only in FixedPoint mode.
	double ReTrigger_Period;								 // The interval between multiple responses of the device is also used as the trigger period in the Timer trigger mode (unit: s).
	uint16_t ReTrigger_Count;				 	    		 // After a trigger, you need to make several responses in addition to the response brought by the trigger.

	Detector_TypeDef Detector;               				 // Detection.
	uint16_t DetectRatio;            						 // Detection ratio.

	GainStrategy_TypeDef GainStrategy;						 // Gain strategy.
	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action.

	uint8_t	AnalogIFBWGrade;								 //Intermediate frequency bandwidth grade.
	uint8_t	IFGainGrade;									 //	Intermediate frequency gain grade.

	uint8_t	EnableDebugMode;								 //	Debug mode. Advanced applications are not recommended. The default value is 0.

	ReferenceClockSource_TypeDef ReferenceClockSource;		 // Reference clock source.
	double ReferenceClockFrequency;						     //	Reference clock frequency.
	uint8_t EnableReferenceClockOut;						 // Enable reference clock output.

	SystemClockSource_TypeDef SystemClockSource; 		     // System clock source.
	double ExternalSystemClockFrequency; 				   	 // External system clock frequency: Hz.

	int8_t Atten;                                            // attenuation.

	DCCancelerMode_TypeDef DCCancelerMode;					 // Suitable for specific equipment. Dc suppression. 0: disables the DCC. 1: Open, high-pass filter mode (better suppression effect, but will damage the signal in the range of DC to 100 KHZ); 2: Open, manual bias mode (need manual calibration, but not low frequency damage signal).

	QDCMode_TypeDef	QDCMode;								 // Suitable for specific equipment. IQ amplitude and phase corrector. QDCOff: disables the QDC function. QDCManualMode: Enable and use manual mode; QDCAutoMode: Enables and uses the automatic QDC mode.

	float QDCIGain;											 // Suitable for specific equipment. Normalized linear gain I, 1.0 indicates no gain, set range 0.8 to 1.2.
	float QDCQGain;											 // Suitable for specific equipment. Normalized linear gain Q, 1.0 indicates no gain, set range 0.8~1.2.
	float QDCPhaseComp;										 // Suitable for specific equipment. Normalized phase compensation coefficient, set range -0.2~+0.2.

	int8_t DCCIOffset;										 // Suitable for specific equipment. I channel DC offset, LSB.
	int8_t DCCQOffset;										 // Suitable for specific equipment. Q channel DC offset, LSB.

	LOOptimization_TypeDef LOOptimization;					 // LO optimization

}DET_Profile_TypeDef;

/*Types of Resolution Bandwidth (RBW) Filters*/
typedef enum
{
	RBWFilter_80PercentABW = 0x00,
	RBWFilter_Gaussian_3dB = 0x01,
	RBWFilter_Gaussian_6dB = 0x02,
	RBWFilter_Gaussian_Impulse = 0x03,
	RBWFilter_Gaussian_Noise = 0x04,
	RBWFilter_Flattop = 0x05
}RBWFilterType_TypeDef;

/*Configuration structure (configuration) for DET*/
typedef struct
{
	double CenterFreq_Hz; 									 // center frequency
	double RefLevel_dBm;									 // Reference Level
	uint32_t DecimateFactor;								 // Decimation Factor for Time-Domain Data

	RxPort_TypeDef RxPort;									 // Radio Frequency (RF) Input Port

	uint32_t BusTimeout_ms;									 // Transmission Timeout Duration

	DET_TriggerSource_TypeDef TriggerSource;				 // Input Trigger Source
	TriggerEdge_TypeDef TriggerEdge;						 // Input Trigger Edge

	TriggerMode_TypeDef	TriggerMode;						 // Input Trigger Mode	
	uint64_t TriggerLength;									 // Number of Sampling Points After Input Trigger, Only Valid in FixedPoints Mode

	TriggerOutMode_TypeDef TriggerOutMode;					 // Trigger Output Mode
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // Trigger Output Pulse Polarity

	double TriggerLevel_dBm;								 // Threshold for Level Trigger
	double TriggerLevel_SafeTime;							 // Level Trigger: Anti-bounce Safety Time, Unit in Seconds
	double TriggerDelay;									 // Level Trigger: Trigger Delay, Unit in Seconds
	double PreTriggerTime;									 // Level Trigger: Pre-trigger Time, Unit in Seconds

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // Timed Trigger: Synchronization Option with External Trigger Edge
	double TriggerTimer_Period;								 // Timed Trigger: Period				

	uint8_t EnableReTrigger;							     // Auto-retrigger: Enables the device to respond multiple times after capturing a single trigger, only available in FixedPoint mode
	double ReTrigger_Period;								 // Auto-retrigger: Time Interval for Multiple Responses, Also Serving as Trigger Period in Timer Trigger Mode, Unit in Seconds
	uint16_t ReTrigger_Count;				 	    		 // Auto-retrigger: Number of Automatic Retriggers Executed After Each Original Trigger Action

	Detector_TypeDef Detector;  							 // Detector
	uint16_t DetectRatio;  									 // Detection Ratio: The detector processes the power trace by sampling, with each DetectRatio of original data points resulting in one output trace point

	GainStrategy_TypeDef GainStrategy;						 // Gain strategy
	PreamplifierState_TypeDef Preamplifier;					 // Preamp action

	uint8_t	AnalogIFBWGrade;								 //	Intermediate Frequency Bandwidth Setting
	uint8_t	IFGainGrade;									 //	Intermediate Frequency Gain Setting

	uint8_t	EnableDebugMode;								 //	Debug Mode: Not Recommended for Advanced Applications to Be Used by Users Directly, Default Value is 0

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	Reference Clock Source
	double ReferenceClockFrequency;							 //	Reference Clock Frequency
	uint8_t EnableReferenceClockOut;						 // Enable Reference Clock Output

	SystemClockSource_TypeDef SystemClockSource; 		     // System Clock Source
	double ExternalSystemClockFrequency; 				   	 // External System Clock Frequency, Hz

	int8_t Atten;                                            // Attenuation

	DCCancelerMode_TypeDef DCCancelerMode;					 // Applicable to specific devices. DC suppression. 0: DCC Off; 1: On, High-pass Filter Mode (better suppression effect, but may cause damage to signals within the DC to 100kHz range); 2: On, Manual Bias Mode (requires manual calibration, but does not damage low-frequency signals)

	QDCMode_TypeDef	QDCMode;								 // Applicable to specific devices. IQ Amplitude and Phase Corrector. QDCOff: Disable QDC function; QDCManualMode: Enable and use manual mode; QDCAutoMode: Enable and use automatic QDC mode.

	float QDCIGain;											 // Applicable to specific devices. Normalized Linear Gain for I Channel: 1.0 indicates no gain, with a setting range of 0.8 to 1.2
	float QDCQGain;											 // Applicable to specific devices. Normalized Linear Gain for Q Channel: 1.0 indicates no gain, with a setting range of 0.8 to 1.2
	float QDCPhaseComp;										 // Applicable to specific devices. Normalized Phase Compensation Coefficient: Setting range is -0.2 to +0.2

	int8_t DCCIOffset;										 // Applicable to specific devices. DC Offset for I Channel, in LSB
	int8_t DCCQOffset;										 // Applicable to specific devices. DC Offset for Q Channel, in LSB

	LOOptimization_TypeDef LOOptimization;					 // Local Oscillator Optimization

	double RBW_Hz;											 // Resolution Bandwidth
	double VBW_Hz;											 // Video Bandwidth
	VBWMode_TypeDef VBWMode;								 // Update Method for VBW
	RBWFilterType_TypeDef RBWFilterType;					 // Types of RBW Filters

}ZSP_Profile_TypeDef;


/* The stream information structure returned after the DET configuration (returned) */
typedef struct
{
	uint64_t PacketCount;	 // The total number of data packets corresponding to the current configuration takes effect only in FixedPoints mode.
	
	uint64_t StreamSamples;  // In Fixedpoints mode, it represents the total number of sampling points corresponding to the current configuration. In Adaptive mode, the value has no physical significance and is 0.
	uint64_t StreamDataSize; // In Fixedpoints mode, it indicates the total number of bytes of samples corresponding to the current configuration. In Adaptive mode, the value has no physical significance and is 0.

	uint32_t PacketSamples;  // Sampling points in packets obtained by each call to DET_GetTrace Indicates the sample points contained in each packet.
	uint32_t PacketDataSize; // The number of bytes of valid data obtained from each call to DET_GetTrace.
	double TimeResolution;   // Time-domain point resolution
	uint32_t GainParameter;	 // Gain related parameters, including Space(31 to 24Bit), PreAmplifierState(23 to 16Bit), StartRFBand(15 to 8Bit), StopRFBand(7 to 0Bit).
}DET_StreamInfo_TypeDef;

/* DET A top-level structure containing configuration, return information, and staging data (configuration, return, staging) */
typedef struct
{
	float* NormalizedPowerStream;            // The normalized power flow stored in the Device pointer
	float DET_ScaleToV;                      // Coefficient from type int to absolute value of voltage (V).

	uint32_t MaxIndex;						 // Index of the maximum power value in the current packet.
	float	 MaxPower_dBm;					 // The maximum power of the current packet.


	DET_Profile_TypeDef DET_Profile;         // Indicates the DET configuration information corresponding to the current DET stream. Det_profileout is usually used to update the DET configuration information.
	DET_StreamInfo_TypeDef DET_StreamInfo;   // Indicates the DET format information corresponding to the current DET stream. Det_profileout is usually used to update the DET format information.

	DET_TriggerInfo_TypeDef DET_TriggerInfo; // DET stream information corresponding to the current DET stream.
	DeviceInfo_TypeDef DeviceInfo;           // Information about the device corresponding to the current DET stream.
	DeviceState_TypeDef DeviceState;         // State about the device corresponding to the current DET stream.

}DETStream_TypeDef;

/*---------------------------------------------------
Real-Time Analysis mode structure (RTA)
-----------------------------------------------------*/
/*RTA configuration structure (basic configuration)*/
typedef struct
{
	double CenterFreq_Hz; 									 // Center frequency.
	double RefLevel_dBm;									 // Reference level.
	double RBW_Hz;											 // RBW
	double VBW_Hz;											 // VBW

	RBWMode_TypeDef RBWMode; 							 	 // RBW update mode, mannually, according to the span.
	VBWMode_TypeDef VBWMode;							 	 // VBW updated mode,mannually,VBW = RBW,VBW = 0.1*RBW, VBW = 0.01*RBW

	uint32_t DecimateFactor;								 // Decimate factor of time domain data.

	Window_TypeDef	Window;									 // Window type.

	SweepTimeMode_TypeDef SweepTimeMode;					 // Sweep time mode.
	double SweepTime;										 // When the sweep time mode is Manual, the parameter is absolute time. When specified as *N, this parameter is the scan time multiplier
	Detector_TypeDef Detector;								 // Detector

	TraceDetectMode_TypeDef TraceDetectMode;				 // Trace detection mode.
	TraceDetector_TypeDef TraceDetector;  				     // Trace detector.
	uint32_t TraceDetectRatio;  						     // Trace detection ratio.The trace detector detects one output spectrum data point per TraceDetectRatio original spectrum data point.

	RxPort_TypeDef	RxPort;									 // Receiving port setting.

	uint32_t BusTimeout_ms;									 // Transmission timeout.

	RTA_TriggerSource_TypeDef TriggerSource;				 // Input trigger source.
	TriggerEdge_TypeDef TriggerEdge;						 // Input trigger edge.

	TriggerMode_TypeDef	TriggerMode;						 // Input trigger mode.
	double TriggerAcqTime;							     	 // The sampling time after the input is triggered takes effect only in FixedPoints mode.

	double TriggerLevel_dBm;								 // Level trigger threshold.
	double TriggerTimer_Period;								 // Timing trigger period,s.					

	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action.

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	Reference clock source.
	double ReferenceClockFrequency;							 //	Reference clock frequency.

}RTA_EZProfile_TypeDef;

/* Configuration structure of RTA (Configuration) */
typedef struct
{
	double CenterFreq_Hz; 									 // Center frequency.
	double RefLevel_dBm;									 // Reference level.
	double RBW_Hz;											 // RBW
	double VBW_Hz;											 // VBW
	RBWMode_TypeDef RBWMode; 							 	 // RBW update mode. Input manually, set automatically according to Span.
	VBWMode_TypeDef VBWMode;							 	 // VBW update mode. Manual input, VBW = RBW, VBW = 0.1*RBW, VBW = 0.01*RBW.

	uint32_t DecimateFactor;								 // Decimate factor of time domain data.

	Window_TypeDef	Window;									 // Window type.

	SweepTimeMode_TypeDef SweepTimeMode;					 // sweep time mode
	double SweepTime;										 // When the sweep time mode is Manual, the parameter is absolute time. When specified as *N, this parameter is the scan time multiplier
	Detector_TypeDef Detector;								 // Detector

	TraceDetectMode_TypeDef TraceDetectMode;				 // trace detection mode
	uint32_t TraceDetectRatio;  						     // Trace detection ratio.
	TraceDetector_TypeDef TraceDetector;  				 // Trace detector.

	RxPort_TypeDef	RxPort;									 // Receiving port setting.

	uint32_t BusTimeout_ms;									 // Transmission timeout.

	RTA_TriggerSource_TypeDef TriggerSource;				 // Input trigger source.
	TriggerEdge_TypeDef TriggerEdge;						 // Input trigger edge.

	TriggerMode_TypeDef	TriggerMode;						 // Input trigger mode.
	double TriggerAcqTime;							     	 // The sampling time after the input is triggered takes effect only in FixedPoints mode.

	TriggerOutMode_TypeDef TriggerOutMode;					 // Trigger output mode.
	TriggerOutPulsePolarity_TypeDef	TriggerOutPulsePolarity; // Trigger the output pulse polarity.

	double TriggerLevel_dBm;								 // Level trigger threshold.
	double TriggerLevel_SafeTime;							 // Safety time of level trigger anti-shaking, unit: s.
	double TriggerDelay;									 // Trigger delay, unit: s.
	double PreTriggerTime;									 // Pre-trigger time, unit: s.

	TriggerTimerSync_TypeDef TriggerTimerSync;			     // Timer trigger and outer trigger edge synchronization options.
	double TriggerTimer_Period;								 // Timing trigger period.					

	uint8_t EnableReTrigger;							     // Enable the device to respond multiple times after capturing a trigger. This function is available only in FixedPoint mode.
	double ReTrigger_Period;								 // The interval between multiple responses of the device is also used as the trigger period in the Timer trigger mode (unit: s).
	uint16_t ReTrigger_Count;				 	    		 // After a trigger, you need to make several responses in addition to the response brought by the trigger.

	GainStrategy_TypeDef GainStrategy;						 // Gain strategy.
	PreamplifierState_TypeDef Preamplifier;					 // Preamplifier action.
	uint8_t AnalogIFBWGrade;								 //	Intermediate frequency bandwidth grade.
	uint8_t	IFGainGrade;									 //	Intermediate frequency gain grade.

	uint8_t	EnableDebugMode;								 //	Debug mode. Advanced applications are not recommended. The default value is 0.

	ReferenceClockSource_TypeDef ReferenceClockSource;		 //	Reference clock source.
	double ReferenceClockFrequency;							 //	Reference clock frequency.
	uint8_t EnableReferenceClockOut;						 // Enable reference clock output.

	SystemClockSource_TypeDef SystemClockSource; 		     // System clock source.
	double ExternalSystemClockFrequency; 				   	 // External system clock frequency: Hz.

	int8_t Atten;                                            // attenuation.

	DCCancelerMode_TypeDef DCCancelerMode;					 // Suitable for specific equipment. Dc suppression. 0: disables the DCC. 1: Open, high-pass filter mode (better suppression effect, but will damage the signal in the range of DC to 100 KHZ); 2: Open, manual bias mode (need manual calibration, but not low frequency damage signal).

	QDCMode_TypeDef	QDCMode;								 // Suitable for specific equipment. IQ amplitude and phase corrector. QDCOff: disables the QDC function. QDCManualMode: Enable and use manual mode; QDCAutoMode: Enables and uses the automatic QDC mode.

	float QDCIGain;											 // Suitable for specific equipment. Normalized linear gain I, 1.0 indicates no gain, set range 0.8 to 1.2.
	float QDCQGain;											 // Suitable for specific equipment. Normalized linear gain Q, 1.0 indicates no gain, set range 0.8~1.2.
	float QDCPhaseComp;										 // Suitable for specific equipment. Normalized phase compensation coefficient, set range -0.2~+0.2.

	int8_t DCCIOffset;										 // Suitable for specific equipment. I channel DC offset, LSB.
	int8_t DCCQOffset;										 // Suitable for specific equipment. Q channel DC offset, LSB.

	LOOptimization_TypeDef LOOptimization;					 // LO optimization.

}RTA_Profile_TypeDef;

/* Package information structure returned after RTA configuration (Returned) */
typedef struct
{
	double StartFrequency_Hz;	 // The start frequency of the spectrum.	
	double StopFrequency_Hz;     // The stop frequency of the spectrum.
	double POI;				     // The shortest duration of the signal with 100% probability of interception, unit: s.

	double TraceTimestampStep;   // Timestamp step of each Trace in each packet of data. (package overall timestamp in TriggerInfo SysTimerCountOfFirstDataPoint).
	double TimeResolution;	 	 // The sampling time of each time domain data which is also the resolution of the timestamp.
	double PacketAcqTime;	 	 // Data collection time of each packet.

	uint32_t PacketCount;        // The total number of data packets corresponding to the current configuration takes effect only in FixedPoints mode.
	uint32_t PacketFrame;        // The number of valid frames per packet.
	uint32_t FFTSize;            // The number of FFTS per frame.
	uint32_t FrameWidth;		 // The number of points after FFT frame interception is also the number of points of each Trace in the packet, which can be used as the number of points on the X-axis of the probability density graph (width).
	uint32_t FrameHeight;		 // The spectrum amplitude range corresponding to the FFT frame can be used as the number of Y-axis points (height) of the probability density map.

	uint32_t PacketSamplePoints; // Number of collection points corresponding to each packet of data.
	uint32_t PacketValidPoints;	 // The number of valid data points in the frequency domain contained in each packet.

	uint32_t MaxDensityValue;	 // Upper limit of individual site element value of probability density bitmap.
	uint32_t GainParameter;		 // Include Space(31~24Bit),PreAmplifierState(23~16Bit),StartRFBand(15~8Bit),StopRFBand(7~0Bit)
}RTA_FrameInfo_TypeDef;

/* The drawing information structure returned by RTA after obtaining it */
typedef struct
{
	float ScaleTodBm;	          // Compression from linear power to logarithmic power. The absolute power of Trace is equal to Trace[] * ScaleTodBm + OffsetTodBm(the absolute power axis of Bitmap is the same below).
	float OffsetTodBm;            // The shift of relative power into absolute power. The absolute power axis range of bitmap (Y-axis) is equal to FrameHeigh * ScaleTodBm + OffsetTodBm(Trace physical power ditto above).
	uint64_t SpectrumBitmapIndex; // The number of times a probability density map is obtained, which can be used as an index when plotting.
}RTA_PlotInfo_TypeDef;

/* RTA A top-level structure containing configuration, return information, and staging data (configuration, return, staging) */
typedef struct
{
	uint8_t* SpectrumStream;                 // The address of the RTA spectrum data stored in the Device pointer.
	uint16_t* SpectrumBitmap;                // The address of the RTA spectrum bitmap data stored in the Device pointer.

	uint32_t MaxIndex;						 // Index of the maximum power value in the current packet.
	float	 MaxPower_dBm;					 // The maximum power of the current packet.

	RTA_Profile_TypeDef RTA_Profile;         // RTA configuration information corresponding to the current RTA flow is updated through RTA_Configuration (RTA_ProfileOut).
	RTA_FrameInfo_TypeDef RTA_FrameInfo;     // The RTA package information corresponding to the current RTA flow is updated through the RTA_Configuration function.

	RTA_PlotInfo_TypeDef RTA_PlotInfo;       // RTA drawing information corresponding to the current RTA flow.
	RTA_TriggerInfo_TypeDef RTA_TriggerInfo; // RTA trigger information corresponding to the current RTA flow.
	DeviceInfo_TypeDef DeviceInfo;           // Information about the device corresponding to the current RTA flow.
	DeviceState_TypeDef DeviceState;         // Indicates the device status corresponding to the current RTA flow.

}RTAStream_TypeDef;

/*---------------------------------------------------
Multi-profile schema Association (MPS)
-----------------------------------------------------*/
typedef union
{
	/* Data of only one mode can be obtained at a time. Analysis is returned to indicate the obtained mode, and ProfileNum is returned to indicate the obtained configuration number */
	SWPTrace_TypeDef SWPSpectrum;
	IQStream_TypeDef IQStream;
	DETStream_TypeDef DETStream;
	RTAStream_TypeDef RTAStream;

}MPSData_TypeDef;

/*---------------------------------------------------
Multi-profile Schema Data Information (MPS)
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
Calibration correlation (CAL)
-----------------------------------------------------*/

/*CAL's configuration struct (configuration)*/
typedef struct
{
	double	RFCFreq_Hz;			   // Device RF center frequency: Hz.
	double	BBCFreq_Hz;			   // Frequency point of calibration signal selected, Hz.
	float	Temperature;		   // Equipment temperature.

	float	RefLevel_dBm;		   // Device reference level, dBm.
	float	CalSignalLevel_dBm;	   // Calibration signal power, dBm.

	float	QDCIGain;			   // IGain optimization value obtained.
	float	QDCQGain;			   // QGain optimization value obtained.
	float	QDCPhaseComp;		   // The PhaseComp optimization value is obtained.
	float	EstimatedOptRejection; // The expected sideband suppression value obtained after the optimization value is substituted.

}CAL_QDCOptParam_TypeDef;

/*---------------------------------------------------
Source functional struct (Signal)
-----------------------------------------------------*/

/* Manual gain control structure in emission mode. The secondary parameter takes effect when the emission source works in SIG_ManualGainCtrl mode */
typedef struct
{
	uint8_t TxPreDSA;      // Preattenuator value; Input range: 0-127; Step 1.
	uint8_t TxPostDSA;     // Rear attenuator value; Input range: 0-127; Step 1.
	uint8_t TxAuxDSA;      // Auxiliary attenuator value; Input range: 0-127; Step 1.
	uint8_t PreAmplifier;  // Preamplifier status: 0: off; 1: Turn on.
	uint8_t PostAmplifier; // Post-amplifier status: 0: off; 1: Turn on.
}TxManualGain_TypeDef;

/*The playback mode of the signal generator IQStream*/
typedef enum
{
	SIG_IQStreamPlayMode_Null = 0x00,		   // Real-time transmission and playback mode.
	SIG_IQStreamPlayMode_RealTime = 0x01,	   // Real-time transmission and playback mode.
	SIG_IQStreamPlayMode_SinglePlay = 0x02,    // Single play.
	SIG_IQStreamPlayMode_ContinousPlay = 0x04, // Continuous play.
	SIG_IQStreamPlayMode_MultiPlay = 0x08,	   // Multiple play.
	SIG_IQStreamPlayMode_Simulation = 0x10 	   // Simulation signal.

}SIG_IQStreamPlayMode_TypeDef;

/*Signal generator IQStream playback trigger source*/
typedef enum
{
	SIG_IQStreamPlayTrigger_Null = 0x00,			// No trigger; Wait for rematch to another trigger type.
	SIG_IQStreamPlayTrigger_Internal = 0x01, 		// Internal trigger.
	SIG_IQStreamPlayTrigger_External = 0x02, 		// External trigger.
	SIG_IQStreamPlayTrigger_Timer = 0x04,   		// Timing trigger.
	SIG_IQStreamPlayTrigger_RFBoard = 0x08,  		// Timing trigger.
	SIG_IQStreamPlayTrigger_Bus = 0x10,				// Bus trigger.

}SIG_IQStreamPlayTrigger_TypeDef;

/*SIG mode user interface definition(configuration)*/
typedef struct
{
	double	CenterFreq_Hz;										// The current center frequency (unit: Hz) takes effect when the signal generator works in SIG_Fixed mode; Input range 1M-1GHz; Step 1Hz.
	double	StartFreq_Hz;										// The initial frequency in frequency sweep mode, expressed in Hz, takes effect when the signal generator is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double	StopFreq_Hz;										// The termination frequency in frequency sweep mode, expressed in Hz, takes effect when the signal generator is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double	StepFreq_Hz;										// Step frequency in frequency sweep mode, expressed in Hz, takes effect when the signal generator works in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double	CurrentLevel_dBm;									// The current power, in dBm, takes effect when the signal generator is working in SIG_Fixed mode; Input range -127 to -5dBm; Step by 0.25dB.
	double	StartLevel_dBm;										// The unit is Hz. This parameter takes effect when the signal generator works in SIG_PowerSweep_* mode. Input range -127 to -5dBm; Step by 0.25dB.
	double	StopLevel_dBm;										// The termination power in power sweep mode, expressed in Hz, takes effect when the signal generator works in SIG_PowerSweep_* mode. Input range -127 to -5dBm; Step by 0.25dB.
	double	StepLevel_dBm;										// Step power in power sweep mode, expressed in Hz, takes effect when the signal generator works in SIG_PowerSweep_* mode. Input range -127 to -5dBm; Step by 0.25dB.
	double	DwellTime_ms;										// Sweep dwell time, effective when the generator works in the *Sweep* mode; Input range 0-1000000; Step 1.
	double  DACSampleRate;										// Specifies the sampling rate for the DAC.
	double	InterpolationFactor;								// signal generator baseband interpolation multiple, this parameter determines the baseband bandwidth of the signal generator, input range :1-1024; Input is an even number except for 1.
	double  ReferenceClockFrequency;							// Reference input frequency in signal generator mode; Currently, this parameter only supports 10MHz reference frequency input. Tuning this parameter can compensate the frequency bias of internal or external reference frequency.
	ReferenceClockSource_TypeDef ReferenceClockSource;          // Reference frequency input source; 0: internal 1: external.
	SIG_OperationMode_TypeDef OperationMode;					// Working mode of signal generator.
	SIG_SweepSource_TypeDef SweepSource;						// Signal generator  Scan source.

	SIG_IQStreamPlayMode_TypeDef	SIG_IQStreamPlayMode;	 // Play mode of signal generator IQStream.
	SIG_IQStreamPlayTrigger_TypeDef	SIG_IQStreamPlayTrigger; // Trigger of plays of the signal generator IQStream.
	uint32_t	SIG_IQStreamPlayLength;						 // Length of plays of the signal generator IQStream.
	uint32_t	SIG_IQStreamPlayCounts;						 // Number of plays of the signal generator IQStream.
	uint32_t SIG_IQStreamPlayPrevDelay;						 // The preview delay of signal generator IQStream playback is effective for each playback.
	uint32_t SIG_IQStreamPlayPostDelay;						 // The post delay of signal generator IQStream playback is effective for each playback.

	uint32_t SIG_IQStreamDownloadStartAddress;				 // The signal generator IQStream is downloaded to the starting memory address in the onboard memory.
	uint32_t SIG_IQStreamDownloadStopAddress;				 // The signal generator IQStream is downloaded to the stopping  memory address in the onboard memory.

	uint32_t SIG_IQStreamPlayStartAddress;					 // The signal generator reads the starting memory address of IQStream from the onboard memory.
	uint32_t SIG_IQStreamPlayStopAddress;					 // The signal source reads the stopping memory address of IQStream from the onboard memory.

	int16_t SIG_IQStreamIdle_DC_I;			 	// Silent DC value of channel I signal in non-play mode.
	int16_t SIG_IQStreamIdle_DC_Q;			 	// Silent DC value of the Q channel signal in non-play mode.

	int16_t SIG_IQStream_DC_Offset_I;			 // The DC offset of the I signal ranges from -32768 to +32767.
	int16_t SIG_IQStream_DC_Offset_Q;			 // The DC offset of the Q signal ranges from -32768 to +32767.

	double	SIG_IQStream_Gain_I;				 // Linear gain of channel I signals: The gain ranges from -256 to 255.
	double	SIG_IQStream_Gain_Q;				 // Linear gain of the Q-channel signal: The gain ranges from -256 to 255.

	double  SIG_IQStreamSimFrequency;			 // When SIG_IQStreamPlayMode=Simualtion mode, specify the baseband frequency of emulation.

	TxPort_TypeDef TxPort;                       // signal generator output port.
	TxTriggerInSource_TypeDef TxTriggerInSource; // The signal generator triggers the input source.
	TxTriggerInMode_TypeDef TxTriggerInMode;     // signal generator output input mode.
	TxTriggerOutMode_TypeDef TxTriggerOutMode;   // The signal generator triggers the output mode.
	TxAnalogIQSource_TypeDef TxAnalogIQSource;   // signal generator simulates IQ input source.
	TxDigitalIQSource_TypeDef TxDigitalIQSource; // signal generator Digital QI input source.
	TxTransferReset_TypeDef TransferResetCmd;    // signal generator baseband transmission reset status.
	TxPackingCmd_TypeDef	TxPackingCmd;		 // Indicates the packet mode status of the signal source.
	TxManualGain_TypeDef TxManualGain;           // Source manual gain control.
}SIG_Profile_TypeDef;

/* Source Mode Return information (to be updated)(Return) */
typedef struct
{
	uint32_t SweepPoints; // scan points
}SIG_Info_TypeDef;

/*---------------------------------------------------
Analog Signal Generator(subfunction of SIG)
-----------------------------------------------------*/

/*working mode of ASG*/
typedef enum
{
	ASG_Mute = 0x00,		   // mute.
	ASG_FixedPoint = 0x01, 	   // Fixed point.
	ASG_FrequencySweep = 0x02, // Frequency sweep.
	ASG_PowerSweep = 0x03 	   // Power sweep.

}ASG_Mode_TypeDef;

/*Input trigger source of ASG*/
typedef enum
{
	ASG_TriggerIn_FreeRun = 0x00,  // Free running.
	ASG_TriggerIn_External = 0x01, // External trigger.
	ASG_TriggerIn_Bus = 0x02       // Timer trigger.

}ASG_TriggerSource_TypeDef;

/*Input trigger mode of ASG*/
typedef enum
{
	ASG_TriggerInMode_Null = 0x00,		  // Free running.
	ASG_TriggerInMode_SinglePoint = 0x01, // Single point trigger (a configuration of frequency or power that is triggered once for a single time).
	ASG_TriggerInMode_SingleSweep = 0x02, // Single scan trigger (trigger one cycle scan at a time).
	ASG_TriggerInMode_Continous = 0x03    // Continuous scan trigger (trigger one continuous operation).
}ASG_TriggerInMode_TypeDef;

/*Trigger output port of ASG*/
typedef enum
{
	ASG_TriggerOut_Null = 0x00, 	// external
	ASG_TriggerOut_External = 0x01, // external
	ASG_TriggerOut_Bus = 0x02 		// internal
}ASG_TriggerOutPort_TypeDef;

/*Trigger output mode of ASG*/
typedef enum
{
	ASG_TriggerOutMode_Null = 0x00,        // Free running.
	ASG_TriggerOutMode_SinglePoint = 0x01, // Single point trigger (a configuration of frequency or power that is triggered once for a single time).
	ASG_TriggerOutMode_SingleSweep = 0x02  // Single scan trigger (trigger one cycle scan at a time).
}ASG_TriggerOutMode_TypeDef;

/*RF port of ASG*/
typedef enum
{
	ASG_Port_External = 0x00, // external
	ASG_Port_Internal = 0x01, // internal
	ASG_Port_ANT = 0x02,	  // ANT Port (TRx series only).
	ASG_Port_TR = 0x03,       // TR Port (TRx series only).
	ASG_Port_SWR = 0x04,      // SWR Port (TRx series only).
	ASG_Port_INT = 0x05       // INT Port (TRx series only).

}ASG_Port_TypeDef;

/*Configuration struck of ASG(configuration )lu*/
typedef struct
{
	double   CenterFreq_Hz; 						   // The current center frequency (unit: Hz) takes effect when the signal generator works in SIG_Fixed mode; Input range 1M-1GHz; Step 1Hz.
	double   Level_dBm; 						  	   // The current power, in dBm, takes effect when the signal generator is working in SIG_Fixed mode; Input range -127 to -5dBm; Step by 0.25dB.

	double   StartFreq_Hz; 						  	   // The initial frequency in frequency sweep mode, expressed in Hz, takes effect when the signal generator is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double   StopFreq_Hz; 							   // The termination frequency in frequency sweep mode, expressed in Hz, takes effect when the signal generator is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double   StepFreq_Hz; 							   // Step frequency in frequency sweep mode, expressed in Hz, takes effect when the signal generator works in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.

	double   StartLevel_dBm; 					  	   // Start power in power sweep mode (unit: Hz).
	double   StopLevel_dBm; 						   // End power in power sweep mode (unit: Hz).
	double   StepLevel_dBm; 						   // Power Step power in sweep mode (unit: Hz).
	
	double   DwellTime_s; 						  	   // In frequency Sweep mode or power sweep mode, the unit is s. When the triggering mode is BUS, the sweep dwell time, the unit is s, takes effect when the signal generator works in the *Sweep* mode. Input range 0-1000000; Step 1.
	
	double ReferenceClockFrequency;					   // Specified reference frequency: both internal and external references are effective.
	ReferenceClockSource_TypeDef ReferenceClockSource; // Select the input source of the reference clock: internal reference or external reference.

	ASG_Port_TypeDef Port; 							   // signal generator output port.

	ASG_Mode_TypeDef	Mode; 						   // Off, dot frequency, frequency scan (external trigger, synchronize to receive), power scan (external trigger, synchronize to receive).										  

	ASG_TriggerSource_TypeDef	TriggerSource; 		   // signal generator trigger input mode.
	ASG_TriggerInMode_TypeDef	TriggerInMode;		   // Trigger mode of the signal source.
	ASG_TriggerOutMode_TypeDef	TriggerOutMode;		   // Trigger mode of signal source.
	
}ASG_Profile_TypeDef;

/* Analog Source Mode Return Information (to be updated)(Return) */
typedef struct
{
	uint32_t SweepPoints; 			// Scan Points
}ASG_Info_TypeDef;

/*---------------------------------------------------
Vector Signal Generator(subfunction of SIG)
-----------------------------------------------------*/

/*working mode of VSG*/
typedef enum
{
	VSG_Mode_Null = 0x00,		 // Mute
	VSG_Mode_RealTime = 0x01,	 // Real time mode.
	VSG_Mode_PlayFromRAM = 0x02, // Reads data from memory and plays it back.
	VSG_Mode_SimulatedCW = 0x04, // Continuous wave patterns generated by internal simulation.
	VSG_Mode_Advanced = 0x08	 // Advanced Mode (not yet available).

}VSG_Mode_TypeDef;

/*Trigger inter source of VSG*/
typedef enum
{
	VSG_TriggerSource_Null = 0x00,     					// No trigger; Wait for rematch to another trigger type.
	VSG_TriggerSource_Internal = 0x01, 					// Internal trigger.
	VSG_TriggerSource_External = 0x02,					// External trigger.
	VSG_TriggerSource_Timer = 0x04,    					// Timing trigger.
	VSG_TriggerSource_RFBoard = 0x08,  					// RF Board trigger.
	VSG_TriggerSource_Bus = 0x10	  					// Bus trigger.

}VSG_TriggerSource_TypeDef;

/*RF output port of VSG*/
typedef enum
{
	VSG_Port_External = 0x00, // External port.
	VSG_Port_Internal = 0x01, // Internal port.
	VSG_Port_ANT = 0x02,      // ANT Port (TRx series only).
	VSG_Port_TR = 0x03,	      // TR Port (TRx series only).
	VSG_Port_SWR = 0x04,      // SWR Port (TRx series only).
	VSG_Port_INT = 0x05       // INT Port (TRx series only).

}VSG_Port_TypeDef;

/*configuration struck of VSG (configuration)*/
typedef struct
{
	double CenterFrequency;			                   // Center frequency of the VSG.
	double Outlevel;				                   // Power output of the VSG.
	double DACSampleRate;			                   // Sample rate of the VSG.
	double InterpolationFactor;		                   // Interpolation factor of the VSG.
	uint32_t Triggerlength;			                   // The length of the IQStream playback of the VSG.
	int32_t TriggerCount;			                   // The number of times the IQStream playback of the VSG.
	uint32_t TriggerDelay_Prev;		                   // The preview delay of IQStream playback is effective for each playback.
	uint32_t TriggerDelay_Post; 	                   // The post delay of signal generator IQStream playback is effective for each playback.
	uint32_t PlayStartAddress;		                   // The VSG IQStream is downloaded to the starting memory address in the onboard memory.
	uint32_t PlayStopAddress;		                   // The VSG IQStream is downloaded to the stopping memory address in the onboard memory.
	double ReferenceClockFrequency;					   // Reference input frequency in signal source mode; Currently, this parameter only supports 10MHz reference frequency input. Tuning this parameter can compensate the frequency bias of internal or external reference frequency.
	ReferenceClockSource_TypeDef ReferenceClockSource; // The reference clock source of the VSG.
	VSG_Mode_TypeDef Mode;							   // The working mode of the VSG.
	VSG_Port_TypeDef Port;							   // The RF output port of the VSG.	
	VSG_TriggerSource_TypeDef TriggerSource;		   // The trigger source of the VSG.
}VSG_Profile_TypeDef;

/*VSG mode advanced parameters (configuration), only for VSG_Mode_Advanced mode*/
typedef struct
{
	double	StartFreq_Hz;                        // The start frequency in frequency sweep mode, expressed in Hz, takes effect when the signal source is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double	StopFreq_Hz;                         // The stop frequency in frequency sweep mode, expressed in Hz, takes effect when the signal source is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double	StepFreq_Hz;                         // The step frequency in frequency sweep mode, expressed in Hz, takes effect when the signal source is operating in SIG_FreqSweep_* mode. Input range 1M-1GHz; Step 1Hz.
	double	StartLevel_dBm;                      // The starting power in power sweep mode, expressed in Hz, takes effect when the signal source works in SIG_PowerSweep_* mode. Input range -127 to -5dBm; Step by 0.25dB.
	double	StopLevel_dBm;                       // The stopping power in power sweep mode, expressed in Hz, takes effect when the signal source works in SIG_PowerSweep_* mode. Input range -127 to -5dBm; Step by 0.25dB.
	double	StepLevel_dBm;                       // The step power in power sweep mode, expressed in Hz, takes effect when the signal source works in SIG_PowerSweep_* mode. Input range -127 to -5dBm; Step by 0.25dB.
	double	DwellTime_ms;                        // Sweep dwell time, effective when the source works in the *Sweep* mode; Input range 0-1000000; Step 1.

	SIG_OperationMode_TypeDef OperationMode;     // Working mode of signal source.
	SIG_SweepSource_TypeDef SweepSource;         // Signal source Scan source.

	int16_t SIG_IQStreamIdle_DC_I;			 	 // Silent DC value of channel I signal in non-play mode.
	int16_t SIG_IQStreamIdle_DC_Q;			 	 // Silent DC value of the Q channel signal in non-play mode.

	int16_t SIG_IQStream_DC_Offset_I;			 // The DC offset of the I signal ranges from -32768 to +32767.
	int16_t SIG_IQStream_DC_Offset_Q;			 // The DC offset of the Q signal ranges from -32768 to +32767.

	double	SIG_IQStream_Gain_I;				 // Linear gain of channel I signals: The gain ranges from -256 to 255.
	double	SIG_IQStream_Gain_Q;				 // Linear gain of the Q-channel signal: The gain ranges from -256 to 255.
	double  SIG_IQStreamSimFrequency;			 // When SIG_IQStreamPlayMode=Simualtion mode, specify the baseband frequency of emulation.

	TxTriggerInSource_TypeDef TxTriggerInSource; // The signal source triggers the input source.
	TxTriggerInMode_TypeDef TxTriggerInMode;     // Signal source output input mode.
	TxTriggerOutMode_TypeDef TxTriggerOutMode;   // The signal source triggers the output mode.
	TxAnalogIQSource_TypeDef TxAnalogIQSource;   // Signal source simulates IQ input source.
	TxDigitalIQSource_TypeDef TxDigitalIQSource; // Signal source Digital QI input source.
	TxTransferReset_TypeDef TransferResetCmd;    // Signal source baseband transmission reset status.
	TxPackingCmd_TypeDef	TxPackingCmd;		 // Indicates the packet mode status of the signal source.
	TxManualGain_TypeDef TxManualGain;           // Source manual gain control.
}VSG_PrivateParam_TypeDef;

/*VSG parameters return information (return)*/
typedef struct
{
	uint32_t PlayAddressRange; 		//Scan points
}VSG_Info_TypeDef;

/*VSG file format*/
typedef enum
{
	VSG_FileFormat_wav = 0x00, // wav format.
	VSG_FileFormat_dat = 0x01  // dat format.

}VSG_FileFormat_TypeDef;

/*VSG information returned from playing the file (return)*/
typedef struct
{
	uint32_t FileSize_Byte; 	   // File size (bytes).
	uint32_t IQStreamLength;	   // The length of IQstream(sampling points).
	uint32_t PlayStartAddress;	   // The starting address of the on-board memory.
	uint32_t PlayStopAddress;	   // The stopping address of the on-board memory.
	uint32_t NextFileStartAddress; // Indicates the starting memory address at which the next file or segment can be downloaded and played.
	uint32_t PlayAddressRange;	   // The address range for the file to play.	
	uint32_t AddressOverRangeFlag; // This flag is set when the write address exceeds the maximum address.
	uint32_t File_Format;		   // File format.
	double	 FileSampleRate;	   // The sampling rate of the IQStream in the file.

}VSG_FileInfo_TypeDef;

/*---------------------------------------------------
Vector Network Analyzer mode struck(VNA)
-----------------------------------------------------*/

/* Vector Network Analyzer Mode User Interface Definition (Configuration) */
typedef struct
{
	double	CenterFreq_Hz;							   // When set to power scan or fixed mode, this parameter is the center frequency; Input range 1M-1GHz; Step 1Hz.
	double	StartFreq_Hz;							   // Initial frequency in S11/S21 scanning mode, expressed in Hz; Input range 1M-1GHz; Step 1Hz.
	double	StopFreq_Hz;							   // Termination frequency in S11/S21 scanning mode, expressed in Hz; Input range 1M-1GHz; Step 1Hz.
	double	StepFreq_Hz;							   // Step frequency in S11/S21 scanning mode, expressed in Hz; Input range 1M-1GHz; Step 1Hz.
	double	TxOutLevel_dBm;							   // This parameter specifies the current transmitting power of the transmitter; Input range -127 to -5dBm; Step by 0.25dB.
	double	TxStartOutLevel_dBm;					   // This parameter specifies the initial transmitting power of the transmitter; Input range -127 to -5dBm; Step by 0.25dB.
	double	TxStopOutLevel_dBm;						   // This parameter specifies the terminal transmission power of the transmitter; Input range -127 to -5dBm; Step by 0.25dB.
	double	TxStepOutLevel_dBm;						   // This parameter specifies the step transmission power of the transmitter; Input range -127 to -5dBm; Step by 0.25dB.
	double	RxRefLevel_dBm;						       // This parameter specifies the offset of the receiver's reference level or reference level with respect to transmitted power or, when synchronized with transmitted power, with respect to transmitted power; Input range -127--5dBm; Step by 0.25dB.
	double	RBW;									   // Analysis bandwidth of the specified receiver; Input range 1Hz-1MHz (to be determined); Step 1Hz.
	double	DwellTime_us;							   // Specifies the dwell time. This parameter is applicable when the transmission delay of the device under test is large. Input range 0-1000000; Step 1.
	double ReferenceClockFrequency;					   // Reference input frequency in signal source mode; Currently, this parameter only supports 10MHz reference frequency input. Tuning this parameter can compensate the frequency bias of internal or external reference frequency.
	ReferenceClockSource_TypeDef ReferenceClockSource; // Reference frequency input source; 0: internal 1: external.
	VNA_OperationMode_TypeDef OperationMode;           // Working mode of network analyzer.
	RxLevelMode_typedef	RxLevelMode;	               // When operating in power sweep mode, this parameter specifies the operating state of the receiver's reference level: fixed and in sync with the transmitter.
	TRxTriggerInSource_typedef TriggerInSource;		   // Trigger input source.
	TRxTriggerInMode_typedef TriggerInMode;		       // Trigger input mode.
	TRxTriggerOutMode_typedef TriggerOutMode;		   // Trigger output mode.
}VNA_Profile_TypeDef;

/* Return information for vector network analysis mode (to be updated)(Returned) */
typedef struct
{
	double	StartFreq_Hz; 		// Initial frequency in S11/S21 scanning mode, expressed in Hz; Input range 1M-1GHz; Step 1Hz.
	double	StopFreq_Hz;  		// Termination frequency in S11/S21 scanning mode, expressed in Hz; Input range 1M-1GHz; Step 1Hz.
	double	StepFreq_Hz;  		// Step frequency in S11/S21 scanning mode, expressed in Hz; Input range 1M-1GHz; Step 1Hz.
	int SweepPoints;      		// Scan number.	
	int AnalysisPoints;	  		// Analysis points.
}VNA_Info_TypeDef;


/*---------------------------------------------------
Analog modulation and demodulation Structure (Analog)
-----------------------------------------------------*/

/* Signal source FM modulation signal interface definition (configuration) */
typedef struct
{
	double SignalFrequency;                    // FM modulation signal frequency: Input range: not yet determined.
	double ModDeviation;                       // FM modulation frequency offset: Input range: not yet determined.
	double SampleRate;                         // Sampling rate: This sampling rate = DAC original sampling rate/interpolation multiple.
	short Amplitude;                           // Signal amplitude; Input range: 0-8191, step 1.
	int resetflag;                             // Reset state. After reconfiguration, the first run should be set to 1 and then to 0.
	AnalogSignalType_TypeDef AnalogSignalType; // Analog signal type.
}SIG_FM_Profile_TypeDef;

typedef struct
{
	double ModRate;                      	// AM modulation rate
	double ModDepth;                       	// AM modulation depth or modulation index
	double ModRate_Avg;                   	// AM modulation rate after multiple averages
	double ModDepth_Avg;                   	// AM modulation depth or modulation index after multiple averages
}AM_DemodParam_TypeDef;

typedef struct
{
	double ModRate;                       // FM modulation signal frequency
	double ModDeviation;                  // FM modulation frequency offset
	double CarrierFreqOffset;			  // Carrier offset
	double ModRate_Avg;                   // The frequency of the FM modulated signal after multiple averages
	double ModDeviation_Avg;              // FM modulation frequency offset after multiple averages
	double CarrierFreqOffset_Avg;		  // Carrier offset after multiple averages	
}FM_DemodParam_TypeDef;
/* Signal source AM modulation signal interface definition (configuration) */
typedef struct
{
	double SignalFrequency;									// AM modulation signal frequency: Input range: not yet determined.
	double ModIndex;										// AM modulation index: Input range: not yet determined.
	double SampleRate;										// Sampling rate: This sampling rate = DAC original sampling rate/interpolation multiple.
	short Amplitude;										// Signal amplitude; 0-8191.
	int resetflag;											// Reset state. After reconfiguration, the first run should be set to 1 and then to 0.
	AnalogSignalType_TypeDef AnalogSignalType;				// Analog signal type.
	AMModType_TypeDef AMModType;							// AM modulation type.
	AM_CarrierSuppression_Typedef AMCarrierSuppression; 	// AM modulated carrier suppression.
}SIG_AM_Profile_TypeDef;

typedef struct
{
	double PulseRepetitionFrequency; 	// Pulse repetition rate (PRF).
	double SampleRate;			     	// Sampling rate: This sampling rate = DAC original sampling rate/interpolation multiple.
	double PulseDepth;				 	// Pulse modulation depth, that is, on-off power ratio, unit is dB; And it must be positive.
	double PulseDuty;				 	// The duty cycle of the pulse:0-1.
	short Amplitude;				 	// Signal amplitude:0-8191.
	int resetflag;				     	// Reset state. After reconfiguration, the first run should be set to 1 and then to 0.
}SIG_Pulse_Profile_TypeDef;

/*---------------------------------------------------
DSP auxiliary function architecture (DSP)
-----------------------------------------------------*/

/* Configuration structure for the internal clock calibration source */
typedef enum
{
	CalibrateByExternal = 0x00, // Clock calibration via Ext trigger.
	CalibrateByGNSS1PPS = 0x01  // Clock calibration was carried out by GNSS-1PPS.
}ClkCalibrationSource_TypeDef;

/* Configuration structure for Fast Fourier Transform mode (Configuration) */
typedef struct
{
	uint32_t FFTSize;	       				 // FFT analysis points.
	uint32_t SamplePts;		  				 // Effective sampling number.
	Window_TypeDef WindowType; 				 // Window type.

	TraceDetector_TypeDef TraceDetector;     // Video detection mode.
	uint32_t DetectionRatio;  				 // Trace detection ratio.
	float Intercept;		  				 // Output spectrum interception, such as Intercept = 0.8, means that 80% of the spectrum results are output.

	bool Calibration;        				 // Calibrate or not.
}DSP_FFT_TypeDef;

/* The parameters needed to generate the filter coefficient (configuration) */
typedef struct
{
	int n;            // Number of filter taps(n > 0 ).
	float fc;         // Cut-off frequency ( Cut-off frequency/sampling rate  0 < fc < 0.5 ).
	float As;         // Stopband attenuation() As > 0 , [dB]).
	float mu;         // Fractional sampling offset( -0.5 < mu < 0.5 ).
	uint32_t Samples; // Sampling number(Samples > 0).

}Filter_TypeDef;

/* The parameters required to generate the sine wave (configuration) */
typedef struct
{
	double Frequency;  // The frequency of the sine wave.
	float Amplitude;   // The magnitude of the sine wave.
	float Phase;       // The phase of the sine wave.
	double SampleRate; // The sampling rate of the sine wave.
	uint32_t Samples;  // The number of sinewave samples.
}Sin_TypeDef;

/* Configuration structure for digital down conversion mode (Configuration) */
typedef struct
{
	double DDCOffsetFrequency; // The frequency offset of complex mixing.
	double SampleRate;         // Sample rate of complex mixing.
	float DecimateFactor;      // Resampling extraction multiple, range :1 ~ 2^16.
	uint64_t SamplePoints;     // Sampling number.
}DSP_DDC_TypeDef;

/* IP3 test structure */
typedef struct
{
	double	LowToneFreq;	   // Bass frequency, in units depending on the data source.
	double	HighToneFreq;	   // High-pitched signal frequency, in units depending on the data source.
	double	LowIM3PFreq;	   // Low frequency intermodulation frequency, unit dependent on data source.
	double	HighIM3PFreq;	   // High frequency intermodulation frequency, unit depending on the data source.

	float	LowTonePower_dBm;  // Bass power, dBm.
	float	HighTonePower_dBm; // Treble power, dBm.
	float	TonePowerDiff_dB;  // Bass power - treble power.
	float	LowIM3P_dBc;	   // LowIM3P_dBc = max(LowTonePower_dBm, HighTonePower_dBm) - LowTonePower_dBm, The intensity of the low frequency crossmodulation product relative to the strongest main signal.
	float	HighIM3P_dBc;	   // HighIM3P_dBc = max(LowTonePower_dBm, HighTonePower_dBm) - HighTonePower_dBm, The degree of the high frequency crossmodulation product with respect to the strongest main signal.

	float	IP3_dBm;			// IP3_dBm

}TraceAnalysisResult_IP3_TypeDef;

/* IP2 test structure */
typedef struct
{
	double	LowToneFreq;	   // Bass frequency, in units depending on the data source.
	double	HighToneFreq;	   // High-pitched signal frequency, in units depending on the data source.
	double	IM2PFreq;	       // Low frequency intermodulation frequency, unit dependent on data source.

	float	LowTonePower_dBm;  // Bass power, dBm.
	float	HighTonePower_dBm; // Treble power, dBm.
	float	TonePowerDiff_dB;  // Bass power - treble power.
	float	IM2P_dBc;		   // IM2P_dBc = max(LowTonePower_dBm, HighTonePower_dBm) - IM2P_dBm, the intensity of the low frequency crossmodulated product relative to the strongest main signal.

	float	IP2_dBm;			// IP2_dBm

}TraceAnalysisResult_IP2_TypeDef;

/* AudioAnalysis structure*/
typedef struct
{
	double AudioVoltage;   // Audio voltage (V).
	double AudioFrequency; // Audio frequency (Hz).
	double SINDA;          // Shinard (dB).
	double THD;            // Total harmonic distortion (%).

}DSP_AudioAnalysis_TypeDef;

/* XdB bandwidth information structure (Return) */
typedef struct
{
	double XdBBandWidth_Hz; // XdB bandwidth (Hz).
	double CenterFreq_Hz;   // Center frequency of XdB bandwidth (Hz).
	double StartFreq_Hz;    // Starting frequency of XdB bandwidth (Hz).
	double StopFreq_Hz;     // Termination frequency of XdB bandwidth (Hz).
	float StartPower_dBm;   // Power for starting frequency of XdB bandwidth (dBm).
	float StopPower_dBm;    // Power for terminating frequency of XdB bandwidth (dBm).
	uint32_t PeakIndex;     // Peak index within the XdB bandwidth.
	double PeakFreq_Hz;     // Peak frequency in XdB bandwidth (Hz).
	float PeakPower_dBm;    // Peak power in XdB bandwidth (dBm).

}TraceAnalysisResult_XdB_TypeDef;

/* Occupied bandwidth information structure (return) */
typedef struct
{
	double OccupiedBandWidth; // Bandwidth of Occupied Bandwidth (Hz).
	double CenterFreq_Hz;     // Occupied bandwidth center frequency (Hz).
	double StartFreq_Hz;      // Occupied bandwidth start frequency (Hz).
	double StopFreq_Hz;       // Occupied bandwidth stop frequency (Hz).
	float StartPower_dBm;     // The power corresponding to the start frequency of the occupied bandwidth (dBm).
	float StopPower_dBm;      // The power corresponding to the stop frequency of the occupied bandwidth (dBm).
	float StartRatio;         // The power ratio corresponding the to start frequency of the occupied bandwidth (dBm).
	float StopRatio;          // The power ratio corresponding the to stop frequency of the occupied bandwidth (dBm).
	uint32_t PeakIndex;       // Peak index in the occupied bandwidth.
	double PeakFreq_Hz;       // Peak frequency in the occupied bandwidth (Hz).
	float PeakPower_dBm;      // Peak power in the occupied bandwidth (dBm).

}TraceAnalysisResult_OBW_TypeDef;

/* Adjacent channel power ratio frequency information structure (input) */
typedef struct
{
	double RBW;                 // Resolution bandwidth (Hz).
	double MainChCenterFreq_Hz; // Main channel center frequency (Hz).
	double MainChBW_Hz;         // Main channel bandwidth (Hz).
	double AdjChSpace_Hz;       // Adjacent channel interval, difference between center frequency of main channel and center frequency of adjacent channel (Hz).
	uint32_t AdjChPair;         //Adjacent channel pairs.1 represents the left and right 2 adjacent channels, and 2 represents the left and right 4 adjacent channels

}DSP_ACPRFreqInfo_TypeDef;

/* Adjacent channel power ratio information structure (return) */
typedef struct
{
	float MainChPower_dBm;       // Main channel power (dBm).
	uint32_t MainChPeakIndex;    // Peak index of the main channel.
	double MainChPeakFreq_Hz;    // Peak frequency of the main channel (Hz).
	float MainChPeakPower_dBm;   // Main channel peak power (dBm).

	double L_AdjChCenterFreq_Hz; // Left adjacent channel center frequency (Hz).
	double L_AdjChBW_Hz;         // Left adjacent channel bandwidth (Hz).
	float L_AdjChPower_dBm;      // Left adjacent channel power (dBm).
	float L_AdjChPowerRatio;     // Left adjacent channel power ratio (left adjacent channel power/main channel power).
	float L_AdjChPowerDiff_dBc;  // Left Neighbor power difference (Left Neighbor Power - Main Channel power dBc).
	float L_AdjChPeakIndex;      // Peak index of the left adjacent track.
	double L_AdjChPeakFreq_Hz;   // Peak frequency of the left channel (Hz).
	float L_AdjChPeakPower_dBm;  // Value power of left adjacent channel (dBm).

	double R_AdjChCenterFreq_Hz; // Right adjacent channel center frequency (Hz).
	double R_AdjChBW_Hz;         // Right adjacent channel bandwidth (Hz).
	float R_AdjChPower_dBm;      // Right adjacent channel power (dBm).
	float R_AdjChPowerRatio;     // Right neighbor power ratio (right neighbor power/main channel power).
	float R_AdjChPowerDiff_dBc;  // Right Neighbor power difference (Right Neighbor Power - Main Channel power dBc).
	float R_AdjChPeakIndex;      // Peak index of the right adjacent track.
	double R_AdjChPeakFreq_Hz;   // Right channel peak frequency (Hz).
	float R_AdjChPeakPower_dBm;  // Value power of right adjacent channel (dBm).

}TraceAnalysisResult_ACPR_TypeDef;

/* Channel power structure (return) */
typedef struct
{
	float ChannelPower_dBm;     // Channel power (dBm).
	float PowerDensity;         // Channel power density (dBm/Hz).
	float ChannelPeakIndex;     // Peak index within the channel.
	double ChannelPeakFreq_Hz;  // Peak frequency in channel (Hz).
	float ChannelPeakPower_dBm; // Peak power in channel (dBm).

}DSP_ChannelPowerInfo_TypeDef;

/*Trace smoothing structure*/
typedef enum
{
	MovingAvrage = 0x00,	//Moving average
	MedianFilter = 0x01,	//Median filtering
	PolynomialFit = 0x02	//Polynomial fit
}SmoothMethod_TypeDef;

typedef enum
{
	SWPPhaseNoiseMeas = 0x00,		//phase noise measurement.
	SWPNoiseMeas = 0x01,			//noise measurement.
	SWPChannelPowerMeas = 0x02,		//chanel power measurement.
	SWPOBWMeas = 0x03,          //occupation bandwidth measurement.
	SWPACPRMeas = 0x04,         //adjacent channel power rate measurement.
	SWPIM3Meas = 0x05           //IP3/IM3 measurement.
}SWPApplication_TypeDef;

//typedef struct
//{
//	/*Frequency：FREQ*/
//	SWP_FreqAssignment_TypeDef FreqAssignment; // How the frequency is specified
//	double StartFreq_Hz; 					   // Start frequency
//	double StopFreq_Hz;	 					   // Termination frequency
//	double CenterFreq_Hz;                      // Center frequency
//	double Span_Hz;                            // Frequency sweep width
//
//	/*amplitude：AMP*/
//	double RefLevel_dBm;					   // Reference level
//
//	/*bandwidth：BW*/
//	double RBW_Hz;		 					   // Resolution bandwidth
//	VBWMode_TypeDef VBWMode;				   // VBW update mode
//
//	/*scan：SWEEP*/
//	Window_TypeDef Window; 					   // The window type used for FFT analysis
//	SpurRejection_TypeDef SpurRejection; 	   // Spurious suppression
//	TraceDetector_TypeDef TraceDetector; 	   // Trace geophones
//	unsigned int TracePoints;  					   // Number of trace points
//}PTest_SWPProfile_TypeDef;
/* -------------------------------------------Device control related interface------------------------------------------------------ */
//HTRA_API int PTest_SWP_Configuration(void** Device, const PTest_SWPProfile_TypeDef* ProfileIn, PTest_SWPProfile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);

HTRA_API int Get_APIVersion(void);

/* The dev interface to open the device, which must be called to obtain device resources before operating on the device */
HTRA_API int Device_Open(void** Device, int DeviceNum, const BootProfile_TypeDef* BootProfile, BootInfo_TypeDef* BootInfo);

/* The dev interface to shut down the device. This function must be called to release device resources after the operation on the device is complete */
HTRA_API int Device_Close(void** Device);

/* The dev interface to set the system power supply status */
HTRA_API int Device_SetSysPowerState(void **Device, SysPowerState_TypeDef SysPowerState);

/* The dev interface to obtain device information, including device serial number, software and hardware version and other related information, non-real-time mode, without interrupting data acquisition, but information is only updated after obtaining data packets */
HTRA_API int Device_QueryDeviceInfo(void** Device, DeviceInfo_TypeDef* DeviceInfo);

/* The dev interface to obtain device information, including device serial number, software and hardware version and other related information, real-time mode, and a short period of time will occupy the data channel */
HTRA_API int Device_QueryDeviceInfo_Realtime(void** Device, DeviceInfo_TypeDef* DeviceInfo);

/* The dev interface to obtain device status, including device temperature, hardware working status, geo-time information (optional support is required), etc., non-real-time mode, without interrupting data acquisition, but information is only updated after obtaining data packets */
HTRA_API int Device_QueryDeviceState(void** Device, DeviceState_TypeDef* DeviceState);

/* The dev interface to obtain device status, including device temperature, hardware working status, geo-time information (optional support is required), etc., real-time mode, and a short period of time will occupy the data channel. */
HTRA_API int Device_QueryDeviceState_Realtime(void** Device, DeviceState_TypeDef* DeviceState);

/* The dev interface, calibrates the reference clock frequency by external trigger signal of known clock period (advanced applications) ExtTriggerPeriod_s: indicates trigger signal period, ExtTriggerCount: indicates trigger number, RewriteRFCal: Indicates whether the calibrated reference clock frequency is rewritten to the calibration file as the default value for subsequent startup (irreversible). RefCLKFreq_Hz: indicates the calibrated reference clock frequency */
HTRA_API int Device_CalibrateRefClock(void** Device, ClkCalibrationSource_TypeDef ClkCalibrationSource, const double TriggerPeriod_s, const uint64_t TriggerCount, const bool RewriteRFCal, double* RefCLKFreq_Hz);

/* The dev interface to update device firmware (advanced application)*/
HTRA_API int Device_UpdateFirmware(int DeviceNum, const Firmware_TypeDef Firmware, const char* path);

/* The dev interface to control the fan mode of the device*/
HTRA_API int Device_SetFanState(void** Device, const FanState_TypeDef FanState, const float ThreshouldTemperature);

/* dev interface to set the frequency response compensation*/
HTRA_API int Devcie_SetFreqResponseCompensation(void** Device, uint8_t State, const double *Frequency_Hz, const float *CorrectVal_dB, uint8_t Points);

/* The dev interface,to set GNSS antenna status (opt.)*/
HTRA_API int Device_SetGNSSAntennaState(void** Device, const GNSSAntennaState_TypeDef GNSSAntennaState);

/* dev interface, obtains GNSS antenna status (opt.), non-real-time mode, continuous data acquisition, but the information is only updated after obtaining the data packet*/
HTRA_API int Device_GetGNSSAntennaState(void** Device, GNSSAntennaState_TypeDef* GNSSAntennaState);

/* dev interface, obtains GNSS antenna status (opt.), real-time data acquisition, data channel will be occupied by a short period of time*/
HTRA_API int Device_GetGNSSAntennaState_Realtime(void** Device, GNSSAntennaState_TypeDef* GNSSAntennaState);

/* dev interface, parsing GNSS time and date information (option support required)*/
HTRA_API int Device_AnysisGNSSTime(double ABSTimestamp, int16_t* hour, int16_t* minute, int16_t* second, int16_t* Year, int16_t* month, int16_t* day);

/* dev interface, obtains GNSS altitude (opt.), non-real-time mode, continuous data acquisition, but the information is only updated after obtaining the data packet*/
HTRA_API int Device_GetGNSSAltitude(void** Device, int16_t* Altitude);

/* dev interface, obtains GNSS altitude (opt.), real-time data acquisition, data channel will be occupied by a short period of time*/
HTRA_API int Device_GetGNSSAltitude_Realtime(void** Device, int16_t* Altitude);

/*dev interface, set the DOCXO working status in GNSS (opt.)*/
HTRA_API int Device_SetDOCXOWorkMode(void** Device, const DOCXOWorkMode_TypeDef DOCXOWorkMode);

/* dev interface, obtains GNSS DOCXO working state (opt.), real-time data acquisition, data channel will be occupied by a short period of time*/
HTRA_API int Device_GetDOCXOWorkMode_Realtime(void** Device, DOCXOWorkMode_TypeDef* DOCXOWorkMode);

/* dev interface, obtains GNSS DOCXO working state (opt.), non-real-time mode, continuous data acquisition, but the information is only updated after obtaining the data packet*/
HTRA_API int Device_GetDOCXOWorkMode(void** Device, DOCXOWorkMode_TypeDef* DOCXOWorkMode);

/* dev interface, obtains GNSS DOCXO device state (opt.), non-real-time mode, continuous data acquisition, but the information is only updated after obtaining the data packet*/
HTRA_API int Device_GetGNSSInfo(void** Device, GNSSInfo_TypeDef* GNSSInfo);

/* dev interface, obtains GNSS device state (opt.), real-time data acquisition, data channel will be occupied by a short period of time*/
HTRA_API int Device_GetGNSSInfo_Realtime(void** Device, GNSSInfo_TypeDef* GNSSInfo);

/* dev interface, obtain the IP addresses and subnet masks of all network devices on the network*/
HTRA_API int Device_GetNetworkDeviceList(uint8_t* DeviceCount, NetworkDeviceInfo_TypeDef DeviceInfo[64], uint8_t LocalIP[4], uint8_t LocalMask[4]);

/* dev interface, configure the IP address and subnet mask of the network device based on the device UID*/
HTRA_API int Device_SetNetworkDeviceIP(const uint64_t DeviceUID, const uint8_t IPAddress[4], const uint8_t SubnetMask[4]);

/* dev interface, this section describes how to configure the IP address and subnet mask of the network device based on the device IP address*/
HTRA_API int Device_SetNetworkDeviceIP_PM1(const uint8_t DeviceIP[4], const uint8_t IPAddress[4], const uint8_t SubnetMask[4]);

/* dev interface, get the full UID information, in a non-real-time manner, without interrupting data acquisition, but the information is only updated after the data packet is obtained*/
HTRA_API int Device_GetFullUID(void** Device, uint64_t* UID_L64, uint32_t* UID_H32);

/* dev interface, get GNSS signal-to-noise ratio (optional support required), in a non-real-time manner, without interrupting data acquisition, but the information is only updated after the data packet is obtained*/
HTRA_API int Device_GetGNSS_SatDate(void** Device, GNSS_SatDate_TypeDef * GNSS_SatDate);

/* dev interface, obtain GNSS signal to noise ratio (optional support required) in real time, which occupies the data channel for a short time*/
HTRA_API int Device_GetGNSS_SatDate_Realtime(void** Device, GNSS_SatDate_TypeDef * GNSS_SatDate);

/* dev interface, update GNSS Firmware (Advanced Applications)*/
HTRA_API int Device_GNSSFirmwareUpdate(int DeviceNum, const char* path);

/* dev interface for obtaining hardware status information*/
HTRA_API int Device_GetHardwareState(void** Device, HardWareState_TypeDef* HardWareState);

/* dev interface, through*/
HTRA_API int Device_QueryDeviceInfoWithBus(int DeviceNum, const BootProfile_TypeDef* BootProfile, BootInfo_TypeDef* BootInfo);


/* dev interface for configuring frequency scanning parameters*/
HTRA_API int Device_SetFreqScan(void** Device, double StartFreq_Hz, double StopFreq_Hz, uint16_t SweepPts);


/* ------------------------------------------The following is the SWP mode (SWEEP related interface)------------------------------------------------------ */

/* SWP mode, configure SWP_Profile to the default value */
HTRA_API int SWP_ProfileDeInit(void** Device, SWP_Profile_TypeDef* UserProfile_O);
HTRA_API int SWP_EZProfileDeInit(void** Device, SWP_EZProfile_TypeDef* UserProfile_O);

/* SWP mode, set SWP mode parameters */
HTRA_API int SWP_Configuration(void** Device, const SWP_Profile_TypeDef* ProfileIn, SWP_Profile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);
HTRA_API int SWP_EZConfiguration(void** Device, const SWP_EZProfile_TypeDef* ProfileIn, SWP_EZProfile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);

/* SWP mode, obtain partial scanning spectrum data in SWP mode */
HTRA_API int SWP_GetPartialSweep(void** Device, double Freq_Hz[], float PowerSpec_dBm[], int* HopIndex, int* FrameIndex, MeasAuxInfo_TypeDef* MeasAuxInfo);
HTRA_API int SWP_GetPartialSweep_PM1(void** Device, SWPTrace_TypeDef* PartialTrace);

/* SWP mode, obtain the completely refreshed spectrum data in SWP mode */
HTRA_API int SWP_GetFullSweep(void** Device, double Freq_Hz[], float PowerSpec_dBm[], MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int SWP_GetFullSweep_PM1(void** Device, SWPTrace_TypeDef* PartialTrace); (not available for now)

/* SWP mode: Obtain spectrum data that is partially refreshed but contains non-refreshed data in SWP mode (advanced application) */
HTRA_API int SWP_GetPartialUpdatedFullSweep(void** Device, double Freq_Hz[], float PowerSpec_dBm[], int* HopIndex, int* FrameIndex, MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int SWP_GetPartialUpdatedFullSweep_RM1(void** Device, SWPTrace_TypeDef* PartialTrace);(not available for now)

/* SWP mode: Reset MaxHold Spectrum data in SWP mode (advanced application) */
HTRA_API int SWP_GetAmplitudeCalibrationTrace(void** Device, double Frequency_Hz[], float Compensate_dB[], float* CalTemperature_C);

/* SWP mode, reset spectrum data of MaxHold and MinHold in SWP mode(advanced application) */
HTRA_API void SWP_ResetTraceHold(void** Device);

/* SWP mode: Give the recommended device configuration based on the application goal */
HTRA_API int SWP_AutoSet(void** Device, SWPApplication_TypeDef Application, const SWP_Profile_TypeDef* ProfileIn, SWP_Profile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo, uint8_t ifDoConfig);

/* --------------------------------------------The following is the time domain stream mode(IQS related interface)----------------------------------------------------- */

/* IQS mode, set IQS_Profile to the default value */
HTRA_API int IQS_ProfileDeInit(void** Device, IQS_Profile_TypeDef* UserProfile_O);
HTRA_API int IQS_EZProfileDeInit(void** Device, IQS_EZProfile_TypeDef* UserProfile_O);

/* IQS mode, set IQS mode parameters */
HTRA_API int IQS_Configuration(void** Device, const IQS_Profile_TypeDef* ProfileIn, IQS_Profile_TypeDef* ProfileOut, IQS_StreamInfo_TypeDef* StreamInfo);
HTRA_API int IQS_EZConfiguration(void** Device, const IQS_EZProfile_TypeDef* ProfileIn, IQS_EZProfile_TypeDef* ProfileOut, IQS_StreamInfo_TypeDef* StreamInfo);

/* IQS mode, initiating the bus trigger */
HTRA_API int IQS_BusTriggerStart(void** Device);

/* IQS mode, terminating the bus trigger */
HTRA_API int IQS_BusTriggerStop(void** Device);

/* IQS mode, wait for multi-machine synchronization trigger signal */
HTRA_API int IQS_MultiDevice_WaitExternalSync(void** Device, const IQS_Profile_TypeDef* ProfileIn);

/* IQS mode, multiple devices can run synchronously */
HTRA_API int IQS_MultiDevice_Run(void **Device);

/* IQS mode, the interwoven IQ data stream under IQS mode is obtained, and the scale factor from integer to absolute amplitude (V units) and the related information of trigger are obtained */
HTRA_API int IQS_GetIQStream(void** Device, void** AlternIQStream, float* ScaleToV, IQS_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);	// Function Polymorphism - Form 0: Unpackaged Data Output
HTRA_API int IQS_GetIQStream_PM1(void **Device, IQStream_TypeDef* IQStream); // Function polymorphism - Form 1: Encapsulates data output
HTRA_API int IQS_GetIQStream_PM2(void **Device, IQStream_TypeDef* IQStream, MeasAuxInfo_TypeDef* MeasAuxInfo); // Function Polymorphism - Form 2: Packaged Data Output
HTRA_API int IQS_GetIQStream_Data(void** Device, int16_t IQ_data[]);

/* IQS mode，initiates timer - External triggers single synchronization*/
HTRA_API int IQS_SyncTimer(void** Device);

/* -------------------------------------------The following is the detection mode (DET correlation interface)------------------------------------------------------ */

/* DET mode, set DET_Profile to the default value */
HTRA_API int DET_ProfileDeInit(void** Device, DET_Profile_TypeDef* UserProfile_O);
HTRA_API int DET_EZProfileDeInit(void** Device, DET_EZProfile_TypeDef* UserProfile_O);

/* DET mode, set parameters related to the DET mode */
HTRA_API int DET_Configuration(void** Device, const DET_Profile_TypeDef* ProfileIn, DET_Profile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);
HTRA_API int DET_EZConfiguration(void** Device, const DET_EZProfile_TypeDef* ProfileIn, DET_EZProfile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);

/* ZeroSpan Mode, Configuring Related Parameters for ZeroSpan Mode */
HTRA_API int ZSP_ProfileDeInit(void** Device, ZSP_Profile_TypeDef* UserProfile_O);
HTRA_API int ZSP_Configuration(void** Device, const ZSP_Profile_TypeDef* ProfileIn, ZSP_Profile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);

/* DET mode, initiating the bus trigger */
HTRA_API int DET_BusTriggerStart(void** Device);

/* DET mode, terminating the bus trigger */
HTRA_API int DET_BusTriggerStop(void** Device);

/* DET mode, obtain the detection data in DET mode, and obtain the scale factor from integer to absolute amplitude (V units) and trigger related information. NormalizedPowerStream is the square root of I square +Q square */
HTRA_API int DET_GetPowerStream(void** Device, float NormalizedPowerStream[], float* ScaleToV, DET_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int DET_GetPowerStream_PM1(void** Device, DETStream_TypeDef* DETStream);(not implemented yet)

/* DET mode, Initiates the timer - External triggers a single synchronization*/
HTRA_API int DET_SyncTimer(void** Device);

/* -------------------------------------------The following is the real-time spectrum analysis mode (RTA related interface)------------------------------------------------------ */

/*  RTA mode, set RTA_Profile to the default value */
HTRA_API int RTA_ProfileDeInit(void** Device, RTA_Profile_TypeDef* UserProfile_O);
HTRA_API int RTA_EZProfileDeInit(void** Device, RTA_EZProfile_TypeDef* UserProfile_O);

/* RTA mode, set parameters related to the RTA mode */
HTRA_API int RTA_Configuration(void** Device, const RTA_Profile_TypeDef* ProfileIn, RTA_Profile_TypeDef* ProfileOut, RTA_FrameInfo_TypeDef* FrameInfo);
HTRA_API int RTA_EZConfiguration(void** Device, const RTA_EZProfile_TypeDef* ProfileIn, RTA_EZProfile_TypeDef* ProfileOut, RTA_FrameInfo_TypeDef* FrameInfo);

/* RTA mode, initiating the bus trigger */
HTRA_API int RTA_BusTriggerStart(void **Device);

/* RTA mode, terminating the bus trigger */
HTRA_API int RTA_BusTriggerStop(void **Device);

/* RTA mode, real-time spectrum data and trigger related information are obtained in RTA mode */
HTRA_API int RTA_GetRealTimeSpectrum(void** Device, uint8_t SpectrumStream[], uint16_t SpectrumBitmap[], RTA_PlotInfo_TypeDef* PlotInfo, RTA_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);

/*RTA mode, to obtain real-time spectrum (without probability density map) and trigger related information in RTA mode*/
HTRA_API int RTA_GetRealTimeSpectrum_Raw(void **Device, uint8_t SpectrumStream[], RTA_PlotInfo_TypeDef* PlotInfo, RTA_TriggerInfo_TypeDef* TriggerInfo, MeasAuxInfo_TypeDef* MeasAuxInfo);
//HTRA_API int RTA_GetRealTimeSpectrum_PM1(void** Device, RTAStream_TypeDef* RTAStream);（not implemented yet）

/*RTA mode, the trigger threshold is issued when the spectrum template is used to trigger. Input the number of concerns, the Freq array and the Power array correspond to each other, just input concerns, and the power between points will be automatically completed*/
HTRA_API int RTA_SpectrumMaskTrigger_SetMask(void** Device, uint32_t NodeNum, double FreqHz_In[], float PowerdBm_In[], double FreqHz_Out[], float PowerdBm_Out[]);

/* RTA mode, initiating spectrum template trigger */
HTRA_API int RTA_SpectrumMaskTrigger_Run(void **Device);

/* RTA mode, spectrum template trigger is stopped */
HTRA_API int RTA_SpectrumMaskTrigger_Stop(void **Device);

/* RTA mode, initiate timer - External trigger single synchronization*/
HTRA_API int RTA_SyncTimer(void** Device);

/* ------------------------------------------- The following are auxiliary signal generator controls (ASG related interfaces) ------------------------------------------------------ */

/* ASG to set ASG_Profile to the default value */
HTRA_API int ASG_ProfileDeInit(void** Device, ASG_Profile_TypeDef* Profile);

/* ASG to set working status */
HTRA_API int ASG_Configuration(void **Device, ASG_Profile_TypeDef *ProfileIn, ASG_Profile_TypeDef *ProfileOut,ASG_Info_TypeDef *ASG_Info);


/* --------------------------------------The following is Analog demodulation (Analog related interface)----------------------------------- */

/* Analog modulation, create */
HTRA_API void ASD_Open(void **AnalogMod);

/* Analog modulation, close */
HTRA_API void ASD_Close(void** AnalogMod);

/* Modulation and demodulation mode, FM demodulation */
HTRA_API int ASD_Demodulate_FM(void** AnalogMod , const IQStream_TypeDef* IQStreamIn, bool reset, float result[], double* carrierOffsetHz);

/* Modulation and demodulation mode, AM demodulation */
HTRA_API int ASD_Demodulate_AM(const IQStream_TypeDef* IQStreamIn, float result[]);

/* Modulation mode, FM demodulation, while returning the demodulated audio model and modulation parameters*/
HTRA_API int ASD_FMDemodulation(void** AnalogMod , const IQStream_TypeDef* IQStreamIn, bool reset, float result[], FM_DemodParam_TypeDef *FM_DemodParam);

/* Moderation mode, AM demodulation, returns both the demodulated audio model and modulation parameters*/
HTRA_API int ASD_AMDemodulation(void** AnalogMod ,const IQStream_TypeDef* IQStreamIn, bool reset, float result[],AM_DemodParam_TypeDef *AM_DemodParam);

/* ------------------------------------------- The following are digital signal processing auxiliary functions (DSP related auxiliary functions) ------------------------------------------------------ */

/* DSP auxiliary function to enable the DSP function */
HTRA_API int DSP_Open(void** DSP);

/* DSP auxiliary function to initialize FFT parameters */
HTRA_API int DSP_FFT_DeInit(DSP_FFT_TypeDef* IQToSpectrum);

/* DSP auxiliary function to configurate FFT parameters */
HTRA_API int DSP_FFT_Configuration(void** DSP, const DSP_FFT_TypeDef* ProfileIn, DSP_FFT_TypeDef* ProfileOut, uint32_t* TracePoints, double* RBWRatio);

/* DSP auxiliary function to convert IQ data into spectral data */
HTRA_API int DSP_FFT_IQSToSpectrum(void** DSP, const IQStream_TypeDef* IQStream, double Freq_Hz[], float PowerSpec_dBm[]);

/* DSP auxiliary function to initialize the LPF parameter */
HTRA_API void DSP_LPF_DeInit(Filter_TypeDef* LPF_ProfileIn);

/* DSP auxiliary function with low pass filter */
HTRA_API void DSP_LPF_Configuration(void** DSP, const Filter_TypeDef* LPF_ProfileIn, Filter_TypeDef* LPF_ProfileOut);

/* DSP auxiliary function to reset cache in LPF */
HTRA_API void DSP_LPF_Reset(void** DSP);

/* DSP auxiliary function, low pass filter _ real signal */
HTRA_API void DSP_LPF_Execute_Real(void** DSP, float Signal[], float LPF_Signal[]);

/* DSP auxiliary function, low pass filter _ complex signal */
HTRA_API void DSP_LPF_Execute_Complex(void** DSP, const IQStream_TypeDef* IQStreamIn, IQStream_TypeDef* IQStreamOut);

/* DSP auxiliary function to initialize DDC configuration parameters */
HTRA_API int DSP_DDC_DeInit(DSP_DDC_TypeDef* DDC_Profile);

/* DSP auxiliary function to configure DDC */
HTRA_API int DSP_DDC_Configuration(void** DSP, const DSP_DDC_TypeDef* DDC_ProfileIn, DSP_DDC_TypeDef* DDC_ProfileOut);

/* DSP helper function to reset cache in DDC */
HTRA_API void DSP_DDC_Reset(void** DSP);

/* DSP helper function to obtain the delay of the DDC */
HTRA_API void DSP_DDC_GetDelay(void** DSP, uint32_t* delay);

/* DSP auxiliary function, DDC execution */
HTRA_API int DSP_DDC_Execute(void** DSP, const IQStream_TypeDef* IQStreamIn, IQStream_TypeDef* IQStreamOut);

/* DSP auxiliary function to analyze the IM3 parameter of the trace */
HTRA_API int DSP_TraceAnalysis_IM3(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, TraceAnalysisResult_IP3_TypeDef* IM3Result);

/* DSP auxiliary function to analyze the IM2 parameter of the trace */
HTRA_API int DSP_TraceAnalysis_IM2(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, TraceAnalysisResult_IP2_TypeDef* IM2Result);

/* DSP auxiliary function to analyze the phase noise parameters of the trace */
HTRA_API int DSP_TraceAnalysis_PhaseNoise(const double Freq_Hz[], const float PowerSpec_dBm[], const double OffsetFreqs[], const uint32_t TracePoints, const uint32_t OffsetFreqsToAnalysis, double CarrierFreqOut[], float PhaseNoiseOut_dBc[]);

/* DSP auxiliary function to analyze the channel power of the trace */
HTRA_API int DSP_TraceAnalysis_ChannelPower(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const double CenterFrequency, const double AnalysisSpan, const double RBW, DSP_ChannelPowerInfo_TypeDef* ChannelPowerResult);

/* DSP auxiliary function to analyze the XdB bandwidth of the trace */
HTRA_API int DSP_TraceAnalysis_XdBBW(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const float XdB, TraceAnalysisResult_XdB_TypeDef* XdBResult);

/* DSP auxiliary function to analyze the occupied bandwidth of the trace */
HTRA_API int DSP_TraceAnalysis_OBW(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const float OccupiedRatio, TraceAnalysisResult_OBW_TypeDef* OBWResult);

/* DSP auxiliary function to analyze the adjacent track power ratio of the trace */
HTRA_API int DSP_TraceAnalysis_ACPR(const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t TracePoints, const DSP_ACPRFreqInfo_TypeDef ACPRFreqInfo, TraceAnalysisResult_ACPR_TypeDef* ACPRResult);

/* Audio analysis function analyzes audio voltage (V), audio frequency (Hz), Sinard (dB) and total harmonic distortion (%) parameters */
HTRA_API void DSP_AudioAnalysis(const double Audio[], const uint64_t SamplePoints, const double SampleRate, DSP_AudioAnalysis_TypeDef* AudioAnalysis);

/* DSP auxiliary function, spectrum interception */
HTRA_API void DSP_InterceptSpectrum(const double StartFreq_Hz, const double StopFreq_Hz, const double Freq_Hz[], const float PowerSpec_dBm[], const uint32_t FullsweepTracePoints, double FrequencyOut[], float PowerSpecOut_dBm[], uint32_t* InterceptPoints);

/* DSP auxiliary function,  trace smooth */
HTRA_API int DSP_TraceSmooth(float* Trace, unsigned int TracePoints, SmoothMethod_TypeDef SmoothMethod, unsigned int IndexOffset, unsigned int WindowLength, unsigned int PolynomialOrder);

/* DSP auxiliary function, disable the DSP function */
HTRA_API int  DSP_Close(void** DSP);





/* -------------------------------------------The following functions are deprecated, not yet available, and not necessary invoked------------------------------------------------------ */


/* --------------------------------------The following is the control of vector network analyzer (VNA related interface, not yet open)------------------------------------------------------ */

/* VNA mode, set VNA_Profile to the default value */
HTRA_API int VNA_ProfileDeInit(void** Device, VNA_Profile_TypeDef* Profile);

/* VNA mode, configurate the work state */
HTRA_API int VNA_Configuration(void** Device, VNA_Profile_TypeDef* ProfileIn, VNA_Profile_TypeDef* ProfileOut, VNA_Info_TypeDef* VNA_Info);

/* VNA mode, get the complete scan result */
HTRA_API int VNA_GetFullSweep(void** Device, double Frequency[], double Result_dB[], double Result_Deg[]);

/* VNA mode, get the partial scan result */
HTRA_API int VNA_GetPartialSweep(void** Device, double* Frequency, double* Result_dB, double* Result_Deg, int* Index);

/* VNA mode, get single IQStream */
HTRA_API int VNA_GetIQStream(void** Device, double* I_Ref, double* Q_Ref, double* I_Meas, double* Q_Meas, int DataLength, int* PacketIndex);

/* VNA mode, calibration */
HTRA_API int VNA_Calibration(void** Device, VNA_Calibration_TypeDef VNA_Calibration);

/* VNA mode, calibration done */
HTRA_API int VNA_Calibration_Done(void** Device);

/* VNA mode, save calibration data*/
HTRA_API int VNA_SaveCalibrationData(void** Device, const char* Calibrationfile);
HTRA_API int VNA_LoadCalibrationData(void** Device, const char* Calibratiofile);

/* -------------------------------------------The following multi-profile mode (MPS related interface, not open yet)------------------------------------------------------ */

/* MPS mode, set SWP mode parameters */
HTRA_API int MPS_SWP_Configuration(void** Device, uint16_t ProfileNum, const SWP_Profile_TypeDef* ProfileIn, SWP_Profile_TypeDef* ProfileOut, SWP_TraceInfo_TypeDef* TraceInfo);

/* MPS mode, set parameters related to RTA mode */
HTRA_API int MPS_RTA_Configuration(void** Device, uint16_t ProfileNum, const RTA_Profile_TypeDef* ProfileIn, RTA_Profile_TypeDef* ProfileOut, RTA_FrameInfo_TypeDef* FrameInfo);

/* MPS mode, set IQS mode parameters */
HTRA_API int MPS_IQS_Configuration(void** Device, uint16_t ProfileNum, const IQS_Profile_TypeDef* ProfileIn, IQS_Profile_TypeDef* ProfileOut, IQS_StreamInfo_TypeDef* StreamInfo);

/* MPS mode: Set the parameters related to the DET mode */
HTRA_API int MPS_DET_Configuration(void** Device, uint16_t ProfileNum, const DET_Profile_TypeDef* ProfileIn, DET_Profile_TypeDef* ProfileOut, DET_StreamInfo_TypeDef* StreamInfo);

/* MPS mode, start the MPS mode */
HTRA_API int MPS_Start(void** Device, uint16_t AllProfileNum);

/* MPS pattern, which gets the data and identifies which pattern and which Profile it is */
HTRA_API int MPS_GetData(void** Device, MPSData_TypeDef* MPSData, uint8_t* Analysis, uint16_t* ProfileNum);

/* MPS mode, simple version to obtain data */
HTRA_API int MPS_GetData_Simple(void** Device, MPSDataInfo_TypeDef* MPSDataInfo, double* Frequency, float* PowerSpec_dBm, int16_t* AlternIQStream, float* DETTrace, uint8_t* SpectrumTrace, uint16_t* SpectrumBitmap);

/* MPS mode, open up memory */
HTRA_API int MPS_DataBuffer_New(void** Device);

/* MPS mode to release memory */
HTRA_API int MPS_DataBuffer_Free(void** Device);


/* -------------------------------------------The following functions are deprecated and should not be used anymore------------------------------------------------------ */

/* The dev interface to control NX series devices to modify IP address (advanced application)*/
HTRA_API int Device_ChangeIPAddr(void** Device, const IPVersion_TypeDef ETH_IPVersion, const uint8_t ETH_IPAddress[], const uint8_t SubnetMask[]);

HTRA_API int Device_GetIPAddr(uint8_t ETH_IPAddress[10][4], uint8_t SubnetMask[10][4], uint8_t* IPAddressNum);

/* The dev interface to control NX series devices to modify IP address (advanced application)*/
HTRA_API int Device_SetIPAddr(void** Device, const IPVersion_TypeDef ETH_IPVersion, const uint8_t ETH_IPAddress[], const uint8_t SubnetMask[]);

/* The dev interface to restart the network of NX series devices for the changed IP addresses to take effect (advanced application)*/
HTRA_API int Device_RestartNetwork(void** Device);

/* The dev interface to control the restart of NX series devices (advanced application) */
HTRA_API int Device_Reboot(void** Device);

/* The dev interface, update device MCU firmware (advanced application)*/
HTRA_API int Device_MCUFirmwareUpdate(int DeviceNum, const char* path);

/* The dev interface, update device FPGA firmware (advanced application)*/
HTRA_API int Device_FPGAFirmwareUpdate(int DeviceNum, const char* path);

/* SWP mode, reset spectrum data of MaxHold in SWP mode(advanced application) */
HTRA_API void SWP_MaxHoldReset(void** Device);

/* IQS mode, Initiate timer - Externally triggered single synchronization*/
HTRA_API int IQS_SyncTimerByExtTrigger_Single(void** Device);

/* DET mode, Initiate timer - Externally triggered single synchronization*/
HTRA_API int DET_SyncTimerByExtTrigger_Single(void** Device);

/* RTA mode, Initiate timer - Externally triggered single synchronization*/
HTRA_API int RTA_SyncTimerByExtTrigger_Single(void** Device);


/* -------------------------------------------The following are unnecessary function calls------------------------------------------------------ */

/* The dev interface to print boot information, including UID,Model,HardwareVersion and etc. */
HTRA_API int Device_PrintBootInformation(BootInfo_TypeDef BootInfo);

/* The dev interface to get the current time(-H-M-S) and optionally print it or not */
HTRA_API void Device_GetLocalTime(int* hour, int* min, int* sec, int ifprint);

/* SWP mode, Get debug information at a specified frequency in SWP mode (advanced applications) */
HTRA_API int SWP_GetDebugInfo(void** Device, double Freq, SWP_DebugInfo_TypeDef* DebugInfo);

/* Cal mode, calibration function (CAL related interface) */
HTRA_API int CAL_QDC_GetOptParam(const IQStream_TypeDef* IQStream, CAL_QDCOptParam_TypeDef* OptParam_O);

/* Dev interface, query device boot information */
HTRA_API int Device_QueryBootInfo(int DeviceNum, const BootProfile_TypeDef* BootProfile, BootInfo_TypeDef* BootInfo, uint32_t* UID_H32);

/* Dev interface, verify device availability */
HTRA_API int Device_CheckDeviceAvailability(int DeviceNum, const BootProfile_TypeDef* BootProfile);
/* The following is signal generator control (SIG and VSG related interface) */

/* SIG parameters are initialized */
HTRA_API int SIG_ProfileDeInit(void** Device, SIG_Profile_TypeDef* Profile);
/* Configuration of signal generator working state */
HTRA_API int SIG_Configuration(void** Device, SIG_Profile_TypeDef* ProfileIn, SIG_Profile_TypeDef* ProfileOut, SIG_Info_TypeDef* SIG_Info);
/* The signal generator updates the packet header and packet tail sizes */
HTRA_API int SIG_UpdateHeaderTailSize(void** Device, uint32_t* HeaderSize, uint32_t* TailSize);
/* SIG sending IQStream */
HTRA_API int SIG_SendIQStream(void** Device, unsigned char* DataBuffer, int DataSize);
/* SIG bus trigger start */
HTRA_API int SIG_BusTriggerStart(void** Device);
/* SIG bus trigger stop */
HTRA_API int SIG_BusTriggerStop(void** Device);

/* VSG parameters are initialized */
HTRA_API int VSG_ProfileDeInit(void** Device, VSG_Profile_TypeDef* Profile);
/* Configuration of VSG working state */
HTRA_API int VSG_Configuration(void** Device, VSG_Profile_TypeDef* ProfileIn, VSG_Profile_TypeDef* ProfileOut, VSG_Info_TypeDef* VSG_Info);
/* The starting address of the VSG configuration download */
HTRA_API int VSG_Download_SetStartAddress(void** Device, uint32_t StartAddress);
/* VSG in IQStream file (single)*/
HTRA_API int VSG_Download_IQFile(void** Device, const char* File, VSG_FileInfo_TypeDef* VSG_FileInfo);
/* VSG in IQStream file (multiple)*/
HTRA_API int VSG_Download_IQFiles(void** Device, char File[8][128], VSG_FileInfo_TypeDef* VSG_FileInfo, uint32_t VSG_FileCount);
/* The VSG updates the packet header and packet tail sizes */
HTRA_API int VSG_UpdateHeaderTailSize(void** Device, uint32_t* HeaderSize, uint32_t* TailSize);
/* The VSG downloads IQStream data */
HTRA_API int VSG_Download_IQData(void** Device, int16_t* I, int16_t* Q, uint32_t length, uint32_t* StartAddress, uint32_t* StopAddress);
/* The VSG downloads IQStream data(interleave)*/
HTRA_API int VSG_Download_IQData_Interleave(void** Device, int16_t* IQ, uint32_t length, uint32_t* StartAddress, uint32_t* StopAddress);
/* VSG bus trigger start */
HTRA_API int VSG_BusTriggerStart(void** Device);
/* VSG bus trigger stop */
HTRA_API int VSG_PrivateParamDeInit(void** Device, VSG_PrivateParam_TypeDef* VSG_PrivateParam);
/* VSG updates the private parameters */
HTRA_API int VSG_UpdatePrivateParam(void** Device, VSG_PrivateParam_TypeDef* VSG_PrivateParam);

/* Analog modulation to generate the signal waveform to be modulated */
HTRA_API int ASD_GenerateSignalWaveform(void** AnalogMod, AnalogSignalType_TypeDef AnalogSignalType, double SignalFrequency, double  SampleRate, int resetflag, float* m, int* SamplePoints);

/* The modulation is simulated to produce the final FM modulated waveform */
HTRA_API int ASD_GenerateFMWaveform(void** AnalogMod, SIG_FM_Profile_TypeDef* SIG_FM_Profile, float* m, short* I, short* Q, int* SamplePoints);
HTRA_API int ASD_GeneratePulseWaveform(void** AnalogMod, SIG_Pulse_Profile_TypeDef* SIG_Pulse_Profile, short* I, short* Q, int* SamplePoints);

/* The modulation is simulated to produce the final AM modulated waveform */
HTRA_API int ASD_GenerateAMWaveform(void** AnalogMod, SIG_AM_Profile_TypeDef* SIG_AM_Profile, float* m, short* I, short* Q, int* SamplePoints);

/* DSP auxiliary function that performs IF calibration */
HTRA_API int DSP_IFCalibration(void** Device, double* Frequency, float* Spectrum, unsigned int SpectrumPoints);

/* DSP auxiliary function to obtain the window type factor of the specified window type */
HTRA_API void DSP_GetWindowCoefficient(Window_TypeDef Window, int n, double Coefficient[]);

/* DSP auxiliary function, performs windowing*/
HTRA_API void DSP_Windowing(DataFormat_TypeDef DataFormat, void* Data_I, const double* Window, uint32_t Samples, double* Data_O);

/* DSP auxiliary function to obtain white gaussian noise */
HTRA_API void DSP_GetWhiteGaussianNoise(uint32_t n, float* noise);

/* DSP auxiliary function, convolution */
HTRA_API void DSP_Convolution(float x[], float y[], int x_size, int y_size, float Output[]);

/* DSP auxiliary function, decimate */
HTRA_API void DSP_Decimate(float xn[], float yn[], uint64_t size, uint32_t DecimateFactor);

/* DSP auxiliary function to generate a sine wave */
HTRA_API void DSP_GenerateSineWaveform(float yn[], const Sin_TypeDef* Sin_Profile);

/* DSP auxiliary function, NCO is performed to generate sine and cosine signals */
HTRA_API void DSP_NCO_Execute(const Sin_TypeDef* NCO_Profile, float* sin, float* cos);

#ifdef __cplusplus
}
#endif

#endif //HTRA_API_H