
#ifndef __TPS23861_H_
#define __TPS23861_H_

#define TPS_SUCCESSFUL                         0x00
#define TPS_ERR_PORT_IN_USE                    0x80
#define TPS_ERR_PORT_NOT_IN_USE                0x81
#define TPS_ERR_NO_PORT_AVAILABLE              0x82
#define TPS_ERR_I2C_ERROR                      0x83
#define TPS_ERR_I2C_ADDRESS_MISMATCH           0x40
#define TPS_ERR_I2C_AUTOBIT_MISMATCH           0x41

#define TPS_GIVE_ME_NEXT_PORT_AVAILABLE        0xff
#define TPS_PORT_NOT_REGISTERED_VALUE          0xff

#define SWIZZLE_BYTES(x)                       {unsigned short y; y = x; x = (((y & 0xff) << 8) | (y >> 8)); }     

#define TPS_MAX_SYSTEM_PORTS                   32

#define TPS_VOLTAGE_SCALE_FACTOR               3662
#define TPS_CURRENT_SCALE_FACTOR               70190

typedef enum {
  TPS_OFF                                    = 0x0,   
  TPS_ON                                     = 0x1
} TPS238x_On_Off_t;

typedef enum {
  TPS238X_PORT_1                             = 1,       
  TPS238X_PORT_2                             = 2,
  TPS238X_PORT_3                             = 3,
  TPS238X_PORT_4                             = 4
} TPS238x_PortNum_t;

#define PORT_1_VALUE                           0x1            
#define PORT_2_VALUE                           0x2            
#define PORT_3_VALUE                           0x4            
#define PORT_4_VALUE                           0x8            

typedef unsigned char TPS238x_Ports_t;

#define TPS238X_ALL_PORTS                    ((TPS238x_Ports_t)0xf)

typedef struct {
    unsigned int      i2cAddress;
    TPS238x_PortNum_t devicePortNum;
} TPS238x_System_Port_Map_t;

typedef struct {
	unsigned char PEC_Power_Enable_Change                    : 1;   
	unsigned char PGC_Power_Good_Change                      : 1;   
	unsigned char DISF_Disconnect_Event                      : 1;   
	unsigned char DETC_Detection_Cycle                       : 1;   
	unsigned char CLASC_Classification_Cycle                 : 1;   
	unsigned char IFAULT_ICUT_ILIM_Fault                     : 1;   
	unsigned char INRF_Inrush_Fault                          : 1;   
	unsigned char SUPF_Supply_Event_Fault                    : 1;   
}TPS238X_Interrupt_Register_t;

#define PEC                                    0x01   
#define PGC                                    0x02   
#define DISF                                   0x04   
#define DETC                                   0x08   
#define CLASC                                  0x10   
#define IFAULT                                 0x20   
#define INRF                                   0x40   
#define SUPF                                   0x80   

typedef struct {
	unsigned char PEMSK_Power_Enable_Unmask                  : 1;   
	unsigned char PGMSK_Power_Good_Unmask                    : 1;   
	unsigned char DIMSK_Disconnect_Unmask                    : 1;   
	unsigned char DEMSK_Detection_Cycle_Unmask               : 1;   
	unsigned char CLMSK_Classificiation_Cycle_Unmask         : 1;   
	unsigned char IFMSK_IFAULT_Unmask                        : 1;   
	unsigned char INMSK_Inrush_Fault_Unmask                  : 1;   
	unsigned char SUMSK_Supply_Event_Fault_Unmask            : 1;   
}TPS238X_Interrupt_Mask_Register_t;

#define PEMSK                                  0x01   
#define PGMSK                                  0x02   
#define DIMSK                                  0x04   
#define DEMSK                                  0x08   
#define CLMSK                                  0x10   
#define IFMSK                                  0x20   
#define INMSK                                  0x40   
#define SUMSK                                  0x80   

typedef struct {
	unsigned char PEC1_Power_Enable_Port_1_Event             : 1;   
	unsigned char PEC2_Power_Enable_Port_2_Event             : 1;   
	unsigned char PEC3_Power_Enable_Port_3_Event             : 1;   
	unsigned char PEC4_Power_Enable_Port_4_Event             : 1;   
	unsigned char PGC1_Power_Good_Port_1_Event               : 1;   
	unsigned char PGC2_Power_Good_Port_2_Event               : 1;   
	unsigned char PGC3_Power_Good_Port_3_Event               : 1;   
	unsigned char PGC4_Power_Good_Port_4_Event               : 1;   
}TPS238X_Power_Enable_Register_t;

#define PEC1                                   0x01   
#define PEC2                                   0x02   
#define PEC3                                   0x04   
#define PEC4                                   0x08   
#define PGC1                                   0x10   
#define PGC2                                   0x20   
#define PGC3                                   0x40   
#define PGC4                                   0x80   
                                 
#define POWER_GOOD_EVENT_SHIFT                 0
#define POWER_ENABLE_EVENT_SHIFT               4

typedef struct {
	unsigned char DETC1_Detection_Cycle_Port_1_Event         : 1;   
	unsigned char DETC2_Detection_Cycle_Port_2_Event         : 1;   
	unsigned char DETC3_Detection_Cycle_Port_3_Event         : 1;   
	unsigned char DETC4_Detection_Cycle_Port_4_Event         : 1;   
	unsigned char CLSC1_Classification_Cycle_Port_1_Event    : 1;   
	unsigned char CLSC2_Classification_Cycle_Port_2_Event    : 1;   
	unsigned char CLSC3_Classification_Cycle_Port_3_Event    : 1;   
	unsigned char CLSC4_Classification_Cycle_Port_4_Event    : 1;   
}TPS238X_Detection_Event_Register_t;

