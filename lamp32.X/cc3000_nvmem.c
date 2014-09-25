
#include "cc3000.h"
#include "cc3000_hci.h"
#include "cc3000_nvmem.h"


hci_status_t cc3000_nvmem_read_sp_version(uns8 *sp_maj, uns8 *sp_min) {

uns8 status;

	cc3000_hci_send_command(HCI_COMMAND_READ_SP_VERSION, NULL, 0);
	cc3000_hci_wait_for(HCI_COMMAND_READ_SP_VERSION, 0);
	
	status = wlan_rx_buffer[4];
	*sp_maj = wlan_rx_buffer[7];
	*sp_min = wlan_rx_buffer[8];

	return status;	
}
/*

NVMEM_NVS_FILEID, NVMEM_NVS_SHADOW_FILEID,
NVMEM_WLAN_CONFIG_FILEID, NVMEM_WLAN_CONFIG_SHADOW_FILEID,
NVMEM_WLAN_DRIVER_SP_FILEID, NVMEM_WLAN_FW_SP_FILEID,
NVMEM_MAC_FILEID, NVMEM_FRONTEND_VARS_FILEID,
NVMEM_IP_CONFIG_FILEID, NVMEM_IP_CONFIG_SHADOW_FILEID,
NVMEM_BOOTLOADER_SP_FILEID, NVMEM_RM_FILEID,
and user files 12-15.

*/


hci_status_t cc3000_nvmem_read(uns8 file_id, uns8 length, uns16 offset, uns8 *buffer) {

	uns8 status;
	
	cc3000_hci_start_command(HCI_COMMAND_NVMEM_READ, 3*4);
	cc3000_hci_send_uns32(file_id);
	cc3000_hci_send_uns32(length);
	cc3000_hci_send_uns32(offset);
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_COMMAND_NVMEM_READ, 0);

	if (cc3000_hci_wait_for(HCI_DATA_NVMEM, 0)==0) {	// status ok
//		serial_print_str(" STATUS OK ");
		uns8 hci_args_length = wlan_rx_buffer[2];	// 8 bit args length
		uns16 payload_length = wlan_rx_buffer[3] + (uns16)(wlan_rx_buffer[4] << 8);
		uns16 data_length = payload_length - hci_args_length;

                int count;
		for (count = 0; count < data_length; count++) {
			*(buffer++) = wlan_rx_buffer[5 + hci_args_length + count];
		}
	}
	return status;
}

/*
NVMEM_WLAN_DRIVER_SP_FILEID, NVMEM_WLAN_FW_SP_FILEID,
NVMEM_MAC_FILEID, NVMEM_BOOTLOADER_SP_FILEID,
and user files 12-15.
*/

hci_status_t cc3000_nvmem_write(uns8 file_id, uns8 length, uns16 offset, uns8 *buffer) {

uns8 status;

	cc3000_hci_start_command(HCI_COMMAND_NVMEM_WRITE,4*4);
	cc3000_hci_send_uns32(file_id);
	cc3000_hci_send_uns32(12);	// random 12 number
	cc3000_hci_send_uns32(length);
	cc3000_hci_send_uns32(offset);

        int count;
	for (count = 0; count < length; count++) {
//		cc3000_hci_send_uns8(buffer[count]);
	}
	
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_COMMAND_NVMEM_WRITE, 0);
	
	return status;
}


hci_status_t cc3000_nvmem_set_mac_address(uns8 *mac) {
	return  cc3000_nvmem_write(NVMEM_MAC_FILEID, MAC_ADDR_LEN, 0, mac);
}


hci_status_t cc3000_nvmem_get_mac_address(uns8 *mac) {
	return  cc3000_nvmem_read(NVMEM_MAC_FILEID, MAC_ADDR_LEN, 0, mac);
}

