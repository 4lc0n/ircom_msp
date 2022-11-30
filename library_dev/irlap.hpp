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
}address_field;         // address field union with address and C/R field



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
    address_field address_field;
    uint8_t* negotiation_parameters;
    uint16_t negotiation_parameters_length;
}information_field_SNRM;


/**
 * @brief Informatino field format for XID (exchange station identification) field
 * 
 */
typedef struct{
    address_field address_field;        // address field (raw): Command: 0xFF, Response: 0xFE
    uint8_t xid_cmd_rsp;                // xid command / response
    uint8_t format_identifier;          // 0x01 always
    XID_format_field xid_format_field;  // format specific fields
}information_field_XID;

typedef struct{
    uint32_t source_device_address;
    uint32_t destination_device_address;
    uint8_t discovery_flags;
    uint8_t slot_number;
    uint8_t version_number;
    uint8_t discover_info[32];
}XID_format_field;


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
    uint8_t* negotiation_parameters;
    uint16_t negotiation_parameters_length;
} information_field_UA;


/**
 * @brief information field for Frame Reject field. 
 * 
 */
typedef struct{
    uint8_t reject_control_field;
    uint8_t nr_ns_field;    // TODO: struct for numbered format
    uint8_t wxyz;
}information_field_FRMR;


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

#endif