#define DETC1                                  0x01   
#define DETC2                                  0x02   
#define DETC3                                  0x04   
#define DETC4                                  0x08   
#define CLSC1                                  0x10   
#define CLSC2                                  0x20   
#define CLSC3                                  0x40   
#define CLSC4                                  0x80   
              
#define DETECTION_EVENT_SHIFT                  0
#define CLASSIFICATION_EVENT_SHIFT             4

typedef struct {
	unsigned char ICUT1_ICUT_Fault_Port_1_Event              : 1;    
	unsigned char ICUT2_ICUT_Fault_Port_2_Event              : 1;    
	unsigned char ICUT3_ICUT_Fault_Port_3_Event              : 1;    
	unsigned char ICUT4_ICUT_Fault_Port_4_Event              : 1;    
	unsigned char DISF1_Disconnect_Port_1_Event              : 1;    
	unsigned char DISF2_Disconnect_Port_2_Event              : 1;    
	unsigned char DISF3_Disconnect_Port_3_Event              : 1;    
	unsigned char DISF4_Disconnect_Port_4_Event              : 1;    
}TPS238X_Fault_Event_Register_t;

#define ICUT1                                  0x01    
#define ICUT2                                  0x02    
#define ICUT3                                  0x04    
#define ICUT4                                  0x08    
#define DISF1                                  0x10    
#define DISF2                                  0x20    
#define DISF3                                  0x40    
#define DISF4                                  0x80    

#define ICUT_EVENT_SHIFT                       0
#define DISCONNECT_EVENT_SHIFT                 4

typedef struct {
	unsigned char INR1_Inrush_Fault_Port_1_Event             : 1;    
	unsigned char INR2_Inrush_Fault_Port_2_Event             : 1;    
	unsigned char INR3_Inrush_Fault_Port_3_Event             : 1;    
	unsigned char INR4_Inrush_Fault_Port_4_Event             : 1;    
	unsigned char ILIM1_Limit_Output_Current_Port_1_Event    : 1;    
	unsigned char ILIM2_Limit_Output_Current_Port_2_Event    : 1;    
	unsigned char ILIM3_Limit_Output_Current_Port_3_Event    : 1;    
	unsigned char ILIM4_Limit_Output_Current_Port_4_Event    : 1;    
}TPS238X_Inrush_ILIM_Event_Register_t;

#define INR1                                   0x01    
#define INR2                                   0x02    
#define INR3                                   0x04    
#define INR4                                   0x08    
#define ILIM1                                  0x10    
#define ILIM2                                  0x20    
#define ILIM3                                  0x40    
#define ILIM4                                  0x80    

#define INRUSH_EVENT_SHIFT                     0
#define ILIM_EVENT_SHIFT                       4

typedef struct {
	unsigned char Reserved_1                                 : 4;
	unsigned char VPUV_VPower_Undervoltage_Event             : 1;   
	unsigned char VDUV_Vdd_UVLO_Event                        : 1;   
	unsigned char Reserved_2                                 : 1;
	unsigned char TSD_Thermal_Shutdown_Event                 : 1;   
}TPS238X_Supply_Event_Register_t;

#define VPUV                                   0x10
#define VDUV                                   0x20
#define TSD                                    0x80

typedef struct {
    unsigned char Detect                                     : 4;
    unsigned char Class                                      : 4;
}TPS238X_Port_Status_Register_t;

typedef enum {
  CLASS_UNKNOWN                              = 0x0,   
  CLASS_1                                    = 0x1,   
  CLASS_2                                    = 0x2,   
  CLASS_3                                    = 0x3,   
  CLASS_4                                    = 0x4,   
  CLASS_0                                    = 0x6,   
  CLASS_OVERCURRENT                          = 0x7,   
  CLASS_MISMATCH                             = 0x8,   
  CLASS_5                                    = 0x9    
} TPS238x_Classification_Status_t;

typedef enum {
  DETECT_UNKNOWN                             = 0x0,   
  DETECT_SHORT_CIRCUIT                       = 0x1,   
  DETECT_RESIST_LOW                          = 0x3,   
  DETECT_RESIST_VALID                        = 0x4,   
  DETECT_RESIST_HIGH                         = 0x5,   
  DETECT_OPEN_CIRCUIT                        = 0x6,   
  DETECT_MOSFET_FAULT                        = 0x8,   
  DETECT_LEGACY                              = 0x9,   
  DETECT_CAP_INVALID_CLAMP_VOLTAGE           = 0xA,   
  DETECT_CAP_INVALID_DELTA_V                 = 0xB,   
  DETECT_CAP_INVALID_LEGACY_RANGE            = 0xC    
} TPS238x_Detection_Status_t;

#define DETECT                                 0x0F
#define CLASS                                  0xF0
#define CLASS_SHIFT                            4
#define DETECT_SHIFT                           0
#define GET_DETECT(x)                          (x & DETECT)
#define GET_CLASS(x)                           (x >> CLASS_SHIFT)

typedef struct {
	unsigned char PE1_Power_Enable_Port_1_Status             : 1;   
	unsigned char PE2_Power_Enable_Port_2_Status             : 1;   
	unsigned char PE3_Power_Enable_Port_3_Status             : 1;   
	unsigned char PE4_Power_Enable_Port_4_Status             : 1;   
	unsigned char PG1_Power_Good_Port_1_Status               : 1;   
	unsigned char PG2_Power_Good_Port_2_Status               : 1;   
	unsigned char PG3_Power_Good_Port_3_Status               : 1;   
	unsigned char PG4_Power_Good_Port_4_Status               : 1;   
}TPS238x_Power_Stauts_Register_t;

