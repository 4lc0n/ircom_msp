#ifndef IRLAP
#define IRLAP

#include <stdint.h>

/**
 * @brief C Style definitions of Constants and Structs used for the
 * IRLAP Communication. 
 * 
 * Author: Juergen Markl
 * Date: 2022-11-30
 */


// definitions for IrLAP
#define IRLAP_BOF       (0xC0)
#define IRLAP_EOF       (0xC1)
#define IRLAP_CE        (0x7D)



typedef struct {
    uint8_t parameter_identification;
    uint8_t parameter_length;
    uint8_t parameter;
}connection_parameter_unit;

typedef struct{
    connection_parameter_unit baud_rate;
    connection_parameter_unit turnaround_time;
    connection_parameter_unit data_size;
    connection_parameter_unit window_size;
    connection_parameter_unit add_BOFs;
    connection_parameter_unit turnaround_time_min;
    connection_parameter_unit link_disconnect_threshold;
}connection_parameter;

// connection parameters
// baud rate
#define BAUD_PI         (0x01)
#define BAUD_2400       (0x01)
#define BAUD_9600       (0x02)
#define BAUD_19200      (0x04)
#define BAUD_38400      (0x08)
#define BAUD_57600      (0x10)
#define BAUD_115200     (0x20)
#define BAUD_576000     (0x40)
#define BAUD_1152000    (0x80)

// max turnaround time
#define TURNAROUND_T_PI  (0x82)
#define TURNAROUND_T_500 (0x01)
#define TURNAROUND_T_250 (0x02) // only valid for 115200 bps or higher
#define TURNAROUND_T_100 (0x04) // only valid for 115200 bps or higher
#define TURNAROUND_T_50  (0x08) // only valid for 115200 bps or higher

// data size
#define DATA_SIZE_PI    (0x83)
#define DATA_SIZE_64    (0x01)    
#define DATA_SIZE_128   (0x02)
#define DATA_SIZE_256   (0x04)
#define DATA_SIZE_512   (0x08)
#define DATA_SIZE_1024  (0x10)
#define DATA_SIZE_2048  (0x20)

// window size (max. acked packages)
#define WINDOW_SIZE_PI  (0x84)
#define WINDOW_SIZE_1   (0x01)
#define WINDOW_SIZE_2   (0x02)
#define WINDOW_SIZE_3   (0x04)
#define WINDOW_SIZE_4   (0x08)
#define WINDOW_SIZE_5   (0x10)
#define WINDOW_SIZE_6   (0x20)
#define WINDOW_SIZE_7   (0x40)

// additional BOFs at 115200 bps
#define ADD_BOFS_PI     (0x85)
#define ADD_BOFS_48     (0x01)
#define ADD_BOFS_24     (0x02)
#define ADD_BOFS_12     (0x04)
#define ADD_BOFS_5      (0x08)
#define ADD_BOFS_3      (0x10)
#define ADD_BOFS_2      (0x20)
#define ADD_BOFS_1      (0x40)
#define ADD_BOFS_0      (0x80)

// divider for BOFs for selected baudrate
#define BOFS_DIV_2400   (48)
#define BOFS_DIV_9600   (12)
#define BOFS_DIV_19200  (6)
#define BOFS_DIV_38400  (3)
#define BOFS_DIV_57600  (2)
#define BOFS_DIV_115200 (1)


// min turnaround time
#define TURNAROUND_T_MIN_PI     (0x86)
#define TURNAROUND_T_MIN_10MS   (0x01)
#define TURNAROUND_T_MIN_5MS    (0x02)
#define TURNAROUND_T_MIN_1MS    (0x04)
#define TURNAROUND_T_MIN_0_5MS  (0x08)
#define TURNAROUND_T_MIN_0_1MS  (0x10)
#define TURNAROUND_T_MIN_50US   (0x20)
#define TURNAROUND_T_MIN_10US   (0x40)
#define TURNAROUND_T_MIN_0      (0x80)

