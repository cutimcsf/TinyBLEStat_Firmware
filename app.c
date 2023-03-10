/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "em_system.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"

#include "peripherals/dac.h"
#include "peripherals/imu.h"
#include "peripherals/adc.h"
#include "peripherals/lmp91000_afe.h"
#include "clarkson_sensor.h"
#include "gatt_db.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
  ADC_initialize();
  IMU_initialize();

  LMP91000_enableSensor(LMP91000_1);
  LMP91000_setOpMode(OP_FET_SHORT_DISABLED, OP_MODE_TEMP_MEAS_ON);
  LMP91000_getValueMilliVolts();

  DAC_writeValue(0xffff);

//  LMP91000_getStatus(&value);
//  if (value) {
//      LMP91000_setOpMode(OP_FET_SHORT_DISABLED, OP_MODE_TEMP_MEAS_ON);
//  }
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  uint64_t uniqueDevNum;
  bd_addr  myAddy;
  char     myDisplayName[26] = {0};

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      uniqueDevNum = SYSTEM_GetUnique();

      myAddy.addr[0] = (uint8_t)(uniqueDevNum>>56);
      myAddy.addr[1] = (uint8_t)(uniqueDevNum>>48);
      myAddy.addr[2] = (uint8_t)(uniqueDevNum>>40);
      myAddy.addr[3] = (uint8_t)(uniqueDevNum>>16);
      myAddy.addr[4] = (uint8_t)(uniqueDevNum>>8);
      myAddy.addr[5] = (uint8_t)uniqueDevNum;

      sprintf(&(myDisplayName[0]), "%s", "TinyBLEStat ");
//      sprintf(&(myDisplayName[12]), "%02X", myAddy.addr[0]);
//      sprintf(&(myDisplayName[14]), "%02X", myAddy.addr[1]);
//      sprintf(&(myDisplayName[16]), "%02X", myAddy.addr[2]);
//      sprintf(&(myDisplayName[18]), "%02X", myAddy.addr[3]);
      sprintf(&(myDisplayName[12]), "%02X:", myAddy.addr[0]);
      sprintf(&(myDisplayName[15]), "%02X", myAddy.addr[5]);



      sl_status_t sc = sl_bt_gatt_server_write_attribute_value(
          gattdb_device_name,
          0,
          strlen(myDisplayName),
          (uint8_t*)myDisplayName);

      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;


    case sl_bt_evt_gatt_server_user_write_request_id:
      handleUserWriteRequest(&evt->data.evt_gatt_server_user_write_request);
      break;

    case sl_bt_evt_gatt_server_user_read_request_id:
      handleUserReadRequest(&evt->data.evt_gatt_server_user_read_request);
      break;

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