#define PE1_STATUS                             0x01   
#define PE2_STATUS                             0x02   
#define PE3_STATUS                             0x04   
#define PE4_STATUS                             0x08   
#define PG1_STATUS                             0x10   
#define PG2_STATUS                             0x20   
#define PG3_STATUS                             0x40   
#define PG4_STATUS                             0x80   
#define POWER_ENABLE_STATUS                   (PE1_STATUS + PE2_STATUS + PE3_STATUS + PE4_STATUS)
#define POWER_GOOD_STATUS                     (PG1_STATUS + PG2_STATUS + PG3_STATUS + PG4_STATUS)
#define POWER_GOOD_SHIFT                       4
#define GET_POWER_ENABLE_STATUS(x)            (x & POWER_ENABLE_STATUS)
#define GET_POWER_GOOD_STATUS(x)              (x >> POWER_GOOD_SHIFT)

typedef struct {
	unsigned char I2C_slave_address                          : 7;    
	unsigned char Auto                                       : 1;    
}TPS238x_I2C_Slave_Address_Register_t;

#define AUTO_BIT                               0x80    
#define I2C_ADDRESS_MASK                       0x7F    

#define TPS238X_ALERT_RESPONSE_ADDRESS         0x0C
#define TPS238X_BROADCAST_ADDRESS              0x30
#define TPS238X_UNLOCK_CODE                    0xAA      

typedef struct {
	unsigned char Port_1_Operating_Mode                      : 2;    
	unsigned char Port_2_Operating_Mode                      : 2;    
	unsigned char Port_3_Operating_Mode                      : 2;    
	unsigned char Port_4_Operating_Mode                      : 2;    
}TPS238x_Operating_Mode_Register_t;

typedef enum {
  OPERATING_MODE_OFF                         = 0x0,   
  OPERATING_MODE_MANUAL                      = 0x1,   
  OPERATING_MODE_SEMI_AUTO                   = 0x2,   
  OPERATING_MODE_AUTO                        = 0x3    
} TPS238x_Operating_Modes_t;

#define OPERATING_MODE_MASK                    0x3

#define OPERATING_PORT_1_MODE                  0x03
#define OPERATING_PORT_2_MODE                  0x0C
#define OPERATING_PORT_3_MODE                  0x30
#define OPERATING_PORT_4_MODE                  0xC0

typedef struct {
	unsigned char DCDE1_Disconnect_Enable_Port_1             : 1;   
	unsigned char DCDE2_Disconnect_Enable_Port_2             : 1;   
	unsigned char DCDE3_Disconnect_Enable_Port_3             : 1;   
	unsigned char DCDE4_Disconnect_Enable_Port_4             : 1;   
	unsigned char Reserved_3                                 : 4;
} TPS238x_Disconnect_Enable_Register_t;

#define DCDE1                                  0x01     
#define DCDE2                                  0x02     
#define DCDE3                                  0x04     
#define DCDE4                                  0x08     

typedef struct {
	unsigned char DETE1_Detection_Enable_Port_1              : 1;     
	unsigned char DETE2_Detection_Enable_Port_2              : 1;     
	unsigned char DETE3_Detection_Enable_Port_3              : 1;     
	unsigned char DETE4_Detection_Enable_Port_4              : 1;     
	unsigned char CLE1_Classification_Enable_Port_1          : 1;     
	unsigned char CLE2_Classification_Enable_Port_2          : 1;     
	unsigned char CLE3_Classification_Enable_Port_3          : 1;     
	unsigned char CLE4_Classification_Enable_Port_4          : 1;     
} TPS238x_Detect_Classification_Enable_Register_t;

#define DETE1                                  0x01     
#define DETE2                                  0x02     
#define DETE3                                  0x04     
#define DETE4                                  0x08     
#define CLE1                                   0x10     
#define CLE2                                   0x20     
#define CLE3                                   0x40     
#define CLE4                                   0x80     

typedef struct {
    unsigned char Reserved_4                                 : 4;
	unsigned char FSE1_Fast_Shutdown_Enable_Port_1           : 1;   
	unsigned char FSE2_Fast_Shutdown_Enable_Port_2           : 1;   
	unsigned char FSE3_Fast_Shutdown_Enable_Port_3           : 1;   
	unsigned char FSE4_Fast_Shutdown_Enable_Port_4           : 1;   
} TPS238x_Fast_Shutdown_t;

#define FSE1                                   0x10
#define FSE2                                   0x20
#define FSE3                                   0x40
#define FSE4                                   0x80

#define FSE_SHIFT                              4

typedef struct {
	unsigned char TDIS_Time_Disconnect_Delay                 : 2;    
    unsigned char TICUT_ICUT_Fault_Timing                    : 2;    
	unsigned char TSTART_Start_Time                          : 2;    
	unsigned char TLIM_ILIM_Fault_Timing                     : 2;    
} TPS238x_Timing_Configuration_Register_t;

typedef enum {
  TLIM_60_MS                                 = 0x0,      
  TLIM_30_MS                                 = 0x1,      
  TLIM_15_MS                                 = 0x2,      
  TLIM_10_MS                                 = 0x3       
} TPS238x_ILIM_Timing_t;

typedef enum {
  TSTART_60_MS                               = 0x0,      
  TSTART_30_MS                               = 0x1,      
  TSTART_120_MS                              = 0x2       
} TPS238x_TStart_Timing_t;