// link disconnect threshold time
#define LINK_DISC_TH_T_PI       (0x08)
#define LINK_DISC_TH_T_3        (0x01)
#define LINK_DISC_TH_T_8        (0x02)
#define LINK_DISC_TH_T_12       (0x04)
#define LINK_DISC_TH_T_16       (0x08)
#define LINK_DISC_TH_T_20       (0x10)
#define LINK_DISC_TH_T_25       (0x20)
#define LINK_DISC_TH_T_30       (0x40)
#define LINK_DISC_TH_T_40       (0x80)



/**
 * @brief Struct format for the address field in LAP frame
 * 
 * LSB: command response bit
 * Bits above: actual address bits (7 bits)
 * 
 */
typedef struct {
    
    uint8_t c_r:1;      // lsb; command - response (1 == cmd), (0 == rsp)
    uint8_t address:7;  // msb's
}address_field_struct;

typedef union {
    address_field_struct c;
    uint8_t raw;
}address_field_t;         // address field union with address and C/R field


// ================================================
// =========== Supervisory Format =================
// ================================================
// supervisory control format
typedef struct {
    
    uint8_t cs:4;           // lsb; control sequence (XX01 bits)
    uint8_t p_f:1;          // poll/final bit field: poll -> request data from sec. final -> last frame from second.
    uint8_t nr:3;           // Nr field for numbered frame number - 1
}s_control_field_struct;

typedef union {
    s_control_field_struct s;
    uint8_t raw;
}s_control_field;         // supervisory control field union with Nr, P/F, X, X, 0, 1


// ================================================
// =========== Information Format =================
// ================================================

// information control format
typedef struct {
    
    uint8_t none:1;         // lsb; always 0 
    uint8_t ns:3;           // Ns field for
    uint8_t p_f:1;          // poll/final bit field: poll -> request data from sec. final -> last frame from second.
    uint8_t nr:3;           // Nr field for numbered frame number - 1
}i_control_field_struct;

typedef union {
    s_control_field_struct s;
    uint8_t raw;
}i_control_field;         // supervisory control field union with Nr, P/F, X, X, 0, 1


// ================================================
// ============ Unnumbered Format =================
// ================================================
/**
 * @brief Information field format for SNRM Frame
 * 
 */
typedef struct {
    uint32_t soure_device_address;
    uint32_t destination_device_address;
    address_field_t address_field;
    connection_parameter negotiation_parameters;
    
}information_field_SNRM;


typedef struct{
    uint32_t source_device_address;
    uint32_t destination_device_address;
    uint8_t discovery_flags;
    uint8_t slot_number;
    uint8_t version_number;
    uint8_t discover_info[32];
}XID_format_field;

/**
 * @brief Informatino field format for XID (exchange station identification) field
 * 
 */
typedef struct{
    address_field_t address_field;        // address field (raw): Command: 0xFF, Response: 0xFE
    uint8_t xid_cmd_rsp;                // xid command / response
    uint8_t format_identifier;          // 0x01 always
    XID_format_field xid_format_field;  // format specific fields
}information_field_XID;




/**
 * @brief Information field format for TEST field; information field optional
 * 
 */
typedef struct{
    uint32_t source_device_address;
    uint32_t destination_device_address;
    uint8_t* information;
    uint16_t information_length;
}information_field_TEST;



/**
 * @brief Information field format for UA unnumbered acknoledgment; information field 
 * not always required
 * 
 */
typedef struct{
    uint32_t source_device_address;
    uint32_t destination_device_address;
    connection_parameter negotiation_parameters;
} information_field_UA;


/**
 * @brief information field for Frame Reject field. 
 * 
 */
typedef struct{
    uint8_t reject_control_field;
    i_control_field nr_ns_field;    
    uint8_t wxyz;
}information_field_FRMR;





#endif