typedef enum {
  TICUT_60_MS                                = 0x0,      
  TICUT_30_MS                                = 0x1,      
  TICUT_120_MS                               = 0x2,      
  TICUT_240_MS                               = 0x3       
} TPS238x_TICUT_Timing_t;

typedef enum {
  TDIS_360_MS                                = 0x0,      
  TDIS_90_MS                                 = 0x1,      
  TDIS_180_MS                                = 0x2,      
  TDIS_720_MS                                = 0x3       
} TPS238x_TDIS_Timing_t;

#define TDIS_MASK                              0x3
#define TDIS_SHIFT                             0x0

#define TICUT_MASK                             0xC
#define TICUT_SHIFT                            0x2

#define TSTART_MASK                            0x30
#define TSTART_SHIFT                           0x4

#define TLIM_MASK                              0xC0
#define TLIM_SHIFT                             0x6

typedef struct {
	unsigned char _250M_Current_Sense_250_mOhm                 : 1;     
	unsigned char Reserved_5                                   : 3;
	unsigned char MAINS_Detection_Voltage_Measurement_Duration : 1;     
	unsigned char Reserved_6                                   : 2;
	unsigned char INTEN_INT_Pin_Mask                           : 1;     
} TPS238x_General_Mask_1_Register_t;

#define _960_A_D_CONV_PER_SECOND               0x10
#define _800_A_D_CONV_PER_SECOND               0x00

#define _250M                                  0x01      
#define MAINS                                  0x10      
#define INTEN                                  0x80      

typedef struct {
	unsigned char RDET1_Restart_Detection_Port_1             : 1;    
	unsigned char RDET2_Restart_Detection_Port_2             : 1;    
	unsigned char RDET3_Restart_Detection_Port_3             : 1;    
	unsigned char RDET4_Restart_Detection_Port_4             : 1;    
	unsigned char RCL1_Restart_Classification_Port_1         : 1;    
	unsigned char RCL2_Restart_Classification_Port_2         : 1;    
	unsigned char RCL3_Restart_Classification_Port_3         : 1;    
	unsigned char RCL4_Restart_Classification_Port_4         : 1;    
} TPS238x_Detect_Class_Restart_Register_t;

#define RDET1                                  0x01    
#define RDET2                                  0x02    
#define RDET3                                  0x04    
#define RDET4                                  0x08    
#define RCL1                                   0x10    
#define RCL2                                   0x20    
#define RCL3                                   0x40    
#define RCL4                                   0x80    

#define RESTART_DETECTION_SHIFT                0
#define RESTART_DETECTION_MASK                 0x0F

#define RESTART_CLASSIFCATION_SHIFT            4
#define RESTART_CLASSIFCATION_MASK             0xF0

typedef struct {
	unsigned char PWON1_Power_On_Port_1                      : 1;     
	unsigned char PWON2_Power_On_Port_2                      : 1;     
	unsigned char PWON3_Power_On_Port_3                      : 1;     
	unsigned char PWON4_Power_On_Port_4                      : 1;     
	unsigned char PWOFF1_Power_On_Port_1                     : 1;     
	unsigned char PWOFF2_Power_On_Port_2                     : 1;     
	unsigned char PWOFF3_Power_On_Port_3                     : 1;     
	unsigned char PWOFF4_Power_On_Port_4                     : 1;     
} TPS238x_Power_Enable_Register_t;

#define PWON1                                  0x01     
#define PWON2                                  0x02     
#define PWON3                                  0x04     
#define PWON4                                  0x08     
#define PWOFF1                                 0x10     
#define PWOFF2                                 0x20     
#define PWOFF3                                 0x40     
#define PWOFF4                                 0x80     

#define POWER_OFF_SHIFT                        4
#define POWER_OFF_MASK                         0xF0

#define POWER_ON_SHIFT                         0
#define POWER_ON_MASK                          0x0F

typedef struct {
	unsigned char RESP1_Reset_Port_1                         : 1;    
	unsigned char RESP1_Reset_Port_2                         : 1;    
	unsigned char RESP1_Reset_Port_3                         : 1;    
	unsigned char RESP1_Reset_Port_4                         : 1;    
	unsigned char RESAL_Reset_Registers                      : 1;    
	unsigned char Reserved_7                                 : 1;
	unsigned char CLINP_Clear_Interrupt_Pin                  : 1;     
	unsigned char CLRAIN_Clear_Interrupt_Bits                : 1;     
} TPS238x_Reset_Register_t;

#define RESP1                                  0x01
#define RESP2                                  0x02
#define RESP3                                  0x04
#define RESP4                                  0x08
#define RESAL                                  0x10
#define CLINP                                  0x40
#define CLRAIN                                 0x80

typedef struct {
	unsigned char ICV_IC_Version_Number                      : 3;     
	unsigned char MFR_ID_Manufacture_ID_Number               : 5;     
} TPS238x_ID_Register_t;

#define ICV                                    0x07
#define MFR_ID                                 0xF8
#define MFR_ID_SHIFT                           3
#define MFR_ID_DEFAULT                         0x0A  
#define ICV_DEFAULT                            0x00
#define ID_REGISTER_DEFAULT                    ((MFR_ID_DEFAULT<<MFR_ID_SHIFT) | ICV_DEFAULT)

typedef struct {
	unsigned char LEGMOD1_Legacy_Detect_Port_1               : 2;   
	unsigned char LEGMOD2_Legacy_Detect_Port_2               : 2;   
	unsigned char LEGMOD3_Legacy_Detect_Port_3               : 2;   
	unsigned char LEGMOD4_Legacy_Detect_Port_4               : 2;   
} TPS238x_Legacy_Detect_Register_t;

typedef enum {
  LEGACY_DETECT_DISABLED                     = 0x0,          
  LEGACY_DETECT_ONLY                         = 0x1,          
  LEGACY_DETECT_STANDARD_THEN_LEGACY         = 0x2           
} TPS238x_Legacy_Detect_t;                   

#define LEGMOD1                                0x03
#define LEGMOD2                                0x0C
#define LEGMOD3                                0x30
#define LEGMOD4                                0xC0

#define LEGACY_MODE_MASK                       3

#define LEGMOD1_SHIFT                          0
#define LEGMOD2_SHIFT                          2
#define LEGMOD3_SHIFT                          4
#define LEGMOD4_SHIFT                          6

typedef struct {
	unsigned char TECLEN1_Two_Event_Classification_Port_1    : 2;   
	unsigned char TECLEN2_Two_Event_Classification_Port_2    : 2;   
	unsigned char TECLEN3_Two_Event_Classification_Port_3    : 2;   
	unsigned char TECLEN4_Two_Event_Classification_Port_4    : 2;   
} TPS238x_Two_Event_Classification_Register_t;

typedef enum {
  TWO_EVENT_DISABLE                          = 0x0,          
  TWO_EVENT_AFTER_CLASS_4                    = 0x1,          
  TWO_EVENT_AFTER_CLASS_5                    = 0x2,          
  TWO_EVENT_AFTER_CLASS_4_OR_5               = 0x3           
} TPS238x_Two_Event_t;

#define TECLEN1                                0x03
#define TECLEN2                                0x0C
#define TECLEN3                                0x30
#define TECLEN4                                0xC0

#define TWO_EVENT_MASK                         3

#define TECLEN1_SHIFT                          0
#define TECLEN2_SHIFT                          2
#define TECLEN3_SHIFT                          4
#define TECLEN4_SHIFT                          6

typedef struct {
	unsigned char _4P12DIS_Disconnect_Mode_Four_Port_1_2     : 3;   
	unsigned char _4P12EN_Enable_Four_Port_Mode_1_2          : 1;   
	unsigned char _4P34DIS_Disconnect_Mode_Four_Port_3_4     : 3;   
	unsigned char _4P34EN_Enable_Four_Port_Mode_3_4          : 1;   
} TPS238x_Four_Port_Mode_Register_t;

typedef enum {
  FOUR_PAIR_DISCONNECT_DISABLED              = 0x0,
  FOUR_PAIR_DISCONNECT_BASED_ON_LOWER_PORT   = 0x1,       
  FOUR_PAIR_DISCONNECT_BASED_ON_HIGHER_PORT  = 0x2,       
  FOUR_PAIR_DISCONNECT_BASED_ON_EITHER_PORT  = 0x3,       
  FOUR_PAIR_DISCONNECT_BASED_ON_BOTH_PORTS   = 0x4        
} TPS238x_Four_Pair_t;

#define _4P12DIS                               0x07
#define _4P12EN                                0x08
#define _4P34DIS                               0x70
#define _4P34EN                                0x80

#define FOUR_PAIR_DISCONNECT_MASK              7

#define _4P12DIS_SHIFT                         0
#define _4P12EN_SHIFT                          3

#define _4P34DIS_SHIFT                         4
#define _4P34EN_SHIFT                          7

typedef struct {
	unsigned char TMR_Timer_Period_10_ms                     : 4;   
	unsigned char CL5EN1_Class_5_Enable_Port_1               : 1;   
	unsigned char CL5EN2_Class_5_Enable_Port_2               : 1;   
	unsigned char CL5EN3_Class_5_Enable_Port_3               : 1;   
	unsigned char CL5EN4_Class_5_Enable_Port_4               : 1;   
} TPS238x_Class_5_Enable_Timer_Register_t;

#define TMR_MASK                               0x0F   
#define CL5EN1                                 0x10   
#define CL5EN2                                 0x20   
#define CL5EN3                                 0x40   
#define CL5EN4                                 0x80   

#define CLASS_5_ENABLE_SHIFT                   4

typedef struct {
	unsigned char DCTH1_Disconnect_Current_Threshold_Port_1  : 2;     
	unsigned char DCTH2_Disconnect_Current_Threshold_Port_2  : 2;     
	unsigned char DCTH3_Disconnect_Current_Threshold_Port_3  : 2;     
	unsigned char DCTH4_Disconnect_Current_Threshold_Port_4  : 2;     
} TPS238x_Disconnect_Threshold_Register_t;

typedef enum {
  DCTH_7_5_MILLIAMP                          = 0x0,      
  DCTH_15_MILLIAMP                           = 0x1,      
  DCTH_30_MILLIAMP                           = 0x2,      
  DCTH_50_MILLIAMP                           = 0x3       
} TPS238x_Disconnect_Threshold_t;

#define DCTH1                                  0x03
#define DCTH2                                  0x0C
#define DCTH3                                  0x30
#define DCTH4                                  0xC0

#define DISCONNECT_THRESHOLD_MASK              3

#define DCTH1_SHIFT                            0
#define DCTH2_SHIFT                            2
#define DCTH3_SHIFT                            4
#define DCTH4_SHIFT                            6

typedef struct {
	unsigned char ICUT_Current_Threshold_Port_1              : 3;    
	unsigned char Reserved_8                                 : 1;
	unsigned char ICUT_Current_Threshold_Port_2              : 3;    
	unsigned char Reserved_9                                 : 1;
} TPS238x_ICUT21_Config_Regsiter_t;

typedef enum {
  ICUT_374_MILLIAMP                          = 0x0,       
  ICUT_110_MILLIAMP                          = 0x1,       
  ICUT_204_MILLIAMP                          = 0x2,       

  ICUT_754_MILLIAMP                          = 0x4,       
  ICUT_592_MILLIAMP                          = 0x5,       
  ICUT_686_MILLIAMP                          = 0x6,       
  ICUT_920_MILLIAMP                          = 0x7        
} TPS238x_ICUT_Config_t;

#define ICUT_THRESHOLD_MASK                    7

#define ICUT_PORT_1                            0x07
#define ICUT_PORT_2                            0x70

#define ICUT_PORT_1_SHIFT                      0
#define ICUT_PORT_2_SHIFT                      4

typedef struct {
	unsigned char ICUT_Current_Threshold_Port_3              : 3;     
	unsigned char Reserved_10                                : 1;
	unsigned char ICUT_Current_Threshold_Port_4              : 3;     
	unsigned char Reserved_11                                : 1;
} TPS238x_ICUT43_Config_Regsiter_t;

#define ICUT_PORT_3                            0x07
#define ICUT_PORT_4                            0x70

#define ICUT_PORT_3_SHIFT                      0
#define ICUT_PORT_4_SHIFT                      4

typedef struct {
	unsigned char Temp_Value;
} TPS238x_Temperature_Register_t;

#define CONVERT_TEMP(x)                        (((x*652)-20000)/1000)       

typedef union {
	struct Input_Voltage_Short_t {
		unsigned short Input_Voltage                         : 14;       
		unsigned short Reserved_12                           :  2;
	} Input_Voltage_Short;

	struct Input_Voltage_Char_t {
		unsigned char  Input_Voltage_LSB                     : 8;
		unsigned char  Input_Voltage_MSB                     : 6;
		unsigned char  Reserved_13                           : 2;
	} Input_Voltage_Char;
} TPS238x_Input_Voltage_Register_u;

typedef unsigned short TPS238x_Input_Voltage_t;

#define TPS2368X_INPUT_VOLTAGE_MASK_SHORT                    0x3FFF

typedef union {
	struct Port_Current_Short_t {
		unsigned short Port_Current                          : 14;    
		unsigned short Reserved_14                           :  2;
	} Port_Current_Short;

	struct Port_Current_Char_t {
		unsigned char  Port_Current_LSB                      : 8;
		unsigned char  Port_Current_MSB                      : 6;
		unsigned char  Reserved_15                           : 2;
	} Port_Current_Char;
} TPS238x_Port_Current_Register_u;

typedef unsigned short TPS238x_Port_Current_t;

#define TPS2368X_PORT_CURRENT_MASK_SHORT                     0x3FFF

typedef union {
	struct Port_Voltage_Short_t {
		unsigned short Port_Voltage                          : 14;       
		unsigned short Reserved_16                           :  2;
	} Port_Voltage_Short;

	struct Port_Voltage_Char_t {
		unsigned char  Port_Voltage_LSB                      : 8;
		unsigned char  Port_Voltage_MSB                      : 6;
		unsigned char  Reserved_17                           : 2;
	} Port_Voltage_Char;
} TPS238x_Port_Voltage_Register_u;

typedef unsigned short TPS238x_Port_Voltage_t;

#define TPS2368X_PORT_VOLTAGE_MASK_SHORT                     0x3FFF

typedef struct {
	unsigned char Reserved_18                                : 4;
	unsigned char POEP1_Foldback_Curve_Port_1                : 1;
	unsigned char POEP1_Foldback_Curve_Port_2                : 1;
	unsigned char POEP1_Foldback_Curve_Port_3                : 1;
	unsigned char POEP1_Foldback_Curve_Port_4                : 1;
} TPS238x_PoE_Plus_Register_t;

typedef enum {
  _1X_ILIM_FOLDBACK_CURVE                    = 0x0,       
  _2X_ILIM_FOLDBACK_CURVE                    = 0x1        
} TPS238x_POE_Plus_Foldback_t;

#define POEP1                                  0x10
#define POEP2                                  0x20
#define POEP3                                  0x40
#define POEP4                                  0x80

#define POE_PLUS_SHIFT                         4

typedef struct {
	unsigned char WDS_Watchdog_Status                        : 1;     
	unsigned char IWD_I2C_Watchdog_Disable                   : 4;     
	unsigned char Reserved_19                                : 3;
} TPS238x_I2C_Watchdog_Register_t;

#define WDS                                    0x01
#define IWD                                    0x1E

#define IWD_SHIFT                              1
#define IWD_MASK_VALUE                         0xB            

typedef struct {
	unsigned char Reserved_20                                : 5;
	unsigned char IGATE_Gate_Pullup_Current                  : 1;     
	unsigned char CLDN_Fault_Cool_Down_Timer                 : 2;     
} TPS238x_Cool_Down_Gate_Drive_Register_t;

typedef enum {
  COOL_DOWN_1_SEC                            = 0x0,     
  COOL_DOWN_2_SEC                            = 0x2,     
  COOL_DOWN_4_SEC                            = 0x3      
} TPS238x_Cool_Down_Timing_t;

#define IGATE                                  0x20
#define CLDN_MASK                              0xC0

#define CLDN_SHIFT                             6

#define GATE_PULLUP_CURRENT_50_MICROAMP        0
#define GATE_PULLUP_CURRENT_25_MICROAMP        1

typedef union {
	struct Port_Detect_Resistance_Short_t {
		unsigned short Port_Detect_Resistance                : 14;   
		unsigned short Detect_Status                         :  2;   
	} Port_Detect_Resistance_Short;

	struct Detect_Resistance_Char_t {
		unsigned char  Port_Detect_Resistance_LSB            : 8;
		unsigned char  Port_Detect_Resistance_MSB            : 6;
		unsigned char  Detect_Status                         : 2;
	} Port_Detect_Resistance_Char;
} TPS238x_Port_Detect_Resistance_Register_u;

typedef unsigned short TPS238x_Port_Detect_Resistance_t;

typedef enum {
  RS_STATUS_GOOD                             = 0x0,      
  RS_STATUS_SHORT_CIRCUIT                    = 0x1,      
  RS_STATUS_OPEN_CIRCUIT                     = 0x2,      
  RS_STATUS_MOSFET_SHORT_FAULT               = 0x3       
} TPS238x_Detect_Resistance_Status_t;

#define PORT_RESISTANCE_MASK_SHORT             0x3FFF
#define RS_MASK_SHORT                          0xC000
#define RS_SHIFT_SHORT                         14

typedef union {
	struct Port_Voltage_Difference_Short_t {
		unsigned short Port_Voltage_Difference               : 12;    
		unsigned short Voltage_Difference_Status             :  4;    
	} Port_Voltage_Difference_Short;

	struct Port_Voltage_Difference_Char_t {
		unsigned char  Port_Voltage_Difference_LSB           : 8;
		unsigned char  Port_Voltage_Difference_MSB           : 6;
		unsigned char  Voltage_Difference_Status             : 2;
	} Port_Voltage_Difference_Char;
} TPS238x_Port_Voltage_Difference_Register_u;

typedef unsigned short TPS238x_Port_Voltage_Difference_t;

typedef enum {
  VDS_STATUS_POWER_ON_RESET                  = 0x0,          
  VDS_STATUS_VALID_MEASUREMENT               = 0x1,          
  VDS_STATUS_TIMEOUT                         = 0x2,          
  VDS_STATUS_FIRST_MEASUREMENT_EXCESS        = 0x3,          
  VDS_STATUS_SECOND_MEASUREMENT_EXCESS       = 0x4,          
  VDS_STATUS_INSUFFICIENT_SIGNAL             = 0x5           
} TPS238x_Detect_Voltage_Difference_Status_t;

#define PORT_VOLTAGE_DIFFERENCE_MASK_SHORT     0x0FFF
#define VDS_MASK_SHORT                         0xF000
#define VDS_SHIFT_SHORT                        12

#define CONVERT_PORT_NUM(x)                    (1 << ((uint8_t)x-1))  

#define TPS238X_INTERRUPT_COMMAND                            0x00
#define TPS238X_INTERRUPT_MASK_COMMAND                       0x01
#define TPS238X_POWER_EVENT_COMMAND                          0x02
#define TPS238X_POWER_EVENT_CLEAR_COMMAND                    0x03
#define TPS238X_DETECTION_EVENT_COMMAND                      0x04
#define TPS238X_DETECTION_EVENT_CLEAR_COMMAND                0x05
#define TPS238X_FAULT_EVENT_COMMAND                          0x06
#define TPS238X_FAULT_EVENT_CLEAR_COMMAND                    0x07
#define TPS238X_INRUSH_LIMIT_EVENT_COMMAND                   0x08
#define TPS238X_INRUSH_LIMIT_EVENT_CLEAR_COMMAND             0x09
#define TPS238X_SUPPLY_EVENT_COMMAND                         0x0A
#define TPS238X_SUPPLY_EVENT_CLEAR_COMMAND                   0x0B
#define TPS238X_PORT_1_STATUS_COMMAND                        0x0C
#define TPS238X_PORT_2_STATUS_COMMAND                        0x0D
#define TPS238X_PORT_3_STATUS_COMMAND                        0x0E
#define TPS238X_PORT_4_STATUS_COMMAND                        0x0F
#define TPS238X_POWER_STATUS_COMMAND                         0x10
#define TPS238X_I2C_SLAVE_ADDRESS_COMMAND                    0x11
#define TPS238X_OPERATING_MODE_COMMAND                       0x12
#define TPS238X_DISCONNECT_ENABLE_COMMAND                    0x13
#define TPS238X_DETECT_CLASS_ENABLE_COMMAND                  0x14
#define TPS238X_PORT_POWER_PRIORITY_COMMAND                  0x15
#define TPS238X_TIMING_CONFIGURATION_COMMAND                 0x16
#define TPS238X_GENERAL_MASK_1_COMMAND                       0x17
#define TPS238X_DETECT_CLASS_RESTART_COMMAND                 0x18
#define TPS238X_POWER_ENABLE_COMMAND                         0x19
#define TPS238X_RESET_COMMAND                                0x1A
#define TPS238X_ID_COMMAND                                   0x1B

#define TPS238X_TEST_ENABLE_COMMAND                          0x1D

#define TPS238X_LEGACY_DETECT_MODE_COMMAND                   0x20
#define TPS238X_TWO_EVENT_CLASSIFICATION_COMMAND             0x21
#define TPS238X_FOUR_PAIR_MODE_COMMAND                       0x22

#define TPS238X_CLASS_FIVE_TIMER_ENABLE_COMMAND              0x27

#define TPS238X_DISCONNECT_THRESHOLD_COMMAND                 0x29
#define TPS238X_ICUT21_CONFIGURATION_COMMAND                 0x2A
#define TPS238X_ICUT43_CONFIGURATION_COMMAND                 0x2B
#define TPS238X_TEMPERATURE_COMMAND                          0x2C

#define TPS238X_INPUT_VOLTAGE_COMMAND                        0x2E
#define TPS238X_INPUT_VOLTAGE_LSB_COMMAND                    0x2E
#define TPS238X_INPUT_VOLTAGE_MSB_COMMAND                    0x2F

#define TPS238X_PORT_1_CURRENT_COMMAND                       0x30
#define TPS238X_PORT_1_CURRENT_LSB_COMMAND                   0x30
#define TPS238X_PORT_1_CURRENT_MSB_COMMAND                   0x31

#define TPS238X_PORT_1_VOLTAGE_COMMAND                       0x32
#define TPS238X_PORT_1_VOLTAGE_LSB_COMMAND                   0x32
#define TPS238X_PORT_1_VOLTAGE_MSB_COMMAND                   0x33

#define TPS238X_PORT_2_CURRENT_COMMAND                       0x34
#define TPS238X_PORT_2_CURRENT_LSB_COMMAND                   0x34
#define TPS238X_PORT_2_CURRENT_MSB_COMMAND                   0x35

#define TPS238X_PORT_2_VOLTAGE_COMMAND                       0x36
#define TPS238X_PORT_2_VOLTAGE_LSB_COMMAND                   0x36
#define TPS238X_PORT_2_VOLTAGE_MSB_COMMAND                   0x37

#define TPS238X_PORT_3_CURRENT_COMMAND                       0x38
#define TPS238X_PORT_3_CURRENT_LSB_COMMAND                   0x38
#define TPS238X_PORT_3_CURRENT_MSB_COMMAND                   0x39

#define TPS238X_PORT_3_VOLTAGE_COMMAND                       0x3A
#define TPS238X_PORT_3_VOLTAGE_LSB_COMMAND                   0x3A
#define TPS238X_PORT_3_VOLTAGE_MSB_COMMAND                   0x3B

#define TPS238X_PORT_4_CURRENT_COMMAND                       0x3C
#define TPS238X_PORT_4_CURRENT_LSB_COMMAND                   0x3C
#define TPS238X_PORT_4_CURRENT_MSB_COMMAND                   0x3D

#define TPS238X_PORT_4_VOLTAGE_COMMAND                       0x3E
#define TPS238X_PORT_4_VOLTAGE_LSB_COMMAND                   0x3E
#define TPS238X_PORT_4_VOLTAGE_MSB_COMMAND                   0x3F

#define TPS238X_POE_PLUS_COMMAND                             0x40
#define TPS238X_FIRMWARE_REVISION_COMMAND                    0x41
#define TPS238X_I2C_WATCHDOG_COMMAND                         0x42
#define TPS238X_DEVICE_ID_COMMAND                            0x43

#define TPS238X_COOL_DOWN_GATE_DRIVE_COMMAND                 0x45

#define TPS238X_PORT_1_DETECT_RESISTANCE_COMMAND             0x60
#define TPS238X_PORT_1_DETECT_RESISTANCE_LSB_COMMAND         0x60
#define TPS238X_PORT_1_DETECT_RESISTANCE_MSB_COMMAND         0x61

#define TPS238X_PORT_2_DETECT_RESISTANCE_COMMAND             0x62
#define TPS238X_PORT_2_DETECT_RESISTANCE_LSB_COMMAND         0x62
#define TPS238X_PORT_2_DETECT_RESISTANCE_MSB_COMMAND         0x63

#define TPS238X_PORT_3_DETECT_RESISTANCE_COMMAND             0x64
#define TPS238X_PORT_3_DETECT_RESISTANCE_LSB_COMMAND         0x64
#define TPS238X_PORT_3_DETECT_RESISTANCE_MSB_COMMAND         0x65

#define TPS238X_PORT_4_DETECT_RESISTANCE_COMMAND             0x66
#define TPS238X_PORT_4_DETECT_RESISTANCE_LSB_COMMAND         0x66
#define TPS238X_PORT_4_DETECT_RESISTANCE_MSB_COMMAND         0x67

#define TPS238X_PORT_1_DETECT_VOLTAGE_DIFF_COMMAND           0x68
#define TPS238X_PORT_1_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x68
#define TPS238X_PORT_1_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x69

#define TPS238X_PORT_2_DETECT_VOLTAGE_DIFF_COMMAND           0x6A
#define TPS238X_PORT_2_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x6A
#define TPS238X_PORT_2_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x6B

#define TPS238X_PORT_3_DETECT_VOLTAGE_DIFF_COMMAND           0x6C
#define TPS238X_PORT_3_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x6C
#define TPS238X_PORT_3_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x6D

#define TPS238X_PORT_4_DETECT_VOLTAGE_DIFF_COMMAND           0x6E
#define TPS238X_PORT_4_DETECT_VOLTAGE_DIFF_LSB_COMMAND       0x6E
#define TPS238X_PORT_4_DETECT_VOLTAGE_DIFF_MSB_COMMAND       0x6F

#define TPS238X_4PWIRED_POWER_ALLOCATION_CONFIG_COMMAND      0x29
#define TPS238X_4P_POLICE_12_CONFIG_COMMAND                  0x2A
#define TPS238X_4P_POLICE_34_CONFIG_COMMAND                  0x2B
#define TPS238X_TEMPERATURE_COMMAND                          0x2C
#define TPS238X_4P_DISCONNECT_PCUT_ILIM_CONFIG_COMMAND       0x2D

#define TPS238X_SRAM_CONTROL_COMMAND                         0x60
#define TPS238X_SRAM_DATA_COMMAND                            0x61

#define TPS238X_SRAM_START_ADDRESS_COMMAND                   0x62
#define TPS238X_SRAM_START_ADDRESS_LSB_COMMAND               0x62
#define TPS238X_SRAM_START_ADDRESS_MSB_COMMAND               0x63

#endif 

